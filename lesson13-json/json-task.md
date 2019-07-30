
# Базовая задача

Разработать приложение, исполняющее запросы на чтение данных из документа
в формате JSON. Запрос считывается из стандартного потока ввода и имеет
синтаксис JSONPath. Приложение должно поддержать оператор доступа к корневому
элементу **$** и оператор доступа к дочернему элементу **[ ]**.

## Описание базовой задачи

Ознакомиться с синтаксисом JSONPath можно по ссылке [JSONPath Syntax][1].
Для проверки запросов в формате JSONPath можно воспользоваться
онлайн-редактором [Jayway JsonPath Evaluator][2].

Необходимо поддержать следующие выражения:

- **$** - доступ к корневому элементу документа JSON;
- **[** \<index\> **]** - доступ к элементу JSON-массива
  с индексом \<index\>;
- **[** '\<property\>' **]** - доступ к элементу JSON-объекта
  со свойством \<property\>;
- **[** * **]** - доступ ко всем элементам JSON-массива или JSON-объекта,
  а также к значениям JSON-элементов;

Исполнение JSON-запроса на чтение состоит в формировании внутреннего множества
JSON-элементов, соответствующего JSON-запросу и выводе сформированного
множества JSON-элементов в стандартный поток вывода.

Изначально, внутреннее множество полагается тождественным JSON-документу.
При применении выражения из JSON-запроса внутреннее множество модифицируется,
т.е. каждое последующее выражение из JSON-запроса применяется к новому
внутреннему множеству JSON-элементов. Внутреннее множество JSON-элементов
необходимо модифицировать до тех пор пока не закончатся выражения в
JSON-запросе, либо пока не опустеет само внутреннее множество JSON-элементов.

В соответствие с заданием выражение **$** может появляться только в начале
JSON-запроса. Это означает, что все последующие выражения в JSON-запросе
будут применяться к внутреннему множеству, тождественному всему JSON-документу.
Указанное поведение соответствует поведению по умолчанию (когда выражение **$**
отсутствует), поэтому поддержка данного выражения сводится к его игнорированию.

В качестве исходного JSON-документа предлагается использовать файл
*./json_example.json*, который находится в директории с текущим заданием.

Для работы с JSON-элементами используется библиотека [Jansson][3].
Необходимо отметить, что в указанной библиотеке при работе с JSON-элементами
учет выделенной памяти ведется с использованием ссылок на JSON-элемент.
При создании JSON-элемента и его включении в другие JSON-элементы количество
ссылок на JSON-элемент увеличивается, при исключении - уменьшается.
В тот момент когда количество ссылок на JSON-элемент становится равным нулю
JSON-элемент удаляется, а память занятая им освобождается.

## Алгоритм решения базовой задачи

1. Прочитать содержимое JSON-документа *./json_example.json*.

2. Прочитать JSON-запрос из стандартного потока ввода.

3. Проанализировать JSON-запрос, разбив его на отдельные выражения.

4. Сформировать внутреннее множество JSON-элементов в соответствие
   с выражениями JSON-запроса. Вывести внутреннее множество JSON-элементов
   в стандартный поток вывода.

5. Вернуться к пункту №2.

[1]: https://support.smartbear.com/alertsite/docs/monitors/api/endpoint/jsonpath.html
[2]: http://jsonpath.herokuapp.com/
[3]: https://jansson.readthedocs.io/en/2.12/

## Примеры для базовой задачи

1. Чтение содержимого JSON-документа.

    ```c
    #include <jansson.h>

    #define PATH_TO_JSON_FILE "./json_example.json"

    char *file;
    json_t *json_data;
    json_error_t json_error;

    file = PATH_TO_JSON_FILE;

    json_data = json_load_file(file, 0, &json_error);
    if (json_data == NULL) {
            /* Handle error here. */
    }
    ```

2. Вывод информации об ошибке `json_error`.

    ```c
    #include <jansson.h>
    #include <stdio.h>

    json_error_t *json_error;

    fprintf(stderr,
            "Error: %s. Source: %s. Line: %d. Column: %d.\n",
            json_error->text,
            json_error->source,
            json_error->line,
            json_error->column);
    ```

3. Чтение строки из стандартного потока ввода.  
   См. Лабораторная работа №7. Базовая часть. Пример №1.

4. Поиск подстроки `substring` внутри строки `json_path`.

    ```c
    #include <string.h>

    const char *json_path;
    const char *substring_start;
    const char *substring_end;
    char *substring;

    substring = "test";
    substring_start = strstr(json_path, substring);
    if (substring_start == NULL) {
            /* Substring not found. */
    } else {
            expression_end = substring_start + strlen(substring);
    }
    ```

5. Проверка типа JSON-элемента `item`.

    ```c
    #include <jansson.h>

    json_t *item;

    if (json_typeof(item) == JSON_OBJECT) {
            /* JSON object. */
    }
    if (json_typeof(item) == JSON_ARRAY) {
            /* JSON array. */
    }
    if (json_typeof(item) == JSON_STRING) {
            /* JSON string. */
    }
    if (json_typeof(item) == JSON_INTEGER) {
            /* JSON integer. */
    }
    if (json_typeof(item) == JSON_REAL) {
            /* JSON real. */
    }
    if (json_typeof(item) == JSON_TRUE) {
            /* JSON true value. */
    }
    if (json_typeof(item) == JSON_FALSE) {
            /* JSON false value. */
    }
    if (json_typeof(item) == JSON_NULL) {
            /* JSON none value. */
    }
    ```

6. Получение JSON-элемента `item` из строки `string`.

    ```c
    #include <jansson.h>

    json_t *item;
    const char *string;

    item = json_string(string);
    if (item == NULL) {
            /* Handle error here. */
    }
    ```

7. Получение строки `string` из JSON-элемента `item`.

    ```c
    #include <jansson.h>

    const json_t *item;
    const char *string;

    string = json_string_value(item);
    if (string == NULL) {
            /* Handle error here. */
    }
    ```

8. Получение JSON-элемента `item` из целого числа `integer`.

    ```c
    #include <jansson.h>

    json_t *item;
    json_int_t integer;

    item = json_integer(integer);
    if (item == NULL) {
            /* Handle error here. */
    }
    ```

9. Получение целого числа `integer` из JSON-элемента `item`.

    ```c
    #include <jansson.h>

    const json_t *item;
    json_int_t integer;

    if (json_typeof(item) == JSON_INTEGER)
            integer = json_integer_value(item);
    ```

10. Уменьшение счетчика ссылок на JSON-элемент `item`.

    ```c
    #include <jansson.h>

    json_t *item;

    json_decref(item);
    ```

11. Увеличение счетчика ссылок на JSON-элемент `item`.

    ```c
    #include <jansson.h>

    json_t *item;

    json_incref(item);
    ```

12. Итерация по элементам JSON-массива `array`.

    ```c
    #include <jansson.h>

    json_t *array;
    size_t index;
    json_t *value;

    json_array_foreach(array, index, value) {
            /* Do something useful with index and value here. */
    }
    ```

13. Добавление JSON-элементов в конец JSON-массива `array`.

    ```c
    #include <jansson.h>

    json_t *array;
    json_t *value_1;
    json_t *value_2;
    int ret;

    ret = json_array_append(array, value_1);
    if (ret == -1) {
            /* Handle error here. */
    }
    /* Do it if you need. */
    json_decref(value_1);

    ret = json_array_append_new(array, value_2);
    if (ret == -1) {
            /* Handle error here. */
    }
    /* Operation json_decref(value_2) is done automatically. */
    ```

14. Итерация по элементам JSON-объекта `object`.

    ```c
    #include <jansson.h>

    json_t *object;
    const char *key;
    json_t *value;

    json_object_foreach(object, key, value) {
            /* Do something useful with key and value here. */
    }
    ```

15. Добавление JSON-элементов в JSON-объект `object`.

    ```c
    #include <jansson.h>

    json_t *object;
    const char *key_1;
    const char *key_2;
    json_t *value_1;
    json_t *value_2;
    int ret;

    ret = json_object_set(object, key_1, value_1);
    if (ret == -1) {
            /* Handle error here. */
    }
    /* Do it if you need. */
    json_decref(value_1);

    ret = json_object_set_new(object, key_2, value_2);
    if (ret == -1) {
            /* Handle error here. */
    }
    /* Operation json_decref(value_2) is done automatically. */
    ```

# Усложненная задача

В дополнение к базовой задаче поддержать упрощенные фильтрующие предикаты
JSONPath **[?( )]**. Выражение предиката выполняет сравнение ближайшего
дочернего элемента с указанным строковым или числовым значением.
При отсутствии значения выбираются все элементы с указанным свойством.

## Описание усложненной задачи

При работе с фильтрующим выражением в JSON-запросе необходимо отметить, что
если текущее множество JSON-элементов состоит из единственного JSON-массива,
то в качестве текущего множества JSON-элементов принимается этот JSON-массив.

Необходимо поддержать следующие выражения:

- **[?(@[** '\<property\>' **]** **==** \<value\> **)]** - выбор всех
  JSON-элементов у которых свойство \<property\> равно целому числу \<value\>;
- **[?(@[** '\<property\>' **]** **==** '\<value\>' **)]** - выбор всех
  JSON-элементов у которых свойство \<property\> равно строке \<value\>;
- **[?(@[** '\<property\>' **])]** - выбор всех
  JSON-элементов у которых есть свойство \<property\>;

# Наблюдение результатов

1. Выбор множества книг:

    ```console
    $['books']
    ```

    ```json
    [
        {
            "name" : "The Great Gatsby",
            "author" : "F. Scott Fitzgerald",
            "country" : "United States",
            "genre" : "Novel",
            "year" : 1925,
            "last_century" : true
        },
        {
            "name" : "Alice's Adventures in Wonderland",
            "author" : "Lewis Carroll",
            "country" : "United Kingdom",
            "genre" : "Fantasy",
            "year" : 1865
        },
        {
            "name" : "The Hunchback of Notre-Dame",
            "author" : "Victor Hugo",
            "country" : "France",
            "genre" : "Romanticism",
            "year" : 1831
        },
        {
            "name" : "The Catcher in the Rye",
            "author" : "J. D. Salinger",
            "country" : "United States",
            "genre" : "Realistic fiction",
            "year" : 1951,
            "last_century" : true
        },
        {
            "name" : "War and Peace",
            "author" : "Leo Tolstoy",
            "country" : "Russia",
            "genre" : "Novel",
            "year" : 1869
        }
    ]
    ```

2. Выбор первой книги в списке:

    ```console
    $['books'][0]
    ```

    ```json
    {
        "name" : "The Great Gatsby",
        "author" : "F. Scott Fitzgerald",
        "country" : "United States",
        "genre" : "Novel",
        "year" : 1925,
        "last_century" : true
    }
    ```

3. Выбор значений всех полей первой книги в списке:

    ```console
    $['books'][0][*]
    ```

    ```json
    [
        "The Great Gatsby",
        "F. Scott Fitzgerald",
        "United States",
        "Novel",
        1925,
        true
    ]
    ```

4. Выбор книг изданных в 1925 году и писателей родившихся в этом же году:

    ```console
    $[*][*][?(@['year'] == 1925)]
    ```

    ```json
    [
        {
            "name" : "The Great Gatsby",
            "author" : "F. Scott Fitzgerald",
            "country" : "United States",
            "genre" : "Novel",
            "year" : 1925,
            "last_century" : true
        },
        {
            "name" : "Arkady Strugatsky",
            "country" : "Russia",
            "year" : 1925,
            "last_century" : true
        }
    ]
    ```

5. Выбор новелл среди книг:

    ```console
    $['books'][?(@['genre'] == 'Novel')]
    ```

    ```json
    [
        {
            "name" : "The Great Gatsby",
            "author" : "F. Scott Fitzgerald",
            "country" : "United States",
            "genre" : "Novel",
            "year" : 1925,
            "last_century" : true
        },
        {
            "name" : "War and Peace",
            "author" : "Leo Tolstoy",
            "country" : "Russia",
            "genre" : "Novel",
            "year" : 1869
        }
    ]
    ```

6. Выбор французских книг и писателей:

    ```console
    $[*][*][?(@['country'] == 'France')]
    ```

    ```json
    [
        {
            "name" : "The Hunchback of Notre-Dame",
            "author" : "Victor Hugo",
            "country" : "France",
            "genre" : "Romanticism",
            "year" : 1831
        },
        {
            "name" : "Jules Verne",
            "country" : "France",
            "year" : 1828
        },
        {
            "name" : "Marcel Proust",
            "country" : "France",
            "year" : 1871
        }
    ]
    ```

7. Выбор книг, написанных в прошлом веке, и писателей, родившихся в том же веке:

    ```console
    $[*][*][?(@['last_century'])]
    ```

    ```json
    [
        {
            "name" : "The Great Gatsby",
            "author" : "F. Scott Fitzgerald",
            "country" : "United States",
            "genre" : "Novel",
            "year" : 1925,
            "last_century" : true
        },
        {
            "name" : "The Catcher in the Rye",
            "author" : "J. D. Salinger",
            "country" : "United States",
            "genre" : "Realistic fiction",
            "year" : 1951,
            "last_century" : true
        },
        {
            "name" : "Arkady Strugatsky",
            "country" : "Russia",
            "year" : 1925,
            "last_century" : true
        }
    ]
    ```

8. Выбор книги "Alice's Adventures in Wonderland":

    ```console
    $['books'][?(@['name'] == 'Alice\'s Adventures in Wonderland')]
    ```

    ```json
    {
        "name" : "Alice's Adventures in Wonderland",
        "author" : "Lewis Carroll",
        "country" : "United Kingdom",
        "genre" : "Fantasy",
        "year" : 1865
    }
    ```
