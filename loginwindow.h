#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QVariant>
#include <QRegularExpression>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QScreen>
#include "tabledatagateway.h"
#include "myregex.h"
#include "myConsts.h"

class TableDataGateway;

namespace Ui {
  class LoginWindow;
}

class LoginWindow : public QWidget
{
  Q_OBJECT

public:
  explicit LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow();

signals:
  void signal_goToRegistrationWindow();
  void signal_goToMainWindow(int);
  int signal_getUserID(const QString&, const QString&);

  // находит совпадение в таблице базы данных по логину и паролю. Возвращаемые значения: 0 - аккаунт существует, 1 - введен неверный пароль, 2 - аккаунта не существует
  int signal_findAMatch(const QString&, const QString&);

public slots:
  void slot_openLogin();

private slots:
  void on_pushButtonEnter_clicked();
  void on_lineEditLogin_textChanged();
  void on_lineEditPassword_textChanged();

  void on_pushButtonGoToRegistration_clicked();

private:
  Ui::LoginWindow *ui;
  TableDataGateway *tdg;

  void paintLineEdit(QLineEdit&);
  void clear();
};

#endif // LOGINWINDOW_H
