#include "tabledatagateway.h"

QSqlDatabase TableDataGateway::db = QSqlDatabase::addDatabase("QSQLITE");

TableDataGateway::TableDataGateway()
{
  db.setDatabaseName(mconst::DB_PATH);
    if(!db.open())
      {
        mBox.setText("Data base error");
        mBox.setInformativeText(db.lastError().text());
        mBox.setIcon(QMessageBox::Critical);
        mBox.exec();
        exit(1);
      }
  }

TableDataGateway::~TableDataGateway()
{ }

int TableDataGateway::slot_findMatches(const QString& login, const QString& password)
  {
    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(AccountID) FROM Account WHERE Login ='" + login + "' AND Password ='" + password + "'"))
    {
      mBox.setText("slot_findMatches");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
    }
    else
    {
      query.next();
      if (query.value(0) == 1)
        return 0;
      else
        {
          query.exec("SELECT COUNT(AccountID) FROM Account WHERE Login ='" + login + "'");
          query.next();
          if (query.value(0) == 1)
            return 1;
        }
        return 2;
    }
     return 0;
  }


void TableDataGateway::slot_createAnAccount(const QString& name, const QString& surname, const QString& login, const QString& password, const QString& phoneNumber, const QString& email, const QString& citizenship, const QString& passportSiries, const QString& passportValidity)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO Account(Name, Surname, Login, Password, PhoneNumber, Email, Citizenship, PassportSeries, PassportValidity) VALUES(?,?,?,?,?,?,?,?,?)");
  query.addBindValue(name);
  query.addBindValue(surname);
  query.addBindValue(login);
  query.addBindValue(password);
  query.addBindValue(phoneNumber);
  query.addBindValue(email);
  query.addBindValue(citizenship);
  query.addBindValue(passportSiries);
  query.addBindValue(passportValidity);

  if (!query.exec())
    {
      mBox.setText("slot_createAnAccount");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
    }
}


void TableDataGateway::slot_fillInListOfCities(QStringList& departureCities)
{
  QSqlQuery query(db);
  query.exec("SELECT Name FROM City");
  while (query.next())
    {
      departureCities << query.value(0).toString();
    }
}


void TableDataGateway::slot_fillInListOfArrivalCities(QStringList& arrivalCities, const QString& departureCity)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT Name FROM City WHERE CityID IN "
                   "(SELECT ArrivalCity FROM Flight WHERE DepartureCity IN"
                   "(SELECT CityID FROM City WHERE Name = '"+departureCity+"'))"))
  {
    mBox.setText("slot_fillInListOfArrivalCities");
    mBox.setInformativeText(query.lastError().text());
    mBox.setIcon(QMessageBox::Critical);
    mBox.exec();
    exit(1);
  }
  while (query.next())
    {
      arrivalCities << query.value(0).toString();
    }
}


void TableDataGateway::slot_fillInListOfDepartureDates(QStringList& departureDates, const QString& departureCity, const QString& arrivalCity)
{
   QSqlQuery query(db);
   if  (!query.exec("SELECT DepartureDate FROM Flight WHERE DepartureCity IN"
                    " (SELECT CityID FROM City WHERE Name = '"+departureCity+"')"
                    " AND"
                    " ArrivalCity IN"
                    " (SELECT CityID FROM City WHERE Name = '"+arrivalCity+"')"))
   {
     mBox.setText("slot_fillInListOfDepartureDates");
     mBox.setInformativeText(query.lastError().text());
     mBox.setIcon(QMessageBox::Critical);
     mBox.exec();
     exit(1);
   }
   while(query.next())
     {
       departureDates << query.value(0).toString();
     }
}


 QString TableDataGateway::slot_setButtonBuy(const int adult, const int child, const int kid, const QString& ticketType, const QVector<int> ids)
{
  QSqlQuery query(db);
  int sum{0};
  for (int iter = 0; iter < ids.size(); ++iter)
    {
  if  (!query.exec(" SELECT Price"
                   " FROM Price"
                   " WHERE FlightNumber = "+QString::number(ids.at(iter))+""
                   " AND "+QString::number(adult + child)+" <= (SELECT Plane.FreePlaces FROM Plane WHERE Plane.FlightNumber IN"
                   " (SELECT Flight.Plane FROM Flight WHERE FlightNumber = "+QString::number(ids.at(iter))+" ))"
                   " AND Class = (SELECT ClassID FROM Class WHERE Name = '"+ticketType+"')"))
     {
      mBox.setText("slot_setButtonBuy");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
        }
    query.next();
    sum += query.value(0).toInt();
    }


  QString string;

  query.next();
  int kidSum {0};
  int childSum {0};
  if (kid > 0)
    {
      kidSum = ((sum * mconst::KID_DISCOUNT) / 100) * kid;
    }
  if (childSum > 0)
    {
      childSum = ((sum * mconst::CHILD_DISCOUNT) / 100) * childSum;
    }
  string = QString::number(kidSum + child + adult * sum) + " BYN, " + ticketType;
  return string;
}


 QVector<Ticket*> TableDataGateway::slot_getTickets(const QString& departureCity, const QString& arrivalCity, const QDate& dateThere, const QDate& dateBack)
 {
   QVector<Ticket*> *tickets = new  QVector<Ticket*>();

   QString depCity{departureCity};
   QString arrCity{arrivalCity};
   QDate there{dateThere};
   QDate back{dateBack};
   int numberOfFlights{1};

   if (dateBack != QDate(0,0,0))
     {
       numberOfFlights = 2;
     }

   QSqlQuery query(db);
   int counter{0};
   for (; counter < numberOfFlights; ++counter)
     {
       if(counter == 1)
         {
           swap(depCity,arrCity);
           QDate temp{dateThere};
           there = back;
           back = temp;
         }
        if  (!query.exec("SELECT ArrivalDate,"
                  " DepartureTime,"
                  " ArrivalTime,"
                  " (SELECT PlaneModel FROM PlaneModel WHERE PlaneID IN"
                  " (SELECT PlaneNumber FROM Plane WHERE FlightNumber = Plane)),"
                  " (SELECT Name FROM Airport WHERE AirportID = DepartureAirport),"
                  " (SELECT Name FROM Airport WHERE AirportID = ArrivalAirport),"
                  " (SELECT Name FROM AirCompany WHERE AirCompanyID = AirCompany),"
                  " (SELECT Abbreviation FROM City WHERE CityID IN"
                  " (SELECT CityID FROM City WHERE Name = '"+depCity+"')),"
                  " (SELECT Abbreviation FROM City WHERE CityID IN"
                  " (SELECT CityID FROM City WHERE Name = '"+arrCity+"')),"
                  " FlightID, "
                  " (SELECT Logo FROM AirCompany WHERE AirCompanyID  = AirCompany),"
                  " (SELECT Name FROM FlightName WHERE FlightID = FlightID)"
                  " FROM Flight WHERE DepartureDate = ('"+there.toString("dd.MM.yyyy")+"')"
                  " AND DepartureCity = (SELECT DepartureCity FROM Flight WHERE DepartureCity IN"
                  " (SELECT CityID FROM City WHERE Name = '"+depCity+"'))"
                  " AND ArrivalCity = (SELECT ArrivalCity FROM Flight WHERE ArrivalCity IN"
                  " (SELECT CityID FROM City WHERE Name = '"+arrCity+"'))"))
          {
            mBox.setText("slot_getTickets");
            mBox.setInformativeText(query.lastError().text());
            mBox.setIcon(QMessageBox::Critical);
            mBox.exec();
            exit(1);
          }

       while (query.next())
         {
           Ticket *ticket = new Ticket();
           ticket->setArrivalDate(query.value(0).toString());
           ticket->setDepartureTime(query.value(1).toString());
           ticket->setArrivalTime(query.value(2).toString());
           ticket->setPlaneModelText(query.value(3).toString());
           ticket->setDepartureAirport(query.value(4).toString());
           ticket->setArrivalAirport(query.value(5).toString());
           ticket->setCompanyText(query.value(6).toString());
           ticket->setDepartureAbbreviation(query.value(7).toString());
           ticket->setArrivalAbbreviation(query.value(8).toString());
           ticket->setId(query.value(9).toInt());
           ticket->setCompanyLogo(query.value(10).toString());
           ticket->setFlightNumer(query.value(11).toInt());
           ticket->setDepartureDate(there.toString("dd.MM.yyyy"));
           ticket->setDepartureCity(depCity);
           ticket->setArrivalCity(arrCity);
           ticket->setFlightDuration();

           if (counter == 0)
             {
               tickets->push_front(ticket);
             }
           else
             {
               tickets->push_back(ticket);
             }
        }
   }
  return *tickets;
}


 void TableDataGateway::slot_fillInListOfAirports(QStringList& departureAirports, const QString& country)
 {
   QSqlQuery query(db);
   if  (!query.exec("SELECT Name FROM Airport WHERE CityID IN"
              "(SELECT CityID FROM City WHERE Name = '"+country+"')"))
   {
       mBox.setText("slot_fillInListOfAirports");
       mBox.setInformativeText(query.lastError().text());
       mBox.setIcon(QMessageBox::Critical);
       mBox.exec();
       exit(1);
   }
   while (query.next())
     {
       departureAirports << query.value(0).toString();
     }
 }


 void TableDataGateway::slot_fillInListOfArrivalAirports(QStringList& arrivalAirports, const QString& departureAirport)
 {
   QSqlQuery query(db);
   if  (!query.exec("SELECT Name FROM Airport WHERE AirportID IN "
                    "(SELECT ArrivalAirport FROM Flight WHERE DepartureAirport IN"
                    "(SELECT AirportID FROM Airport WHERE Name = '"+departureAirport+"'))"))
   {
     mBox.setText("slot_fillInListOfArrivalAirports");
     mBox.setInformativeText(query.lastError().text());
     mBox.setIcon(QMessageBox::Critical);
     mBox.exec();
     exit(1);
   }
   while (query.next())
     {
       arrivalAirports << query.value(0).toString();
     }
 }


void TableDataGateway::slot_fillInListOfAircompanies(QStringList& aircompanies, const QString& depCity, const QString& arrCity)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT Name FROM AirCompany WHERE AirCompanyID IN"
             "(SELECT AirCompany FROM Flight WHERE DepartureCity IN"
             "(SELECT CityID FROM City WHERE Name = '"+depCity+"'))"
             "AND"
             "(SELECT AirCompany FROM Flight WHERE ArrivalCity IN"
             "(SELECT CityID FROM City WHERE Name = '"+arrCity+"'))"))
  {
      mBox.setText("slot_fillInListOfAircompanies!");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
  }
  while (query.next())
    {
      aircompanies << query.value(0).toString();
    }
}


int TableDataGateway::slot_getUserID(const QString& login, const QString& password)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT AccountID FROM Account WHERE Login = '"+login+"' AND Password = '"+password+"'"))
  {
      mBox.setText("slot_getUserID!");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
  }

  query.next();
  return query.value(0).toInt();
}


QString TableDataGateway::slot_getUserName(int userID)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT Name FROM Account WHERE AccountID = '"+QString::number(userID)+"'"))
  {
      mBox.setText("slot_getUserName!");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
  }

  query.next();
  return query.value(0).toString();
}


QStringList TableDataGateway::slot_getEmailAndPhoneNumber(const int userID)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT Email, PhoneNumber FROM Account WHERE AccountID = '"+QString::number(userID)+"'"))
  {
      mBox.setText("slot_getEmailAndPhoneNumber!");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
  }

  query.next();
  QStringList list;
  list << query.value(0).toString() << query.value(1).toString();
  return list;
}


int TableDataGateway::slot_isAdmin(const int userID)
{
  QSqlQuery query(db);
  if  (!query.exec("SELECT Token FROM Account WHERE AccountID = '"+QString::number(userID)+"'"))
  {
      mBox.setText("slot_isAdmin!");
      mBox.setInformativeText(query.lastError().text());
      mBox.setIcon(QMessageBox::Critical);
      mBox.exec();
      exit(1);
  }
  query.next();
  return query.value(0).toInt();
}


QSqlDatabase TableDataGateway::slot_getDB()
{
  return db;
}
