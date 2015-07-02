#ifndef CLIENTADMINNEWUSER_H
#define CLIENTADMINNEWUSER_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include "db.h"
#include "clientstart.h"

class ClientAdminNewUser : public QDialog
{
    Q_OBJECT

signals:
    void s_exit();

public:
    ClientAdminNewUser(Db *,QWidget *parent = 0);
    ~ClientAdminNewUser();

private slots:
    void on_dialogNewUserCreate_clicked();  // used by admin to create a new user
    void on_dialogNewUserBack_clicked();    // used by admin to escape from insertion gui

private:

    //graphic setup
    void setup();

    // logical objects
    Db * db;
    bool (*validate)(int,int,QObject*,QObject*);    // function pointer

    // graphic objects
    QLabel *dialogNewUserLabelPassword_2,
            *dialogNewUserLabelPassword,
            *dialogNewUserLabelUsername,
            *dialogNewUserLabelTitle,
            *dialogNewUserErrPassword,
            *dialogNewUserErrUsername,
            *dialogNewUserLabelType;
    QLineEdit   *dialogNewUserPassword,
                *dialogNewUserPasswordConfirm,
                *dialogNewUserUsername;
    QPushButton *dialogNewUserBack,
                *dialogNewUserCreate;
    QRadioButton    *dialogNewUserTypeBasic,
                    *dialogNewUserTypeExecutive,
                    *dialogNewUserTypeBusiness;
};

#endif // CLIENTADMINNEWUSER_H
