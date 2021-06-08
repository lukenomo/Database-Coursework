#include "adminpanelwindow.h"
#include "ui_adminpanelwindow.h"

AdminPanelWindow::AdminPanelWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AdminPanelWindow)
{
  ui->setupUi(this);

  tdg = new TableDataGateway;
  connect(this, &AdminPanelWindow::signal_getDB, tdg, &TableDataGateway::slot_getDB);

  model = new QSqlRelationalTableModel(this, emit signal_getDB());
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  ui->tableView->setSortingEnabled(true);
}


AdminPanelWindow::~AdminPanelWindow()
{
  delete ui;
  delete tdg;
  delete model;
}


void AdminPanelWindow::slot_openAdminPanel()
{
  this->show();
}


void AdminPanelWindow::on_pushButtonBack_clicked()
{
  model->clear();
  this->hide();
  emit signal_goToMainWindow();
}


void AdminPanelWindow::on_radioButtonAirport_toggled()
{
  model->setTable("Airport");
  model->setRelation(model->fieldIndex("CityID"), QSqlRelation("City", "CityID", "Name"));
  model->select();
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
}


void AdminPanelWindow::on_radioButtonAccount_toggled()
{
  model->setTable("Account");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonAirCompany_toggled()
{
  model->setTable("AirCompany");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonFlight_toggled()
{
  model->setTable("Flight");
  model->setRelation(model->fieldIndex("FlightNumber"), QSqlRelation("FlightName", "FlightID", "Name"));
  model->setRelation(model->fieldIndex("DepartureCity"), QSqlRelation("City", "CityID", "Name"));
  model->setRelation(model->fieldIndex("ArrivalCity"), QSqlRelation("City", "CityID", "Name"));
  model->setRelation(model->fieldIndex("Aircompany"), QSqlRelation("Aircompany", "AircompanyID", "Name"));
  model->setRelation(model->fieldIndex("Plane"), QSqlRelation("Plane", "PlaneID", "PlaneNumber"));
  model->setRelation(model->fieldIndex("DepartureAirport"), QSqlRelation("Airport", "AirportID", "Name"));
  model->setRelation(model->fieldIndex("ArrivalAirport"), QSqlRelation("Airport", "AirportID", "Name"));
  model->setRelation(model->fieldIndex("DepartureAirport"), QSqlRelation("Airport", "AirportID", "Name"));
  model->select();
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
}


void AdminPanelWindow::on_radioButtonCountry_toggled()
{
  model->setTable("Country");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonPlane_toggled()
{
  model->setTable("Plane");
  model->setRelation(model->fieldIndex("FlightNumber"), QSqlRelation("FlightName", "FlightID", "Name"));
  model->setRelation(model->fieldIndex("PlaneNumber"), QSqlRelation("PlaneModel", "PlaneID", "PlaneModel"));
  model->select();
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
}


void AdminPanelWindow::on_radioButtonPlaneModel_toggled()
{
  model->setTable("PlaneModel");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonFlightName_toggled()
{
  model->setTable("FlightName");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonClass_toggled()
{
  model->setTable("Class");
  model->select();
  ui->tableView->setModel(model);
}


void AdminPanelWindow::on_radioButtonPrice_toggled()
{
  model->setTable("Price");
  model->setRelation(model->fieldIndex("FlightNumber"), QSqlRelation("FlightName", "FlightID", "Name"));
  model->setRelation(model->fieldIndex("Class"), QSqlRelation("Class", "ClassID", "Name"));
  model->select();
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
}


void AdminPanelWindow::on_radioButtonCity_toggled()
{
  model->setTable("City");
  model->setRelation(model->fieldIndex("CountryID"), QSqlRelation("Country", "CountriesID", "Name"));
  model->setRelation(model->fieldIndex("Class"), QSqlRelation("Class", "ClassID", "Name"));
  model->select();
  ui->tableView->setModel(model);
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
}


void AdminPanelWindow::on_pushButtonSubmit_clicked()
{
    model->submitAll();
}


void AdminPanelWindow::on_pushButtonReselect_clicked()
{
    model->select();
}


void AdminPanelWindow::on_pushButtonRevert_clicked()
{
    model->revertAll();
}


void AdminPanelWindow::on_pushButtonAddRow_clicked()
{
    model->insertRow(model->rowCount());
}


void AdminPanelWindow::on_pushButtonDeleteRow_clicked()
{
  int selectedRow {ui->tableView->currentIndex().row()};
  if (selectedRow >= 0)
      model->removeRow(selectedRow);
}
