
# Базовая задача

Разработать драйвер аппаратного таймера, управляемый через атрибуты файловой
системы **sysfs**. Атрибуты должны давать возможность устанавливать верхнюю
границу (порог) счетчика аппаратного таймера и отображать его текущее значение.

## Описание базовой задачи

Необходимо поддержать следующие атрибуты файловой системы **sysfs**:

- **maximum_count** - установка и отображение верхней границы счетчика
  аппаратного таймера (чтение и запись);
- **current_count** - отображение текущего значения счетчика
  аппаратного таймера (чтение).

Для выполнения поставленной задачи потребуется работа с регистрами процессора
**Centillium Atlanta 100**. В работе будут задействованы следующие регистры:

- **Timer Configuration Register** - общие настройки таймеров;

    Автоматический сброс счетчика аппаратного таймера Timer 0
    при достижении порога Counter 0.

    | Биты | Значение |    Описание    |
    |:----:|:--------:|:--------------:|
    |  20  |     0    | сброс выключен |
    |  20  |     1    |  сброс включен |

    Временной интервал шага счетчика аппаратного таймера Timer 0.

    |  Биты | Значение | Описание |
    |:-----:|:--------:|:--------:|
    | 17-16 |    00    |   1 мкс  |
    | 17-16 |    01    |  10 мкс  |

- **Interrupt Status/Clear Register** - подтверждение обработки прерываний;

    Подтверждение обработки прерывания для Timer 0 и порога Counter 0.

    | Биты | Значение |          Описание          |
    |:----:|:--------:|:--------------------------:|
    |   2  |     0    | прерывание не подтверждено |
    |   2  |     1    |   прерывание подтверждено  |

- **Interrupt Enable Register** - генерация прерываний;

    Генерация прерывания для Timer 0 при достижении порога Counter 0.

    | Биты | Значение |          Описание          |
    |:----:|:--------:|:--------------------------:|
    |   2  |     0    | прерывание не генерируется |
    |   2  |     1    |   прерывание генерируется  |

- **Timer 0 Compare 0 Register** - верхняя граница счетчика (порог)
    Counter 0 для Timer 0;

    | Биты |     Значение    |   Описание   |
    |:----:|:---------------:|:------------:|
    | 15-0 | 0x0000 - 0xFFFF | предел счета |

- **Timer 0 Counter Register** - текущее значение счетчика
    аппаратного таймера Timer 0.

    | Биты |     Значение    |     Описание     |
    |:----:|:---------------:|:----------------:|
    | 15-0 | 0x0000 - 0xFFFF | текущее значение |

Смещения регистров относительно начала виртуального адресного пространства:

|               Регистр               | Смещение |
|:-----------------------------------:|:--------:|
|   **Timer Configuration Register**  |  0x0000  |
| **Interrupt Status/Clear Register** |  0x0004  |
|    **Interrupt Enable Register**    |  0x000C  |
|    **Timer 0 Compare 0 Register**   |  0x0014  |
|     **Timer 0 Counter Register**    |  0x0018  |

## Алгоритм решения базовой задачи

1. Инициализировать модуль и зарегистрировать драйвер.

2. Получить доступ к виртуальному адресному пространству таймера.

3. Включить автоматический сброс счетчика аппаратного таймера Timer 0 при
   достижении верхней границы счетчика Counter 0. Установить временной интервал
   шага счетчика аппаратного таймера Timer 0 равным **10 мкс**.
   Инициализировать верхнюю границу счетчика Counter 0 аппаратного таймера
   Timer 0 значением **0xFFFF**.

4. Создать группу атрибутов файловой системы **sysfs**. При чтении атрибутов
   пользователем необходимо осуществить запись запрашиваемого значения в буфер,
   ассоциированный с атрибутом. При записи значений в атрибуты пользователем
   необходимо прочитать значение, находящееся в буфере, ассоциированном с
   атрибутом и применить его к регистрам процессора **Centillium Atlanta 100**.

5. Освободить ресурсы, отменить регистрацию драйвера, деинициализировать модуль.

## Примеры для базовой задачи

1. Инициализация модуля и регистрация драйвера.
   Деинициализация модуля и отмена регистрации драйвера.

    ```c
    #include <linux/module.h>
    #include <linux/platform_device.h>
    #include <linux/of.h>

    #define DEVICE_DRIVER_NAME "a100_wdt"

    static int a100_wdt_probe(struct platform_device *pdev);
    static int a100_wdt_remove(struct platform_device *pdev)

    static const struct of_device_id a100_wdt_of_match[] = {
            { .compatible = "ctlm,a100-wdt" },
            {}
    };

    static struct platform_driver a100_wdt_drv = {
            .driver = {
                    .owner = THIS_MODULE,
                    .name = DEVICE_DRIVER_NAME,
                    .of_match_table = a100_wdt_of_match,
            },
            .probe = a100_wdt_probe,
            .remove = a100_wdt_remove,
    };

    static int a100_wdt_init(void)
    {
            return platform_driver_register(&a100_wdt_drv);
    }

    static void a100_wdt_exit(void)
    {
            platform_driver_unregister(&a100_wdt_drv);
    }

    module_init(a100_wdt_init);
    module_exit(a100_wdt_exit);
    ```

2. Динамическое выделение памяти, инициализированной значением `0`.

    ```c
    #include <linux/slab.h>

    struct a100_wdt {
            struct platform_device *pdev;
    };

    struct a100_wdt *wdt_data;

    wdt_data = kzalloc(sizeof(struct a100_wdt), GFP_KERNEL);
    if (wdt_data == NULL) {
            /* Handle error here. */
    }
    ```

3. Освобождение динамически выделенной памяти `wdt_data`.

   ```c
    #include <linux/slab.h>

    struct a100_wdt *wdt_data;

    kfree(wdt_data);
    ```

4. Получение информации о ресурсах устройства `pdev`.

    ```c
    #include <linux/platform_device.h>

    struct resource *res;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL) {
            /* Handle error here. */
    }
    ```

5. Отображение физической области памяти в виртуальную область памяти.

    ```c
    #include <linux/platform_device.h>
    #include <linux/io.h>

    struct resource *res;
    void __iomem *base;

    base = ioremap(res->start, resource_size(res));
    if (base == NULL) {
            /* Handle error here. */
    }
    ```

6. Отмена отображения физической области памяти в виртуальную область памяти.

    ```c
    #include <linux/platform_device.h>
    #include <linux/io.h>

    void __iomem *base;

    iounmap(base);
    ```

7. Ассоциирование пользовательских данных `wdt_data` с устройством `pdev`.

    ```c
    #include <linux/platform_device.h>

    platform_set_drvdata(pdev, wdt_data);
    ```

8. Получение пользовательских данных `wdt_data` из устройства `pdev`.

    ```c
    #include <linux/platform_device.h>

    struct a100_wdt *wdt_data;

    wdt_data = platform_get_drvdata(pdev);
    ```

9. Получение пользовательских данных `wdt_data` из устройства `dev`.

    ```c
    #include <linux/platform_device.h>

    struct platform_device *pdev;
    struct a100_wdt *wdt_data;

    pdev = to_platform_device(dev);
    wdt_data = platform_get_drvdata(pdev);

10. Чтение из регистра.

    ```c
    #include <linux/io.h>

    #define A100_TIMERS_T0_COUNTER 0x0018

    void __iomem *base;
    u32 reg_value;

    reg_value = readl(base + A100_TIMERS_T0_COUNTER);
    ```

11. Запись в регистр.

    ```c
    #include <linux/io.h>

    #define A100_TIMERS_T0_COUNTER 0x0018

    void __iomem *base;
    u32 reg_value;

    reg_value = 0x0;
    writel(reg_value, base + A100_TIMERS_T0_COUNTER);
    ```

12. Изменение бита под номером `n` на значение `bit_value` в регистре.

    ```c
    #include <linux/io.h>

    #define A100_TIMERS_T0_C0 0x0014

    void __iomem *base;
    u32 reg_value;
    u32 bit_value;

    reg_value = readl(base + A100_TIMERS_T0_C0);

    bit_value = 1;

    if (bit_value == 1) {
        reg_value |= BIT(n);
        writel(reg_value, base + A100_TIMERS_T0_C0);
    }

    if (bit_value == 0) {
        reg_value &= ~BIT(n);
        writel(reg_value, base + A100_TIMERS_T0_C0);
    }
    ```

13. Создание группы атрибутов файловой системы **sysfs** для устройства `pdev`.

    ```c
    #include <linux/platform_device.h>
    #include <linux/sysfs.h>

    #define TIMER_0_GRP_NAME "timer-0"

    static ssize_t maximum_count_show(struct device *dev,
                                      struct device_attribute *attr,
                                      char *buf);

    static ssize_t maximum_count_store(struct device *dev,
                                       struct device_attribute *attr,
                                       const char *buf,
                                       size_t count);

    static ssize_t current_count_show(struct device *dev,
                                      struct device_attribute *attr,
                                      char *buf);

    static DEVICE_ATTR_RW(maximum_count);
    static DEVICE_ATTR_RO(current_count);

    static struct attribute *timer_0_attrs[] = {
            &dev_attr_maximum_count.attr,
            &dev_attr_current_count.attr,
            NULL
    };

    static const struct attribute_group timer_0_group = {
            .name  = TIMER_0_GRP_NAME,
            .attrs  = timer_0_attrs
    };

    static int a100_wdt_probe(struct platform_device *pdev)
    {
            int ret;

            ret = sysfs_create_group(&pdev->dev.kobj, &timer_0_group);
            if (ret < 0) {
                    /* Handle error here. */
            }

            return 0;
    }
    ```

    Стоит отметить, что функцию `sysfs_create_group()` необходимо вызывать
    в конце функции `probe()`. Это связано с тем обстоятельством, что после
    возврата из функции `sysfs_create_group()` в файловой системе **sysfs**
    появляются соответствующие атрибуты, при этом обрабатывать эти атрибуты
    можно только после полной инициализации драйвера для устройства `pdev`.

14. Удаление группы атрибутов файловой системы **sysfs** для устройства `pdev`.

    ```c
    #include <linux/platform_device.h>
    #include <linux/sysfs.h>

    #define TIMER_0_GRP_NAME "timer-0"

    static const struct attribute_group timer_0_group;

    sysfs_remove_group(&pdev->dev.kobj, &timer_0_group);
    ```

15. Запись целочисленного значения в атрибут файловой системы **sysfs**.

    ```c
    static ssize_t attr_show(struct device *dev,
                             struct device_attribute *attr,
                             char *buf)
    {
            u32 reg_value;

            reg_value = 0xFFFF;

            return scnprintf(buf, PAGE_SIZE, "0x%08X\n", reg_value);
    }
    ```

16. Чтение целочисленного значения из атрибута файловой системы **sysfs**.

    ```c
    static ssize_t attr_store(struct device *dev,
                              struct device_attribute *attr,
                              const char *buf,
                              size_t count)
    {
            u32 reg_value;

            ret = kstrtoul(buf, 0, &reg_value);
            if (ret < 0) {
                    /* Handle error here. */
                    return ret;
            }

            return count;
    }
    ```

# Усложненная задача

В дополнение к базовой задаче реализовать обработку аппаратного прерывания,
которое таймер генерирует при достижении верхней границы (порога) счетчика.
В файловой системе **sysfs** должен быть реализован дополнительный атрибут
для отображения общего количества обработанных прерываний, полученных от
аппаратного таймера.

## Описание усложненной задачи

Необходимо поддержать дополнительный атрибут файловой системы **sysfs**:

- **interrupt_count** - отображение общего количества обработанных прерываний,
  полученных от аппаратного таймера (чтение).

## Рекомендации по выполнению усложненной задачи

Для работы с прерываниями их необходимо разрешить. Это можно сделать установив
**2-й** бит регистра **Interrupt Enable Register** в логическую **1**. Также
стоит отметить, что после обработки прерывания его необходимо подтвердить.
Подтверждение прерывания осуществляется путем установки **2-го** бита регистра **Interrupt Status/Clear Register** в логическую **1**. Подтверждать необходимо
каждое обработанное прерывание.

## Примеры для усложненной задачи

1. Регистрация обработчика прерываний для устройства `pdev`.

    ```c
    #include <linux/interrupt.h>

    #define DEV_NAME "a100-wdt"

    struct a100_wdt *wdt_data;
    int irq;
    int ret;

    static irqreturn_t a100_wdt_isr(int irq, void *data)
    {
            struct a100_wdt *wdt_data;

            wdt_data = data;

            return IRQ_HANDLED;
    }

    irq = platform_get_irq(pdev, 1);
    if (irq < 0) {
            /* Handle error here. */
    }

    ret = request_irq(irq, a100_wdt_isr, 0, DEV_NAME, wdt_data);
    if (ret < 0) {
            /* Handle error here. */
    }
    ```

2. Отмена регистрации обработчика прерываний для устройства `pdev`.

    ```c
    #include <linux/interrupt.h>

    struct a100_wdt *wdt_data;
    int irq;

    free_irq(irq, wdt_data);
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

Атрибуты файловой системы **sysfs** разрабатываемого драйвера имеют путь:
*/sys/devices/platform/soc/63c0000.<driver_name>/<group_name>/<attribute_name>*.
Например, если драйвер с именем **a100-wdt** имеет атрибут **maximum_count**,
который находится в группе атрибутов с именем **timer-0**, то вышеуказанный
путь примет вид:
*/sys/devices/platform/soc/63c0000.a100-wdt/timer-0/current_count*.

Для чтения атрибута файловой системы **sysfs** разрабатываемого драйвера
необходимо воспользоваться командой:

```console
$ cat <path_to_attribute>
```

Для записи в атрибут файловой системы **sysfs** разрабатываемого драйвера
необходимо воспользоваться командой:

```console
$ echo <value> > <path_to_attribute>
```
