#ifndef CLIENTSTART_H
#define CLIENTSTART_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>
#include <QFileDialog>
// graphic part
#include "clientadmin.h"
#include "clientuser.h"
// logical part
#include "db.h"
#include "linkediner.h"

class ClientStart : public QMainWindow
{
    Q_OBJECT

public:
    ClientStart(QWidget *parent = 0);
    ~ClientStart();
    static bool isValid(int,int,QObject*,QObject* =0);

private slots:
    void feed(int,int); //  used to show some alert message
    void saveRequest(); //  used to save update done from admin/client gui
    void undoUpdate();  //  used to undo update from admin/client gui
    void exitAdminGui();    // used at the escape from admin gui
    void exitUserGui(); //  used at the escape from user gui
    void on_username_returnPressed();   // it allows directed pressing return
    void on_password_returnPressed();   // it allows directed pressing return
    void on_loginButton_clicked();  // require user login
    void on_adminButton_clicked();  // require admin gui
    void on_actionApri_triggered(); // used for choose an existing db
    void on_actionNuovo_triggered();    // used for choose a destination for new db
    void on_actionSalva_triggered();    // used to save udpate
    void on_actionEsci_triggered(); // used to exit from Linkedin gui

private:

    // graphic setup
    void setup();

    // logical object
    Db * db;
    QWidget * guiAdmin;
    QWidget * guiUser;

    // graphic object
    QAction *actionEsci,
            *actionApri,
            *actionNuovo,
            *actionSalva,
            *actionLinkedin;
    QWidget *centralWidget;
    QLineEdit *username;
    QLineEdit *password;
    QLabel *labelSecond,
            *labelFirst,
            *labelThird;
    QPushButton *adminButton;
    QPushButton *loginButton;
    QMenuBar *menuBar;
    QMenu *menuFile,
            *menuDatabase,
            *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
};

#endif // CLIENTSTART_H
