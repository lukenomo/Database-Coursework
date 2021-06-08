#ifndef ADMINPANELWINDOW_H
#define ADMINPANELWINDOW_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include "tabledatagateway.h"

class TableDataGateway;

namespace Ui {
  class AdminPanelWindow;
}

class AdminPanelWindow : public QWidget
{
  Q_OBJECT

public:
  explicit AdminPanelWindow(QWidget *parent = nullptr);
  ~AdminPanelWindow();

public slots:
  void slot_openAdminPanel();

signals:
  void signal_goToMainWindow();
  QSqlDatabase signal_getDB();

private slots:
  void on_pushButtonBack_clicked();
  void on_radioButtonAirport_toggled();
  void on_radioButtonAirCompany_toggled();
  void on_radioButtonAccount_toggled();
  void on_radioButtonFlight_toggled();
  void on_radioButtonCountry_toggled();
  void on_radioButtonPlane_toggled();
  void on_radioButtonPlaneModel_toggled();
  void on_radioButtonFlightName_toggled();;
  void on_radioButtonClass_toggled();
  void on_radioButtonPrice_toggled();
  void on_radioButtonCity_toggled();
  void on_pushButtonSubmit_clicked();
  void on_pushButtonReselect_clicked();
  void on_pushButtonRevert_clicked();
  void on_pushButtonAddRow_clicked();
  void on_pushButtonDeleteRow_clicked();

private:
  Ui::AdminPanelWindow *ui;
  QSqlRelationalTableModel *model;
  TableDataGateway *tdg;
};

#endif // ADMINPANELWINDOW_H
