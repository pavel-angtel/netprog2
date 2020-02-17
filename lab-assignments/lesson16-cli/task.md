
# Базовая задача

Разработать сетевой CLI-интерфейс для установки и считывания состояния
сигналов GPIO. Команды и аргументы принимаются через потоковый сокет.
Редактирование командной строки выполняется с помощью библиотеки
**GNU Readline Library**. Состояние сигналов GPIO хранится во внутренних
переменных приложения.

## Описание базовой задачи

См. Лабораторная работа №3.  
См. Лабораторная работа №14.

Конфигурационная СУБД и JSON-документ, содержащий конфигурацию для СУБД,
берутся из лабораторной работы №14.

В CLI-интерфейсе необходимо поддержать следующие команды:

- **read \<gpio>** - прочитать значение сигнала **gpio**;
- **write \<gpio> \<value>** - записать в сигнал **gpio** значение **value**.

Реализация CLI-интерфейса осуществляется с помощью [GNU Readline Library][1].

В базовой задаче сетевой CLI-интерфейс состоит из следующих частей:

1. **CLI-клиент** - приложение, которое запускается на локальном или удаленном
   ПК, подключается к **CLI-серверу** и обеспечивает двунаправленный обмен
   данными между терминальным устройством пользователя и **CLI-сервером**.
   В качестве **CLI-клиента** используется приложение **telnet**.
   Описание протокола **TELNET** см. в [RFC 854][2], [RFC 857][3], [RFC 858][4].
2. **CLI-сервер** - приложение, которое запускается на локальном ПК,
   ожидает подключения от **CLI-клиента**, выполняет обработку команд,
   поступивших от **CLI-клиента** (в т.ч. обрабатываются нажатия специальных
   клавиш, редактирование команд, поиск в истории и т.д.), и обеспечивает
   их выполнение.

В целях упрощения реализации **CLI-сервера** предполагается, что он
одновременно обслуживает не более одного **CLI-клиента**.

В качестве GPIO-сигналов необходимо использовать следующие: **472** - **479**.

## Алгоритм решения базовой задачи

1. Принять следующие макроопределения для CLI-сервера:

    ```c
    #define SOCK_CLI_ADDRESS INADDR_ANY
    #define SOCK_CLI_PORT 50000
    #define LISTEN_BACKLOG 1
    ```

    В данном случае:

    - **SOCK_CLI_ADDRESS** - публичный адрес CLI-сервера, где значение
      **INADDR_ANY** означает, что CLI-сервер будет ожидать подключения
      CLI-клиентов на любом из своих локальных сетевых интерфейсов;
    - **SOCK_CLI_PORT** - номер порта для идентификации CLI-сервера;
    - **LISTEN_BACKLOG** - размер очереди ожидания для CLI-клиентов;

2. Инициализировать внутренние структуры данных CLI-сервера, которые
   содержат конфигурационные и статусные данные для GPIO-сигналов.
   Начальные конфигурационные значения должны быть выбраны таким образом,
   чтобы желтые светодиоды, соответствующие указанному диапазону GPIO-сигналов,
   были выключены.

3. Создать серверный потоковый INET-сокет с помощью функции [socket(2)][5].
   Перевести созданный сокет в слушающий режим с помощью
   функции [listen(2)][6]. Принять запрос на подключение от CLI-клиента
   с помощью функции [accept(2)][7].

4. Настроить параметры соединения с CLI-клиентом, т.е. параметры telnet-сессии.

5. С помощью библиотеки [GNU Readline Library][1] поддержать двунаправленный
   обмен данными между CLI-сервером и CLI-клиентом. В данном обмене CLI-сервер
   должен обеспечивать осуществление ECHO-контроля введенных CLI-клиентом
   данных, а также обработку нажатий специальных клавиш, редактирование команд
   и поиск в истории команд. Результатом данного обмена должна стать строка,
   содержащая данные переданные CLI-клиентом, и представляющая собой команду
   CLI-интерфейса.

6. Обработать команду, переданную CLI-клиентом. В случае, если команда является
   некорректной, то необходимо уведомить об этом CLI-клиента и вернуться
   к пункту №5.

7. Выполнить команду, переданную CLI-клиентом. При необходимости обновить
   конфигурационные и статусные данные для GPIO-сигналов. В случае ошибки
   выполнения команды уведомить об этом CLI-клиента и вернуться к пункту №5.

8. Вернуться к пункту №5.

[1]:  https://tiswww.case.edu/php/chet/readline/readline.html
[2]:  https://tools.ietf.org/html/rfc854
[3]:  https://tools.ietf.org/html/rfc857
[4]:  https://tools.ietf.org/html/rfc858
[5]:  https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2
[6]:  https://www.opennet.ru/man.shtml?topic=listen&category=2&russian=0
[7]:  https://www.opennet.ru/man.shtml?topic=accept&category=2&russian=0
[8]:  https://www.opennet.ru/man.shtml?topic=setsockopt&category=2&russian=0
[9]:  https://www.opennet.ru/man.shtml?topic=htonl&category=3&russian=0
[10]: https://www.opennet.ru/man.shtml?topic=bind&category=2&russian=0
[11]: https://www.opennet.ru/man.shtml?topic=fdopen&category=3&russian=0
[12]: https://www.opennet.ru/man.shtml?topic=fflush&category=3&russian=0
[13]: https://www.opennet.ru/man.shtml?topic=fclose&category=3&russian=0
[14]: https://www.opennet.ru/man.shtml?topic=connect&category=2&russian=0

## Примеры для базовой задачи

См. Лабораторная работа №3.  
См. Лабораторная работа №14.

1. Создание потокового INET-сокета.

    ```c
    #include <sys/socket.h>
    #include <netinet/in.h>

    int sfd;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
            /* Handle error here. */
    }
    ```

2. Установка опции **SO_REUSEADDR** для потокового INET-сокета.

    ```c
    #include <sys/socket.h>

    int sfd;
    int optval;
    int ret;

    optval = 1;
    ret = setsockopt(sfd,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     &optval,
                     sizeof(optval));
    if (ret == -1) {
            /* Handle error here. */
    }
    ```

    Функция [setsockopt(2)][8] служит для установки опций сокета.
    Опция SO_REUSEADDR позволяет осуществлять повторную привязку
    сокета к указанному адресу и порту даже если сокет находится
    в состоянии TIME_WAIT.

3. Заполнение структуры, описывающей адрес и порт для потокового INET-сокета.

    ```c
    #include <string.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    #define SOCK_CLI_ADDRESS INADDR_ANY
    #define SOCK_CLI_PORT 50000

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(SOCK_CLI_ADDRESS);
    addr.sin_port = htons(SOCK_CLI_PORT);
    ```

    Функции [htonl(3)][9] и [htons(3)][9] служат для преобразования порядка
    следования байтов, используемого на локальном ПК, в стандартный
    сетевой порядок следования байтов.

4. Привязка потокового INET-сокета к публичным адресу и порту.

    ```c
    #include <sys/socket.h>
    #include <netinet/in.h>

    int sfd;
    struct sockaddr_in addr;
    int ret;

    ret = bind(sfd, (const struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
            /* Handle error here. */
    }
    ```

    Функция [bind(2)][10] используется для привязки сокета
    к публичным адресу и порту.

5. Перевод потокового INET-сокета из передающего в слушающий режим.

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

    Функция [listen(2)][6] служит для перевода сокета в слушающий режим.

6. Настройка параметров **telnet**-сессии.

    ```c
    #include <sys/socket.h>

    #define TELNET_CONFIGURATION_SIZE 6

    static int send_tc(int sfd)
    {
            unsigned char request[TELNET_CONFIGURATION_SIZE];
            ssize_t send_size;

            /* IAC WILL ECHO */
            request[0] = 255;
            request[1] = 251;
            request[2] = 1;

            /* IAC WILL SUPPRESS_GO_AHEAD */
            request[3] = 255;
            request[4] = 251;
            request[5] = 3;

            send_size = send(sfd,
                             request,
                             TELNET_CONFIGURATION_SIZE,
                             MSG_NOSIGNAL);
            if (send_size == -1) {
                    if (errno == EPIPE) {
                            /* CLI-client closed the connection. */
                    }
                    /* Handle error here. */
            }
            if (send_size != TELNET_CONFIGURATION_SIZE) {
                    /* Partial write to socket. */
            }

            return 0;
    }

    static int recv_tc(int sfd)
    {
            unsigned char response[TELNET_CONFIGURATION_SIZE];
            unsigned char expected_response[TELNET_CONFIGURATION_SIZE];
            ssize_t recv_size;
            int i;

            /* IAC DO ECHO */
            expected_response[0] = 255;
            expected_response[1] = 253;
            expected_response[2] = 1;

            /* IAC DO SUPPRESS_GO_AHEAD */
            expected_response[3] = 255;
            expected_response[4] = 253;
            expected_response[5] = 3;

            recv_size = recv(sfd,
                             response,
                             TELNET_CONFIGURATION_SIZE,
                             0);
            if (recv_size == -1) {
                    /* Handle error here. */
            }
            if (recv_size == 0) {
                    /* CLI-client closed the connection. */
            }
            if (recv_size != TELNET_CONFIGURATION_SIZE) {
                    /* Partial read from socket. */
            }

            client->tc_rx_count += recv_size;

            for (i = 0; i < TELNET_CONFIGURATION_SIZE; i++) {
                    if (response[i] != expected_response[i]) {
                            /* CLI-client is not configured. */
                    }
            }

            return 0;
    }
    ```

7. Получение указателя на поток ввода/вывода `fp`
   из файлового дескриптора `cfd`.

    ```c
    #include <stdio.h>

    int cfd;
    FILE *fp;

    fp = fdopen(cfd, "r+");
    if (fp == NULL) {
            /* Handle error here. */
    }
    ```

    Функция [fdopen(3)][11] служит для открытия потока ввода/вывода.

8. Принудительная запись пользовательского буфера потока ввода/вывода `fp`.

    ```c
    #include <stdio.h>

    FILE *fp;

    fflush(fp);
    ```

    Подробнее см. [fflush(3)][12].

9. Закрытие потока ввода/вывода `fp`.

    ```c
    #include <stdio.h>

    FILE *fp;

    fclose(fp);
    ```

    Подробнее см. [fclose(3)][13].

10. Установка клиентского потокового INET-сокета `cfd`
    в качестве потока ввода/вывода для GNU Readline Library.

    ```c
    #include <stdio.h>
    #include <readline/readline.h>

    int cfd;
    FILE *in_out_stream;

    in_out_stream = fdopen(cfd, "r+");

    rl_instream = in_out_stream;
    rl_outstream = in_out_stream;
    ```

11. Возврат каретки и переход на новую строку.

    ```c
    #include <stdio.h>

    FILE *in_out_stream;

    fprintf(in_out_stream, "\r");
    fflush(in_out_stream);
    ```

12. Отключение автодополнения по нажатию клавиши **TAB**
    для GNU Readline Library.

    ```c
    #include <readline/readline.h>

    rl_bind_key('\t', rl_insert);
    ```

13. Добавление строки `line` в историю команд для GNU Readline Library.

    ```c
    #include <readline/readline.h>
    #include <readline/history.h>

    char *line;

    add_history(line);
    ```

14. Регистрация обработчика строк для GNU Readline Library.

    ```c
    #include <readline/readline.h>
    #include <readline/history.h>
    #include <sys/select.h>
    #include <stdbool.h>
    #include <libubox/avl.h>
    #include <libubox/avl-cmp.h>

    int rl_global_var;

    static void cb_line_handler(char *line)
    {
            if (line == NULL || strcmp(line, "exit") == 0) {
                        if (line == NULL) {
                                fprintf(rl_outstream, "\r\n");
                                fflush(rl_outstream);
                        } else {
                                fprintf(rl_outstream, "\r");
                                fflush(rl_outstream);
                                free(line);
                        }
                        rl_clear_history();
                        rl_callback_handler_remove();
                } else {
                        if (*line != '\0')
                                add_history(line);
                        fprintf(rl_outstream, "\r");
                        fflush(rl_outstream);

                        /* Handle command here (line argument).
                           You may use global variables if you need. */

                        free(line);
            }
    }

    int main(int argc, char *argv[])
    {
            /* Listening socket. */
            int sfd;

            /* Client socket. */
            int cfd;

            AVL_TREE(gpios_dict, avl_strcmp, false, NULL);

            rl_global_var = &gpios_dict;

            while (true) {
                    fd_set readfds;
                    int nfds;

                    /* Init and use select() here. */

                    if (FD_ISSET(sfd, &readfds)) {
                            int in_out_stream;
                            const char *prompt;

                            /* Accept connection here. */

                            in_out_stream = fdopen(cfd, "r+");
                            if (in_out_stream == NULL) {
                                    /* Handle error here. */
                            }
                            rl_instream = in_out_stream;
                            rl_outstream = in_out_stream;

                            prompt = "command$ ";

                            rl_callback_handler_install(prompt, cb_line_handler);

                            rl_bind_key('\t', rl_insert);
                    }

                    if (FD_ISSET(cfd, &readfds)) {

                            /* ... */

                            rl_callback_read_char();
                    }
            }
    }
    ```

    Функция `rl_callback_read_char()` будет вызываться каждый раз при получении
    нового символа данных от CLI-клиента. Функция `cb_line_handler()`
    будет вызываться каждый раз при получении полной строки данных от
    CLI-клиента.

15. Отмена регистрации обработчика строк и
    очистка внутреннего состояния для GNU Readline Library.

    ```c
    #include <readline/readline.h>
    #include <readline/history.h>

    rl_clear_history();
    rl_callback_handler_remove();
    ```

# Усложненная задача

В дополнение к базовой задаче реализовать обмен с СУБД
для записи и чтения состояния сигналов GPIO через JSON-RPC.

## Описание усложненной задачи

См. Лабораторная работа №3.  
См. Лабораторная работа №14.

В усложненной задаче сетевой CLI-интерфейс состоит из следующих частей:

1. **CLI-клиент**.
2. **CLI-сервер**.
3. **СУБД** - приложение, которое запускается на локальном ПК,
   ожидает подключения от **CLI-сервера**, выполняет обработку команд
   на чтение/запись конфигурационных и/или статусных данных и обеспечивает
   рассылку уведомлений, подписавшемуся на получение таких уведомлений
   **CLI-серверу**, в случае изменения конфигурационных и/или статусных данных.
4. **ППН** - приложение, которое запускается на локальном ПК, применяет
   настройки GPIO-сигналов, хранящиеся в виде конфигурационных данных в СУБД,
   и записывает значения GPIO-сигналов после применения настроек в виде
   статусных данных обратно в СУБД.

Общая схема взаимодействия компонентов
CLI-интерфейса выглядит следующим образом:

1. CLI-клиент посылает команду CLI-серверу.
2. CLI-сервер обрабатывает команду и в зависимости от типа команды либо
   записывает конфигурационные данные в СУБД (команда write),
   либо читает статусные данные (команда read).
3. ППН подписывается на получение уведомлений от СУБД при изменении
   конфигурационных данных GPIO-сигналов.
4. При получении уведомления от СУБД ППН применяет текущую конфигурацию,
   хранящуюся в СУБД к GPIO-сигналам через
   */sys/class/gpio/gpio\<number>/value*, записывая значение
   GPIO-сигналов после применения текущей конфигурации
   в виде статусных данных обратно в СУБД.

## Алгоритм решения усложненной задачи

CLI-сервер:

1. В дополнение к макроопределениям базовой задачи
   принять следующее макроопределение для публичного адреса СУБД:

    ```c
    #define SOCK_CDB_PATH "/tmp/sock_seqpacket_cdb"
    ```

2. Создать серверный потоковый UNIX-сокет типа SOCK_SEQPACKET
   с помощью функции [socket(2)][5]. Подключиться к СУБД с использованием
   публичного адреса СУБД с помощью функции [connect(2)][14].

3. Создать серверный потоковый INET-сокет с помощью функции [socket(2)][5].
   Перевести созданный сокет в слушающий режим с помощью
   функции [listen(2)][6]. Принять запрос на подключение от CLI-клиента
   с помощью функции [accept(2)][7].

4. Настроить параметры соединения с CLI-клиентом, т.е. параметры telnet-сессии.

5. С помощью библиотеки [GNU Readline Library][1] поддержать двунаправленный
   обмен данными между CLI-сервером и CLI-клиентом. В данном обмене CLI-сервер
   должен обеспечивать осуществление ECHO-контроля введенных CLI-клиентом
   данных, а также обработку нажатий специальных клавиш, редактирование команд
   и поиск в истории команд. Результатом данного обмена должна стать строка,
   содержащая данные переданные CLI-клиентом, и представляющая собой команду
   CLI-интерфейса.

6. Обработать команду, переданную CLI-клиентом. В случае если поступила команда
   **write**, то CLI-сервер должен записать в СУБД в виде конфигурационных
   данных для GPIO-сигнала **gpio** значение **value**. В случае если поступила
   команда **read**, то CLI-сервер должен прочитать из СУБД в виде статусных
   данных значение для GPIO-сигнала **gpio**. Если поступившая команда является
   некорректной, то CLI-сервер должен уведомить об этом CLI-клиента и вернуться
   к пункту №5.

7. Вернуться к пункту №5.

ППН:

1. Принять следующее макроопределение для публичного адреса СУБД:

    ```c
    #define SOCK_CDB_PATH "/tmp/sock_seqpacket_cdb"
    ```

2. Создать серверный потоковый UNIX-сокет типа SOCK_SEQPACKET
   с помощью функции [socket(2)][5]. Подключиться к СУБД с использованием
   публичного адреса СУБД с помощью функции [connect(2)][14].

3. Прочитать из СУБД конфигурационные данные для GPIO-сигналов, применить
   конфигурацию и обновить статусные данные для GPIO-сигналов в СУБД.

4. Подписаться на получение уведомлений от СУБД в случае изменения
   конфигурационных данных для GPIO-сигналов.

5. Перейти в состояние ожидания получения уведомлений.

6. При получении уведомления применить конфигурацию, содержащуюся в уведомлении
   и обновить статусные данные для GPIO-сигналов в СУБД.

7. Вернуться к пункту №5.

# Наблюдение результатов

Базовая задача:

1. Запустить CLI-сервер на устройстве Корунд-2О-8Е:

    ```console
    $ ./lab_16_bt_cli
    ```

2. Запустить CLI-клиент на локальном ПК:

    ```console
    $ telnet 192.168.0.8 50000
    Trying 192.168.0.8...
    Connected to 192.168.0.8.
    Escape character is '^]'.
    command$
    ```

3. Ввести команды через CLI-клиент на локальном ПК:

    ```console
    command$ read 472
    1
    command$ write 472 0 /* Желтый светодиод должен загореться. */
    command$ read 472
    0
    command$ write 472 1 /* Желтый светодиод должен погаснуть. */
    command$ read 472
    1
    ```

Усложненная задача:

1. Запустить СУБД на устройстве Корунд-2О-8Е:

    ```console
    $ ./lab_14_cdb -t -f ./configuration.json
    ```

2. Запустить ППН на устройстве Корунд-2О-8Е:

    ```console
    $ ./lab_16_at_subscriber
    ```

3. Запустить CLI-сервер на устройстве Корунд-2О-8Е:

    ```console
    $ ./lab_16_at_cli
    ```

4. Запустить CLI-клиент на локальном ПК:

    ```console
    $ telnet 192.168.0.8 50000
    Trying 192.168.0.8...
    Connected to 192.168.0.8.
    Escape character is '^]'.
    command$
    ```

5. Ввести команды через CLI-клиент на локальном ПК:

    ```console
    command$ read 472
    1
    command$ write 472 0 /* Желтый светодиод должен загореться. */
    command$ read 472
    0
    command$ write 472 1 /* Желтый светодиод должен погаснуть. */
    command$ read 472
    1
    ```
