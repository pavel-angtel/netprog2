
# Базовая задача

Разработать драйвер для управления сигналами GPIO через команды **ioctl**.
Драйвер должен поддерживать команды чтения и записи состояния выходных
сигналов, а также уведомлять приложение об изменении состояния
входных сигналов. Драйвер узнает об изменении состояния сигнала через
аппаратное прерывание.

## Описание базовой задачи

Необходимо поддержать следующие символьные устройства:

- **a100_wdt_device_0_led_N** - установка и отображение сигнала
  LED GPIO через **ioctl**, где **N** принимает значения от **0** до **7**;
- **a100_wdt_device_0_digital_in_N** - получение уведомления об изменении
  состояния DIGITAL_IN GPIO, где **N** принимает значения от **0** до **1**;

Для управления устройствами через команды **ioctl** предоставляется готовое
пользовательское приложение. Пользовательское приложение и драйвер должны
использовать согласованное представление о командах и аргументах системного
вызова [ioctl(2)][1]. В связи с этим и драйвер и пользовательское приложение
должны включать одинаковый заголовочный файл:

```c
#ifndef _IOCTL_CMDS_H
#define _IOCTL_CMDS_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define IOC_MAGIC 0xFF

enum operation {
        READ_OPERATION,
        WRITE_OPERATION
};

struct rw_command {
        enum operation operation;
        int value;
};

#define IOC_RW_GPIO     _IOW(IOC_MAGIC, 0, struct rw_command)
#define IOC_BLINK_GPIO  _IOW(IOC_MAGIC, 1, int)

#endif
```

## Алгоритм решения базовой задачи

1. Инициализировать модуль. При инициализации модуля создать символьные
   устройства в пространстве ядра ОС. Для создаваемых символьных устройств
   указать в качестве доступных файловых операций (среди прочих) операции
   **unlocked_ioctl** и **poll**. Зарегистрировать драйвер.

2. Создать файлы символьных устройств в пространстве пользователя ОС.

3. Освободить ресурсы, отменить регистрацию драйвера, деинициализировать модуль.

[1]: https://www.opennet.ru/man.shtml?topic=ioctl&category=2
[2]: https://www.opennet.ru/man.shtml?topic=poll&category=2&russian=0
[3]: https://www.opennet.ru/man.shtml?topic=select&category=2&russian=0

## Примеры для базовой задачи

1. Инициализация символьных устройств, добавляемых в ядро ОС.

    ```c
    #include <linux/cdev.h>

    static int a100_wdt_open(struct inode *inode,
                             struct file *file);

    static int a100_wdt_release(struct inode *inode,
                                struct file *file);

    static long a100_wdt_ioctl(struct file *file,
                               unsigned int cmd,
                               unsigned long arg);

    static unsigned int a100_wdt_poll(struct file *file,
                                      poll_table *wait);

    static struct cdev a100_wdt_cdev;

    const struct file_operations a100_wdt_fops = {
            .owner = THIS_MODULE,
            .open = a100_wdt_open,
            .release = a100_wdt_release,
            .unlocked_ioctl = a100_wdt_ioctl,
            .poll = a100_wdt_poll
    };

    static int a100_wdt_init(void) {
            cdev_init(&a100_wdt_cdev, &a100_wdt_fops);
    }
    ```

   Функция, соответствующая операции **unlocked_ioctl**, будет вызвана при
   выполнении команды через системный вызов [ioctl(2)][1] пользовательским
   приложением. Функция, соответствующая операции **poll**, будет вызвана при
   выполнении системных вызовов [poll(2)][2] или [select(2)][3]
   пользовательским приложением.

2. Получение сигналов GPIO из структуры **Device Tree** для LED и DIGITAL_IN.

    ```c
    #include <linux/gpio.h>
    #include <linux/err.h>

    #define CONSUMER_ID_LEDS "led"
    #define CONSUMER_ID_DIGITAL_INS "digital-in"
    #define LEDS_PER_DEVICE 8
    #define ATTRIBUTES_PER_DEVICE 10

    static int a100_wdt_probe(struct platform_device *pdev) {
            int i;

            for (i = 0; i < ATTRIBUTES_PER_DEVICE; i++) {
                    struct gpio_desc *gpio;

                    if (i < LEDS_PER_DEVICE) {
                            gpio = gpiod_get_index_optional(&pdev->dev,
                                                            CONSUMER_ID_LEDS,
                                                            i,
                                                            GPIOD_OUT_HIGH);
                    } else {
                            gpio = gpiod_get_index_optional(&pdev->dev,
                                                            CONSUMER_ID_DIGITAL_INS,
                                                            i - LEDS_PER_DEVICE,
                                                            GPIOD_IN);
                    }
                    if (IS_ERR(gpio)) {
                            /* Handle error here. */
                    }
            }

    }
    ```

3. Освобождение сигнала GPIO.  
   См. Лабораторная работа №10. Усложненная задача. Пример №2.

4. Регистрация обработчика прерываний для сигнала GPIO с описанием `gpio`.

    ```c
    #include <linux/gpio.h>
    #include <linux/interrupt.h>

    #define DEVICE_NAME "a100-wdt"

    struct a100_wdt_attribute {
            /* ... */
            int irq;
            int irq_count;
    };

    static irqreturn_t a100_wdt_isr(int irq, void *data)
    {
            struct a100_wdt_attribute *a100_wdt_attribute_data = data;

            a100_wdt_attribute_data->irq_count++;

            /* ... */

            return IRQ_HANDLED;
    }

    static int a100_wdt_probe(struct platform_device *pdev) {
            struct a100_wdt_attribute *a100_wdt_attribute_data;
            struct gpio_desc *gpio;
            int irq;

            irq = gpiod_to_irq(gpio);
            if (irq < 0) {
                    /* Handle error here. */
            }
            a100_wdt_attribute_data->irq = irq;
            a100_wdt_attribute_data->irq_count = 0;

            ret = request_irq(irq,
                              a100_wdt_isr,
                              0,
                              DEVICE_NAME,
                              a100_wdt_attribute_data);
            if (irq < 0) {
                    /* Handle error here. */
            }
    }
    ```

5. Отмена регистрации обработчика прерываний для сигнала GPIO.

    ```c
    #include <linux/interrupt.h>

    struct a100_wdt_attribute *a100_wdt_attribute_data;

    free_irq(a100_wdt_attribute_data->irq, a100_wdt_attribute_data);
    ```

6. Обработка пользовательской команды `cmd` с аргументом `arg`.

    ```c
    #include <linux/gpio.h>
    #include <linux/uaccess.h>

    struct a100_wdt_attribute {
            /* ... */
            struct gpio_desc *gpio;
    };

    static long a100_wdt_ioctl(struct file *file,
                               unsigned int cmd,
                               unsigned long user_arg)
    {
        struct a100_wdt_attribute *a100_wdt_attribute_data;
        struct rw_command rw_command;
        int ret;

        a100_wdt_attribute_data = file->private_data;

        switch (cmd) {
        case IOC_RW_GPIO:
            ret = copy_from_user(&rw_command,
                                 (struct rw_command *) user_arg,
                                 sizeof(struct rw_command));
            if (ret != 0) {
                    /* Handle error here. */
            }

            if (rw_command.operation == WRITE_OPERATION) {
                if (rw_command.value != 0 && rw_command.value != 1) {
                        /* Handle error here. */
                }
                gpiod_set_value_cansleep(a100_wdt_attribute_data->gpio,
                                         rw_command.value);
                break;
            }

            if (rw_command.operation == READ_OPERATION) {
                rw_command.value = gpiod_get_value_cansleep(
                        a100_wdt_attribute_data->gpio);
                ret = copy_to_user((struct rw_command *) user_arg,
                                   &rw_command,
                                   sizeof(struct rw_command));
                if (ret != 0) {
                        /* Handle error here. */
                }

                break;
            }
        default:
                /* Handle error here. */
                return -ENOTTY;
        }

        return 0;
    }
    ```

7. Инициализация очереди ожидания событий `wait_queue`.

    ```c
    #include <linux/wait.h>
    #include <linux/poll.h>

    struct a100_wdt_attribute {
            /* ... */
            wait_queue_head_t wait_queue;
            int data_ready;
    };

    static int a100_wdt_probe(struct platform_device *pdev) {
            struct a100_wdt_attribute *a100_wdt_attribute_data;

            /* ... */

            init_waitqueue_head(&a100_wdt_attribute_data->wait_queue);
            a100_wdt_attribute_data->data_ready = 0;
    }
    ```

8. Добавление файлового открытия `file` в очередь ожидания событий `wait_queue`.

    ```c
    #include <linux/wait.h>
    #include <linux/poll.h>

    struct a100_wdt_attribute {
            /* ... */
            wait_queue_head_t wait_queue;
            int data_ready;
    };

    static unsigned int a100_wdt_poll(struct file *file, poll_table *wait)
    {
            struct a100_wdt_attribute *a100_wdt_attribute_data;

            a100_wdt_attribute_data = file->private_data;

            poll_wait(file, &a100_wdt_attribute_data->wait_queue, wait);

            /* The event has occurred. */
            if (a100_wdt_attribute_data->data_ready)
                    return POLLIN;

            /* The event did not occur. */
            return 0;
    }
    ```

9. Установление готовности (событие poll) по прерыванию IRQ.

    ```c
    #include <linux/wait.h>
    #include <linux/poll.h>

    struct a100_wdt_attribute {
            /* ... */
            wait_queue_head_t wait_queue;
            int data_ready;
    };

    static irqreturn_t a100_wdt_isr(int irq, void *data)
    {
            struct a100_wdt_attribute *a100_wdt_attribute_data = data;

            /* ... */

            a100_wdt_attribute_data->data_ready = 1;
            wake_up_interruptible(&a100_wdt_attribute_data->wait_queue);

            return IRQ_HANDLED;
    }
    ```

10. Отмена установления готовности (событие poll) по прерыванию IRQ.

    ```c
    #include <linux/wait.h>
    #include <linux/poll.h>

    struct a100_wdt_attribute {
            /* ... */
            wait_queue_head_t wait_queue;
            int data_ready;
    };

    static long a100_wdt_ioctl(struct file *file,
                               unsigned int cmd,
                               unsigned long user_arg)
    {
            struct a100_wdt_attribute *a100_wdt_attribute_data;
            struct rw_command rw_command;

            a100_wdt_attribute_data = file->private_data;

            switch (cmd) {
            case IOC_RW_GPIO:
                    /* ... */

                    if (rw_command.operation == WRITE_OPERATION) {
                            /* ... */
                            break;
                    }

                    if (rw_command.operation == READ_OPERATION) {
                            /* ... */

                            a100_wdt_attribute_data->data_ready = 0;

                            break;
                    }
            default:
                    /* Handle error here. */
                    return -ENOTTY;
            }

            return 0;
    }
    ```

# Усложненная задача

В дополнение к базовой задаче реализовать команду для перевода желтых
светодиодов в состояние периодического мигания с настраиваемым периодом.
Мигание должно быть реализовано с помощью механизма отложенной работы
ядра Linux с миллисекундной точностью.

## Примеры для усложненной задачи

1. Инициализация отложенной работы `work`.

    ```c
    #include <linux/workqueue.h>

    struct a100_wdt_attribute {
            /* ... */
            struct delayed_work work;
            unsigned long delay;
    };

    static void a100_wdt_reverse_gpio(struct work_struct *work)
    {
            struct a100_wdt_attribute *a100_wdt_attribute_data;

            a100_wdt_attribute_data = container_of(to_delayed_work(work),
                                                   struct a100_wdt_attribute,
                                                   work);

            /* ... */

            schedule_delayed_work(&a100_wdt_attribute_data->work,
                                  a100_wdt_attribute_data->delay);
    }

    static int a100_wdt_probe(struct platform_device *pdev) {
            struct a100_wdt_attribute *a100_wdt_attribute_data;

            /* ... */

            INIT_DELAYED_WORK(&a100_wdt_attribute_data->work,
                              a100_wdt_reverse_gpio);
            a100_wdt_attribute_data->delay = 0;
    }
    ```

2. Перевод задержки из миллисекунд в количество импульсов системного таймера.

    ```c
    #include <linux/workqueue.h>

    int blink_interval;
    unsigned long delay;

    delay = msecs_to_jiffies(blink_interval);
    ```

3. Добавление отложенной работы `work` в системную очередь.

    ```c
    #include <linux/workqueue.h>

    struct delayed_work *work;
    unsigned long delay;

    schedule_delayed_work(work, delay);
    ```

4. Удаление отложенной работы `work` из системной очереди.

    ```c
    #include <linux/workqueue.h>

    struct delayed_work *work;

    cancel_delayed_work_sync(work);
    ```

   При выгрузке устройства в функции `a100_wdt_remove()` необходимо отменить
   всю имеющуюся для него в системной очереди работу.

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
*/dev/a100_wdt_device_0_led_0* или */dev/a100_wdt_device_0_digital_in_0*.

Команды устройству подаются с помощью пользовательского приложения:

```console
$ ./lab_11_at_app_mips --help
```
