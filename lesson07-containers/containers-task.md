
# Базовая задача

Разработать приложение для динамического добавления и извлечения строк текста
из встроенных списка и словаря. Команды на добавление, удаление и отображение
элементов контейнера считываются из стандартного потока ввода приложения.
Вывод осуществляется в эмулятор терминала. Вместе с каждым значением должно
быть выведено время его добавления в контейнер.

## Описание базовой задачи

Для работы с элементами контейнера необходимо поддержать следующие команды:

- **la \<value>** - добавить элемент со значением **value** в конец списка;
- **ld \<index>** - удалить элемент с индексом **index** из списка;
- **lg \<index>** - отобразить элемент с индексом **index**;
- **ls** - отобразить полное содержимое списка;
- **da \<key> \<value>** - добавить элемент с ключом **key** и
  значением **value** в словарь;
- **dd \<key>** - удалить элемент с ключом **key** из словаря;
- **dg \<key>** - отобразить элемент с ключом **key**;
- **ds** - отобразить полное содержимое словаря;

## Алгоритм решения базовой задачи

1. Прочитать команду из стандартного потока ввода.

2. Проанализировать прочитанную команду.

3. В соответствие с введенной командой выполнить необходимое действие.
   Если команда соответствует добавлению элемента в словарь и в словаре уже
   имеется элемент с таким же ключом, то значение старого элемента должно быть
   заменено значением нового. Также стоит отметить, что при добавлении
   элементов в список и в словарь под них необходимо выделять память.

4. Вернуться к пункту №1.

[1]: https://www.opennet.ru/man.shtml?topic=getline&category=3&russian=0
[2]: https://www.opennet.ru/man.shtml?topic=scanf&category=3&russian=0
[3]: http://www.opennet.ru/man.shtml?topic=strftime&category=3&russian=0
[4]: https://www.opennet.ru/man.shtml?topic=localtime&category=3&russian=0
[5]: https://www.opennet.ru/man.shtml?topic=select&category=2&russian=0

## Примеры для базовой задачи

1. Чтение строки из стандартного потока ввода.

    ```c
    #include <stdio.h>

    char *lineptr = NULL;
    size_t n = 0;
    ssize_t getline_size;

    getline_size = getline(&lineptr, &n, stdin);
    if (getline_size == -1) {
            free(lineptr);
            /* Handle error here. */
    }

    /* Do something useful with lineptr here. */

    free(lineptr);
    ```

    Функция [getline(3)][1] служит для чтения строк из потока ввода/вывода.

2. Чтение форматированной строки.

    ```c
    #include <stdio.h>

    int sscanf_items;
    char *lineptr;
    char *svalue;
    int dvalue;

    sscanf_items = sscanf(lineptr, "%ms %d", &svalue, &dvalue);
    if (sscanf_items != 2) {
            /* Handle error here. */
    }

    /* Do something useful with svalue and dvalue here. */

    free(svalue);
    ```

    Функция [sscanf(3)][2] служит для чтения форматированной строки.
    Модификатор `%ms` служит для указания на последовательность любых символов
    не являющихся пробельными с одновременным выделением памяти под эту
    последовательность. Функция возвращает количество успешно прочитанных
    значений.

3. Инициализация списка с именем `list`.

    ```c
    #include <libubox/list.h>

    struct list_head list;

    INIT_LIST_HEAD(&list);
    ```

4. Добавление элемента `item` в конец списка `list`.

    ```c
    #include <libubox/list.h>

    struct list_item {
            char *value;
            struct list_head list_member;
    };

    struct list_item *item;

    /* Memory allocation for the item here. */

    list_add_tail(&item->list_member, &list);
    ```

5. Удаление элемента `item` из списка `list`.

    ```c
    #include <libubox/list.h>

    list_del(&item->list_member);
    ```

6. Итерация по всем элементам списка `list`.

    ```c
    #include <libubox/list.h>

    struct list_item *item;

    list_for_each_entry(item, &list, list_member) {
            /* Do something useful with item here. */
    }
    ```

7. Безопасная итерация по всем элементам списка `list` (можно удалять элементы).

    ```c
    #include <libubox/list.h>

    struct list_item *item, *item_next;

    list_for_each_entry_safe(item, item_next, &list, list_member) {
            /* Do something useful with item here. */
    }
    ```

8. Инициализация словаря с именем `dict`.

    ```c
    #include <libubox/avl.h>
    #include <libubox/avl-cmp.h>

    struct avl_tree dict;

    avl_init(&dict, avl_strcmp, false, NULL);
    ```

    Второй аргумент функции `avl_init()` является указателем на функцию
    сравнения ключей. В данном случае подразумевается использование строковых
    ключей, поэтому в качестве аргумента задается функция `avl_strcmp()`.
    Четвертый аргумент - это значение, передаваемое в функцию сравнения ключей.
    Третий аргумент задает возможность наличия одинаковых ключей.

9. Добавление элемента `item` в словарь `dict` с ключом `"test"`.

    ```c
    #include <libubox/avl.h>

    struct dict_item {
            char *key;
            char *value;
            struct avl_node dict_member;
    };

    struct dict_item *item;

    /* Memory allocation for the item here. */

    item->key = "test";
    item->dict_member.key = item->key;

    avl_insert(&dict, &item->dict_member);
    ```

10. Извлечение элемента `item` с ключом `"test"` из словаря `dict`.

    ```c
    #include <libubox/avl.h>

    struct dict_item *item;

    item = avl_find_element(&dict, "test", item, dict_member);
    ```

11. Удаление элемента `item` из словаря `dict`.

    ```c
    #include <libubox/avl.h>

    avl_delete(&dict, &item->dict_member);
    ```

12. Итерация по всем элементам словаря `dict`.

    ```c
    #include <libubox/avl.h>

    struct dict_item *item;

    avl_for_each_element(&dict, item, dict_member) {
            /* Do something useful with item here. */
    }
    ```

13. Безопасная итерация по всем элементам словаря `dict` (можно удалять элементы).

    ```c
    #include <libubox/avl.h>

    struct dict_item *item, *item_next;

    avl_for_each_element_safe(&dict, item, dict_member, item_next) {
            /* Do something useful with item here. */
    }
    ```

14. Отображение текущего времени.

    ```c
    #include <stdio.h>
    #include <sys/time.h>
    #include <time.h>

    #define TS_SIZE 32

    struct timeval ct;
    char ct_str[TS_SIZE];

    gettimeofday(&ct, NULL);

    strftime(ct_str, TS_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&ct.tv_sec));

    fprintf(stdout, "Time: %s.%06ld\n", ct_str, ct.tv_usec);
    ```

    Функция [strftime(3)][3] служит для получения форматированного времени.
    Функция [localtime(3)][4] служит для получения покомпонентного времени.

15. Ожидание готовности чтения для файлового дескриптора `fd`.  
    См. Лабораторная работа №6, пример №4.

# Усложненная задача

В дополнение к базовой задаче реализовать автоматическое удаление устаревших
элементов контейнеров. В момент добавления элемента приложение должно
устанавливать таймер, при срабатывании которого элемент автоматически
удаляется из контейнера. Если до срабатывания таймера такой же элемент
добавляется повторно, то время срабатывания таймера откладывается.

## Рекомендации по выполнению усложненной задачи

Для организации таймера необходимо задействовать функцию [select(2)][5].

## Примеры для усложненной задачи

1. Добавление времени `b` ко времени `a` с сохранением результата в `c`.  
   См. Лабораторная работа №6, пример №3.

2. Вычитание времени `b` из времени `a` с сохранением результата в `c`.  
   См. Лабораторная работа №6, пример №4.

3. Сравнение времени `b` и времени `a`.  
   См. Лабораторная работа №6, пример №5.

4. Получение текущего времени.  
   См. Лабораторная работа №6, пример №6.

# Сборка приложения под архитектуру x86

Инструкции по сборке приложения под архитектуру x86 находятся по пути
**/path_to_netprog2/build/angtel-projects/x86/build-instructions.md**, где
**path_to_netprog2** - путь к проекту **netprog2** на локальном диске.
