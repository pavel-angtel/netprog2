
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

Конфигурационная СУБД поддерживает работу с группами запросов и ответов.

При нажатии клавиши **Enter** СУБД отображает текущую конфигурацию.

Конфигурация и статусные данные, хранящиеся в СУБД имеют вид:

```json
{
    "interfaces": {
        "ge1": {
            "address": {
                "value": "68:eb:c5:00:02:01",
                "status": {
                    "code": null,
                    "message": null,
                    "data": null
                }
            },
            "another_object": {}
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
            "jsonpath": "$['interfaces']['ge1']['address']"
        },
        "id": 1
    }
    ```

   Путь к JSON-элементу указывается с помощью свойства **"jsonpath"**.

2. Формат ответа на запрос о получении значения JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "result": {
            "value": "68:eb:c5:00:02:01",
            "status": {
                "code": null,
                "message": null,
                "data": null
            }
        },
        "id": 1
    }
    ```

   Значение JSON-элемента указывается с помощью свойства **"result"**.

3. Формат ответа на некорректный запрос:

    ```json
    {
        "jsonrpc": "2.0",
        "error": {
            "code": -300,
            "message": "Invalid JSONPath",
            "data": "$?['interfaces']"
        },
        "id": 1
    }
    ```

   Информация об ошибке указывается с помощью свойства **"error"**.

4. Формат запроса на установку значений JSON-элементов:

    ```json
    [
        {
            "jsonrpc": "2.0",
            "method": "set",
            "params": {
                "jsonpath": "$['interfaces']['ge1']['address']['status']",
                "value": {
                    "code": 0,
                    "message": "Command execution: OK"
                }
            },
            "id": "address"
        },
        {
            "jsonrpc": "2.0",
            "method": "set",
            "params": {
                "jsonpath": "$['interfaces']['ge1']['mtu']['status']",
                "value": {
                    "code": 0,
                    "message": "Command execution: OK"
                }
            },
            "id": "mtu"
        },
        {
            "jsonrpc": "2.0",
            "method": "set",
            "params": {
                "jsonpath": "$['interfaces']['ge1']['operstate']['status']",
                "value": {
                    "code": -1,
                    "message": "Command execution: FAILED"
                }
            },
            "id": "operstate"
        }
    ]
    ```

   Новое значение JSON-элемента указывается с помощью свойства **"value"**.

5. Формат запроса на удаление JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "method": "delete",
        "params": {
            "jsonpath": "$['interfaces']['ge1']['operstate']['status']",
        },
        "id": "dge1os"
    }
    ```

6. Формат запроса на удаление JSON-элемента:

    ```json
    {
        "jsonrpc": "2.0",
        "method": "remove",
        "params": {
            "jsonpath": "$['interfaces']['ge1']",
        },
        "id": "rge1"
    }
    ```

## Алгоритм решения базовой задачи

1. Принять следующее макроопределение для публичного адреса СУБД:

    ```c
    #define SOCK_DATABASE_PATH "/tmp/sock_seqpacket_database"
    ```

2. Отправить конфигурационной СУБД запрос на получение конфигурации
   и статуса всех сетевых интерфейсов.

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
Статус выполнения операций по применению настроек записать в СУБД.

## Описание усложненной задачи

JSON-элемент со статусными данными имеет вид:

```json
{
    "code": null,
    "message": null,
    "data": null
}
```

В данном случае:

- **"code"** - статусный код выполнения команды из пакета **iproute2**;
- **"message"** - опциональное сообщение о результатах применения настроек;
- **"data"** - опциональные дополнительные данные;

Статусные данные записываются для каждого атрибута сетевого интерфейса
применение которого реализует клиентское приложение. Запись осуществляется
с использованием свойства **"status"** соответствующего JSON-объекта.

# Наблюдение результатов

Запуск конфигурационной СУБД осуществляется с помощью команды:

```console
$ ./lab_14_database_mips -f ./configuration.json -t
```
