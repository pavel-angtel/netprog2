# Подготовка рабочего места

Содержание:

1. Установка и подготовка к работе ОС openSUSE Leap 15.1
   - 1.1 Создание загрузочного USB-флеш-накопителя
     - 1.1.1 Получение образа ОС openSUSE Leap 15.1
     - 1.1.2 Проверка полученного образа ОС openSUSE Leap 15.1
     - 1.1.3 Запись полученного образа ОС openSUSE Leap 15.1 на
             USB-флеш-накопитель
   - 1.2 Установка ОС openSUSE Leap 15.1
     - 1.2.1 Загрузка с USB-флеш-накопителя
     - 1.2.2 Конфигурирование параметров установки ОС openSUSE Leap 15.1
     - 1.2.3 Установка ПО и конфигурирование ОС openSUSE Leap 15.1
       - 1.2.2.1 Доступ в сеть Интернет
       - 1.2.2.2 Установка необходимого ПО
       - 1.2.2.3 Конфигурирование локального файлового сервера
       - 1.2.2.4 Конфигурирование TFTP-сервера
       - 1.2.2.5 Конфигурирование FTP-сервера
       - 1.2.2.6 Конфигурирование программы minicom
       - 1.2.2.7 Установка набора средств разработки (SDK)
2. Подключение устройства Корунд-2O-8E к ПК
   - 2.1 Состав и назначение портов устройства Корунд-2O-8E
   - 2.2 Подключение устройства Корунд-2O-8E к ПК

## 1. Установка и подготовка к работе ОС openSUSE Leap 15.1

Приводимые ниже шаги более подробно описаны в руководстве
[openSUSE Startup Guide][Section_1_Link_1].

### 1.1 Создание загрузочного USB-флеш-накопителя

Приводимые ниже шаги предполагают наличие уже установленной ОС openSUSE
в которой будут производится действия по созданию загрузочного
USB-флеш-накопителя.

#### 1.1.1 Получение образа ОС openSUSE Leap 15.1

Образ ОС openSUSE Leap 15.1 можно получить с официального сайта
[openSUSE Leap 15.1][Section_1.1.1_Link_1].

![Раздел 1.1.1 Рисунок 1][Section_1.1.1_Figure_1]

1. Выберите раздел **Installation x86_64**.
2. Кликнете по ссылке **DVD-Image**.
3. Кликнете по ссылке **Checksum**.

#### 1.1.2 Проверка полученного образа ОС openSUSE Leap 15.1

Полагаем, что файлы образа и контрольной суммы ОС openSUSE Leap 15.1
находятся в директории *~/Downloads/*.

Прежде всего, необходимо проверить цифровую подпись файла с контрольной суммой.
Для этого требуется:

1. Импортировать ключ с заданным идентификатором с доверенного сервера ключей:

    ```console
    gpg --recv-keys 0x22C07BA534178CD02EFE22AAB88B2FD43DBDC284
    ```

2. Удостовериться в том, что импортирован правильный ключ:

    ```console
    gpg --fingerprint "openSUSE Project Signing Key <opensuse@opensuse.org>"
    ```

   Отпечаток ключа должен совпадать с нижеуказанным:

    ```text
    22C0 7BA5 3417 8CD0 2EFE  22AA B88B 2FD4 3DBD C284
    ```

3. Осуществить проверку цифровой подписи файла с контрольной суммой:

    ```console
    cd ~/Downloads/
    gpg --verify openSUSE-Leap-15.1-DVD-x86_64.iso.sha256
    ```

    Отпечаток ключа должен совпадать с нижеуказанным:

    ```text
    22C0 7BA5 3417 8CD0 2EFE  22AA B88B 2FD4 3DBD C284
    ```

После проверки цифровой подписи файла с контрольной суммой необходимо вычислить контрольную сумму файла с образом ОС openSUSE Leap 15.1 и убедиться, что две
контрольные суммы совпадают.

```console
sha256sum -c openSUSE-Leap-15.1-DVD-x86_64.iso.sha256
```

В выводе приведенной выше команды должна содержаться строка:

```text
openSUSE-Leap-15.1-DVD-x86_64.iso: OK
```

#### 1.1.3 Запись полученного образа ОС openSUSE Leap 15.1 на USB-флеш-накопитель

Для записи загрузочного USB-флеш-накопителя используется программа
**SUSE Studio Image Writer**. Доступное для записи пространство на
USB-флеш-накопителе должно составлять не менее **4 GB**.

Перед запуском программу **SUSE Studio Image Writer** необходимо установить
с помощью команды:

```console
zypper install imagewriter
```

Для запуска программы введите в командной строке:

```console
imagewriter
```

![Раздел 1.1.3 Рисунок 1][Section_1.1.3_Figure_1]
![Раздел 1.1.3 Рисунок 2][Section_1.1.3_Figure_2]

**Внимание: В результате выполнения следующих шагов все имеющиеся на
USB-носителе данные будут утеряны!**

1. Выберите файл с образом ОС openSUSE Leap 15.1.
2. Выберите USB-флеш-накопитель.
3. Нажмите на кнопку **Write**.
4. Подтвердите перезапись данных. При необходимости воспользуйтесь паролем
   системного администратора.
5. По окончании записи извлеките USB-флеш-накопитель.

### 1.2 Установка ОС openSUSE Leap 15.1

#### 1.2.1 Загрузка с USB-флеш-накопителя

Вставьте USB-флеш-накопитель в USB-порт. Включите или перезагрузите ПК.
Во время загрузки ПК войдите в **UEFI**/**BIOS**, нажав клавишу **Delete**,
**F2**, **F12** и т.п. в зависимости от используемого ПК.

В **UEFI**/**BIOS** измените порядок загрузки (**Boot Priority**) таким образом,
чтобы загрузочный USB-флеш-накопитель оказался на первом месте или выберите
(если доступно) непосредственную загрузку с указанного USB-флеш-накопителя без
изменения порядка загрузки.

#### 1.2.2 Конфигурирование параметров установки ОС openSUSE Leap 15.1

![Раздел 1.2.2 Рисунок 1][Section_1.2.2_Figure_1]

После загрузки с USB-флеш-накопителя выберите пункт **Installation**.

![Раздел 1.2.2 Рисунок 2][Section_1.2.2_Figure_2]

В меню **Language, Keyboard and License Agreement**:

1. Выберите язык системы, используемый по умолчанию.
   Рекомендуется **English (US)**.
2. Выберите раскладку клавиатуры, используемую по умолчанию.
   Рекомендуется **English (US)**.
3. Нажмите на кнопку **Next**.

В случае появления меню **Network Settings**:

1. Нажмите на кнопку **Edit**.
2. Выберите пункт **Dynamic Address** (DHCP IPv4/IPv6).
3. Нажмите на кнопку **Next**.

![Раздел 1.2.2 Рисунок 3][Section_1.2.2_Figure_3]

Во всплывающем окне **YaST2** с вопросом **Activate online repositories now?**:

1. Нажмите на кнопку **Yes**.

![Раздел 1.2.2 Рисунок 4][Section_1.2.2_Figure_4]

В меню **List of Online Repositories**:

1. Удостоверьтесь, что выбраны репозитории:
   - Update Repository (Non-Oss)
   - Non-OSS Repository
   - Main Update Repository
   - Main Repository
2. Нажмите на кнопку **Next**.

![Раздел 1.2.2 Рисунок 5][Section_1.2.2_Figure_5]

В меню **System Role**:

1. Выберите графическое окружение. Рекомендуется **Desktop with KDE Plasma**.
2. Нажмите на кнопку **Next**.

![Раздел 1.2.2 Рисунок 6][Section_1.2.2_Figure_6]
![Раздел 1.2.2 Рисунок 7][Section_1.2.2_Figure_7]
![Раздел 1.2.2 Рисунок 8][Section_1.2.2_Figure_8]

В меню **Suggested Partitioning**:

1. Нажмите на кнопку **Guided Setup**.
2. Нажмите на кнопку **Next**.
3. Выберите для **Root Partition** тип файловой системы.
   Рекомендуется **Ext4**.
4. Отметьте пункт **Propose Separate Home Partition**.
5. Выберите для **Home Partition** тип файловой системы.
   Рекомендуется **Ext4**.
6. Отметьте пункт **Enlarge to RAM Size for Suspend**.
7. Нажмите на кнопку **Next**.
8. Нажмите на кнопку **Next**.

**Внимание: Убедитесь, что размер раздела Root Partition (/)
составляет не менее 60 GB!**

![Раздел 1.2.2 Рисунок 9][Section_1.2.2_Figure_9]

В меню **Clock and Time Zone**:

1. Выберите страну **Russia**.
2. Выберите город **Moscow**.
3. Нажмите на кнопку **Next**.

![Раздел 1.2.2 Рисунок 10][Section_1.2.2_Figure_10]

В меню **Local User**:

1. Введите фамилию и имя.
2. Введите имя пользователя.
3. Введите пароль. Данный пароль будет использоваться также в качестве
   пароля системного администратора если выбран пункт
   **Use this password for system administrator**.
   В качестве пароля используйте слово "student".
4. Подтвердите пароль.
5. Рекомендуется снять отметку с пункта **Automatic Login**.
6. Нажмите на кнопку **Next**.

В случае появления всплывающего окна **YaST2** с вопросом
**Really use this password?** нажмите на кнопку **Yes** или придумайте
более сложный пароль удовлетворяющий рекомендациям установщика.

![Раздел 1.2.2 Рисунок 11][Section_1.2.2_Figure_11]
![Раздел 1.2.2 Рисунок 12][Section_1.2.2_Figure_12]

**Внимание: В результате выполнения следующих шагов все имеющиеся на
жестком диске данные будут утеряны!**

В меню **Installation Settings**:

1. Внимательно изучите сводные параметры установки.
   В случае согласия с заданными параметрами нажмите на кнопку **Install**.
   В случае, если вы хотите изменить параметры нажмите на кнопку **Back**.
2. Подтвердите установку нажав на кнопку **Install**.

Дождитесь завершения установки и автоматической перезагрузки ПК.

В случае, если вы изменяли порядок загрузки (**Boot Priority**) в
**UEFI**/**BIOS**, то во время перезагрузки ПК войдите в **UEFI**/**BIOS** и
восстановите порядок загрузки к первоначальному виду.

После перезагрузки ПК извлеките USB-флеш-накопитель.

#### 1.2.3 Установка ПО и конфигурирование ОС openSUSE Leap 15.1

Для того, чтобы иметь возможность полноценно работать с ОС openSUSE Leap 15.1
и коммутатором **Корунд-2O-8E** требуется предварительное конфигурирование ОС
и установка дополнительного ПО.

![Раздел 1.2.3 Рисунок 1][Section_1.2.3_Figure_1]

Войдите в свою учетную запись, введя пароль, указанный на этапе установки.

Для администрирования ОС openSUSE Leap 15.1 используется программа **YaST**.
Чтобы запустить **YaST** используйте следующую последовательность:
**Application Launcher** -> **Applications** -> **System** ->
**Administrator Settings**. При запросе пароля введите пароль системного
администратора.

В качестве псевдотерминала (эмулятора терминала) используется программа
**Terminal**. Чтобы запустить **Terminal** используйте следующую
последовательность: **Application Launcher** -> **Applications** ->
**System** -> **Terminal**.

В **Application Launcher** можно осуществлять поиск приложения вводя его имя.

##### 1.2.3.1 Доступ в сеть Интернет

Запустите **Terminal**. Введите команду:

```console
$ ip address
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UNKNOWN group default qlen 1000
    link/ether 00:0c:29:1a:39:81 brd ff:ff:ff:ff:ff:ff
    inet 172.16.251.129/24 brd 172.16.251.255 scope global noprefixroute dynamic eth0
       valid_lft 1108sec preferred_lft 1108sec
    inet6 fe80::9d94:eee4:5d6a:75f3/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 50:3e:aa:60:88:02 brd ff:ff:ff:ff:ff:ff
    inet6 fe80::60f5:49b3:ff1a:ec0f/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
```

Вы должны увидеть похожий вывод.
В данном случае мы видим три сетевых интерфейса: **lo**, **eth0** и **eth1**.
Интерфейс **lo** является виртуальным сетевым интерфейсом. Интерфейсы **eth0**
и **eth1** являются физическими сетевыми интерфейсами.

Интерфейс **eth0** соответствует физическому сетевому интерфейсу на материнской
плате и обеспечивает доступ в сеть Интернет. Интерфейс **eth1** соответствует
физическому сетевому интерфейсу в виде USB-Ethernet-адаптера, подключенного к
USB-порту.

Стоит отметить, что в вашем случае наименование и количество интерфейсов
может не совпадать с приведенным выше выводом. Для установления соответствия
между интерфейсами, отображаемыми по команде **ip address** и физическими
интерфейсами можно осуществлять подключение и отключение устройств к физическим
интерфейсам, наблюдая при этом за изменением состояния интерфейсов,
отображаемых по команде **ip address** (**DOWN**, **UP** или **UNKNOWN**).

Чтобы узнать имя компьютера в сети необходимо выполнить команду:

```console
$ hostname
linux-s051
```

В данном случае имя компьютера это **linux-s051**.

![Раздел 1.2.3.1 Рисунок 1][Section_1.2.3.1_Figure_1]
![Раздел 1.2.3.1 Рисунок 2][Section_1.2.3.1_Figure_2]
![Раздел 1.2.3.1 Рисунок 3][Section_1.2.3.1_Figure_3]
![Раздел 1.2.3.1 Рисунок 4][Section_1.2.3.1_Figure_4]
![Раздел 1.2.3.1 Рисунок 5][Section_1.2.3.1_Figure_5]
![Раздел 1.2.3.1 Рисунок 6][Section_1.2.3.1_Figure_6]

Откройте **YaST**, выберите раздел **Network Settings**:

1. Выберите раздел **Global Options**.
2. Выберите в качестве **Network Setup Method** пункт **Wicked Service**.
3. Выберите раздел **Overview**.
4. Выберите интерфейс **eth0**.
5. Нажмите на кнопку **Edit**.
6. Удостоверьтесь, что выбран пункт **Dynamic Address** (DHCP IPv4/IPv6).
7. Нажмите на кнопку **Next**.
8. Выберите интерфейс **eth1**.
9. Нажмите на кнопку **Edit**.
10. В поле **IP Address** введите **192.168.0.6**.
11. В поле **Subnet Mask** введите **255.255.255.0**.
12. В поле **Hostname** введите **linux-s051**.
13. Нажмите на кнопку **Next**.
14. Нажмите на кнопку **OK**.

Теперь вывод команды **ip address** должен выглядеть следующим образом:

```console
$ ip address
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UNKNOWN group default qlen 1000
    link/ether 00:0c:29:1a:39:81 brd ff:ff:ff:ff:ff:ff
    inet 172.16.251.129/24 brd 172.16.251.255 scope global noprefixroute dynamic eth0
       valid_lft 896sec preferred_lft 896sec
    inet6 fe80::9d94:eee4:5d6a:75f3/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 50:3e:aa:60:88:02 brd ff:ff:ff:ff:ff:ff
    inet 192.168.0.6/24 brd 192.168.0.255 scope global eth1
       valid_lft forever preferred_lft forever
    inet6 fe80::60f5:49b3:ff1a:ec0f/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
```

Для проверки доступа в сеть Интернет выполните команду:

```console
$ ping yandex.ru
PING yandex.ru (77.88.55.80) 56(84) bytes of data.
64 bytes from yandex.ru (77.88.55.80): icmp_seq=1 ttl=128 time=4.12 ms
64 bytes from yandex.ru (77.88.55.80): icmp_seq=2 ttl=128 time=4.00 ms
64 bytes from yandex.ru (77.88.55.80): icmp_seq=3 ttl=128 time=3.96 ms
64 bytes from yandex.ru (77.88.55.80): icmp_seq=4 ttl=128 time=6.41 ms
```

Для проверки доступа в локальную сеть выполните команду:

```console
$ ping 172.17.17.9
PING 172.17.17.9 (172.17.17.9) 56(84) bytes of data.
64 bytes from 172.17.17.9: icmp_seq=1 ttl=128 time=0.704 ms
64 bytes from 172.17.17.9: icmp_seq=2 ttl=128 time=0.483 ms
64 bytes from 172.17.17.9: icmp_seq=3 ttl=128 time=0.631 ms
64 bytes from 172.17.17.9: icmp_seq=4 ttl=128 time=0.466 ms
```

Для проверки наличия связи с устройством **Корунд-2O-8E** подключите
устройство (см. Раздел №2) и выполните команду:

```console
$ ping 192.168.0.8
PING 192.168.0.8 (192.168.0.8) 56(84) bytes of data.
64 bytes from 192.168.0.8: icmp_seq=1 ttl=64 time=5.13 ms
64 bytes from 192.168.0.8: icmp_seq=2 ttl=64 time=2.38 ms
64 bytes from 192.168.0.8: icmp_seq=3 ttl=64 time=2.13 ms
64 bytes from 192.168.0.8: icmp_seq=4 ttl=64 time=2.50 ms
```

Вывод последних трех команд должен соответствовать тому, что указан в примерах.

##### 1.2.3.2 Установка необходимого ПО

![Раздел 1.2.3.2 Рисунок 1][Section_1.2.3.2_Figure_1]
![Раздел 1.2.3.2 Рисунок 2][Section_1.2.3.2_Figure_2]
![Раздел 1.2.3.2 Рисунок 3][Section_1.2.3.2_Figure_3]
![Раздел 1.2.3.2 Рисунок 4][Section_1.2.3.2_Figure_4]

Откройте **YaST**, выберите раздел **Software Management**:

1. Выберите меню **View** -> **Patterns**.
2. Выберите раздел **Patterns**.
3. Отметьте шаблон **File Server**.
4. Отметьте шаблон **Base Development**.
5. Отметьте шаблон **C/C++ Development**.
6. Отметьте шаблон **Linux Kernel Development**.
7. Отметьте шаблон **Python 3 Development**.
8. Отметьте шаблон **Help and Support Documentation**.
9. Отметьте шаблон **Documentation**.
10. Нажмите на кнопку **Accept**.
11. Нажмите на кнопку **Continue**.
12. По завершении установки нажмите на кнопку **Continue** если желаете
    продолжить работу с **Software Management** или на кнопку **Finish**,
    чтобы выйти из **Software Management**.

![Раздел 1.2.3.2 Рисунок 5][Section_1.2.3.2_Figure_5]

Откройте **YaST**, выберите раздел **Software Management**:

1. Выберите раздел **Search**.
2. В строке поиска напишите "imagewriter". Нажмите на кнопку **Search**.
3. Отметьте пакет **imagewriter**.
4. Нажмите на кнопку **Accept**.
   При необходимости нажмите на кнопку **Continue**.

Примечание: Можно отметить несколько пакетов для установки.

В соответствие с вышеописанным способом установите следующие пакеты:

- **krusader**
- **telnet**
- **minicom**
- **wireshark**

В качестве редактора исходного кода рекомендуется использовать
**Visual Studio Code**. Для его установки в эмуляторе терминала необходимо
ввести следующие команды
(при необходимости воспользуйтесь паролем системного администратора):

```console
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc

sudo sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=\
https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ntype=rpm-md\
\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > \
/etc/zypp/repos.d/vscode.repo'

sudo zypper refresh
sudo zypper install code
```

Запуск **Visual Studio Code** осуществляется с помощью следующей
последовательности: **Application Launcher** -> **Applications** ->
**Development** -> **Text Editor** или с помощью команды:

```console
code
```

##### 1.2.3.3 Конфигурирование локального файлового сервера

Для добавления локального файлового сервера в список автоматического
монтирования выполните команды:

```console
sudo cp /etc/fstab /etc/fstab.bak

sudo sh -c 'echo -e "//172.17.17.9/Users /mnt cifs \
user=voronov,password=Qwerty0763\$@ 0 0" >> /etc/fstab'
```

Вышеприведенные операции необходимо выполнить только один раз.

Для подключения локального файлового сервера выполните команду:

```console
sudo mount /mnt
```

В дальнейшем монтирование локального файлового сервера будет происходить
автоматически.

##### 1.2.3.4 Конфигурирование TFTP-сервера

![Раздел 1.2.3.4 Рисунок 1][Section_1.2.3.4_Figure_1]

Откройте **YaST**, выберите раздел **TFTP Server**:

1. В меню **After writing configuration** выберите **Start**.
2. В меню **After reboot** выберите **Start on demand**.
3. Отметьте пункт **Open Port in Firewall**.
4. Нажмите на кнопку **OK**.

Файлы TFTP-сервера располагаются в папке */srv/tftpboot/*.

Узнайте имена пользователя и группы для домашнего каталога:

```console
$ ls -la /home/ | grep "$(whoami)"
drwxr-xr-x 21 raven users  4096 Jul  8 14:45 raven
```

Измените пользователя и группу для папки */srv/tftpboot/* в соответствие с
аналогичными параметрами для домашнего каталога:

```console
sudo chown raven:users /srv/tftpboot/
```

##### 1.2.3.5 Конфигурирование FTP-сервера

![Раздел 1.2.3.5 Рисунок 1][Section_1.2.3.5_Figure_1]
![Раздел 1.2.3.5 Рисунок 2][Section_1.2.3.5_Figure_2]
![Раздел 1.2.3.5 Рисунок 3][Section_1.2.3.5_Figure_3]

Откройте **YaST**, выберите раздел **FTP Server**:

1. Выберите раздел **Start-Up**.
2. В меню **After writing configuration** выберите **Start**.
3. В меню **After reboot** выберите **Start on boot**.
4. Выберите раздел **General**.
5. Отметьте пункт **Chroot Everyone**.
6. Выберите раздел **Expert Settings**.
7. Отметьте пункт **Open Port in Firewall**.
8. Нажмите на кнопку **Finish**.

Файлы FTP-сервера располагаются в папке */srv/ftp/*.

Измените пользователя и группу для папки */srv/ftp/* в соответствие с
аналогичными параметрами для домашнего каталога:

```console
sudo chown raven:users /srv/ftp/
```

##### 1.2.3.6 Конфигурирование программы minicom

Подключите устройство **Корунд-2O-8E** (см. Раздел №2).

Для определения USB-устройства, которое соответствует подключенному к ПК
кабелю USB-RS-232 выполните команду:

```console
$ dmesg | grep ttyUSB
[  205.134352] usb 1-2.2: pl2303 converter now attached to ttyUSB0
```

USB-устройство типа **pl2303 converter** и будет требуемым устройством.

Для подключения к последовательному порту RS-232 выполните команду:

```console
sudo minicom -D /dev/ttyUSB0
```

![Раздел 1.2.3.6 Рисунок 1][Section_1.2.3.6_Figure_1]
![Раздел 1.2.3.6 Рисунок 2][Section_1.2.3.6_Figure_2]
![Раздел 1.2.3.6 Рисунок 3][Section_1.2.3.6_Figure_3]
![Раздел 1.2.3.6 Рисунок 4][Section_1.2.3.6_Figure_4]
![Раздел 1.2.3.6 Рисунок 5][Section_1.2.3.6_Figure_5]

Для конфигурирования программы **minicom** нажмите комбинацию клавиш
**Ctrl+A**, отпустите клавиши и нажмите клавишу **Z**.

1. Нажмите клавишу **O**.
2. Выберите пункт меню **Serial port setup**.
3. Нажмите клавишу **E**.
4. Нажмите клавишу **E**. Нажмите клавишу **Enter** два раза.
5. Выберите пункт меню **Save setup as dfl**.
6. Выберите пункт меню **Exit**.
7. Обратите внимание на изменение скорости работы последовательного порта.

Включите или перезагрузите (выключите/включите питание) устройство
**Корунд-2O-8E**. Вы должны увидеть лог загрузки.

Для выхода из программы **minicom** нажмите комбинацию клавиш
**Ctrl+A**, отпустите клавиши и нажмите клавишу **Q**.
Выберите вариант **Yes**.

##### 1.2.3.7 Создание каталога для размещения локальных проектов

Создайте каталог для размещения локальных проектов:

```console
mkdir ~/projects
```

Скопируйте во вновь созданный каталог Git-репозиторий **netprog2**:

```console
cd ~/projects/
git clone https://github.com/pavel-angtel/netprog2.git
```

##### 1.2.3.8 Установка набора средств разработки (SDK)

Для добавления русской раскладки клавиатуры используйте следующую
последовательность: **Application Launcher** -> **Computer** ->
**Configure Desktop** -> **Input Devices** -> **Layouts** ->
**Configure layouts** -> **Add** -> **Russian**.

Измените пользователя и группу для папки */usr/* в соответствие с аналогичными
параметрами для домашнего каталога:

```console
sudo chown raven:users /usr/
```

Скопируйте **SDK** в каталог */usr/* и установите **SDK**:

```console
rsync -axHAWXS --numeric-ids --info=progress2 \
/mnt/Разработчики/Общая\ папка/linux_projects/bin/mips/\
sdk-x86_64-mips32el-nf-toolchain-nodistro.0.sh /usr/

/usr/sdk-x86_64-mips32el-nf-toolchain-nodistro.0.sh
```

После запуска сценария распаковки и установки **SDK** согласитесь с
предложенным по умолчанию местом установки и подтвердите установку в обоих
случаях нажимая клавишу **Enter**.

Добавьте путь к кросс-компилятору в переменную окружения **PATH**:

```console
cp ~/.bashrc ~/.bashrc.bak

echo -e "export PATH=\$PATH:\
/usr/angtel-sdk/sysroots/x86_64-oesdk-linux/usr/bin/mipsel-oe-linux-musl/" >> \
~/.bashrc

. ~/.bashrc
```

Проверьте, что приложения и драйверы корректно собираются и устанавливаются:

```console
cd ~/projects/netprog2/build/angtel-projects/app-hello-world-example/
make install
```

```console
cd ~/projects/netprog2/build/angtel-projects/mod-hello-world-example/
make install
```

Подключитесь к устройству **Корунд-2O-8E** через программу **telnet**
(логин "root"):

```console
telnet 192.168.0.8
```

Измените права на чтение, запись и исполнение для файлов
**app-hello-world-example** и **mod-hello-world-example.ko**:

```console
chmod 755 ./app-hello-world-example ./mod-hello-world-example.ko
```

Запустите приложение:

```console
$ ./app-hello-world-example
Hello, World!
```

Загрузите и выгрузите драйвер:

```console
insmod ./mod_hello_world_example.ko
rmmod mod_hello_world_example
```

Вывод от драйвера вы можете наблюдать в minicom-сессии. Для выхода из
telnet-сессии введите в командной строке "exit".

## 2. Подключение устройства Корунд-2O-8E к ПК

### 2.1 Состав и назначение портов устройства Корунд-2O-8E

![Раздел 2.1 Рисунок 1][Section_2.1_Figure_1]

На устройстве Корунд-2O-8E располагаются следующие интерфейсы:

1. Розетка питания устройства Корунд-2O-8E (**ПИТ.1** и **ПИТ.2**)
2. Розетка последовательного порта устройства Корунд-2O-8E (**RS-232**)
3. Слот для SD-карт (**SD КАРТА**)
4. Индикаторы подачи питания и аварии (**ВКЛ** и **АВАРИЯ**)
5. Сухие контакты (**ВХОДЫ 1** и **2**)
6. Медные Ethernet-интерфейсы (**3|4**, **5|6**, **7|8** и **9|10**)
7. Оптические Ethernet-интерфейсы (**1|2**)

### 2.2 Подключение устройства Корунд-2O-8E к ПК

![Раздел 2.2 Рисунок 1][Section_2.2_Figure_1]

Переходник питания:

1. Вилка питания переходника питания.
   Подключается к розетке питания устройства Корунд-2O-8E.
2. Розетка питания переходника питания.

![Раздел 2.2 Рисунок 2][Section_2.2_Figure_2]

Адаптер питания:

1. Вилка питания адаптера питания (выход 48 В).
   Подключается к розетке питания переходника питания.
2. Вилка питания адаптера питания (вход 220 В).
   Подключается к сетевой розетке 220 В.

![Раздел 2.2 Рисунок 3][Section_2.2_Figure_3]

Переходник USB-RS-232:

**Внимание: Подключение и отключение кабеля для последовательного порта
RS-232 должно осуществляться при выключенном питании!**

1. Вилка переходника USB-RS-232 (RS-232).
   Подключается к розетке последовательного порта устройства Корунд-2O-8E.
2. Вилка переходника USB-RS-232 (USB).
   Подключается к USB-порту ПК.

![Раздел 2.2 Рисунок 4][Section_2.2_Figure_4]

Переходник USB-Ethernet:

1. Розетка переходника USB-Ethernet (Ethernet).
2. Вилка переходника USB-Ethernet (USB).
   Подключается к USB-порту ПК.

![Раздел 2.2 Рисунок 5][Section_2.2_Figure_5]

Ethernet-кабель:

1. Вилка Ethernet-кабеля.
   Подключается к Ethernet-порту №9 устройства Корунд-2O-8E.
2. Вилка Ethernet-кабеля.
   Подключается к розетке переходника USB-Ethernet.

![Раздел 2.2 Рисунок 6][Section_2.2_Figure_6]

Устройство Корунд-2O-8E в сборе.

<!-- Links -->

[Section_1_Link_1]: https://doc.opensuse.org/documentation/leap/startup/single-html/book.opensuse.startup/index.html

[Section_1.1.1_Link_1]: https://software.opensuse.org/distributions/leap

<!-- Figures -->

[Section_1.1.1_Figure_1]: ./images/1.1.1-1.png "Раздел 1.1.1 Рисунок 1"

[Section_1.1.3_Figure_1]: ./images/1.1.3-1.png "Раздел 1.1.3 Рисунок 1"
[Section_1.1.3_Figure_2]: ./images/1.1.3-2.png "Раздел 1.1.3 Рисунок 2"

[Section_1.2.2_Figure_1]: ./images/1.2.2-1.png "Раздел 1.2.2 Рисунок 1"
[Section_1.2.2_Figure_2]: ./images/1.2.2-2.png "Раздел 1.2.2 Рисунок 2"
[Section_1.2.2_Figure_3]: ./images/1.2.2-3.png "Раздел 1.2.2 Рисунок 3"
[Section_1.2.2_Figure_4]: ./images/1.2.2-4.png "Раздел 1.2.2 Рисунок 4"
[Section_1.2.2_Figure_5]: ./images/1.2.2-5.png "Раздел 1.2.2 Рисунок 5"
[Section_1.2.2_Figure_6]: ./images/1.2.2-6.png "Раздел 1.2.2 Рисунок 6"
[Section_1.2.2_Figure_7]: ./images/1.2.2-7.png "Раздел 1.2.2 Рисунок 7"
[Section_1.2.2_Figure_8]: ./images/1.2.2-8.png "Раздел 1.2.2 Рисунок 8"
[Section_1.2.2_Figure_9]: ./images/1.2.2-9.png "Раздел 1.2.2 Рисунок 9"
[Section_1.2.2_Figure_10]: ./images/1.2.2-10.png "Раздел 1.2.2 Рисунок 10"
[Section_1.2.2_Figure_11]: ./images/1.2.2-11.png "Раздел 1.2.2 Рисунок 11"
[Section_1.2.2_Figure_12]: ./images/1.2.2-12.png "Раздел 1.2.2 Рисунок 12"

[Section_1.2.3_Figure_1]: ./images/1.2.3-1.png "Раздел 1.2.3 Рисунок 1"

[Section_1.2.3.1_Figure_1]: ./images/1.2.3.1-1.png "Раздел 1.2.3.1 Рисунок 1"
[Section_1.2.3.1_Figure_2]: ./images/1.2.3.1-2.png "Раздел 1.2.3.1 Рисунок 2"
[Section_1.2.3.1_Figure_3]: ./images/1.2.3.1-3.png "Раздел 1.2.3.1 Рисунок 3"
[Section_1.2.3.1_Figure_4]: ./images/1.2.3.1-4.png "Раздел 1.2.3.1 Рисунок 4"
[Section_1.2.3.1_Figure_5]: ./images/1.2.3.1-5.png "Раздел 1.2.3.1 Рисунок 5"
[Section_1.2.3.1_Figure_6]: ./images/1.2.3.1-6.png "Раздел 1.2.3.1 Рисунок 6"

[Section_1.2.3.2_Figure_1]: ./images/1.2.3.2-1.png "Раздел 1.2.3.2 Рисунок 1"
[Section_1.2.3.2_Figure_2]: ./images/1.2.3.2-2.png "Раздел 1.2.3.2 Рисунок 2"
[Section_1.2.3.2_Figure_3]: ./images/1.2.3.2-3.png "Раздел 1.2.3.2 Рисунок 3"
[Section_1.2.3.2_Figure_4]: ./images/1.2.3.2-4.png "Раздел 1.2.3.2 Рисунок 4"
[Section_1.2.3.2_Figure_5]: ./images/1.2.3.2-5.png "Раздел 1.2.3.2 Рисунок 5"

[Section_1.2.3.4_Figure_1]: ./images/1.2.3.4-1.png "Раздел 1.2.3.4 Рисунок 1"

[Section_1.2.3.5_Figure_1]: ./images/1.2.3.5-1.png "Раздел 1.2.3.5 Рисунок 1"
[Section_1.2.3.5_Figure_2]: ./images/1.2.3.5-2.png "Раздел 1.2.3.5 Рисунок 2"
[Section_1.2.3.5_Figure_3]: ./images/1.2.3.5-3.png "Раздел 1.2.3.5 Рисунок 3"

[Section_1.2.3.6_Figure_1]: ./images/1.2.3.6-1.png "Раздел 1.2.3.6 Рисунок 1"
[Section_1.2.3.6_Figure_2]: ./images/1.2.3.6-2.png "Раздел 1.2.3.6 Рисунок 2"
[Section_1.2.3.6_Figure_3]: ./images/1.2.3.6-3.png "Раздел 1.2.3.6 Рисунок 3"
[Section_1.2.3.6_Figure_4]: ./images/1.2.3.6-4.png "Раздел 1.2.3.6 Рисунок 4"
[Section_1.2.3.6_Figure_5]: ./images/1.2.3.6-5.png "Раздел 1.2.3.6 Рисунок 5"

[Section_2.1_Figure_1]: ./images/2.1-1.png "Раздел 2.1 Рисунок 1"

[Section_2.2_Figure_1]: ./images/2.2-1.png "Раздел 2.2 Рисунок 1"
[Section_2.2_Figure_2]: ./images/2.2-2.png "Раздел 2.2 Рисунок 2"
[Section_2.2_Figure_3]: ./images/2.2-3.png "Раздел 2.2 Рисунок 3"
[Section_2.2_Figure_4]: ./images/2.2-4.png "Раздел 2.2 Рисунок 4"
[Section_2.2_Figure_5]: ./images/2.2-5.png "Раздел 2.2 Рисунок 5"
[Section_2.2_Figure_6]: ./images/2.2-6.png "Раздел 2.2 Рисунок 6"
