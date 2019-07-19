
# Базовая задача

Разработать драйвер аппаратного таймера, управляемый через символьное
устройство. Запись в символьное устройство задает верхнюю границу счетчика,
чтение из символьного устройства возвращает текущее значение счетчика.

## Описание базовой задачи

Необходимо поддержать следующие символьные устройства:

- **a100_wdt_device_0_timer_0** - установка и отображение верхней границы
  счетчика аппаратного таймера (чтение и запись);

Для выполнения поставленной задачи потребуется работа с регистрами процессора
**Centillium Atlanta 100**. См. описание базовой задачи лабораторной работы №9.

## Алгоритм решения базовой задачи

1. Инициализировать модуль. При инициализации модуля создать символьные
   устройства в пространстве ядра ОС. Зарегистрировать драйвер.

2. Получить доступ к виртуальному адресному пространству таймера.

3. Включить автоматический сброс счетчика аппаратного таймера Timer 0 при
   достижении верхней границы счетчика Counter 0. Установить временной интервал
   шага счетчика аппаратного таймера Timer 0 равным **10 мкс**.
   Инициализировать верхнюю границу счетчика Counter 0 аппаратного таймера
   Timer 0 значением **0xFFFF**.

4. Создать файлы символьных устройств в пространстве пользователя ОС.
   При чтении данных из файла символьного устройства необходимо осуществить
   запись запрашиваемых данных в буфер, ассоциированный с файлом символьного
   устройства. Данные при этом копируются из пространства ядра ОС в
   пространство пользователя ОС. При записи данных в файл символьного
   устройства необходимо прочитать записываемые данные, находящееся в буфере,
   ассоциированном с файлом символьного устройства и применить их к регистрам
   процессора **Centillium Atlanta 100**. Данные в этом случае копируются из
   пространства пользователя ОС в пространство ядра ОС.

5. Освободить ресурсы, отменить регистрацию драйвера, деинициализировать модуль.

## Примеры для базовой задачи

1. Регистрация диапазона номеров для символьных устройств, добавляемых в
   ядро ОС, начиная с номера `baseminor`.

    ```c
    #include <linux/fs.h>

    #define DEVICE_NAME "a100-wdt"
    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    static int a100_wdt_init(void) {
            int ret;
            dev_t dev;
            unsigned int baseminor;

            baseminor = 0;

            ret = alloc_chrdev_region(&dev,
                                      baseminor,
                                      MAX_CHAR_DEVICES,
                                      DEVICE_NAME);
            if (ret < 0) {
                    /* Handle error here. */
            }

            /* ... */
    }
    ```

2. Освобождение диапазона номеров для символьных устройств, добавляемых в
   ядро ОС.

    ```c
    #include <linux/fs.h>
    #include <linux/cdev.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    static struct cdev a100_wdt_cdev;

    static void a100_wdt_exit(void)
    {
            /* ... */

            unregister_chrdev_region(a100_wdt_cdev.dev, MAX_CHAR_DEVICES);
    }
    ```

3. Инициализация символьных устройств, добавляемых в ядро ОС.

    ```c
    #include <linux/cdev.h>

    static int a100_wdt_open(struct inode *inode,
                             struct file *file);

    static int a100_wdt_release(struct inode *inode,
                                struct file *file);

    static ssize_t a100_wdt_read(struct file *file,
                                 char __user *user_buffer,
                                 size_t user_size,
                                 loff_t *offset);

    static ssize_t a100_wdt_write(struct file *file,
                                  const char __user *user_buffer,
                                  size_t user_size,
                                  loff_t *offset);

    static struct cdev a100_wdt_cdev;

    const struct file_operations a100_wdt_fops = {
            .owner = THIS_MODULE,
            .open = a100_wdt_open,
            .release = a100_wdt_release,
            .read = a100_wdt_read,
            .write = a100_wdt_write
    };

    static int a100_wdt_init(void) {
            cdev_init(&a100_wdt_cdev, &a100_wdt_fops);

            /* ... */
    }
    ```

4. Добавление символьных устройств в ядро ОС.

    ```c
    #include <linux/cdev.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    static struct cdev a100_wdt_cdev;

    static int a100_wdt_init(void) {
            dev_t dev;
            int ret;

            ret = cdev_add(&a100_wdt_cdev, dev, MAX_CHAR_DEVICES);
            if (ret < 0) {
                    /* Handle error here. */
            }

            /* ... */
    }
    ```

5. Удаление символьных устройств из ядра ОС.

    ```c
    #include <linux/cdev.h>

    static struct cdev a100_wdt_cdev;

    static void a100_wdt_exit(void)
    {
            /* ... */

            cdev_del(&a100_wdt_cdev);
    }
    ```

6. Создание класса устройств.

    ```c
    #include <linux/platform_device.h>
    #include <linux/err.h>

    #define DEVICE_NAME "a100-wdt"

    static struct class *a100_wdt_class;

    static int a100_wdt_init(void) {
            a100_wdt_class = class_create(THIS_MODULE, DEVICE_NAME);
            if (IS_ERR(a100_wdt_class)) {
                    /* Handle error here. */
            }

            /* ... */
    }
    ```

7. Удаление класса устройств.

    ```c
    static struct class *a100_wdt_class;

    static void a100_wdt_exit(void)
    {
            /* ... */

            class_destroy(a100_wdt_class);
    }
    ```

8. Создание списка устройств шины `list_of_devices` и битовой маски
   используемых символьных устройств `bitmap_of_char_devices`.

    ```c
    #include <linux/bitmap.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    LIST_HEAD(list_of_devices);
    DECLARE_BITMAP(bitmap_of_char_devices, MAX_CHAR_DEVICES);
    ```

9. Установка бита в `1` для символьного устройства с номером `char_device`
   в битовой маске `bitmap_of_char_devices`.

    ```c
    #include <linux/bitmap.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    DECLARE_BITMAP(bitmap_of_char_devices, MAX_CHAR_DEVICES);

    int char_device;

    set_bit(char_device, bitmap_of_char_devices);
    ```

10. Установка бита в `0` для символьного устройства с номером `char_device`
    в битовой маске `bitmap_of_char_devices`.

    ```c
    #include <linux/bitmap.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    DECLARE_BITMAP(bitmap_of_char_devices, MAX_CHAR_DEVICES);

    int char_device;

    clear_bit(char_device, bitmap_of_char_devices);
    ```

11. Поиск первого бита со значением `0` в битовой маске
    `bitmap_of_char_devices`.

    ```c
    #include <linux/bitmap.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    DECLARE_BITMAP(bitmap_of_char_devices, MAX_CHAR_DEVICES);

    int bit_index;

    bit_index = find_first_zero_bit(bitmap_of_char_devices,
                                    MAX_CHAR_DEVICES);
    ```

12. Добавление устройств класса.

    ```c
    #include <linux/platform_device.h>
    #include <linux/kdev_t.h>

    #define MAX_DEVICES 1
    #define ATTRIBUTES_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * ATTRIBUTES_PER_DEVICE)

    DECLARE_BITMAP(bitmap_of_char_devices, MAX_CHAR_DEVICES);

    static struct cdev a100_wdt_cdev;
    static struct class *a100_wdt_class;

    /* Initialize private data for platform device here. */

    static int a100_wdt_probe(struct platform_device *pdev) {
            int i;

            /* ... */

            for (i = 0; i < ATTRIBUTES_PER_DEVICE; i++) {
                    int char_device;
                    struct device *device;

                    char_device = find_first_zero_bit(bitmap_of_char_devices,
                                                      MAX_CHAR_DEVICES);
                    a100_wdt_attribute_data->char_device = char_device;
                    set_bit(char_device, bitmap_of_char_devices);

                    /* Initialize private data for attribute here. */

                    device = device_create(a100_wdt_class,
                                           NULL,
                                           MKDEV(MAJOR(a100_wdt_cdev.dev),
                                                 char_device),
                                           NULL,
                                           "a100_wdt_device_%d_timer_%d",
                                           char_device / ATTRIBUTES_PER_DEVICE,
                                           i);
                    if (IS_ERR(device)) {
                            /* Handle error here. */
                    }
            }

            /* Add private data for platform device to list here. */
    }
    ```

13. Удаление устройств класса.

    ```c
    #include <linux/platform_device.h>
    #include <linux/kdev_t.h>

    #define ATTRIBUTES_PER_DEVICE 1

    static struct cdev a100_wdt_cdev;
    static struct class *a100_wdt_class;

    static int a100_wdt_remove(struct platform_device *pdev) {
            int i;

            /* ... */

            for (i = 0; i < ATTRIBUTES_PER_DEVICE; i++) {
                    /* Get private data for attribute here. */

                    device_destroy(a100_wdt_class,
                                   MKDEV(MAJOR(a100_wdt_cdev.dev),
                                         a100_wdt_attribute_data->char_device));
                    clear_bit(a100_wdt_attribute_data->char_device,
                              bitmap_of_char_devices);
            }

            /* Delete private data for platform device from list here. */

            /* ... */
    }
    ```

14. Получение младшего номера устройства `minor` из `struct inode *inode`.

    ```c
    struct inode *inode
    int minor;

    minor = iminor(inode);
    ```

15. Ассоциирование данных `data` с открытым файлом символьного устройства.

    ```c
    struct file *file
    void *data;

    file->private_data = data;
    ```

16. Копирование данных из пространства ядра ОС в пространство пользователя ОС
    (буфер `user_buffer`).

    ```c
    #include <linux/uaccess.h>

    #define REG_VALUE_STR_SIZE 12

    char reg_value_str[REG_VALUE_STR_SIZE];
    int ret;

    ret = copy_to_user(user_buffer, reg_value_str, REG_VALUE_STR_SIZE);
    if (ret != 0) {
            /* Handle error here. */
    }
    ```

17. Копирование данных размером `user_size` из пространства пользователя ОС
    (буфер `user_buffer`) в пространство ядра ОС.

    ```c
    #include <linux/uaccess.h>

    #define REG_VALUE_STR_SIZE 12

    char reg_value_str[REG_VALUE_STR_SIZE];
    int ret;

    if (user_size > REG_VALUE_STR_SIZE) {
            /* Handle error here. */
    }

    ret = copy_from_user(reg_value_str, user_buffer, user_size);
    if (ret != 0) {
            /* Handle error here. */
    }
    ```

18. Чтение данных пользователем из файла символьного устройства.

    ```c
    #include <linux/uaccess.h>

    #define REG_VALUE_STR_SIZE 12

    static ssize_t a100_wdt_read(struct file *file,
                                 char __user *user_buffer,
                                 size_t user_size,
                                 loff_t *offset)
    {
            char reg_value_str[REG_VALUE_STR_SIZE];
            int data_size;
            int ret;

            if (*offset == 0) {
                    /* Fill reg_value_str array. */
            }

            data_size = min(REG_VALUE_STR_SIZE - (size_t) *offset, user_size);
            if (data_size <= 0)
                    return 0;

            ret = copy_to_user(user_buffer,
                               reg_value_str + (size_t) *offset,
                               data_size);
            if (ret != 0) {
                    /* Handle error here. */
            }

            *offset += data_size;

            return data_size;
    }
    ```

19. Запись данных пользователем в файл символьного устройства.

    ```c
    #include <linux/uaccess.h>

    #define REG_VALUE_STR_SIZE 12

    static ssize_t a100_wdt_read(struct file *file,
                                 char __user *user_buffer,
                                 size_t user_size,
                                 loff_t *offset)
    {
            char reg_value_str[REG_VALUE_STR_SIZE];
            unsigned long reg_value;
            int ret;

            if (user_size > REG_VALUE_STR_SIZE) {
                    /* Handle error here. */
            }

            memset(reg_value_str, 0, REG_VALUE_STR_SIZE);

            ret = copy_from_user(reg_value_str, user_buffer, user_size);
            if (ret != 0) {
                    /* Handle error here. */
            }

            ret = kstrtoul(reg_value_str, 0, &reg_value);
            if (ret < 0) {
                    /* Handle error here. */
            }

            /* Write reg_value to appropriate Centillium Atlanta 100 register */

            return user_size;
    }
    ```

Стоит отметить, что отмену регистрации драйвера
(вызов функции `platform_driver_unregister()`) необходимо выполнять до вызова
функций `class_destroy()`, `cdev_del()` и `unregister_chrdev_region()`.

# Усложненная задача

Разработать драйвер для установки и считывания состояния сигналов GPIO с
управлением через набор символьных устройств. Доступные драйверу сигналы GPIO
извлекаются из структуры Device Tree. Установка и считывание состояний выводов
осуществляется через подсистему **gpiolib**. Каждый сигнал управляется через
отдельное символьное устройство. Запись в устройство устанавливает значение
выходного сигнала. Чтение возвращает значение входного сигнала.

## Описание усложненной задачи

Необходимо поддержать следующие символьные устройства:

- **a100_wdt_device_0_gpio_N** - установка и отображение сигнала
  GPIO (чтение и запись), где **N** принимает значения от **0** до **7**;

## Рекомендации по выполнению усложненной задачи

В структуре **Device Tree** с устройством **a100-wdt** ассоциирован
набор определенных свойств этого устройства. Среди этого набора присутствует
свойство **led-gpios**, которое представляет собой массив сигналов GPIO.
Наличие свойства **led-gpios** указывает на то, что перечисленные в массиве
сигналы GPIO являются частью устройства и доступны при работе с этим
устройством. Обращение из драйвера к свойству устройства осуществляется
посредством использования функционального префикса **CONSUMER_ID**. В данном
случае функциональным префиксом **CONSUMER_ID** является строка **"led"**.

## Примеры для усложненной задачи

1. Получение сигнала GPIO из структуры **Device Tree** с индексом `gpio_index`.

    ```c
    #include <linux/gpio.h>
    #include <linux/err.h>

    #define CONSUMER_ID "led"

    static int a100_wdt_probe(struct platform_device *pdev) {
            struct gpio_desc *gpio;
            int gpio_index;

            /* ... */

            gpio = gpiod_get_index_optional(&pdev->dev,
                                            CONSUMER_ID,
                                            gpio_index,
                                            GPIOD_OUT_HIGH);
            if (IS_ERR(gpio)) {
                    /* Handle error here. */
            }

            /* ... */
    }
    ```

2. Освобождение сигнала GPIO.

    ```c
    struct gpio_desc *gpio;

    gpiod_put(gpio);
    ```

3. Чтение сигнала GPIO в `gpio_value`.

    ```c
    struct gpio_desc *gpio;
    int gpio_value;

    gpio_value = gpiod_get_value_cansleep(gpio);
    ```

4. Запись сигнала GPIO из `gpio_value`.

    ```c
    struct gpio_desc *gpio;
    int gpio_value;

    gpiod_set_value_cansleep(gpio, gpio_value);
    ```

# Наблюдение результатов

Для загрузки драйвера в ядро ОС необходимо воспользоваться командой:

```console
$ insmod <path_to_driver>
```

Для выгрузки драйвера из ядра ОС необходимо воспользоваться командой:

```console
$ rmmod <driver_name>
```

Например, если файл драйвера с именем **a100-wdt.ko** находится в директории
*/home/root/*, то команды для загрузки и выгрузки драйвера будут следующими:

```console
$ insmod /home/root/a100-wdt.ko
$ rmmod a100-wdt
```

Для просмотра загруженных в ядро ОС модулей, в т.ч. драйверов,
необходимо воспользоваться командой:

```console
$ lsmod
```

Символьные устройства находятся в директории */dev/*, например:
*/dev/a100_wdt_device_0_timer_0* или */dev/a100_wdt_device_0_gpio_5*.

Для чтения данных из символьного устройства необходимо воспользоваться командой:

```console
$ cat <path_to_character_device>
```

Для записи данных в символьное устройство необходимо воспользоваться командой:

```console
$ echo <value> > <path_to_character_device>
```
