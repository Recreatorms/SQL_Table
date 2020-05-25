#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QAction>
MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui_Auth.show();

  connect(&ui_Auth, &AuthWindow::guest_button_clicked, this, &MainWindow::enterAsGuest);
  connect(&ui_Auth, &AuthWindow::login_button_clicked, this, &MainWindow::authorize);
  ui->setupUi(this);
  connect(ui->actionLogout, &QAction::triggered, this, &MainWindow::deauthorize);
  this->setDisabled(true);
  ui->sexComboBox->addItem("Male");
  ui->sexComboBox->addItem("Female");

  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("database");
  bool is_open;
  is_open = db.open();
  qDebug() << is_open;
  QSqlQuery query;
  query.exec("CREATE TABLE faculty ( id_faculty INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128) NOT NULL)");
  query.exec("CREATE TABLE groups ( id_group INTEGER PRIMARY KEY AUTOINCREMENT, id_faculty INTEGER REFERENCES faculty(id_faculty), name VARCHAR(128) NOT NULL)");
  query.exec("CREATE TABLE students ( id_student INTEGER PRIMARY KEY AUTOINCREMENT, firstName VARCHAR(128) NOT NULL, lastName VARCHAR(128) NOT NULL, middleName VARCHAR(128), sex CHAR(1) CHECK(sex IN ('m', 'f')), id_group INTEGER REFERENCES groups(id_group), info VARCHAR(2048))");
  query.exec("SELECT COUNT(*) FROM faculty");
  query.next();
  QSqlQuery query1("SELECT name FROM faculty");
  for (int i = 0; i < query.value(0).toInt(); i++) {
      query1.next();
      ui->facultyComboBox->addItem(query1.value(0).toString());
    }

  query.exec("SELECT COUNT(*) FROM groups");
  query.next();
  query1.exec("SELECT name FROM groups");
  for (int i = 0; i < query.value(0).toInt(); i++) {
      query1.next();
      ui->groupsComboBox->addItem(query1.value(0).toString());
    }
  connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::click);
  connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::addMember);
  connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::showFullTable);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::authorize()
{

  if (ui_Auth.getLogin() == "admin" && ui_Auth.getPass() == "admin") {
      this->setDisabled(false);
      ui_Auth.clearInfo();
      ui_Auth.close();
      currentRole = "admin";
    } else if (ui_Auth.getLogin() == "moderator" && ui_Auth.getPass() == "moderator") {
      this->setDisabled(false);
      ui_Auth.clearInfo();
      ui_Auth.close();
      currentRole = "moderator";
    }
}

void MainWindow::deauthorize()
{
  ui_Auth.show();
  currentRole = "guest";
  this->setDisabled(true);


}

void MainWindow::enterAsGuest()
{
  this->setDisabled(false);
  ui_Auth.clearInfo();
  ui_Auth.close();
  currentRole = "guest";
}

void MainWindow::click() {
  if (currentRole == "admin") {
      QSqlQuery query;
      query.exec(ui->textEdit->toPlainText());
      model.setQuery(query);

      if (model.lastError().isValid())
        qDebug() << model.lastError();

      ui->tableView->setModel(&model);
      ui->tableView->show();
    }
}

void MainWindow::addMember() {
  if (currentRole == "admin") {
      QString queryText = "INSERT INTO ";
      queryText += ui->tabWidget->tabBar()->tabText(ui->tabWidget->currentIndex());
      switch (ui->tabWidget->currentIndex()) {
        case 0: {
            QString facultyName = ui->facultyTextEdit->toPlainText();
            if (facultyName != "") {
                QSqlQuery check;
                check.exec("SELECT id_faculty FROM faculty WHERE name = '" + facultyName + "'");
                bool bCheck = check.first();
                if (bCheck == false) {
                    queryText += " (name) VALUES ('";
                    queryText += facultyName + "') ";
                    ui->facultyComboBox->addItem(facultyName);
                    break;
                  }
              }
            return;
          }
        case 1: {
            QString idFaculty = ui->facultyComboBox->currentText(),
                groupName = ui->groupTextEdit->toPlainText();
            if (idFaculty != "" && groupName != "") {
                QSqlQuery check;
                check.exec("SELECT id_group FROM groups WHERE id_faculty = '" + idFaculty + "' AND name = '" + groupName + "'");
                bool bCheck = check.first();
                if (bCheck == false) {
                    queryText += "(id_faculty, name) VALUES ('";
                    queryText += idFaculty + "', '" + groupName + "')";
                    ui->groupsComboBox->addItem(groupName);
                    break;
                  }
              }
            return;
          }
        case 2: {
            QString firstName = ui->firstNameLineEdit->text(),
                lastName =  ui->lastNameLineEdit->text(),
                middleName = ui->middleNameLineEdit->text(),
                idGroup = ui->groupsComboBox->currentText(),
                info = ui->studentInfoTextEdit->toPlainText();
            if (firstName != "" && lastName != "" && idGroup != "") {
                QSqlQuery check;
                check.exec("SELECT id_group FROM students WHERE firstName = '" + firstName + "' AND lastName = '" + lastName + "' AND middleName = '" + middleName + "' AND id_group = '" + idGroup +"'");
                bool bCheck = check.first();
                if (bCheck == false) {
                    queryText += "(firstName, lastName, middleName, sex, id_group, info) VALUES ('";
                    queryText += firstName + "', '" + lastName + "', '" + middleName + "', '" + ((ui->sexComboBox->currentText() == "Male" ) ? 'm' : 'f') + "', '" + idGroup + "', '" + info + "')";
                    break;
                  }
              }
            return;
          }
        }

      QSqlQuery query;
      query.exec(queryText);
      showFullTable();
    }
}

void MainWindow::showFullTable()
{
  QSqlQuery query;
  QString queryText = "SELECT * FROM ";
  queryText += ui->tabWidget->tabBar()->tabText(ui->tabWidget->currentIndex());
  query.exec(queryText);
  model.setQuery(query);

  if (model.lastError().isValid())
    qDebug() << model.lastError();

  ui->tableView->setModel(&model);
  ui->tableView->show();


}

