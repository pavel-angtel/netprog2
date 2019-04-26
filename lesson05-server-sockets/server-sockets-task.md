
# Базовая задача

Разработать сервер для приема текстовых сообщений через пакетный UNIX-сокет.
Каждое сообщение содержит одну строку текста. Принятые сообщения необходимо
отправлять обратно клиенту и выводить в стандартный поток вывода эмулятора
терминала.

## Описание базовой задачи

Для решения поставленной задачи предоставляется готовый клиент. Данный клиент
отправляет сообщения через пакетный UNIX-сокет на сервер и осуществляет
проверку корректности ответов от сервера. Пример запуска клиента:

```console
$ ./lab_05_client -d
```

## Алгоритм решения базовой задачи

1. Принять следующее макроопределение для публичного адреса сервера:

    ```c
    #define D_ADDRESS "/tmp/sock_dgram_server"
    ```

2. Создать серверный пакетный UNIX-сокет с помощью функции [socket(2)][1].

3. Привязать серверный пакетный UNIX-сокет к публичному адресу, т.е. к
   **D_ADDRESS**. Для этого необходимо воспользоваться функцией [bind(2)][2].
   Стоит отметить, что для  привязки UNIX-сокета к публичному адресу необходимо
   чтобы файл, соответствующий данному адресу, отсутствовал в файловой системе.
   Для удаления файла можно воспользоваться функцией [remove(3)][3].

4. Получить сообщение от клиента, воспользовавшись функцией [recvfrom(2)][4].
   Сохранить адрес клиента. Отобразить сообщение в эмуляторе терминала.

5. Отправить полученное от клиента сообщение обратно клиенту. Для этого
   требуется использовать функцию [sendto(2)][5].

6. Вернуться к пункту №4.

[1]:  https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2
[2]:  https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=bind&category=2
[3]:  https://www.opennet.ru/man.shtml?topic=remove&category=3&russian=0
[4]:  https://www.opennet.ru/man.shtml?topic=recvfrom&category=2&russian=0
[5]:  https://www.opennet.ru/man.shtml?topic=sendto&category=2&russian=0
[6]:  https://www.opennet.ru/man.shtml?topic=memset&category=3&russian=0
[7]:  https://www.opennet.ru/man.shtml?topic=snprintf&category=3&russian=0
[8]:  https://www.opennet.ru/man.shtml?topic=read&category=2&russian=0
[9]:  https://www.opennet.ru/man.shtml?topic=recv&category=2&russian=0
[10]: https://www.opennet.ru/man.shtml?topic=listen&category=2&russian=0
[11]: https://www.opennet.ru/man.shtml?topic=accept&category=2&russian=0
[12]: https://www.opennet.ru/man.shtml?topic=fflush&category=3&russian=0
[13]: https://www.opennet.ru/man.shtml?topic=printf&category=3&russian=0

## Примеры для базовой задачи

1. Удаление файла, соответствующего публичному адресу сервера.

    ```c
    #include <stdio.h>
    #include <errno.h>

    const char *socket_path = D_ADDRESS;
    int ret;

    ret = remove(socket_path);
    if (ret == -1 && errno != ENOENT) {
            /* Handle error here. */
    }
    ```

2. Привязка UNIX-сокета к публичному адресу.

    ```c
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <string.h>
    #include <stdio.h>

    int sfd;
    struct sockaddr_un addr;
    const char *socket_path = D_ADDRESS;
    int ret;

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", socket_path);

    ret = bind(sfd, (const struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
            /* Handle error here. */
    }
    ```

    Функция [memset(3)][6] служит для инициализации области памяти.  
    Функция [snprintf(3)][7] служит для создания форматированной строки.

3. Получение сообщения от клиента с сохранением адреса.

    ```c
    #include <sys/socket.h>

    #define READ_SIZE 256

    struct sockaddr_un client_address;
    socklen_t client_address_length;
    unsigned char buffer[READ_SIZE];
    ssize_t recvfrom_size;

    client_address_length = sizeof(struct sockaddr_un);
    recvfrom_size = recvfrom(sfd,
                             buffer,
                             READ_SIZE,
                             0,
                             (struct sockaddr *) &client_address,
                             &client_address_length);
    if (recvfrom_size == -1) {
            /* Handle error here. */
    }
    ```

4. Отправка сообщения клиенту.  
   См. Лабораторная работа №4, пример №5.

# Усложненная задача

Разработать сервер для приема текстовых символов через потоковый UNIX-сокет.
Сервер должен поддерживать повторную установку соединения после разрыва со
стороны клиента. Принятые символы необходимо выводить в стандартный поток
вывода эмулятора терминала.

## Рекомендации по выполнению усложненной задачи

В качестве публичного адреса сервера использовать следующее макроопределение:

```c
#define S_ADDRESS "/tmp/sock_stream_server"
```

Разрыв соединения с клиентом детектируется во время чтения данных от клиента.
Когда функции [read(2)][8], [recv(2)][9] или [recvfrom(2)][4] возвращают `0`
(при условии, что количество запрошенных для чтения байт данных больше нуля)
это означает, что клиент разорвал соединение.

## Примеры для усложненной задачи

1. Перевод UNIX-сокета из передающего в слушающий.

    ```c
    #include <sys/socket.h>

    #define LISTEN_BACKLOG 1

    int sfd;
    int ret;

    ret = listen(sfd, LISTEN_BACKLOG);
    if (ret == -1) {
            /* Handle error here. */
    }
    ```

    Функция [listen(2)][10] служит для перевода UNIX-сокета в слушающий режим.

2. Прием соединения от клиента.

    ```c
    #include <sys/socket.h>

    int sfd;
    int cfd;

    cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) {
            /* Handle error here. */
    }
    ```

    Функция [accept(2)][11] служит для приема соединений на слушающем
    UNIX-сокете.

3. Детектирование разрыва соединения с клиентом.

    ```c
    #include <sys/socket.h>

    #define READ_SIZE 256

    int sfd;
    unsigned char buffer[READ_SIZE];
    ssize_t recv_size;

    recv_size = recv(sfd, buffer, READ_SIZE, 0);
    if (recv_size == -1) {
            /* Handle error here. */
    }
    if (recv_size == 0) {
            /* Connection closed by client. */
    }
    ```

4. Принудительная запись пользовательского буфера потока вывода с помощью
   функции [fflush(3)][12] при использовании функций семейства [printf(3)][13].

    ```c
    #include <stdio.h>

    fflush(stdout);
    ```

# Наблюдение результатов

Для наблюдения работы серверного приложения необходимо запустить два эмулятора
терминала. В первом эмуляторе терминала запустить серверное приложение.
Во втором - клиентское приложение.

Запуск клиента (пакетный UNIX-сокет):

```console
$ ./lab_05_client -d
```

Запуск клиента (потоковый UNIX-сокет):

```console
$ ./lab_05_client -s
```

После этого необходимо передать данные в стандартный поток ввода клиента и
наблюдать результат получения данных на сервере. Стоит отметить, что при
вводе данных через эмулятор терминала, данные в стандартном потоке ввода
клиента появятся только после нажатия клавиши **Enter**.
