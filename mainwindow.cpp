#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    query.exec("CREATE TABLE students ( id_student INTEGER PRIMARY KEY AUTOINCREMENT, firstName VARCHAR(128) NOT NULL CHECK(firstName NOT IN(SELECT firstName FROM students)), lastName VARCHAR(128) NOT NULL, middleName VARCHAR(128), sex CHAR(1) CHECK(sex IN ('m', 'f')), id_group INTEGER REFERENCES groups(id_group), info VARCHAR(2048))");

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

void MainWindow::click()
{
    QSqlQuery query;
    query.exec(ui->textEdit->toPlainText());
    model.setQuery(query);

    if (model.lastError().isValid())
        qDebug() << model.lastError();

    ui->tableView->setModel(&model);
    ui->tableView->show();
}

void MainWindow::addMember()
{
    QString queryText = "INSERT INTO ";
    queryText += ui->tabWidget->tabBar()->tabText(ui->tabWidget->currentIndex());
    switch (ui->tabWidget->currentIndex()) {
    case 0: {
        queryText += " (name) VALUES ('";
        queryText += ui->facultyTextEdit->toPlainText() + "') ";
        ui->facultyComboBox->addItem(ui->facultyTextEdit->toPlainText());
        break;
    }
    case 1: {
        queryText += "(id_faculty, name) VALUES ('";
        queryText += ui->facultyComboBox->currentText() + "', '" + ui->groupTextEdit->toPlainText() + "')";
        ui->groupsComboBox->addItem(ui->groupTextEdit->toPlainText());
        break;
    }
    case 2: {
        queryText += "(firstName, lastName, middleName, sex, id_group, info) VALUES ('";
        queryText += ui->firstNameLineEdit->text() + "', '" + ui->lastNameLineEdit->text() + "', '" + ui->middleNameLineEdit->text() + "', '" + ((ui->sexComboBox->currentText() == "Male" ) ? 'm' : 'f') + "', '" + ui->groupsComboBox->currentText() + "', '" + ui->studentInfoTextEdit->toPlainText() + "')";
        break;
    }
    }

    QSqlQuery query;
    query.exec(queryText);
    showFullTable();
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

