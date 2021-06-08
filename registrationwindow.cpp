#include "registrationwindow.h"
#include "ui_registrationwindow.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::RegistrationWindow)
{
  ui->setupUi(this);
  tdg = new TableDataGateway();

  connect(this, &RegistrationWindow::signal_findAMatch, tdg, &TableDataGateway::slot_findMatches);
  connect(this, &RegistrationWindow::signal_createAnAccount, tdg, &TableDataGateway::slot_createAnAccount);

  ui->lineEditPassportSeries->setInputMask(mconst::PASSPORTSERIES_MASK);
  ui->lineEditPassportValidity->setInputMask(mconst::PASSPORTVALIDITY_MASK);
  ui->lineEditPhoneNumber->setInputMask(mconst::TELEPHONENUMBER_MASK);
}


void RegistrationWindow::on_pushButtonGoToMainWindow_clicked()
{
  // проверка lineEdit-ов на пустоту

  bool isPainted {false};

  for(QLineEdit* lineEdit : findChildren<QLineEdit*>())
    {
      if (lineEdit->text() == "" || lineEdit->text() == ".." || lineEdit->text() == "+()--")
        {
          paintLineEdit(*lineEdit);
          isPainted = true;
        }
    }

  if (isPainted)
    {
      QMessageBox::critical(this, "Error", "Fill in the blank fields");
      return;
    }


    // проверка имени

    if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_NAME).exactMatch(ui->lineEditName->text()))
      {
        paintLineEdit(*ui->lineEditName);
        QMessageBox::critical(this, "Error", "Incorrect name format");
        return;
      }


    // проверка фамилии

    if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_NAME).exactMatch(ui->lineEditSurname->text()))
      {
        paintLineEdit(*ui->lineEditSurname);
        QMessageBox::critical(this, "Error", "Incorrect surname format");
        return;
      }


    // проверка логина

    if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_LOGIN).exactMatch(ui->lineEditLogin->text()))
      {
        paintLineEdit(*ui->lineEditLogin);
        QMessageBox::critical(this, "Error", "Incorrect login format");
        return;
      }


    // проверка пароля

    if (ui->lineEditPassword->text().length() < 6)
      {
        paintLineEdit(*ui->lineEditPassword);
        QMessageBox::critical(this, "Error", "The password must contains at least 6 letters");
        return;
      }

    if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_PASSWORD).exactMatch(ui->lineEditPassword->text()))
    {
        paintLineEdit(*ui->lineEditPassword);
        QMessageBox::critical(this, "Error", "Incorrect password format");
        return;
    }


    // проверка E-mail

    if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_EMAIL).exactMatch(ui->lineEditEmail->text()))
      {
        paintLineEdit(*ui->lineEditEmail);
        QMessageBox::critical(this, "Error", "Incorrect e-mail format");
        return;
      }


    // проверка скрока действия паспорта

    if (ui->lineEditPassportValidity->text().length() < 10)
      {
        paintLineEdit(*ui->lineEditPassportValidity);
        QMessageBox::critical(this, "Error", "Enter the full validity date of your passport");
        return;
      }


    // проверка серии паспорта

    if (ui->lineEditPassportSeries->text().length() < 2)
      {
        paintLineEdit(*ui->lineEditPassportSeries);
        QMessageBox::critical(this, "Error", "Enter full passport series (2 letters)");
        return;
      }


    // проверка номера мобильного телефона

    if (ui->lineEditPhoneNumber->text().length() < 18)
      {
        paintLineEdit(*ui->lineEditPhoneNumber);
        QMessageBox::critical(this, "Error", "Enter full phone number");
        return;
      }


  // если проверка пройдена успешно - запись в таблицу и переход на следующую форму.

    int match = emit signal_findAMatch(ui->lineEditLogin->text(), ui->lineEditPassword->text());

    if (match == 0)
      {
         QMessageBox::critical(this, "Error", "An account with the same login and password already exists");
      }
    else if (match == 1)
      {
         QMessageBox::critical(this, "Error", "An account with the same login already exists");
      }
    else
      {
        emit signal_createAnAccount(ui->lineEditName->text(), ui->lineEditSurname->text(), ui->lineEditLogin->text(), ui->lineEditPassword->text() ,ui->lineEditPhoneNumber->text(), ui->lineEditEmail->text(), ui->lineEditCitizenship->text(), ui->lineEditPassportSeries->text(), ui->lineEditPassportValidity->text());

        this->close();
        emit signal_goToMainWindow(emit signal_getUserID(ui->lineEditLogin->text(), ui->lineEditPassword->text()));
        clear();
      }
    }


RegistrationWindow::~RegistrationWindow()
{
  delete ui;
  delete tdg;
}


void RegistrationWindow::slot_openRegistration()
{
  this->show();
}


void RegistrationWindow::on_pushButtonGoToLoginWindow_clicked()
{
  this->close();
  emit signal_goToLoginWindow();
  clear();
}


void RegistrationWindow::on_lineEditName_textChanged()
{
  ui->lineEditName->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditSurname_textChanged()
{
  ui->lineEditSurname->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditLogin_textChanged()
{
  ui->lineEditLogin->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditPassword_textChanged()
{
  ui->lineEditPassword->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditEmail_textChanged()
{
  ui->lineEditEmail->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditCitizenship_textChanged()
{
  ui->lineEditCitizenship->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditPassportValidity_textChanged()
{
  ui->lineEditPassportValidity->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditPassportSeries_textChanged()
{
  ui->lineEditPassportSeries->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::on_lineEditPhoneNumber_textChanged()
{
  ui->lineEditPhoneNumber->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void RegistrationWindow::paintLineEdit(QLineEdit& lineEdit)
{
  lineEdit.setStyleSheet(mconst::LINEEDIT_ERROR_STYLE);
}


void RegistrationWindow::clear()
{
  QList<QLineEdit*> lineEditList = findChildren<QLineEdit*>();
  foreach (auto item, lineEditList)
    {
      item->clear();
    }
}
