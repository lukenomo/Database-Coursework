#include "resultswindow.h"
#include "ui_resultswindow.h"

ResultsWindow::ResultsWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ResultsWindow)
{  
  ui->setupUi(this);

  tdg = new TableDataGateway();

  departureAirports = new QStringList();
  arrivalAirports   = new QStringList();
  aircompanies      = new QStringList();

  vbLayout = new QVBoxLayout(this);
  vbLayout->setAlignment(Qt::AlignTop);

  connect(this, &ResultsWindow::signal_getTickets, tdg, &TableDataGateway::slot_getTickets);
  connect(this, &ResultsWindow::signal_setButtonBuy, tdg, &TableDataGateway::slot_setButtonBuy);
  connect(ui->comboBoxDepartureAirport, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditDepartureAirport, &QLineEdit::setText);
  connect(ui->comboBoxArrivalAirport, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditArrivalAirport, &QLineEdit::setText);
  connect(ui->comboBoxDepartureAirport_2, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditDepartureAirport_2, &QLineEdit::setText);
  connect(ui->comboBoxArrivalAirport_2, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditArrivalAirport_2, &QLineEdit::setText);
  connect(ui->comboBoxDepartureAircompany, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditDepartureAircompany, &QLineEdit::setText);
  connect(ui->comboBoxDepartureAircompany_2, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditDepartureAircompany_2, &QLineEdit::setText);
  connect(this, &ResultsWindow::signal_getEmailAndPhoneNumber, tdg, &TableDataGateway::slot_getEmailAndPhoneNumber);
  connect(this, &ResultsWindow::signal_getAirports, tdg, &TableDataGateway::slot_fillInListOfAirports);
  connect(this, &ResultsWindow::signal_getAircompanies, tdg, &TableDataGateway::slot_fillInListOfAircompanies);
  connect(this, &ResultsWindow::signal_fillInListOfArrivalAirports, tdg, &TableDataGateway::slot_fillInListOfArrivalAirports);

  startSettings();
}


ResultsWindow::~ResultsWindow()
{
  deleteFrames();
  delete vbLayout;
  delete departureAirports;
  delete arrivalAirports;
  delete aircompanies;
  delete tdg;
  delete ui;
}


void ResultsWindow::slot_closeMainOpenResults()
{
  this->show();
}


void ResultsWindow::on_pushButtonBack_clicked()
{
  this->hide();
  emit signal_closeResultsOpenMain();
  clear();
  deleteFrames();
}


void ResultsWindow::slot_ticketSettings(const QString& departureCity, const QString& arrivalCity, const int adult, const int child, const int kid, const QString& ticketType, const int userID, const QDate& dateThere, const QDate& dateBack)
{
  QVector<Ticket*> vec{emit signal_getTickets(departureCity, arrivalCity, dateThere, dateBack)};
  ids = new QVector<int>;

  if (dateBack == QDate(0,0,0))
    {
      for (int iterator = 0; iterator < vec.size(); ++iterator)
        {
          QFrame *frame = new QFrame();
          createTicketFrame(*frame, QString::number(iterator), mconst::SINGLE_FRAME);

          ids->clear();
          ids->push_back(vec[iterator]->getId());

          QPushButton *buttonBuy = new QPushButton(frame);
          createButtonBuy(*buttonBuy, QString::number(iterator), mconst::SINGLE_BUTTON_POS);
          buttonBuy->setText(emit signal_setButtonBuy(adult, child, kid, ticketType, *ids));
          buttonBuy->show();

          connect(buttonBuy, &QPushButton::clicked, this, &ResultsWindow::on_pushButtonBuy_clicked);
          initializeTicket(*frame, *vec[iterator], *ids, 0);

          if(buttonBuy->text().split(" ").at(0).toInt() != 0)
            {
              vbLayout->addWidget(frame);
            }
        }

      ui->OffersWidgetContents->setLayout(vbLayout);

      frameList = ui->OffersWidgetContents->findChildren<QFrame*>(QRegExp("[0-9]+"));

      fillInDepartureAirports(departureCity, *ui->comboBoxDepartureAirport, *ui->lineEditDepartureAirport);
      fillInAircompanies(*ui->comboBoxDepartureAircompany, *ui->lineEditDepartureAircompany, departureCity, arrivalCity);

      ui->labelWay->setText(departureCity + " - " + arrivalCity);
      numberOfWays = 1;
    }
  else
    {
      int frontIter{0};
      int backIter{vec.size() - 1};

      while(vec[frontIter]->getDepartureCity() != vec[backIter]->getDepartureCity())
        {
          QFrame *frame = new QFrame();
          createTicketFrame(*frame, QString::number(frontIter), mconst::DOUBLE_FRAME);

          ids->clear();
          ids->push_back(vec[frontIter]->getId());
          ids->push_back(vec[backIter]->getId());

          initializeTicket(*frame, *vec[frontIter], *ids, 0);
          initializeTicket(*frame, *vec[backIter], *ids, 1);

          QPushButton *buttonBuy = new QPushButton(frame);
          createButtonBuy(*buttonBuy, QString::number(frontIter), mconst::DOUBLE_BUTTON_POS);
          buttonBuy->setText(emit signal_setButtonBuy(adult, child, kid, ticketType, *ids));
          buttonBuy->show();
          connect(buttonBuy, &QPushButton::clicked, this, &ResultsWindow::on_pushButtonBuy_clicked);

           if(buttonBuy->text().split(" ").at(0).toInt() != 0)
             {
               vbLayout->addWidget(frame);
             }

           ++frontIter;
           --backIter;
        }

      ui->OffersWidgetContents->setLayout(vbLayout);

      frameList = ui->OffersWidgetContents->findChildren<QFrame*>(QRegExp("[0-9]+"));

      fillInDepartureAirports(departureCity, *ui->comboBoxDepartureAirport, *ui->lineEditArrivalAirport);
      fillInDepartureAirports(arrivalCity, *ui->comboBoxDepartureAirport_2, *ui->lineEditArrivalAirport_2);
      fillInAircompanies(*ui->comboBoxDepartureAircompany, *ui->lineEditDepartureAircompany, departureCity, arrivalCity);
      fillInAircompanies(*ui->comboBoxDepartureAircompany_2, *ui->lineEditDepartureAircompany_2, arrivalCity, departureCity);

      addFilters();

      ui->labelWay->setText(departureCity + " - " + arrivalCity + " - " + departureCity);

      numberOfWays = 2;
    }

    setFont();

    this->userID = userID;

    delete ids;

    qDeleteAll(vec.begin(), vec.end());
}


void ResultsWindow::initializeTicket(QFrame& frame, Ticket& ticket, QVector<int>& ids, int position)
{
  QLabel *airCompany              = new QLabel(ticket.getCompanyText(),&frame);
  QLabel *planeModel              = new QLabel(ticket.getPlaneModelText(),&frame);
  QLabel *departureAirport        = new QLabel(ticket.getDepartureAirport(),&frame);
  QLabel *arrivalAirport          = new QLabel(ticket.getArrivalAirport(),&frame);
  QLabel *departureCity           = new QLabel(ticket.getDepartureCity(),&frame);
  QLabel *arrivalCity             = new QLabel(ticket.getArrivalCity(),&frame);
  QLabel *departureTime           = new QLabel(ticket.getDepartureTime().toString("hh.mm"),&frame);
  QLabel *arrivalTime             = new QLabel(ticket.getArrivalTime().toString("hh.mm"),&frame);
  QLabel *departureDate           = new QLabel(ticket.getDepartureDate().toString("dd.MM.yyyy"),&frame);
  QLabel *arrivalDate             = new QLabel(ticket.getArrivalDate().toString("dd.MM.yyyy"),&frame);
  QLabel *arrivalAbbreviation     = new QLabel(ticket.getArrivalAbbreviation(),&frame);
  QLabel *departureAbbreviation   = new QLabel(ticket.getDepartureAbbreviation(),&frame);
  QLabel *airCompanyLogo          = new QLabel(ticket.getCompanyLogo(),&frame);
  QLabel *flightDuration          = new QLabel(secondsToString(ticket.getFlightDuration()),&frame);
  QLabel *flightDurationInSeconds = new QLabel(QString::number(ticket.getFlightDuration()),&frame);
  QLabel *flightNumber            = new QLabel("R.N: " + QString::number(ticket.getFlightNumber()),&frame);
  QLabel *planeWay                = new QLabel(&frame);

  setAircompanyLogo(*airCompanyLogo);

  flightDurationInSeconds->hide();

  departureAirport->setObjectName(mconst::DEPARTURE_AIRPORT_NAME);
  arrivalAirport->setObjectName(mconst::ARRIVAL_AIRPORT_NAME);
  departureTime->setObjectName(mconst::DEPARTURE_TIME_NAME);
  arrivalTime->setObjectName(mconst::ARRIVAL_TIME_NAME);
  airCompany->setObjectName(mconst::AIRCOMPANY_NAME);
  flightDurationInSeconds->setObjectName(mconst::FLIGHTDURATION_INSECONDS_NAME);

  int step{0};

  if (position == 0)
  {
    step = 0;
  }
  else
  {
    step = mconst::STEP;
  }
 airCompanyLogo->move(1,1 + step);
 airCompany->move(100,10 + step);
 planeModel->move(250,10 + step);
 flightNumber->move(500,10 + step);
 departureCity->move(70,50 + step);
 arrivalCity->move(500,50 + step);
 departureAbbreviation->move(70,70 + step);
 arrivalAbbreviation->move(500,70 + step);
 departureAirport->move(70,90 + step);
 arrivalAirport->move(500,90 + step);
 departureDate->move(70,110 + step);
 arrivalDate->move(500,110 + step);
 departureTime->move(70,130 + step);
 arrivalTime->move(500,130 + step);
 flightDuration->move(210,90+ step);

 QPixmap pix(mconst::WAY_PIC);
 planeWay->setPixmap(pix);
 planeWay->move(250,60 + step);

 ids.push_back(ticket.getId());
}


// bubble sort

void ResultsWindow::sortByPrice()
{
  QPushButton* buttonBuyCurrent;
  QPushButton* buttonBuyNext;
    for (int iterI = 0; iterI + 1 < frameList.size(); ++iterI)
      {
         for (int iterJ = 0; iterJ + 1 < frameList.size() - iterI; ++iterJ)
           {
             buttonBuyCurrent = frameList[iterJ]->findChild<QPushButton*>(frameList[iterJ]->objectName());
             buttonBuyNext = frameList[iterJ + 1]->findChild<QPushButton*>(frameList[iterJ + 1]->objectName());
             int currentPrice = buttonBuyCurrent->text().split(" ").at(0).toInt();
             int nextPrice = buttonBuyNext->text().split(" ").at(0).toInt();

             if (nextPrice < currentPrice)
               {
                 int tempX = frameList[iterJ]->x();
                 int tempY = frameList[iterJ]->y();

                 frameList[iterJ]->move(frameList[iterJ + 1]->x(),frameList[iterJ + 1]->y());
                 frameList[iterJ + 1]->move(tempX, tempY);

                 auto tempFrame = frameList[iterJ];
                 frameList[iterJ] = frameList[iterJ + 1];
                 frameList[iterJ + 1] = tempFrame;

               }
           }
      }
}


// bubble sort

void ResultsWindow::sortByDuration()
{
QList<QLabel*> curDateDiff;
QList<QLabel*> nextDateDiff;
  for (int iterI = 0; iterI + 1 < frameList.size(); ++iterI)
    {
       for (int iterJ = 0; iterJ + 1 < frameList.size() - iterI; ++iterJ)
         {
            curDateDiff = frameList[iterJ]->findChildren<QLabel*>(mconst::FLIGHTDURATION_INSECONDS_NAME);
            nextDateDiff = frameList[iterJ + 1]->findChildren<QLabel*>(mconst::FLIGHTDURATION_INSECONDS_NAME);

            int curDuration = 0;
            int nextDuration = 0;
            for (int ticIter = 0; ticIter < curDateDiff.size(); ++ticIter)
              {
                curDuration += curDateDiff[ticIter]->text().toInt();
                nextDuration += nextDateDiff[ticIter]->text().toInt();
              }

            if(nextDuration < curDuration)
            {
                int tempX = frameList[iterJ]->x();
                int tempY = frameList[iterJ]->y();

                frameList[iterJ]->move(frameList[iterJ + 1]->x(),frameList[iterJ + 1]->y());
                frameList[iterJ + 1]->move(tempX, tempY);

                auto tempFrame = frameList[iterJ];
                frameList[iterJ] = frameList[iterJ + 1];
                frameList[iterJ + 1] = tempFrame;
            }
         }
    }
}



QString ResultsWindow::secondsToString(qint64 seconds)
{
  const qint64 DAY = 86400;
  qint64 days = seconds / DAY;
  QTime t = QTime(0,0).addSecs(seconds % DAY);
  return QString(mconst::SECS_TO_DHS_MASK)
    .arg(days).arg(t.hour()).arg(t.minute()).arg(t.second());
}


void ResultsWindow::on_pushButtonTimeFilter_clicked()
{
    ui->frameTimeFilter->isHidden()? ui->frameTimeFilter->show(): ui->frameTimeFilter->hide();
    if (ui->frameTimeFilter->isHidden() == false)
      {
        ui->frameAirportFilter->hide();
        ui->frameDurationFilter->hide();
        ui->frameAircompanyFilter->hide();
        ui->framePriceFilter->hide();
      }
}


void ResultsWindow::on_pushButtonAirportFilter_clicked()
{
    ui->frameAirportFilter->isHidden()? ui->frameAirportFilter->show(): ui->frameAirportFilter->hide();
    if (ui->frameAirportFilter->isHidden() == false)
      {
        ui->frameTimeFilter->hide();
        ui->frameDurationFilter->hide();
        ui->frameAircompanyFilter->hide();
        ui->framePriceFilter->hide();
      }
}


void ResultsWindow::on_pushButtonDurationFilter_clicked()
{
  ui->frameDurationFilter->isHidden()? ui->frameDurationFilter->show(): ui->frameDurationFilter->hide();
  if (ui->frameDurationFilter->isHidden() == false)
    {
      ui->frameTimeFilter->hide();
      ui->frameAirportFilter->hide();
      ui->frameAircompanyFilter->hide();
      ui->framePriceFilter->hide();
    }
}


void ResultsWindow::on_pushButtonPriceFilter_clicked()
{
    ui->framePriceFilter->isHidden()? ui->framePriceFilter->show(): ui->framePriceFilter->hide();
    if (ui->framePriceFilter->isHidden() == false)
      {
        ui->frameTimeFilter->hide();
        ui->frameAirportFilter->hide();
        ui->frameDurationFilter->hide();
        ui->frameAircompanyFilter->hide();
      }
}


void ResultsWindow::on_pushButtonAircompanyFilter_clicked()
{
    ui->frameAircompanyFilter->isHidden()? ui->frameAircompanyFilter->show(): ui->frameAircompanyFilter->hide();
    if (ui->frameAircompanyFilter->isHidden() == false)
      {
        ui->frameTimeFilter->hide();
        ui->frameAirportFilter->hide();
        ui->frameDurationFilter->hide();
        ui->framePriceFilter->hide();
    }
}


void ResultsWindow::on_pushButtonApply_clicked()
{
  for (int iter = 0; iter < frameList.size(); ++iter)
    {
      const int controllSum{mconst::CONTROLL_SUM};
      bool isItHidden = false;

      QList<QLabel*> comparableDepartureTimeList = frameList[iter]->findChildren<QLabel*>(mconst::DEPARTURE_TIME_NAME);
      QList<QLabel*> comparableArrivalAirportList = frameList[iter]->findChildren<QLabel*>(mconst::ARRIVAL_AIRPORT_NAME);
      QList<QLabel*> comparableDepartureAirportList = frameList[iter]->findChildren<QLabel*>(mconst::DEPARTURE_AIRPORT_NAME);
      QList<QLabel*> comparableDurationList = frameList[iter]->findChildren<QLabel*>(mconst::FLIGHTDURATION_INSECONDS_NAME);
      QList<QLabel*> comparableAircompanyList = frameList[iter]->findChildren<QLabel*>(mconst::AIRCOMPANY_NAME);
      QList<QPushButton*> comparablePriceList = frameList[iter]->findChildren<QPushButton*>(frameList[iter]->objectName());

      for (int ticIter = 0; ticIter < numberOfWays; ++ticIter)
        {
            QLineEdit* minTime;
            QLineEdit* maxTime;
            QLineEdit* arrivalAirport;
            QLineEdit* departureAirport;
            QLineEdit* duration;
            QLineEdit* minPrice;
            QLineEdit* maxPrice;
            QLineEdit* aircompany;
            int token = 0;

          if (ticIter == 0)
            {
              minTime = ui->lineEditMinTime;
              maxTime = ui->lineEditMaxTime;
              arrivalAirport = ui->lineEditArrivalAirport;
              departureAirport = ui->lineEditDepartureAirport;
              duration = ui->lineEditMinFlightDuration;
              minPrice = ui->lineEditMinPrice;
              maxPrice = ui->lineEditMaxPrice;
              aircompany = ui->lineEditDepartureAircompany;
            }
          if (ticIter == 1)
            {
              minTime = ui->lineEditMinTime_2;
              maxTime = ui->lineEditMaxTime_2;
              arrivalAirport = ui->lineEditArrivalAirport_2;
              departureAirport = ui->lineEditDepartureAirport_2;
              duration = ui->lineEditMinFlightDuration_2;
              minPrice = ui->lineEditMinPrice_2;
              maxPrice = ui->lineEditMaxPrice_2;
              aircompany = ui->lineEditDepartureAircompany_2;
            }

          QTime choosedMinTime{00,00,00,00};
          if(minTime->text() != ":" && minTime->text().size() > mconst::MIN_TIMELABEL_LENGTH)
            {
              choosedMinTime = QTime::fromString(minTime->text());
              QTime comparableMinTime = QTime::fromString(comparableDepartureTimeList[ticIter]->text(), mconst::CONVERT_TIME_MASK);

              if (choosedMinTime > comparableMinTime)
                {
                  --token;
                }
              else
                {
                   ++token;
                }
            }
          else
            {
               ++token;
            }

          QTime choosedMaxTime{mconst::MAX_TIME_H, mconst::MAX_TIME_M};
          if(maxTime->text() != ":" && maxTime->text().size() > mconst::MIN_TIMELABEL_LENGTH)
            {
              choosedMaxTime = QTime::fromString(maxTime->text());
              QTime comparableMaxTime = QTime::fromString(comparableDepartureTimeList[ticIter]->text(), mconst::CONVERT_TIME_MASK);

              if (choosedMaxTime < comparableMaxTime)
                {
                  --token;
                }
              else
                {
                   ++token;
                }
            }
          else
            {
               ++token;
            }

          QString choosedArrivalAirport = "";
          if(arrivalAirport->text() != "")
            {
              choosedArrivalAirport = arrivalAirport->text();
              QString comparableArrivalAirport = comparableArrivalAirportList[ticIter]->text();

              if (choosedArrivalAirport != comparableArrivalAirport)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }

          QString choosedDepartureAirport = "";
          if(departureAirport->text() != "")
            {
              choosedDepartureAirport = departureAirport->text();
              QString comparableDepartureAirport = comparableDepartureAirportList[ticIter]->text();

              if (choosedDepartureAirport != comparableDepartureAirport)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }

          int choosedDuratioon{mconst::SECS_IN_A_DAY};
          if(duration->text() != ":" && duration->text().size() > mconst::MIN_TIMELABEL_LENGTH)
            {
              choosedDuratioon = QTime(0,0).secsTo(QTime::fromString(ui->lineEditMinFlightDuration->text()));
              int comparableDuration = comparableDurationList[ticIter]->text().toInt();
              if (choosedDuratioon > comparableDuration)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }

          int choosedMinPrice{mconst::MIN_PRICE};
          if(minPrice->text() != "")
            {
              choosedMinPrice = minPrice->text().toInt();
              int comparableMinPrice = (comparablePriceList[ticIter]->text().split(" ").at(0)).toInt();

              if (choosedMinPrice > comparableMinPrice)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }

          int choosedMaxPrice{mconst::MAX_PRICE};
          if(maxPrice->text() != "")
            {
              choosedMaxPrice = maxPrice->text().toInt();
              int comparableMaxPrice = (comparablePriceList[ticIter]->text().split(" ").at(0)).toInt();

              if (choosedMaxPrice < comparableMaxPrice)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }

          QString choosedAircompany = "";
          if(aircompany->text() != "")
            {
              choosedAircompany = aircompany->text();
              QString comparableAircompany = comparableAircompanyList[ticIter]->text();

              if (choosedAircompany != comparableAircompany)
                {
                  --token;
                }
              else
                {
                  ++token;
                }
            }
          else
            {
               ++token;
            }


          if (token < controllSum)
            {
              frameList[iter]->hide();
              isItHidden = true;
            }
          else if (isItHidden == false)
            {
              frameList[iter]->show();
            }
        }
    }
}


void ResultsWindow::on_lineEditDepartureAirport_textChanged()
{
  refreshArrivalAirportsBox(*ui->comboBoxArrivalAirport, *ui->lineEditArrivalAirport, *ui->lineEditDepartureAirport);
}


void ResultsWindow::on_lineEditDepartureAirport_2_textChanged()
{
   refreshArrivalAirportsBox(*ui->comboBoxArrivalAirport_2, *ui->lineEditArrivalAirport_2, *ui->lineEditDepartureAirport_2);
}


void ResultsWindow::refreshArrivalAirportsBox(QComboBox& combobox, QLineEdit& lineEdit, const QLineEdit& depAirport)
{
  combobox.clear();
  combobox.clearEditText();
  lineEdit.clear();
  arrivalAirports->clear();

  emit signal_fillInListOfArrivalAirports(*arrivalAirports, depAirport.text());
  QCompleter *arrivalAirportCompleter = new QCompleter(*arrivalAirports, this);
  lineEdit.setCompleter(arrivalAirportCompleter);
  combobox.addItems(*arrivalAirports);
  combobox.setCompleter(arrivalAirportCompleter);

  delete arrivalAirportCompleter;
}

 void ResultsWindow::fillInDepartureAirports(const QString& country, QComboBox& combobox, QLineEdit& lineEdit)
 {
   departureAirports->clear();

   emit signal_getAirports(*departureAirports, country);
   QCompleter *departureAirportsCompleter = new QCompleter(*departureAirports, this);
   lineEdit.setCompleter(departureAirportsCompleter);
   combobox.addItems(*departureAirports);
   combobox.setCompleter(departureAirportsCompleter);

   delete departureAirportsCompleter;
 }


  void ResultsWindow::fillInAircompanies(QComboBox& combobox, QLineEdit& lineEdit, const QString& depCountry, const QString& arrCountry)
  {
    departureAirports->clear();
    emit signal_getAircompanies(*departureAirports, depCountry, arrCountry);
    QCompleter *departureAirportsCompleter = new QCompleter(*departureAirports, this);
    lineEdit.setCompleter(departureAirportsCompleter);
    combobox.addItems(*departureAirports);
    combobox.setCompleter(departureAirportsCompleter);

    delete departureAirportsCompleter;
  }


void ResultsWindow::addFilters()
{
  ui->frameAircompanyFilter->setMinimumHeight(mconst::BIG_FLITER_HEIGHT);
  ui->frameAirportFilter->setMinimumHeight(mconst::BIG_FLITER_HEIGHT);
  ui->frameDurationFilter->setMinimumHeight(mconst::BIG_FLITER_HEIGHT);
  ui->framePriceFilter->setMinimumHeight(mconst::SMALL_FILTER_HEIGHT);
  ui->frameTimeFilter->setMinimumHeight(mconst::SMALL_FILTER_HEIGHT);

  QList<QLineEdit*> lienEditList = ui->Settings->findChildren<QLineEdit*>();
  foreach(auto item, lienEditList)
    {
       item->show();
    }

  QList<QComboBox*> cBoxList = ui->Settings->findChildren<QComboBox*>();
  foreach(auto item, cBoxList)
    {
       item->show();
    }
}


void ResultsWindow::setAircompanyLogo(QLabel& aircompanyLogo)
{
  QPixmap pixmap(mconst::COMPANY_LOGO_DIR+aircompanyLogo.text()+"");
  aircompanyLogo.setPixmap(pixmap);
}


void ResultsWindow::on_pushButtonBuy_clicked()
{
  QStringList list{emit signal_getEmailAndPhoneNumber(userID)};
  QString message = QString("Confirmation code has been sent to your phone number %1. Immediately after confirmation, an electronic copy of your ticket will be sent to your email - %2").arg(list.value(1)).arg(list.value(0));
  QMessageBox::information(this, "Confirmation", message);
}


void ResultsWindow::setFont()
{
  QList<QWidget*> widgetList = ui->OffersWidgetContents->findChildren<QWidget*>();
  foreach(auto item, widgetList)
    {
      item->setStyleSheet(mconst::TICKET_FONT);
    }
}


void ResultsWindow::startSettings()
{
   QList<QFrame*> frameList = ui->Settings->findChildren<QFrame*>();
   foreach(auto item, frameList)
     {
       item->hide();
       item->setFrameStyle(QFrame::WinPanel);
       item->setLineWidth(1);
       item->setFrameShadow(QFrame::Raised);
     }

    QList<QLabel*> labelList = ui->Settings->findChildren<QLabel*>();
    foreach(auto item, labelList)
      {
         item->show();
      }

    QIntValidator validator(mconst::MIN_PRICE, mconst::MAX_PRICE);

    ui->lineEditMinPrice->setValidator( new QIntValidator(mconst::MIN_PRICE, mconst::MAX_PRICE, this));
    ui->lineEditMaxPrice->setValidator( new QIntValidator(mconst::MIN_PRICE, mconst::MAX_PRICE, this));
    ui->lineEditMinTime->setInputMask(mconst::TIME_MASK);
    ui->lineEditMaxTime->setInputMask(mconst::TIME_MASK);
    ui->lineEditMinFlightDuration->setInputMask(mconst::TIME_MASK);

    ui->lineEditMinPrice_2->setValidator( new QIntValidator(mconst::MIN_PRICE, mconst::MAX_PRICE, this));
    ui->lineEditMaxPrice_2->setValidator( new QIntValidator(mconst::MIN_PRICE, mconst::MAX_PRICE, this));
    ui->lineEditMinTime_2->setInputMask(mconst::TIME_MASK);
    ui->lineEditMaxTime_2->setInputMask(mconst::TIME_MASK);
    ui->lineEditMinFlightDuration_2->setInputMask(mconst::TIME_MASK);
}


void ResultsWindow::createTicketFrame(QFrame& frame ,const QString& name, const int height)
{
  frame.setMinimumHeight(height);
  frame.setObjectName(name);
  frame.setFrameShape(QFrame::Panel);
}


void ResultsWindow::createButtonBuy(QPushButton& button, const QString& name, const int posY)
{
   button.setObjectName(name);
   button.move(mconst::BUTTON_XPOS, posY);
   button.setMinimumHeight(mconst::BUTTONHEIGHT);
   button.setMinimumWidth(mconst::BUTTONBUY_WIDTH);
}


void ResultsWindow::clear()
{
  QList<QLineEdit*> widgetList = findChildren<QLineEdit*>();
  foreach(auto item, widgetList)
    {
      item->clear();
    }
}


void ResultsWindow::deleteFrames()
{
  QList<QFrame*> frameList = ui->OffersWidgetContents->findChildren<QFrame*>("([0-9]+)");
  foreach(auto frame, frameList)
    {
       QList<QWidget*> widgetList = frame->findChildren<QWidget*>();
       foreach(auto widget, widgetList)
             {
               delete widget;
             }
       delete  frame;
    }
}


void ResultsWindow::on_radioButtonCheaper_toggled()
{
  sortByPrice();
}


void ResultsWindow::on_radioButtonFaster_toggled()
{
  sortByDuration();
}


void ResultsWindow::on_pushButtonClear_clicked()
{
    clear();
}
