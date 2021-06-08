#ifndef TICKET_H
#define TICKET_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QLabel>
#include "tabledatagateway.h"

class TableDataGateway;

class Ticket: public QWidget
{
  Q_OBJECT

public:
  Ticket();
  ~Ticket();

signals:
  void makeATicket(const QTime*, const QTime*, const QDate*, const QDate*, const QLabel*, const QLabel*);

private:
  QTime *departureTime;
  QTime *arrivalTime;

  QDate *depatureDate;
  QDate *arrivalDate;

  QString *company;
  QString *planeModel;

  QString *departureAirport;
  QString *arrivalAirport;

  QString *departureCity;
  QString *arrivalCity;

  QString *departureAbbreviation;
  QString *arrivalAbbreviation;

  qint64 flightDuration;

  int id;
  int flightNumber;

  QString *companyLogo;

  TableDataGateway *tdg;

public:
 void setDepartureTime(const QString&);
 void setArrivalTime(const QString&);
 void setDepartureDate(const QString&);
 void setArrivalDate(const QString&);
 void setCompanyText(const QString&);
 void setPlaneModelText(const QString&);
 void setDepartureAirport(const QString&);
 void setArrivalAirport(const QString&);
 void setDepartureCity(const QString&);
 void setArrivalCity(const QString&);
 void setArrivalAbbreviation(const QString&);
 void setDepartureAbbreviation(const QString&);
 void setFlightDuration();
 void setId(const int);
 void setCompanyLogo(const QString&);
 void setFlightNumer(const int);

 QTime& getDepartureTime();
 QTime& getArrivalTime();
 QDate& getDepartureDate();
 QDate& getArrivalDate();
 QString& getCompanyText();
 QString& getPlaneModelText();
 QString& getDepartureAirport();
 QString& getArrivalAirport();
 QString& getDepartureCity();
 QString& getArrivalCity();
 QString& getArrivalAbbreviation();
 QString& getDepartureAbbreviation();
 int getFlightDuration();
 int getId();
 QString& getCompanyLogo();
 int getFlightNumber();

 void qStringToQTime(QStringList&,  const QString&);
 void qStringToQDate(QStringList&,  const QString&);

 friend bool operator== (const Ticket &t1, const Ticket &t2)
  {
      return (t1.id == t2.id);
  }
};

#endif // TICKET_H
