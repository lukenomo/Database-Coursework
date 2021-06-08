#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include "tabledatagateway.h"
#include "myregex.h"
#include "myConsts.h"

class TableDataGateway;

namespace Ui {
  class RegistrationWindow;
}

class RegistrationWindow : public QWidget
{
  Q_OBJECT

public:
  explicit RegistrationWindow(QWidget *parent = nullptr);

  ~RegistrationWindow();

signals:
  void signal_goToLoginWindow();
  void signal_goToMainWindow(int);
  int signal_getUserID(const QString&, const QString&);

  // находит совпадение в таблице базы данных по логину и паролю. Возвращаемые значения: 0 - аккаунт существует, 1 - введен неверный пароль, 2 - аккаунта не существует
  int signal_findAMatch(const QString&, const QString&);

  // принимает все все данные с lineEdit-ов на форме
  void signal_createAnAccount(const QString&, const QString&,  const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&);

public slots:
  void slot_openRegistration();

private slots:
  void on_pushButtonGoToLoginWindow_clicked();
  void on_pushButtonGoToMainWindow_clicked();
  void on_lineEditName_textChanged();
  void on_lineEditSurname_textChanged();
  void on_lineEditPassportSeries_textChanged();
  void on_lineEditLogin_textChanged();
  void on_lineEditPassword_textChanged();
  void on_lineEditEmail_textChanged();
  void on_lineEditCitizenship_textChanged();
  void on_lineEditPassportValidity_textChanged();
  void on_lineEditPhoneNumber_textChanged();

private:
  Ui::RegistrationWindow *ui;
  TableDataGateway *tdg;
  void paintLineEdit(QLineEdit&);
  void clear();
};

#endif // REGISTRATIONWINDOW_H
