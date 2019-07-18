
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

1. Инициализировать модуль. При инициализации модуля создать символьное
   устройство в пространстве ядра ОС. Зарегистрировать драйвер.

2. Получить доступ к виртуальному адресному пространству таймера.

3. Включить автоматический сброс счетчика аппаратного таймера Timer 0 при
   достижении верхней границы счетчика Counter 0. Установить временной интервал
   шага счетчика аппаратного таймера Timer 0 равным **10 мкс**.
   Инициализировать верхнюю границу счетчика Counter 0 аппаратного таймера
   Timer 0 значением **0xFFFF**.

4. Создать символьной устройство в пространстве пользователя ОС.
   При чтении данных из символьного устройства необходимо осуществить запись
   запрашиваемых данных в буфер, ассоциированный с символьным устройством.
   Данные при этом копируются из пространства ядра ОС в пространство
   пользователя ОС. При записи данных в символьное устройство необходимо
   прочитать записываемые данные, находящееся в буфере, ассоциированном
   с символьным устройством и применить их к регистрам процессора
   **Centillium Atlanta 100**. Данные в этом случае копируются из пространства
   пользователя ОС в пространство ядра ОС.

5. Освободить ресурсы, отменить регистрацию драйвера, деинициализировать модуль.

## Примеры для базовой задачи

1. Регистрация диапазона номеров для символьных устройств, добавляемых в
   ядро ОС, начиная с номера `baseminor`.

    ```c
    #include <linux/fs.h>

    #define MAX_DEVICES 1
    #define PARAMETERS_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * PARAMETERS_PER_DEVICE)
    #define DEVICE_NAME "a100-wdt"

    int ret;
    dev_t dev;
    unsigned int baseminor;

    baseminor = 0;

    ret = alloc_chrdev_region(&dev, baseminor, MAX_CHAR_DEVICES, DEVICE_NAME);
    if (ret < 0) {
            /* Handle error here. */
    }
    ```

2. Освобождение диапазона номеров для символьных устройств, добавляемых в
   ядро ОС.

   ```c
    #include <linux/fs.h>

    #define MAX_DEVICES 1
    #define PARAMETERS_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * PARAMETERS_PER_DEVICE)

    dev_t dev;

    unregister_chrdev_region(dev, MAX_CHAR_DEVICES);
    ```

3. Инициализация символьных устройств, добавляемых в ядро ОС.

    ```c
    #include <linux/cdev.h>

    static int a100_wdt_open(struct inode *inode, struct file *file);
    static int a100_wdt_release(struct inode *inode, struct file *file);

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

    cdev_init(&a100_wdt_cdev, &a100_wdt_fops);
    ```

4. Добавление символьных устройств в ядро ОС.

   ```c
    #include <linux/cdev.h>

    #define MAX_DEVICES 1
    #define PARAMETERS_PER_DEVICE 1
    #define MAX_CHAR_DEVICES (MAX_DEVICES * PARAMETERS_PER_DEVICE)

    static struct cdev a100_wdt_cdev;

    int ret;
    dev_t dev;

    ret = cdev_add(&a100_wdt_cdev, dev, MAX_CHAR_DEVICES);
    if (ret < 0) {
            /* Handle error here. */
    }
    ```

5. Удаление символьных устройств из ядра ОС.

   ```c
    #include <linux/cdev.h>

    static struct cdev a100_wdt_cdev;

    cdev_del(&a100_wdt_cdev);
    ```

6. Создание класса устройств в **sysfs**.

   ```c
    static struct class *a100_wdt_class = NULL;

    a100_wdt_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (a100_wdt_class == NULL) {
            /* Handle error here. */
    }
    ```

7. Удаление класса устройств из **sysfs**.

   ```c
    static struct class *a100_wdt_class;

    class_destroy(a100_wdt_class);
    ```

8. Добавление устройств класса `a100_wdt_class`.

   ```c
    <linux/kdev_t.h>

    static struct cdev a100_wdt_cdev;

    struct device *device;
    int device_index;
    int parameter_index;

    device = NULL;
    device_index = 0;
    parameter_index = 0;

    device = device_create(a100_wdt_class,
                           NULL,
                           MKDEV(MAJOR(a100_wdt_cdev.dev),
                           parameter_index),
                           NULL,
                           "a100_wdt_device_%d_timer_%d",
                           device_index,
                           device_parameter_index);
    if (device == NULL) {
            /* Handle error here. */
    }
    ```

9. Удаление устройств класса `a100_wdt_class`.

   ```c
    <linux/kdev_t.h>

    static struct cdev a100_wdt_cdev;

    int parameter_index;

    parameter_index = 0;

    device_destroy(a100_wdt_class,
                   MKDEV(MAJOR(a100_wdt_cdev.dev), parameter_index));
    ```

10. Получение младшего номера устройства из `struct inode *inode`.

    ```c
    struct inode *inode
    int minor;

    minor = iminor(inode);
    ```

11. Ассоциирование данных с открытым символьным устройством.

    ```c
    struct file *file
    void *data;

    file->private_data = data;
    ```

12. Копирование данных из пространства ядра ОС в пространство пользователя ОС.

    ```c
    static ssize_t a100_wdt_read(struct file *file,
                                 char __user *user_buffer,
                                 size_t user_size,
                                 loff_t *offset)
    {
            char data[] = "test";
            size_t data_size;
            int ret;

            data_size = strlen(data) + 1;

            ret = copy_to_user(user_buffer, data, data_size);
            if (ret != 0) {
                    /* Handle error here. */
            }

            return data_size;
    }
    ```

13. Копирование данных из пространства пользователя ОС в пространство ядра ОС.

    ```c
    #define PARAMETER_DATA_SIZE 256

    static ssize_t a100_wdt_write(struct file *file,
                                  const char __user *user_buffer,
                                  size_t user_size,
                                  loff_t *offset)
    {
            char data[PARAMETER_DATA_SIZE];

            ret = copy_from_user(data, user_buffer, user_size);
            if (ret != 0) {
                    /* Handle error here. */
            }

            return user_size;
    }
    ```

# Усложненная задача

Разработать драйвер для установки и считывания состояния сигналов GPIO с
управлением через набор символьных устройств. Доступные драйверу сигналы GPIO
извлекаются из структуры Device Tree. Установка и считывание состояний выводов
осуществляется через подсистему **gpiolib**. Каждый сигнал управляется через
отдельное символьное устройство. Запись в устройство устанавливает значение
выходного сигнала. Чтение возвращает значение входного сигнала.

## Описание усложненной задачи

Необходимо поддержать следующие символьные устройства:

- **a100_wdt_device_0_gpio_(0-7)** - установка и отображение сигнала
  GPIO (чтение и запись);

## Рекомендации по выполнению усложненной задачи

TODO

## Примеры для усложненной задачи

1. Получение сигнала GPIO из структуры Device Tree с индексом `0`.

    ```c

    #define CONSUMER_ID "led"

    struct platform_device *pdev;
    struct gpio_desc *gpio;
    int device_parameter_index;

    device_parameter_index = 0;

    gpio = gpiod_get_index_optional(&pdev->dev,
                                    CONSUMER_ID,
                                    device_parameter_index,
                                    GPIOD_OUT_HIGH);
    if (gpio == NULL) {
            /* Handle error here. */
    }
    ```

2. Освобождение сигнала GPIO.

    ```c
    struct gpio_desc *gpio;

    gpiod_put(gpio);
    ```

3. Чтение сигнала GPIO.

    ```c
    struct gpio_desc *gpio;
    int gpio_value;

    gpio_value = gpiod_get_value_cansleep(gpio);
    ```

4. Запись сигнала GPIO.

    ```c
    struct gpio_desc *gpio;
    int gpio_value;

    gpio_value = 0;

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
*/home/root*, то команды для загрузки и выгрузки драйвера будут следующими:

```console
$ insmod /home/root/a100-wdt.ko
$ rmmod a100-wdt
```

Для просмотра загруженных в ядро ОС модулей, в т.ч. драйверов,
необходимо воспользоваться командой:

```console
$ lsmod
```

Символьные устройства находятся в директории */dev*, например:
*/dev/a100_wdt_device_0_timer_0* или */dev/a100_wdt_device_0_gpio_5*.

Для чтения данных из символьного устройства необходимо воспользоваться командой:

```console
$ cat <path_to_character_device>
```

Для записи данных в в символьное устройство необходимо воспользоваться командой:

```console
$ echo <value> > <path_to_character_device>
```
