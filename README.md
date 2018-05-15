# Simple Version Control System
Идея на проект - да се реализира strip-down версия на git. 

## Компилация
```
cd src
./build.sh
```

## Поддиректории

### Automata
Построява автомат, който може да интерпретира:
* ?
....Коя да е буква в езика

* *
....Произволен низ (включително и празен)

* scanf scanset

* Кой да е синглетон. Някой от тях трябва да бъдат escape-нати (например *).

## BracketExpr
Служа за parse-ване на scanset. Това е отделено от Automata, понеже е нужно и
на друго място.

## States
Описва състоянията на автомата. Автоматът е всъщност свързън списък от
състояния. Той попринцип може да има и по-сложна структура (т.е. по-голяма
разклоненост), но ограничението за формат на регулярен израз ни позволява да
направим по-лесна и ефикасна имплементация.

## CommitLog
Пази информация за комитите. Те се представят чрез id, кратко описание, време,
автор и дърво (йерархия от файлове и директории).

## FileArchiver
Организира всички файлове. Когато стане commit, трябва всеки файл, който е нов
за целия проект да бъде запазен. Когато стане checkout, трябва старите файлове
да бъдат възстановени.

## FileBase
Абстрактен клас, описва файл - той може да бъде директория или такъв съдържащ
информация.

## FileHashTable
Поможен клас на Directory, описва *хеш таблица от файлове*, в случая файловете
са абстрактния клас.

## FileTreeFunctions
Функциите, който правят манипулациите върху дърветата.

## ForwardList
Свързан списък. Има добавена функционалност, която е нужна за Set.

# HashFunctions
Помощни хеширащи функции. Може да се хешира файл, пойнтър или стринг. Това са
все данни, но хеширащите функция са различни и се използват в различни
ситуации.

## Main
Парсва command line аргументите.

## Manipulators
Stream манипулатори. Помагат при i/о операциите.

## MessageDigest
Wrap-ва openssl библиотеката, така че да придостви функционалност за хеширане
на файл.

## Set
Имплементира нещо като multiset, само че с друго API.

## SystemSpecific
Функции, който се обръщат към операционната система и правят проекта os
dependent.

## Условие на задачата

* -init <directory>
....Инициализира дадената директория като хранилище, като добавя нужните
файлове/директории, за следенето на състоянието ѝ.

* -commit <repodirectory> <author name> <commit description>
....Прави commit с текущите промени в хранилището, в който присъства даденото
име на автора на промените и описание. Възможно е подаване на част от
съдържанието -- например отделен файл или под-директория. По желание може да
имплементирате и работа с файлови маски. (най-общо * и ?)

* -log <repo directory>
....Извежда по подходящ начин на екрана историята на commit-ите, включително
тяхното id, автора им, кога са направени и описанието, свързано с тях.

* -status <repo directory>
....Извежда на екрана информация за текущия commit и за всички файлове в
дадената директория, които не са включени в последния commit. Ако няма такива,
съобщава това по подходящ начин.

* -checkout <repo directory> <commit id>
....Връща работната директория в състоянието, което е имала, когато е бил
създаден commit с даденото id.

....Когато директорията е върната назад в историята, не може да се правят нови
commit-и, докато тя не се върне обратно в последното ѝ състояние.

....Връщането към последна версия се случва чрез специална версия на същата
команда, където вместо id на commit се подава думата “HEAD” (HEAD означава
id-то на последния commit-в историята).

....Горната команда може да се изпълни и когато хранилището е в последната си
версия. Тогава работата директория се замества с последния commit, изчиствайки
всички промени, които не са били commit-нати.

* -revert <repo directory> <commit id> <file path>
....Ако даденият файл от commit-а, с дадено id, съществува, то той се добавя
към работната директория. Ако файл със същия път вече съществува, той се
презаписва. Пътят на файла е относителен, спрямо пътя на хранилището. По
желание може да имплементирате и поддръжка на файлови маски за пътя.
