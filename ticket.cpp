#include "ticket.h"

Ticket::Ticket()
{ }


Ticket::~Ticket()
{ }


void Ticket::setDepartureTime(const QString& time)
{
  QStringList strList;
  qStringToQTime(strList, time);
  departureTime = new QTime(strList.at(0).toInt(),strList.at(1).toInt());
}

void Ticket::setArrivalTime(const QString& time)
{
  QStringList strList;
  qStringToQTime(strList, time);
  arrivalTime = new QTime(strList.at(0).toInt(),strList.at(1).toInt());
}

void Ticket::setDepartureDate(const QString& date)
{
  QStringList strList;
  qStringToQDate(strList, date);
  depatureDate = new QDate(strList.at(2).toInt(),strList.at(1).toInt(), strList.at(0).toInt());
}

void Ticket::setFlightDuration()
{
  QDateTime departureDateTime (getDepartureDate(), getDepartureTime());
  QDateTime arrivalDateTime (getArrivalDate(), getArrivalTime());

  quint64 start = departureDateTime.toSecsSinceEpoch();
  quint64 end = arrivalDateTime.toSecsSinceEpoch();
  flightDuration = end - start;
}


void Ticket::setArrivalDate(const QString& date)
{
  QStringList strList;
  qStringToQDate(strList, date);
  arrivalDate = new QDate(strList.at(2).toInt(),strList.at(1).toInt(), strList.at(0).toInt());
}


void Ticket::setCompanyText(const QString& name)
{
  company = new QString(name);
}


void Ticket::setPlaneModelText(const QString& model)
{
  planeModel = new QString(model);
}


void Ticket::setDepartureAirport(const QString& name)
{
  departureAirport = new QString(name);
}


void Ticket::setArrivalAirport(const QString& name)
{
  arrivalAirport = new QString(name);
}


void Ticket::setDepartureCity(const QString& name)
{
  departureCity = new QString(name);
}


void Ticket::setArrivalCity(const QString& name)
{
  arrivalCity = new QString(name);
}


void Ticket::setArrivalAbbreviation(const QString & abb)
{
  arrivalAbbreviation = new QString(abb);
}


void Ticket::setDepartureAbbreviation(const QString & abb)
{
  departureAbbreviation = new QString(abb);
}


void Ticket::setId(const int iD)
{
  id = iD;
}


void Ticket::setCompanyLogo(const QString& logo)
{
  companyLogo = new QString(logo);
}


void Ticket::setFlightNumer(const int number)
{
  flightNumber = number;
}


void Ticket::qStringToQTime(QStringList& strList, const QString &stringTime)
{
 strList = stringTime.split(QLatin1Char(':'), Qt::SkipEmptyParts);           // проверка на пустоту и на формат
}


void Ticket::qStringToQDate(QStringList& strList, const QString &stringDate)
{
 strList = stringDate.split(QLatin1Char('.'), Qt::SkipEmptyParts);             // проверка на пустоту и на формат
}


QTime& Ticket::getDepartureTime()
{
  return *departureTime;
}


QTime& Ticket::getArrivalTime()
{
  return *arrivalTime;
}


QDate& Ticket::getDepartureDate()
{
  return *depatureDate;
}


QDate& Ticket::getArrivalDate()
{
  return *arrivalDate;
}


QString& Ticket::getCompanyText()
{
  return *company;
}


QString& Ticket::getPlaneModelText()
{
  return *planeModel;
}


QString& Ticket::getDepartureAirport()
{
  return *departureAirport;
}


QString& Ticket::getArrivalAirport()
{
  return *arrivalAirport;
}


QString& Ticket::getDepartureCity()
{
  return *departureCity;
}


QString& Ticket::getArrivalCity()
{
  return *arrivalCity;
}


QString& Ticket::getArrivalAbbreviation()
{
  return *arrivalAbbreviation;
}


QString& Ticket::getDepartureAbbreviation()
{
  return *departureAbbreviation;
}


int Ticket::getFlightDuration()
{
  return flightDuration;
}


int Ticket::getId()
{
  return id;
}


QString& Ticket::getCompanyLogo()
{
  return *companyLogo;
}


int Ticket::getFlightNumber()
{
  return flightNumber;
}

