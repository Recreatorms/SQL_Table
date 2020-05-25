#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>

namespace Ui {
  class AuthWindow;
}

class AuthWindow : public QWidget
{
  Q_OBJECT


public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();
    QString getLogin();
    QString getPass();
    void clearInfo();
signals:
    void login_button_clicked();
    void guest_button_clicked();
private slots:
    void on_nameLineEdit_textEdited(const QString &arg1);

    void on_passwordLineEdit_textEdited(const QString &arg1);

    void on_loginPushButton_clicked();
    void on_guestPushButton_clicked();
private:
    Ui::AuthWindow *ui;
    QString m_username;
    QString m_userpass;
};

#endif // AUTH_WINDOW_H
