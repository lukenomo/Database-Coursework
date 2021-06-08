#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDate>
#include <QVector>
#include <QPushButton>
#include <QtMath>
#include <QCompleter>
#include <QSlider>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QMessageBox>
#include "ticket.h"
#include "myConsts.h"
#include "myregex.h"

class Ticket;

namespace Ui {
  class ResultsWindow;
}

class ResultsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit ResultsWindow(QWidget *parent = nullptr);

  ~ResultsWindow();

  void sortByPrice();
  void sortByDuration();

  QString secondsToString(qint64);

signals:
  void signal_closeResultsOpenMain();
  QVector<Ticket*> signal_getTickets(const QString&, const QString&, const QDate&, const QDate&);
  QString signal_setButtonBuy(int, int, int, const QString&, const QVector<int>);
  void signal_getAirports(QStringList&, const QString&);
  void signal_getAircompanies(QStringList&, const QString&, const QString&);
  void signal_fillInListOfArrivalAirports(QStringList&, const QString&);
  QStringList signal_getEmailAndPhoneNumber(const int);

public slots:
  void slot_closeMainOpenResults();
  void slot_ticketSettings(const QString&, const QString&, int, int, int, const QString&, const int, const QDate&, const QDate& = QDate(0,0,0));

private slots:
  // переход на главную форму
  void on_pushButtonBack_clicked();

  // открытие фильтров
  void on_pushButtonTimeFilter_clicked();
  void on_pushButtonAirportFilter_clicked();
  void on_pushButtonDurationFilter_clicked();

  // применение фильтров
  void on_pushButtonApply_clicked();

  // заполнение боксов аэропортами
  void fillInDepartureAirports(const QString&, QComboBox&, QLineEdit&);
  void fillInAircompanies(QComboBox&, QLineEdit&, const QString&, const QString&);
  void refreshArrivalAirportsBox(QComboBox&, QLineEdit&, const QLineEdit&);

  // расширяет фреймы фильтрации при двойном билете
  void addFilters();

  void on_lineEditDepartureAirport_textChanged();
  void on_lineEditDepartureAirport_2_textChanged();
  void on_pushButtonPriceFilter_clicked();
  void on_pushButtonAircompanyFilter_clicked();
  void on_radioButtonCheaper_toggled();
  void on_radioButtonFaster_toggled();

  void on_pushButtonClear_clicked();

private:
  Ui::ResultsWindow *ui;
  QVBoxLayout *vbLayout;
  TableDataGateway *tdg;
  QList<QFrame*> frameList;
  QStringList *departureAirports;
  QStringList *arrivalAirports;
  QStringList *aircompanies;
  QVector<int> *ids;

  int numberOfWays;

  int userID;

  void setAircompanyLogo(QLabel&);

  // заполнение фрейма билетом
  void initializeTicket(QFrame&, Ticket&, QVector<int>&, int);

  void setTicketsFontStyle();
  void on_pushButtonBuy_clicked();
  void setFont();

  // настройки при запуске формы
  void startSettings();
  void createTicketFrame(QFrame&, const QString&, const int);
  void createButtonBuy(QPushButton&, const QString&, const int);
  void clear();

  // удаление фреймов с билетами
  void deleteFrames();
};

#endif // RESULTSWINDOW_H

