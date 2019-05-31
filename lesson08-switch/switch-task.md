
# Базовая задача

Разработать двухпортовый повторитель для Ethernet-кадров.
Прием и передача кадров осуществляется через сокеты семейства **AF_PACKET**.
Каждый кадр, принятый на одном порте повторителя, всегда передается на другой
порт повторителя вне зависимости от MAC-адреса получателя в Ethernet-кадре.

## Описание базовой задачи

Для осуществления сетевого взаимодействия операционная система платы использует
определенный набор глобальных ресурсов. Данный набор включает в себя стеки
протоколов IPv4 и IPv6, таблицы маршрутизации, сетевые устройства и т.д.

В качестве механизма абстракции используются сетевые пространства имен, которые
позволяют для каждого процесса в системе установить свой изолированный
виртуальный набор глобальных ресурсов. На плате находятся пять сетевых
пространств имен: **default**, **ns1**, **ns2**, **ns3** и **ns4**.
В каждом из них находятся определенные физические интерфейсы.

Для исполнения процесса в определенном пространстве имен используется команда:

```console
$ nsenter --net=/etc/ns/<имя_пространства> -- <команда>
```

Например, для отображения IP-адресов интерфейсов, находящихся в сетевом
пространстве имен **ns1**, необходимо ввести команду:

```console
$ nsenter --net=/etc/ns/ns1 -- ip address
```

В целях идентификации каждому физическому или виртуальному сетевому устройству
операционная система назначает уникальный индекс.

Соответствие между сетевыми пространствами имен, физическими интерфейсами
и их индексами указано в следующей таблице:

| Имя интерфейса | Индекс | Пространство имен |
|:--------------:|:------:|:-----------------:|
|       ge1      |    3   |      default      |
|       ge2      |    4   |        ns1        |
|       ge3      |    5   |      default      |
|       ge4      |    6   |        ns2        |
|       ge5      |    7   |      default      |
|       ge6      |    8   |        ns3        |
|       ge7      |    9   |      default      |
|       ge8      |   10   |        ns4        |
|       ge9      |   11   |      default      |
|      ge10      |   12   |      default      |

В качестве портов для повторителя Ethernet-кадров необходимо выбрать
любые два интерфейса из сетевого пространства имен **default**
за исключением интерфейсов **ge9** и **ge10**.

## Алгоритм решения базовой задачи

1. Для каждого порта повторителя создать сокет семейства **AF_PACKET** с типом
   **SOCK_RAW** и привязать его к этому порту. Для привязки используется индекс
   сетевого интерфейса, соответствующего порту повторителя.

2. Прочитать сетевой пакет из сокета, который соответствует первому порту
   повторителя. Для ожидания готовности данных для чтения необходимо
   использовать функцию [select(2)][1].

3. Записать сетевой пакет в сокет, соответствующий второму порту повторителя.

4. Вернуться к пункту №2.

[1]: https://www.opennet.ru/man.shtml?topic=select&category=2&russian=0
[2]: https://www.opennet.ru/man.shtml?topic=socket&category=2&russian=0
[3]: https://www.opennet.ru/man.shtml?topic=htons&category=3&russian=0
[4]: https://www.opennet.ru/man.shtml?topic=packet&category=7&russian=0
[5]: https://www.opennet.ru/man.shtml?topic=netdevice&category=7&russian=0
[6]: https://www.opennet.ru/man.shtml?topic=memset&category=3&russian=0
[7]: https://www.opennet.ru/man.shtml?topic=bind&category=2&russian=0
[8]: https://www.opennet.ru/man.shtml?topic=recvfrom&category=2&russian=0
[9]: https://www.opennet.ru/man.shtml?topic=send&category=2&russian=0

## Примеры для базовой задачи

1. Создание сокета `sfd` семейства **AF_PACKET** с типом **SOCK_RAW**.

    ```c
    #include <sys/socket.h>
    #include <linux/if_ether.h>
    #include <arpa/inet.h>

    int sfd;

    sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sfd == -1) {
            /* Handle error here. */
    }
    ```

    Функция [socket(2)][2] служит для создания сокета.  
    Функция [htons(3)][3] служит для конвертации порядка байтов.  
    Подробнее см. man-страницы [packet(7)][4] и [netdevice(7)][5].

2. Привязка сокета `sfd` к интерфейсу с индексом `ifindex`.

    ```c
    #include <sys/socket.h>
    #include <linux/if_packet.h>
    #include <linux/if_ether.h>
    #include <arpa/inet.h>
    #include <string.h>

    int sfd;
    int ifindex;

    struct sockaddr_ll addr;
    int ret;

    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifindex;
    addr.sll_protocol = htons(ETH_P_ALL);

    ret = bind(sfd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
            /* Handle error here. */
    }
    ```

    Функция [memset(3)][6] служит для инициализации области памяти.  
    Функция [bind(2)][7] служит для привязки сокета к интерфейсу.

3. Чтение входящего сетевого пакета в буфер `buffer` из сокета `sfd`.

    ```c
    #include <sys/socket.h>
    #include <linux/if_packet.h>
    #include <string.h>

    #define MAX_PACKET_SIZE 65536

    int sfd;

    unsigned char buffer[MAX_PACKET_SIZE];
    ssize_t recvfrom_size;
    struct sockaddr_ll src_addr;
    socklen_t addrlen;

    addrlen = sizeof(src_addr);
    memset(buffer, 0, MAX_PACKET_SIZE);
    recvfrom_size = recvfrom(sfd,
                             buffer,
                             MAX_PACKET_SIZE,
                             0,
                             (struct sockaddr *) &src_addr,
                             &addrlen);
    if (recvfrom_size == -1) {
            /* Handle error here. */
    }

    if (src_addr.sll_pkttype == PACKET_OUTGOING) {
            /* Packet is outgoing. Skip packet. */
    } else {
            /* Handle packet here. */
    }
    ```

    Функция [recvfrom(2)][8] служит для чтения данных из сокета.

4. Запись сетевого пакета размером `recvfrom_size` из буфера `buffer`
   в сокет `sfd`.

    ```c
    #include <sys/socket.h>

    #define MAX_PACKET_SIZE 65536

    int sfd;

    unsigned char buffer[MAX_PACKET_SIZE];
    ssize_t recvfrom_size;
    ssize_t send_size;

    send_size = send(sfd, buffer, recvfrom_size, 0);
    if (send_size != recvfrom_size) {
            /* Handle error here. */
    }
    ```

    Функция [send(2)][9] служит для записи данных в сокет.

5. Ожидание готовности чтения для файлового дескриптора `fd`.  
   См. Лабораторная работа №6, пример №4.

# Усложненная задача

Разработать трехпортовый Ethernet-коммутатор с изучением MAC-адресов.
Прием и передача кадров осуществляется через сокеты семейства **AF_PACKET**.
MAC-адрес отправителя из каждого принятого кадра используется для модификации
таблицы MAC-адресов. Устаревшие MAC-адреса должны автоматически удаляться из
таблицы по истечении определенного интервала времени **AGING_TIME**.
В качестве данного интервала установить значение в 20 секунд.
Выбор множества выходных портов для передачи кадра осуществляется на основе
значения MAC-адреса получателя в соответствие с текущей таблицей MAC-адресов.

## Рекомендации по выполнению усложненной задачи

Логика работы коммутатора описана в материалах лекции.
Работа с таймерами описана в лабораторной работе №6.
Работа с контейнерами описана в лабораторной работе №7.

## Примеры для усложненной задачи

1. Извлечение MAC-адресов отправителя `src_mac` и получателя `dst_mac`
   из буфера `buffer`.

    ```c
    #include <linux/if_ether.h>

    #define MAX_PACKET_SIZE 65536

    unsigned char buffer[MAX_PACKET_SIZE];
    struct ethhdr *ethhdr;

    unsigned char *src_mac;
    unsigned char *dst_mac;

    ethhdr = (struct ethhdr *) buffer;
    src_mac = ethhdr->h_source;
    dst_mac = ethhdr->h_dest;
    ```

2. Преобразование MAC-адреса `mac` в строку `mac_str`.

    ```c
    #define MAC_STR_SIZE 18

    unsigned char *mac;
    char mac_str[MAC_STR_SIZE];

    if (snprintf(mac_str,
                 MAC_STR_SIZE,
                 "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                 mac[0],
                 mac[1],
                 mac[2],
                 mac[3],
                 mac[4],
                 mac[5]) >= MAC_STR_SIZE) {
            /* Handle error here. */
    }
    ```

# Наблюдение результатов

Для проверки работоспособности Ethernet-повторителя и Ethernet-коммутатора
необходимо соединить медными кабелями порты, находящиеся в разных сетевых
пространствах имен, с портами Ethernet-повторителя или Ethernet-коммутатора и
выполнить команду **ping**, проверяя доступность интерфейса в первом
сетевом пространстве имен из второго сетевого пространства имен.

Предположим, мы реализуем Ethernet-повторитель на портах **ge5** и **ge7**.
Для проверки будем использовать сетевые пространства имен **ns1** и **ns2**.
В пространстве **ns1** находится порт **ge2** c IP-адресом **192.168.0.9**.
В пространстве **ns2** находится порт **ge4** c IP-адресом **192.168.0.11**.

Для проверки работы Ethernet-повторителя необходимо соединить медными кабелями
порт **ge2** с **ge5**, а порт **ge4** с портом **ge7**.
После этого выполнить команды:

```console
$ nsenter --net=/etc/ns/ns2 -- ping 192.168.0.9 -c 1
$ nsenter --net=/etc/ns/ns1 -- ping 192.168.0.11 -c 1
```

Если связь есть, то Ethernet-повторитель работает. Аналогичные шаги необходимо
предпринять для проверки работоспособности Ethernet-коммутатора, однако при
этом необходимо контролировать корректность работы с таблицей MAC-адресов.

Предположим, мы реализуем Ethernet-коммутатор на портах **ge3**, **ge5**
и **ge7**. В этом случае, после проведения вышеописанной проверки для
Ethernet-повторителя необходимо до истечения интервала времени **AGING_TIME**
отсоединить порт **ge2** от **ge5** и присоединить его к порту **ge3**.
Затем выполнить команду:

```console
$ nsenter --net=/etc/ns/ns2 -- ping 192.168.0.9 -c 1
```

До истечения интервала времени **AGING_TIME** связь должна отсутствовать,
а после истечения - восстановиться.

# Сборка приложения под архитектуру x86

Инструкции по сборке приложения под архитектуру x86 находятся по пути
**/path_to_netprog2/build/angtel-projects/x86/build-instructions.md**, где
**path_to_netprog2** - путь к проекту **netprog2** на локальном диске.
