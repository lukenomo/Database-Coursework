#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStringList>
#include <QCompleter>
#include <QDateEdit>
#include <QCalendarWidget>
#include "registrationwindow.h"
#include "loginwindow.h"
#include "tabledatagateway.h"
#include "resultswindow.h"
#include "adminpanelwindow.h"
#include "myConsts.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void signal_getCities(QStringList&);
  void signal_fillInListOfArrivalCities(QStringList&, const QString&);
  void signal_fillInListOfDepartureDates(QStringList&, const QString&, const QString&);
  QString signal_getUserName(int);
  void signal_goToResultsWindow();
  void signal_goToLoginWindow();
  void signal_ticketSettings(const QString&, const QString&, int, int, int, const QString&, const int, const QDate&, const QDate& = QDate(0,0,0));
  int signal_isAdmin(const int);
  void signal_goToAdminPanelWindow();

public slots:
  void slot_openMainWithID(int);
  void slot_openMain();

private slots:
  void on_lineEditDepartureCity_textChanged();
  void on_lineEditArrivalCity_textChanged();

  // выбор типа билета: в один конец или туда - обратно
  void on_pushButtonTwoWays_clicked();

  // переход на форму результатов
  void on_pushButtonFind_clicked();

  // калькулятор мест
  void on_pushButtonShowCalculator_clicked();
  void on_pushButtonPlusAdults_clicked();
  void on_pushButtonMinuseAdults_clicked();
  void on_pushButtonPlusChildren_clicked();
  void on_pushButtonPlusKids_clicked();
  void on_pushButtonMinusKids_clicked();
  void on_radioButtonEconomy_toggled();
  void on_radioButtonBusiness_toggled();
  void on_pushButtonMinusChildren_clicked();

  // переход на форму авторизации
  void on_pushButtonLogOut_clicked();

  // переход на форму редактирования бд
  void on_pushButtonAdminPanel_clicked();


private:
  Ui::MainWindow *ui;
  RegistrationWindow *regw;
  LoginWindow *lw;
  TableDataGateway *tdg;
  ResultsWindow *resw;
  AdminPanelWindow *apw;

  QStringList *arrivalCities;
  QStringList *departureCities;
  QStringList *backDepartureDates;
  QStringList *thereDepartureDates;

  QCalendarWidget* dateEditThereDeparture;
  QCalendarWidget* dateEditBackDeparture;

  int passengerCounter;
  int userID;

  void markCalendar(QStringList&, QCalendarWidget*, QCalendarWidget*, bool);
  void tuneTicket();
  void startSettings();
  void paintLineEdit(QLineEdit&);
  void showAdminPanelButton();
  void clear();
};

#endif // MAINWINDOW_H
