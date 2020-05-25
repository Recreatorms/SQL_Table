#include "authWindow.h"
#include "ui_authWindow.h"

AuthWindow::AuthWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AuthWindow)
{
  ui->setupUi(this);
}

AuthWindow::~AuthWindow()
{
  delete ui;
}

void AuthWindow::on_nameLineEdit_textEdited(const QString &arg1)
{
    AuthWindow::m_username = arg1;
}

void AuthWindow::on_passwordLineEdit_textEdited(const QString &arg1)
{
    AuthWindow::m_userpass = arg1;
}

void AuthWindow::on_loginPushButton_clicked()
{
    emit login_button_clicked();
}

void AuthWindow::on_guestPushButton_clicked()
{
    emit guest_button_clicked();
}

QString AuthWindow::getLogin()
{
    return AuthWindow::m_username;
}

QString AuthWindow::getPass()
{
  return AuthWindow::m_userpass;
}

void AuthWindow::clearInfo()
{
  this->ui->nameLineEdit->clear();
  this->ui->passwordLineEdit->clear();
  m_username = "";
  m_userpass = "";
}
