#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include "authWindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
   void authorize();
   void deauthorize();
   void enterAsGuest();
   void click();
   void addMember();
   void showFullTable();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    AuthWindow ui_Auth;
    QSqlQueryModel model;
    QString currentRole;
};
#endif // MAINWINDOW_H
