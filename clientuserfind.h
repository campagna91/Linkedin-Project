#ifndef CLIENTUSERFIND_H
#define CLIENTUSERFIND_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>
#include "clientuser.h"
#include "db.h"
#include "linkediner.h"


class ClientUserFind : public QWidget
{
    Q_OBJECT

signals:
    void s_exit();

public:
    ClientUserFind(Db * db, Linkediner * u, QWidget *parent = 0);
    ~ClientUserFind();
    void showResult(Linkediner*);

public slots:
    void find(QString);

private slots:
    void on_finderBack_clicked();   //  used by user to return user gui
    void on_finderFind_clicked();   //  used by user to find an existing user
    void on_finderAdd_clicked();    //  used bu user to add foud user into proper associated people list

private:

    // graphic setup
    void setup();

    // logical objects
    Db * db;
    Linkediner * actualUser;

    // graphic object
    QLabel *finderBasicMessage;
    QLineEdit *finderPeople;
    QPushButton *finderFind,
                *finderBack,
                *finderAdd;
    QListWidget *finderDetailsWork,
                *finderDetailsLanguage,
                *finderDetailsContact,
                *finderDetailsPeople,
                *finderDetailsQualification;
    QTabWidget *finderDetails;
    QWidget *tab,
            *tab_2,
            *tab_3,
            *tab_5,
            *tab_4;
};

#endif // CLIENTUSERFIND_H
