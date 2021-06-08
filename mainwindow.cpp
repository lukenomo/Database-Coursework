#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  regw = new RegistrationWindow();
  lw   = new LoginWindow();
  lw->show();
  tdg  = new TableDataGateway();
  resw = new ResultsWindow();
  apw = new AdminPanelWindow();

  arrivalCities       = new QStringList();
  departureCities     = new QStringList();
  backDepartureDates  = new QStringList();
  thereDepartureDates = new QStringList();

  connect(this, &MainWindow::signal_goToLoginWindow, lw, &LoginWindow::slot_openLogin);
  connect(this, &MainWindow::signal_goToAdminPanelWindow, apw, &AdminPanelWindow::slot_openAdminPanel);
  connect(apw, &AdminPanelWindow::signal_goToMainWindow, this, &MainWindow::slot_openMain);
  connect(lw, &LoginWindow::signal_goToMainWindow, this, &MainWindow::slot_openMainWithID);
  connect(lw, &LoginWindow::signal_goToRegistrationWindow, regw, &RegistrationWindow::slot_openRegistration);
  connect(regw, &RegistrationWindow::signal_goToLoginWindow, lw, &LoginWindow::slot_openLogin);
  connect(regw, &RegistrationWindow::signal_goToMainWindow, this, &MainWindow::slot_openMainWithID);
  connect(this, &MainWindow::signal_getCities, tdg, &TableDataGateway::slot_fillInListOfCities);
  connect(this, &MainWindow::signal_fillInListOfArrivalCities, tdg, &TableDataGateway::slot_fillInListOfArrivalCities);
  connect(ui->comboBoxDepartureCity, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditDepartureCity, &QLineEdit::setText);
  connect(ui->comboBoxArrivalCity, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated),ui->lineEditArrivalCity, &QLineEdit::setText);
  connect(this, &MainWindow::signal_fillInListOfDepartureDates, tdg, &TableDataGateway::slot_fillInListOfDepartureDates);
  connect(this, &MainWindow::signal_goToResultsWindow, resw, &ResultsWindow::slot_closeMainOpenResults);
  connect(this, &MainWindow::signal_ticketSettings, resw, &ResultsWindow::slot_ticketSettings);
  connect(resw, &ResultsWindow::signal_closeResultsOpenMain, this, &MainWindow::slot_openMain);
  connect(this, &MainWindow::signal_getUserName, tdg, &TableDataGateway::slot_getUserName);
  connect(this, &MainWindow::signal_isAdmin, tdg, &TableDataGateway::slot_isAdmin);

  // Зполнение городов отправки при инициализации формы

  emit signal_getCities(*departureCities);

  // установока подсказок и заполнение бокса

  QCompleter *departureCityCompleter = new QCompleter(*departureCities, this);
  ui->lineEditDepartureCity->setCompleter(departureCityCompleter);
  ui->comboBoxDepartureCity->addItems(*departureCities);
  ui->comboBoxDepartureCity->setCompleter(departureCityCompleter);
  ui->dateEditDeparture->date();
  delete departureCityCompleter;

  // настройки календаря по умолчанию
  QDate currenDate = QDate::currentDate();
  ui->dateEditDeparture->setCalendarPopup(true);
  ui->dateEditDeparture->setMinimumDate(currenDate);
  ui->dateEditDeparture->setMaximumDate(currenDate.addMonths(1));
  ui->dateEditBack->setCalendarPopup(true);
  ui->dateEditBack->setMinimumDate(currenDate);
  ui->dateEditBack->setMaximumDate(currenDate.addMonths(1));

  // Места и тип билета по умолчанию
  passengerCounter = mconst::MIN_PASSANGERS;
  ui->radioButtonEconomy->setChecked(true);
  ui->pushButtonShowCalculator->setText(QString::number(passengerCounter) +"  "+ ui->radioButtonEconomy->text());

  ui->frameCalculator->hide();
  ui->dateEditBack->hide();
  ui->pushButtonAdminPanel->hide();
}


MainWindow::~MainWindow()
{
  delete arrivalCities;
  delete departureCities;
  delete backDepartureDates;
  delete thereDepartureDates;
  delete ui;
  delete lw;
  delete regw;
  delete resw;
  delete apw;
  delete tdg;
}


void MainWindow::slot_openMainWithID(int userID)
{
  this->show();
  this->userID = userID;
  ui->labelUserName->setText("Hello, " + emit signal_getUserName(userID));
  showAdminPanelButton();
}


void MainWindow::slot_openMain()
{
  this->show();
}


void MainWindow::on_lineEditDepartureCity_textChanged()
{
  ui->comboBoxArrivalCity->clear();
  ui->comboBoxArrivalCity->clearEditText();
  ui->lineEditArrivalCity->clear();
  arrivalCities->clear();

  emit signal_fillInListOfArrivalCities(*arrivalCities, ui->lineEditDepartureCity->text());

  QCompleter *arrivalCityCompleter = new QCompleter(*arrivalCities, this);
  ui->lineEditArrivalCity->setCompleter(arrivalCityCompleter);
  ui->comboBoxArrivalCity->addItems(*arrivalCities);
  ui->comboBoxArrivalCity->setCompleter(arrivalCityCompleter);

  delete arrivalCityCompleter;

  markCalendar(*thereDepartureDates, dateEditThereDeparture, ui->dateEditDeparture->calendarWidget(), false);
  markCalendar(*backDepartureDates, dateEditBackDeparture, ui->dateEditBack->calendarWidget(), false);

  ui->lineEditDepartureCity->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void MainWindow::on_pushButtonTwoWays_clicked()
{
  ui->dateEditBack->isHidden()? ui->dateEditBack->show(): ui->dateEditBack->hide();
}


void MainWindow::on_lineEditArrivalCity_textChanged()
{
  markCalendar(*thereDepartureDates, dateEditThereDeparture, ui->dateEditDeparture->calendarWidget(), false);
  markCalendar(*backDepartureDates, dateEditBackDeparture, ui->dateEditBack->calendarWidget(), false);

  emit signal_fillInListOfDepartureDates(*thereDepartureDates, ui->lineEditDepartureCity->text(), ui->lineEditArrivalCity->text());

  emit signal_fillInListOfDepartureDates(*backDepartureDates, ui->lineEditArrivalCity->text(), ui->lineEditDepartureCity->text());

  markCalendar(*thereDepartureDates, dateEditThereDeparture, ui->dateEditDeparture->calendarWidget(), true);
  markCalendar(*backDepartureDates, dateEditBackDeparture, ui->dateEditBack->calendarWidget(), true);

  ui->lineEditArrivalCity->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void MainWindow::markCalendar(QStringList& dates, QCalendarWidget* calendarController, QCalendarWidget* calendar, bool mark)
{
  QTextCharFormat format;
  QBrush brush;
  QColor color;
  int r, g, b, a;

  if (mark)
    {
      r  = mconst::rMark;
      g  = mconst::gMark;
      b  = mconst::bMark;
      a  = mconst::aMark;
    }
  else
    {
      r  = mconst::rUnmark;
      g  = mconst::gUnmark;
      b  = mconst::bUnmark;
      a  = mconst::aUnmark;
    }

  color.setRgb(r,g,b,a);
  brush.setColor(color);
  format.setBackground(brush);

  calendarController = calendar;

   for (QString& date: dates)
     {
       calendarController->setDateTextFormat(QDate::fromString(date,"dd.MM.yyyy"),format);
     }

   calendarController->show();
}


void MainWindow::on_pushButtonShowCalculator_clicked()
{
  ui->frameCalculator->isHidden()?ui->frameCalculator->show():ui->frameCalculator->hide();
}


// калькулятор мест

void MainWindow::on_pushButtonPlusAdults_clicked()
{
  if (passengerCounter < mconst::MAX_PASSANGERS)
    {
      ++passengerCounter;
      ui->labelAdultsCount->setText(QString::number(ui->labelAdultsCount->text().toInt() + 1));
      tuneTicket();
    }
  else
    {
       QMessageBox::critical(this, "Error", "The maximum number to search " + QString::number(mconst::MAX_PASSANGERS));
    }
}

void MainWindow::on_pushButtonMinuseAdults_clicked()
{
  if (passengerCounter > mconst::MIN_ADULTS)
    {
      if (ui->labelKids->text().toInt() < ui->labelAdultsCount->text().toInt())
        {
          --passengerCounter;
          ui->labelAdultsCount->setText(QString::number(ui->labelAdultsCount->text().toInt() - 1));
          tuneTicket();
        }
      else
        {
          QMessageBox::critical(this, "Error", "Number of babies cannot bigger than number of adults in one order");
        }
    }
  else
    {
       QMessageBox::critical(this, "Error", "The minimum number to search "  + QString::number(mconst::MIN_ADULTS));
    }
}


void MainWindow::on_pushButtonPlusChildren_clicked()
{
  if (passengerCounter < mconst::MAX_PASSANGERS)
    {
      if (ui->labelChildrenCount->text().toInt() < mconst::MAX_CHILDREN)
        {
          ++passengerCounter;
          ui->labelChildrenCount->setText(QString::number(ui->labelChildrenCount->text().toInt() + 1));
          tuneTicket();
        }
      else
        {
          QMessageBox::critical(this, "Error", "You choosed the maximum number of children: " + QString::number(mconst::MAX_CHILDREN));
        }
    }
  else
    {
       QMessageBox::critical(this, "Error", "The maximum number to search: " + QString::number(mconst::MAX_PASSANGERS));
    }
}

void MainWindow::on_pushButtonMinusChildren_clicked()
{
  if (ui->labelChildrenCount->text().toInt() > mconst::MIN_CHILDREN)
    {
      --passengerCounter;
      ui->labelChildrenCount->setText(QString::number(ui->labelChildrenCount->text().toInt() - 1));
      tuneTicket();
    }
  else
    {
       QMessageBox::critical(this, "Error", "The minimum number to search: " + QString::number(mconst::MIN_CHILDREN));
    }
}


void MainWindow::on_pushButtonPlusKids_clicked()
{
  if (passengerCounter < mconst::MAX_PASSANGERS)
    {
      if (ui->labelKidsCount->text().toInt() < ui->labelAdultsCount->text().toInt())
        {
           ++passengerCounter;
           ui->labelKidsCount->setText(QString::number(ui->labelKidsCount->text().toInt() + 1));
           tuneTicket();
        }
        else
        {
           QMessageBox::critical(this, "Error", "Number of kids can't bigger than number of adults in one order");
        }
    }
  else
    {
        QMessageBox::critical(this, "Error", "The maximum number to search: " + QString::number(mconst::MAX_PASSANGERS));
    }
}

void MainWindow::on_pushButtonMinusKids_clicked()
{
  if (ui->labelKidsCount->text().toInt() > mconst::MIN_KIDS)
    {
       --passengerCounter;
       ui->labelKidsCount->setText(QString::number(ui->labelKidsCount->text().toInt() - 1));
       tuneTicket();
    }
  else
    {
       QMessageBox::critical(this, "Error", "The minimum number to search: " + QString::number(mconst::MIN_KIDS));
    }
}


void MainWindow::on_radioButtonEconomy_toggled()
{
  tuneTicket();
}

void MainWindow::on_radioButtonBusiness_toggled()
{
  tuneTicket();
}

void MainWindow::tuneTicket()
{
   QList<QRadioButton*> allRButtons = ui->groupBoxTicketType->findChildren<QRadioButton *>();
   for (const QRadioButton* item: qAsConst(allRButtons))
     {
       if (item->isChecked())
         {
             ui->pushButtonShowCalculator->setText(QString::number(passengerCounter) +",  "+ item->text());
         }
     }
}

void MainWindow::on_pushButtonFind_clicked()
{
  QString ticketType;
  QList<QRadioButton*> allRButtons = ui->groupBoxTicketType->findChildren<QRadioButton*>();

  for (const QRadioButton* item: qAsConst(allRButtons))
    {
      if (item->isChecked())
        {
          ticketType = item->text();
        }
    }

  if ((ui->lineEditArrivalCity->text() != "") || (ui->lineEditDepartureCity->text() != ""))
    {
      if(ui->lineEditArrivalCity->text() != "")
        {
          if(ui->lineEditDepartureCity->text() != "")
            {
              if (ui->dateEditBack->isHidden())
                     {
                       emit signal_ticketSettings(ui->lineEditDepartureCity->text(),
                                                  ui->lineEditArrivalCity->text(),
                                                  ui->labelAdultsCount->text().toInt(),
                                                  ui->labelChildrenCount->text().toInt(),
                                                  ui->labelKidsCount->text().toInt(),
                                                  ticketType,
                                                  userID,
                                                   QDate::fromString(ui->dateEditDeparture->text(),          mconst::CONVERT_DATE_MASK));
                     }
                   else
                     {
                       emit signal_ticketSettings(ui->lineEditDepartureCity->text(),
                                                     ui->lineEditArrivalCity->text(),
                                                     ui->labelAdultsCount->text().toInt(),
                                                     ui->labelChildrenCount->text().toInt(),
                                                     ui->labelKidsCount->text().toInt(),
                                                     ticketType,
                                                     userID,
                                                     QDate::fromString(ui->dateEditDeparture->text(),     mconst::CONVERT_DATE_MASK),
                                                     QDate::fromString(ui->dateEditBack->text(),   mconst::CONVERT_DATE_MASK));
                     }
            }
         else
            {
              paintLineEdit(*ui->lineEditDepartureCity);
              QMessageBox::critical(this, "Error", "Fill in departure city");
              return;
            }

        }
     else
        {
          paintLineEdit(*ui->lineEditArrivalCity);
          QMessageBox::critical(this, "Error", "Fill in arrival city");
          return;
        }
    }
  else
    {
      paintLineEdit(*ui->lineEditDepartureCity);
      paintLineEdit(*ui->lineEditArrivalCity);
      QMessageBox::critical(this, "Error", "Fill in the blank fields");
      return;
    }

  this->close();
  resw->show();
}


void MainWindow::on_pushButtonLogOut_clicked()
{
  this->close();
  emit signal_goToLoginWindow();
  clear();
}


void MainWindow::paintLineEdit(QLineEdit& lineEdit)
{
  lineEdit.setStyleSheet(mconst::LINEEDIT_ERROR_STYLE);
}


void MainWindow::clear()
{
  QList<QLineEdit*> lineEditList = findChildren<QLineEdit*>();
  foreach (auto item, lineEditList)
    {
      item->clear();
    }
}


void MainWindow::on_pushButtonAdminPanel_clicked()
{
  this->close();
  clear();
  emit signal_goToAdminPanelWindow();
}

void MainWindow::showAdminPanelButton()
{

  if (emit signal_isAdmin(userID) == 1)
    {
      ui->pushButtonAdminPanel->show();
    }
  else
    {
      ui->pushButtonAdminPanel->hide();
    }
}


void MainWindow::slot_closeApp()
{
  this->~MainWindow();
}
