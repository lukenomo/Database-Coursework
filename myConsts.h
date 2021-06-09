#ifndef MYCONSTS_H
#define MYCONSTS_H

#include <QString>
#include <QCoreApplication>

namespace mconst
{
  // калькулятор пассажиров
  const int MAX_PASSANGERS {10};
  const int MAX_CHILDREN   {5};
  const int MIN_ADULTS     {1};
  const int MIN_CHILDREN   {0};
  const int MIN_KIDS       {0};

  // цвет пометки по умолчанию
  const int rUnmark {225};
  const int gUnmark {225};
  const int bUnmark {225};
  const int aUnmark {0};

  // цвет отметки
  const int rMark {0};
  const int gMark {224};
  const int bMark {199};
  const int aMark {78};

  // стиль lineEdit-a при вводе некорректных значение
  const QString LINEEDIT_ERROR_STYLE {"color: rgb(0, 170, 127); border-style: outset; border-width: 2px; border-color: red; background-color: rgb(67, 81, 108); font: 14pt 'Britannic Bold';"};
  const QString LINEEDIT_DEFAULT_STYLE  {"color: rgb(0, 170, 127); border: rgb(84, 96, 117); background-color: rgb(67, 81, 108); font: 14pt 'Britannic Bold';"};
  const QString TICKET_FONT {"font: 9pt;"};

  // маски ввода
  const QString PASSPORTSERIES_MASK   {"aa"};
  const QString PASSPORTVALIDITY_MASK {"00.00.0000"};
  const QString TELEPHONENUMBER_MASK  {"+000(000)000-00-00"};
  const QString CONVERT_TIME_MASK     {"hh.mm"};
  const QString CONVERT_DATE_MASK     {"dd.MM.yyyy"};
  const QString TIME_MASK             {"00:00"};
  const QString SECS_TO_DHS_MASK      {"%1 days, %2 hours, %3 minutes, %4 seconds"};

  // имена виджетов
  const QString FLIGHTDURATION_INSECONDS_NAME {"flightDurationInSeconds"};
  const QString DEPARTURE_AIRPORT_NAME        {"departureAirport"};
  const QString ARRIVAL_AIRPORT_NAME          {"arrivalAirport"};
  const QString DEPARTURE_TIME_NAME           {"departureTime"};
  const QString ARRIVAL_TIME_NAME             {"arrivalTime"};
  const QString AIRCOMPANY_NAME               {"aircompany"};

  // размеры виджетов
  const int SINGLE_FRAME {200};
  const int DOUBLE_FRAME {430};

  // позиция виджетов
  const int SINGLE_BUTTON_POS  {70};
  const int DOUBLE_BUTTON_POS  {185};
  const int BUTTONBUY_WIDTH    {180};
  const int BUTTONHEIGHT       {40};
  const int BUTTON_XPOS        {700};

  // суммы и числовые ограничения
  const int SECS_IN_A_DAY        {86400};
  const int CONTROLL_SUM         {8};
  const int MAX_PRICE            {1000000};
  const int MIN_PRICE            {0};
  const int STEP                 {200};
  const int MIN_TIMELABEL_LENGTH {4};
  const int MAX_TIME_H           {23};
  const int MAX_TIME_M           {59};
  const int MIN_PASSANGERS       {1};
  const int CHILD_DISCOUNT       {50};
  const int KID_DISCOUNT         {90};
  const int BIG_FRAME_HEIGHT     {250};
  const int SMALL_FRAME_HEIGHT   {250};
  const int SMALL_FILTER_HEIGHT  {100};
  const int BIG_FLITER_HEIGHT    {200};

  // пути файлов
  const QString COMPANY_LOGO_DIR {":/companyLogo/resources/"};
  const QString WAY_PIC          {":/planeWay/resources/planeWay.png"};
  const QString LOGO_2           {":/mainLogo/resources/6.PNG"};
  const QString DB_PATH          {"->absolute_path_to_db<-"};
}

#endif // MYCONSTS_H
