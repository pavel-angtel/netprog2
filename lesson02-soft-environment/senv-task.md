
# Базовая задача

Разработать приложение для вывода статуса сетевого интерфейса.
Множество запрашиваемых статусных параметров передается через аргументы
командной строки. Значения параметров читаются из атрибутов сетевого
интерфейса в виртуальной файловой системе sysfs.

## Описание базовой задачи

Необходимо использовать следующий набор статусных параметров:

- **address**
- **mtu**
- **operstate**

Для доступа к статусным параметрам используются пути вида:
*/sys/class/net/**имя_интерфейса**/**имя_статусного_параметра***.
Например, при выборе сетевого интерфейса с именем **ge1** пути для
вышеуказанных статусных параметров примут вид:

- */sys/class/net/ge1/address*
- */sys/class/net/ge1/mtu*
- */sys/class/net/ge1/operstate*

## Алгоритм решения базовой задачи

1. Осуществить анализ аргументов командной строки с помощью [getopt(3)][1].
   В качестве аргументов командной строки использовать следующие:

   - **-a** для аргумента **address**
   - **-m** для аргумента **mtu**
   - **-o** для аргумента **operstate**
   - **-i** для аргумента **имя_интерфейса**

   Все аргументы являются опциональными. Аргументы **-a**, **-m** и **-o**
   могут отсутствовать. В случае отсутствия аргумента **-i** необходимо
   записать соответствующее сообщение об ошибке в стандартный поток ошибки
   (stderr) и завершить программу с кодом возврата **EXIT_FAILURE**.

2. После анализа аргументов командной строки прочитать значения соответствующих
   статусных параметров для соответствующего сетевого интерфейса.
   Для работы с файлами необходимо использовать или потоки ввода/вывода
   (функции [fopen(3)][2], [fread(3)][3] и [fclose(3)][4]) или файловые
   дескрипторы (функции [open(2)][5], [read(2)][6] и [close(2)][7]).
   Стоит отметить, что для использования прочитанных из соответствующих файлов
   данных в качестве C-строк необходимо добавить к этим данным завершающий
   нулевой байт.

3. Отобразить полученные значения статусных параметров в эмуляторе терминала.

[1]: http://man7.org/linux/man-pages/man3/getopt.3.html

[2]: http://man7.org/linux/man-pages/man3/fopen.3.html
[3]: http://man7.org/linux/man-pages/man3/fread.3.html
[4]: http://man7.org/linux/man-pages/man3/fclose.3.html

[5]: http://man7.org/linux/man-pages/man2/open.2.html
[6]: http://man7.org/linux/man-pages/man2/read.2.html
[7]: http://man7.org/linux/man-pages/man2/close.2.html

## Примеры для базовой задачи

1. Анализ аргументов командной строки.

    ```c
    #include <getopt.h>

    /* Short options. */
    const char *optstring = "ha::i:";

    int opt;

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'a':
                if (optarg) {
                        /* Option with value. */
                } else {
                        /* Option without value. */
                }
                break;
        case 'i':
                /* Option with value. */
                break;
        case 'h':
                /* Print help here with EXIT_SUCCESS. */
        case '?':
        case ':':
        default:
                /* Print help here with EXIT_FAILURE. */
        }
    }

    if (optind < argc) {
            /* Handling of positional arguments. */
    }
    ```

2. Генерация путевого имени.

    ```c
    #include <stdio.h>

    #define MAX_PATH_SIZE 256

    char path[MAX_PATH_SIZE];

    /* Terminating null byte is added automatically. */
    if (snprintf(path,
                 MAX_PATH_SIZE,
                 "/sys/class/net/%s/%s",
                 interface_name,
                 interface_parameter_name) >= MAX_PATH_SIZE) {
            /* Output was truncated. Handle error here. */
    }
    ```

3. Открытие файла для получения ассоциированного потока.

    ```c
    #include <stdio.h>

    FILE *fp;

    fp = fopen(path, "r");
    if (fp == NULL) {
            /* Handle error here. */
    }
    ```

4. Открытие файла для получения ассоциированного файлового дескриптора.

    ```c
    #include <fcntl.h>

    int fd;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
            /* Handle error here. */
    }
    ```

5. Чтение содержимого файла с помощью потока.

    ```c
    #include <stdio.h>

    #define MAX_BUFFER_SIZE 256

    char buffer[MAX_BUFFER_SIZE + 1];
    size_t nr;

    /* Terminating null byte is not added automatically. */
    nr = fread(buffer, 1, MAX_BUFFER_SIZE, fp);
    if (ferror(fp)) {
            /* Handle error here. */
    }
    buffer[nr] = '\0';
    ```

6. Чтение содержимого файла с помощью файлового дескриптора.

    ```c
    #include <unistd.h>

    #define MAX_BUFFER_SIZE 256

    char buffer[MAX_BUFFER_SIZE + 1];
    ssize_t nr;

    /* Terminating null byte is not added automatically. */
    nr = read(fd, buffer, MAX_BUFFER_SIZE);
    if (nr == -1) {
            /* Handle error here. */
    }
    buffer[nr] = '\0';
    ```

7. Запись в файл с помощью потока.

    ```c
    #include <stdio.h>

    size_t nw;

    nw = fwrite(buffer, 1, number_of_bytes, fp);
    if (nw != number_of_bytes) {
            /* Handle error here. */
    }
    ```

8. Запись в файл с помощью файлового дескриптора.

    ```c
    #include <unistd.h>

    ssize_t nw;

    nw = write(fd, buffer, number_of_bytes);
    if (nw != number_of_bytes) {
            /* Handle error here. */
    }
    ```

9. Закрытие потока, ассоциированного с открытым файлом.

    ```c
    #include <stdio.h>

    fclose(fp);
    ```

10. Закрытие файлового дескриптора, ассоциированного с открытым файлом.

    ```c
    #include <unistd.h>

    close(fd);
    ```

11. Завершение программы.

    ```c
    #include <stdlib.h>

    /* Program succeeds. */
    exit(EXIT_SUCCESS);

    /* Program fails. */
    exit(EXIT_FAILURE);
    ```

# Усложненная задача

В дополнение к базовой задаче реализовать конфигурирование сетевого интерфейса.
Наборы параметров конфигурации и статуса совпадают. Задание конфигурации
осуществляется с помощью пакета **iproute2**.

## Алгоритм решения усложненной задачи

1. Реализовать функциональность базовой задачи.

2. Для конфигурирования сетевого интерфейса, например **ge1**, необходимо
   использовать следующие команды:

   - *ip link set ge1 address **MAC***
   - *ip link set ge1 mtu **MTU***
   - *ip link set ge1 **OPERSTATE***

   Пример:

   - *ip link set ge1 address 68:EB:C5:00:00:1A*
   - *ip link set ge1 mtu 500*
   - *ip link set ge1 down*

   Для выполнения соответствующих команд из из приложения требуется
   использовать функцию [system(3)][8].

[8]: http://man7.org/linux/man-pages/man3/system.3.html

## Примеры для усложненной задачи

1. Выполнение shell-команды.

    ```c
    #include <stdlib.h>

    if (system(command) != 0) {
            /* Handle error here. */
    }
    ```

## Наблюдение результатов

Для конфигурирования сетевых интерфейсов рекомендуется воспользоваться
последовательным портом платы (RS-232) и программой **Wireshark**.
Также потребуется использование программы [ping(8)][9]. Конфигурирование
осуществлять для интерфейса **br0**.

[9]: http://man7.org/linux/man-pages/man8/ping.8.html

1. Изменение MAC-адреса сетевого интерфейса.
   - Запустите программу **Wireshark**.
   - Запустите на плате программу **ping**.
     Команда: *ping 192.168.0.6 -c 5*.
   - В программе **Wireshark** для ICMP-пакета, полученного от платы изучите
     раздел **Ethernet II** поле **Source**. В нем содержится MAC-адрес.
   - Измените MAC-адрес и повторите процедуру.

2. Изменение MTU сетевого интерфейса.
   - Запустите программу **Wireshark**.
   - Запустите на плате программу **ping**.
     Команда: *ping 192.168.0.6 -s **MTU** -c 5*, где **MTU** - желаемый размер
     полезной нагрузки ICMP-пакета в байтах.
   - Если размер полезной нагрузки ICMP-пакета окажется больше размера MTU,
     заданного для соответствующего интерфейса, то в программе **Wireshark**
     можно будет наблюдать фрагментацию ICMP-пакетов (Fragmented IP protocol).

3. Изменение рабочего состояния сетевого интерфейса.
   - Запустите на плате программу **ping**. Команда: *ping 192.168.0.6*.
   - Измените рабочего состояния на **down**. Компьютер должен перестать
     получать ICMP-пакеты. Зеленый светодиод физического интерфейса должен
     погаснуть.
   - Измените рабочего состояния на **up**. Компьютер должен начать
     получать ICMP-пакеты. Зеленый светодиод физического интерфейса должен
     загореться.
   - Остановите программу **ping**, нажав комбинацию клавиш **Ctrl+C**.
