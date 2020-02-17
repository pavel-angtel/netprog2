
# Базовая задача

Разработать клиентское приложение для конфигурационной СУБД.
Приложение читает настройки сетевых интерфейсов по протоколу JSON-RPC
и выводит их в стандартный поток вывода эмулятора терминала.

## Описание базовой задачи

Ознакомиться с форматом JSON-RPC запросов и ответов можно по ссылке
[JSON-RPC 2.0 Specification][1].
Ознакомиться с синтаксисом JSONPath можно по ссылке [JSONPath Syntax][2].
Для работы с JSON-элементами используется библиотека [Jansson][3].

В качестве исходного JSON-документа, содержащего конфигурацию для СУБД,
предлагается использовать файл *./configuration.json*,
который находится в директории с текущим заданием.
Исполняемые файлы с конфигурационной СУБД *./lab_14_cdb_mips* или
*./lab_14_cdb_x86* также находятся в директории с текущим заданием.

Конфигурационная СУБД поддерживает следующие выражения:

- **$** - доступ к корневому элементу документа JSON;
- **[** \<index\> **]** - доступ к элементу JSON-массива
  с индексом \<index\>;
- **[** '\<property\>' **]** - доступ к элементу JSON-объекта
  со свойством \<property\>;

Конфигурационная СУБД поддерживает следующие методы:

- **get** - получение значения JSON-элемента;
- **set** - установка значения JSON-элемента;
- **delete** - удаление JSON-элемента (JSON-элемент должен существовать);
- **remove** - удаление JSON-элемента (JSON-элемент может не существовать);
- **subscribe** - подписка на уведомления об изменении JSON-элемента;
- **unsubscribe** - отмена подписки на уведомления об изменении JSON-элемента;

Конфигурационная СУБД поддерживает работу с группами запросов и ответов.

При нажатии клавиши **Enter** СУБД отображает текущую конфигурацию.

Конфигурация и статусные данные, хранящиеся в СУБД имеют вид:

```json
{
    "interfaces_configuration": {
        "ge1": {
            "address": "68:eb:c5:00:02:01",
            "mtu": 1010,
            "link": "up"
        },
        "ge3": {
            "address": "68:eb:c5:00:02:03",
            "mtu": 1030,
            "link": "down"
        },
        "another_object": {}
    },
    "interfaces_status": {
        "ge1": {
            "address": "68:eb:c5:00:02:01",
            "mtu": 1010,
            "link": "up"
        },
        "ge3": {
            "address": null,
            "mtu": null,
            "link": null
        },
        "another_object": {}
    }
}
```

Примеры запросов к конфигурационной СУБД и ответов от конфигурационной СУБД:

1. Формат запроса на получение значения JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "method": "get",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']['address']"
        },
        "id": "sr_get"
    }
    ```

   Путь к JSON-элементу указывается с помощью свойства **"jsonpath"**.

2. Формат ответа на запрос о получении значения JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "result": "68:eb:c5:00:02:01",
        "id": "sr_get"
    }
    ```

   Значение JSON-элемента указывается с помощью свойства **"result"**.

3. Формат группового запроса на получение значений JSON-элементов:

    ```json
    [
        {
            "jsonrpc": "2.0",
            "method": "get",
            "params": {
                "jsonpath": "$['interfaces_configuration']['ge1']['address']"
            },
            "id": "br_get_address"
        },
        {
            "jsonrpc": "2.0",
            "method": "get",
            "params": {
                "jsonpath": "$['interfaces_configuration']['ge1']['mtu']"
            },
            "id": "br_get_mtu"
        },
        {
            "jsonrpc": "2.0",
            "method": "get",
            "params": {
                "jsonpath": "$['interfaces_configuration']['ge1']['link']"
            },
            "id": "br_get_link"
        }
    ]
    ```

4. Формат группового ответа на запрос о получении значений JSON-элементов:

    ```json
    [
        {
            "jsonrpc": "2.0",
            "result": "68:eb:c5:00:02:01",
            "id": "br_get_address"
        },
        {
            "jsonrpc": "2.0",
            "result": 1010,
            "id": "br_get_mtu"
        },
        {
            "jsonrpc": "2.0",
            "result": "up",
            "id": "br_get_link"
        }
    ]
    ```

5. Формат запроса на установку значения JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "method": "set",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']['address']",
            "value": "68:eb:c5:00:03:01"
        },
        "id": "sr_set"
    }
    ```

    Новое значение JSON-элемента указывается с помощью свойства **"value"**.

6. Формат ответа на запрос об установке значения JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "result": null,
        "id": "sr_set"
    }
    ```

7. Формат запроса на удаление JSON-элемента (элемент должен существовать):

    ```json
    {
        "jsonrpc": "2.0",
        "method": "delete",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']['address']"
        },
        "id": "sr_delete"
    }
    ```

8. Формат запроса на удаление JSON-элемента (элемент может не существовать):

    ```json
    {
        "jsonrpc": "2.0",
        "method": "remove",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']['address']"
        },
        "id": "sr_remove"
    }
    ```

9. Формат ответа на запрос об удалении JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "result": null,
        "id": "sr_delete"
    }
    ```

    или

    ```json
    {
        "jsonrpc": "2.0",
        "result": null,
        "id": "sr_remove"
    }
    ```

10. Формат запроса на подписку
    (получение уведомлений об изменении JSON-элемента):

    ```json
    {
        "jsonrpc": "2.0",
        "method": "subscribe",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']"
        },
        "id": "sr_subscribe"
    }
    ```

11. Формат ответа на запрос о подписке
    (получение уведомлений об изменении JSON-элемента):

    ```json
    {
        "jsonrpc": "2.0",
        "result": null,
        "id": "sr_subscribe"
    }
    ```

12. Формат уведомления об изменении JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "result": {
            "jsonpath": "$['interfaces_configuration']['ge1']",
            "value": {
                "address": "68:eb:c5:00:02:01",
                "mtu": 1400,
                "link": "up"
            }
        }
    }
    ```

    Новое значение JSON-элемента указывается с помощью свойства **"value"**.

13. Формат запроса на отмену подписки
    (получение уведомлений об изменении JSON-элемента):

    ```json
    {
        "jsonrpc": "2.0",
        "method": "unsubscribe",
        "params": {
            "jsonpath": "$['interfaces_configuration']['ge1']"
        },
        "id": "sr_unsubscribe"
    }
    ```

14. Формат ответа на запрос об отмене подписки
    (получение уведомлений об изменении JSON-элемента):

    ```json
    {
        "jsonrpc": "2.0",
        "result": null,
        "id": "sr_unsubscribe"
    }
    ```

15. Формат ответа на некорректный запрос:

    ```json
    {
        "jsonrpc": "2.0",
        "error": {
            "code": -300,
            "message": "Invalid JSONPath",
            "data": "?['interfaces_configuration']['ge1']['address']"
        },
        "id": "sr_get_invalid_5"
    }
    ```

    Информация об ошибке указывается с помощью свойства **"error"**.

## Алгоритм решения базовой задачи

1. Принять следующее макроопределение для публичного адреса СУБД:

    ```c
    #define SOCK_DATABASE_PATH "/tmp/sock_seqpacket_cdb"
    ```

2. Отправить конфигурационной СУБД запрос на получение конфигурации
   всех сетевых интерфейсов.

3. Вывести ответ от СУБД в стандартный поток вывода эмулятора терминала.

4. Завершить программу.

[1]: https://www.jsonrpc.org/specification
[2]: https://support.smartbear.com/alertsite/docs/monitors/api/endpoint/jsonpath.html
[3]: https://jansson.readthedocs.io/en/2.12/

## Примеры для базовой задачи

1. Создание сокета `sfd` семейства **AF_UNIX** с типом **SOCK_SEQPACKET**.

    ```c
    #include <sys/socket.h>

    int sfd;

    sfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (sfd == -1) {
            /* Handle error here. */
    }
    ```

   Поведение сокета с типом **SOCK_SEQPACKET** аналогично поведению
   потокового сокета **SOCK_STREAM** с чтением и записью данных как в
   пакетном сокете **SOCK_DGRAM**, т.е. виде дейтаграмм.

2. Сериализация JSON-элемента `json_object` библиотеки Jansson.

    ```c
    #include <jansson.h>
    #include <string.h>

    json_t *json_object;
    char *buffer;
    size_t size;

    buffer = json_dumps(json_object, 0);
    if (buffer == NULL) {
            /* Handle error here. */
    }
    size = strlen(buffer) + 1;
    ```

3. Десериализация JSON-элемента `json_object` библиотеки Jansson.

    ```c
    #include <jansson.h>

    char *json_object_string;
    json_error_t json_error;
    json_t *json_object;

    json_object = json_loads(json_object_string, 0, &json_error);
    if (json_object == NULL) {
            /* Handle error here. */
    }
    ```

4. Работа с JSON-элементами библиотеки Jansson.  
   См. Лабораторная работа №13.

# Усложненная задача

В дополнение к базовой задаче реализовать применение настроек сетевых
интерфейсов, хранящихся в конфигурационной СУБД, с помощью пакета **iproute2**.
Фактические настройки сетевых интерфейсов, полученные после попытки
применения настроек из конфигурационной СУБД, записать в виде статусных
данных в конфигурационную СУБД.

## Описание усложненной задачи

Перед выполнением операций по применению настроек сетевых
интерфейсов необходимо заполнить статусные данные значениями по умолчанию.

Статусные данные со значениями по умолчанию имеют вид:

```json
{
    "interfaces_status": {
        "ge1": {
            "address": null,
            "mtu": null,
            "link": null
        },
        "another_object": {}
    }
}
```

Статусные данные с фактическими значениями имеют вид:

```json
{
    "interfaces_status": {
        "ge1": {
            "address": "68:eb:c5:00:02:01",
            "mtu": 1010,
            "link": "up"
        },
        "another_object": {}
    }
}
```

Статусные данные записываются для каждого атрибута сетевого интерфейса
применение которого реализует клиентское приложение.

# Наблюдение результатов

Запуск конфигурационной СУБД осуществляется с помощью команды:

```console
$ ./lab_14_database_mips -f ./configuration.json -t
```
