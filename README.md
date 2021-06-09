# Database-Coursework

## About the program
The program "Aviakassa" is a course project in the discipline "Databases and Database Management Systems" in which I applied my knowledge of Qt, C++, OOP and curiously enough SQL

## What this program was created for
The program was written not only for the reason "I need to write a coursework on the database" but also with the desire to apply my own knowledge and skills in creating multi-file applications with an object-oriented approach to writing C++ code.

## Means used
* Programming language: C ++
* Database: SQLite DBMS
* Application architecture: the Table Data Gateway design pattern was selected and implemented to work with the database
* GUI: Qt Creator.

## How to start the program
* Download the project
* Compile and run .pro file 
* In the file myConsts.h on line 82 in the constant DB_PATH specify the absolute path to the AirDataBase file
* In the AirDataBase file in the Flight table, change the departure and arrival dates to, say, tomorrow date, and if the arrival time is less than the departure time, set the date after tomorrow. P.S. change the dates of departure and arrival only in those records in the cities of departure and arrival of which the values "3" and "5" are indicated, as well as "5" and "3" because there are more of these records)

 ## Recommendations for use 
* To gain access to all the functionality of the program, use the login "admin" and the password "123Qwe% 532EQ" 
* watch P.S. above




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




## О программе
Программа "Aviakassa" является курсовым проектом по дисциплине "Базы данных и системы управления базами данных" в которой применены знания Qt, C++, ООП, и, как ни странно, SQL.

## Для чего создана программа
Программа была написана не только лишь по причине "нужно написать курсач по БД", но и с желанием применить собственные знания и навыки в создании многофайловых приложений с объектно-ориентированным подходом к написанию кода на языке С++.

## Используемые средства
* Язык программирования: С++
* База данных: СУБД SQLite
* Архитектура приложения: для работы с базой данных был выбран и реализован шаблон проектирования Table Data Gateway  
* GUI: Qt Creator.

## Как запустить программу
* Скачать проект
* Скомпилировать и запустить *.pro файл
* В файле myConsts.h на 82 строке в константе DB_PATH указать абсолютный путь к базе данных AirDataBase
* В базе данных AirDataBase в таблице Flight изменить даты вылета и прилета на, допустим, завтрашнее число и если время прилета меньше времени вылета установить послезавтрашнее число.
P.S. измените даты вылета и прилета только в тех записях, в городах вылета и прилета которых указаны значения "3" и "5", а также "5" и "3", поскольку этих записей больше)

## Рекомендации по использованию
* Для получения доступа ко всему функционалу программы использовать логин "admin" и пароль "123Qwe%532EQ"
* Смотреть P.S. выше



