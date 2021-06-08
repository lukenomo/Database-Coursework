#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LoginWindow)
{
  ui->setupUi(this);
  tdg = new TableDataGateway();

  connect(this, &LoginWindow::signal_findAMatch, tdg, &TableDataGateway::slot_findMatches);
  connect(this, &LoginWindow::signal_getUserID, tdg, &TableDataGateway::slot_getUserID);

  QPixmap logo(mconst::LOGO_2);
  QPalette palette;
  palette.setBrush(QPalette::Background, logo);
  this->setPalette(palette);
}


LoginWindow::~LoginWindow()
{
  delete ui;
  delete tdg;
}


void LoginWindow::on_pushButtonEnter_clicked()
{
    // проверка lineEdit-ов на  пустоту

   if (ui->lineEditLogin->text() == "" && ui->lineEditPassword->text() == "")
    {
       paintLineEdit(*ui->lineEditLogin);
       paintLineEdit(*ui->lineEditPassword);
       QMessageBox::critical(this, "Error", "Fill in the blank fields");

       return;
    }

   if (ui->lineEditLogin->text() == "")
     {
       paintLineEdit(*ui->lineEditLogin);
       QMessageBox::critical(this, "Error", "Enter login");
       return;
     }

   if (ui->lineEditPassword->text() == "")
     {
       paintLineEdit(*ui->lineEditPassword);
       QMessageBox::critical(this, "Error", "Enter password");
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

   if (!creg::getRegularExpression(creg::RegularExpressions::REGEXP_PASSWORD).exactMatch(ui->lineEditPassword->text()))
     {
       paintLineEdit(*ui->lineEditPassword);
       QMessageBox::critical(this, "Error", "Incorrect password format");
       return;
     }


     int queryReslut = emit signal_findAMatch(ui->lineEditLogin->text(), ui->lineEditPassword->text());
     if (queryReslut == 0)
       {
         this->close();
         int temp = emit signal_getUserID(ui->lineEditLogin->text(), ui->lineEditPassword->text());
         emit signal_goToMainWindow(temp);
         clear();
       }

     else if (queryReslut == 1)
       {
         paintLineEdit(*ui->lineEditPassword);
         QMessageBox::critical(this, "Error", "Wrong password");
       }
     else
       {
         paintLineEdit(*ui->lineEditLogin);
         QMessageBox::critical(this, "Error", "No such account");
       }
}


void LoginWindow::slot_openLogin()
{
  this->show();
}


void LoginWindow::on_lineEditLogin_textChanged()
{
  ui->lineEditLogin->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void LoginWindow::on_lineEditPassword_textChanged()
{
  ui->lineEditPassword->setStyleSheet(mconst::LINEEDIT_DEFAULT_STYLE);
}


void LoginWindow::paintLineEdit(QLineEdit& lineEdit)
{
  lineEdit.setStyleSheet(mconst::LINEEDIT_ERROR_STYLE);
}


void LoginWindow::clear()
{
  QList<QLineEdit*> lineEditList = findChildren<QLineEdit*>();
  foreach (auto item, lineEditList)
    {
      item->clear();
    }
}


void LoginWindow::on_pushButtonGoToRegistration_clicked()
{
  this->close();
  emit signal_goToRegistrationWindow();
  clear();
}
