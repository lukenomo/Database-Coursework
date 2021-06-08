#ifndef TABLEDATAGATEWAY_H
#define TABLEDATAGATEWAY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QVector>
#include <QDate>
#include <QApplication>
#include <QPushButton>
#include "ticket.h"
#include "myConsts.h"

class Ticket;

class TableDataGateway: public QObject
{
  Q_OBJECT

public:
   TableDataGateway();
   ~TableDataGateway();

private:
   static QSqlDatabase db;
   QMessageBox mBox;

public slots:
    int slot_findMatches(const QString&, const QString&);
    void slot_createAnAccount(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&);
    QString slot_setButtonBuy(const int, const int, const int, const QString&, const QVector<int>);
    void slot_fillInListOfCities(QStringList&);
    void slot_fillInListOfAirports(QStringList&, const QString&);
    void slot_fillInListOfAircompanies(QStringList&, const QString&, const QString&);
    void slot_fillInListOfArrivalCities(QStringList&, const QString&);
    void slot_fillInListOfArrivalAirports(QStringList&, const QString&);
    void slot_fillInListOfDepartureDates(QStringList&, const QString&, const QString&);
    QVector<Ticket*> slot_getTickets(const QString&, const QString&, const QDate&, const QDate&);
    int slot_getUserID(const QString&, const QString&);
    QString slot_getUserName(int);
    QStringList slot_getEmailAndPhoneNumber(const int);
    int slot_isAdmin(const int);
    QSqlDatabase slot_getDB();
};


#endif // TABLEDATAGATEWAY_H
