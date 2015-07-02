#ifndef CLIENTADMIN_H
#define CLIENTADMIN_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>
#include <iostream>
#include "clientstart.h"
#include "clientadminnewuser.h"
#include "db.h"

using namespace std;

class ClientAdminNewUser;
class ClientAdmin : public QWidget
{
    Q_OBJECT

signals:
    void s_save();  //  used for save udpate
    void s_notSave();   //  used for undo update
    void s_exit();  //  used for escape from client gui

public:
    ClientAdmin(Db*,QWidget *parent = 0);
    ~ClientAdmin();
    void loadCount();   //  load info counts
    void frameUsersFilterLoad();    //  load admin filter
    void frameUsersLoad();  //  load user list
    void frameDetailsLoad();    //  load user detail after double click
    void frameAccountLoad();    //  load account information
    void frameProfileLoad();    //  load profile information
    void frameLanguageLoad();   //  load language list
    void frameWorkLoad();   //  load work list
    void frameQualificationLoad();  //  load qualification list
    void frameNetLoad();    // load contact ( friend ) list
    void filter(string email, string sector ,int year); // filter user list
    void clearHiddenBottons();  // hide some bottons

private slots:

    // general slots
    void on_exit_clicked(); // used when admin require escape
    void exitDialogNewUser();   //  used when admin escape from insertion of new user
    void on_frameUsersAdd_clicked();    //  used when admin want add a new user
    void on_frameUsersDelete_clicked(); // used when admin remove an existing user
    void on_frameUsersFilterYear_valueChanged(int arg1);    // used when admin change year filter
    void on_frameUsersList_itemDoubleClicked(QListWidgetItem *item);    //  used when admin click an user from list to see more information about it
    void on_frameUsersFilterUsername_textChanged(const QString &arg1); // used when admin change username filter
    void on_frameUsersFilterSector_currentTextChanged(const QString &arg1); //  used when admin change sector filter

    // account slots
    void on_frameAccountUpdate_clicked();   //  used by admin to update user account informations
    void on_frameAccountChangetype_clicked();   // used by admin to change account type
    void on_frameAccountNewPassword_textChanged(const QString &arg1);   // used by admin to change user password without confirm

    // profile slots
    void on_frameProfileAddNewAddress_clicked();    //  used by admin to add new contact in user's contact-list
    void on_frameProfileUndoUpdate_clicked();   //  used by admin to undo updates not still saved
    void on_frameProfileDeleteAddress_clicked();    //  used by admin to delete a contact from user's contact-list
    void on_frameProfileUpdate_clicked();   //  used by admin to update profile with new informations
    void on_frameProfileName_textChanged(const QString &arg1);  //  triggered at change of user's name
    void on_frameProfileBirth_textChanged(const QString &arg1); //  triggered at change of user's birth
    void on_frameProfileSurname_textChanged(const QString &arg1);   //  triggered at change of user's surname
    void on_frameProfileNationality_textChanged(const QString &arg1);   //  triggered at change of user's nationality

    // work slots
    void on_frameNewWorkDeleteSkill_clicked();  // used by admin to delete a skill from work-skill-list
    void on_frameWorkList_itemDoubleClicked(QListWidgetItem *item); //  used by admin to udpate or remove a work
    void on_frameNewWorkAddSkill_clicked(); //  used by admin for add skill to work-skill-list
    void on_frameNewWorkAdd_clicked();  //  used by admin to add a work
    void on_frameWorkUpdate_clicked();  // used by admin to update an existing work
    void on_frameWorkUndoUpdate_clicked();  // used by admin to undo update not still saved
    void on_frameWorkDelete_clicked();  // used by admin to delete an existing work

    // language slots
    void on_frameLanguageUpdate_clicked();  // used by admin to udpate an existing language
    void on_frameLanguageUndoUpdate_clicked();  //  used by admin to undo udpate not still saved
    void on_frameNewLanguageAdd_clicked();  // used by admin for add a new language
    void on_frameLanguageDelete_clicked();  // used by admin to delete an existing language
    void on_frameLanguageList_itemDoubleClicked(QListWidgetItem *item); // used by admin for update or remove an existing language

    // qualification slots
    void on_frameQualificationUpdate_clicked(); // used by admin for update an existing qualification
    void on_frameQualificationUndoUpdate_clicked(); // used by admin to undo update not still saved
    void on_frameNewQualificationAdd_clicked(); // used by admin to add a new qualification
    void on_frameQualificationDelete_clicked(); // used by admin to delete an existing qualification
    void on_frameQualificationList_itemDoubleClicked(QListWidgetItem *item);    // used by admin for remove or update and existing qualification

    // net slot
    void on_frameNetDelete_clicked();   // used by admin to delete an associated person
    void on_frameNewNetAdd_clicked();   // used by admin to add an new person in associated people list

private:

    // graphic setup
    void setup();

    // logical objects
    ClientAdminNewUser * dialogNewUser;
    Db * db;
    Linkediner * actualUser;
    Works * actualWork;
    Qualifications * actualQualification;
    Languages * actualLanguage;
    bool (*validate)(int,int,QObject*,QObject*);

    // graphic objects
    QPushButton *frameAccountUpdate,
                *exit,
                *frameAccountChangetype,
                *frameProfileUpdate,
                *frameProfileUndoUpdate,
                *frameProfileAddNewAddress,
                *frameProfileDeleteAddress,
                *frameLanguageUndoUpdate,
                *frameLanguageDelete,
                *frameNewLanguageAdd,
                *frameLanguageUpdate,
                *frameQualificationDelete,
                *frameQualificationUpdate,
                *frameQualificationUndoUpdate,
                *frameNewQualificationAdd,
                *frameWorkDelete,
                *frameWorkUpdate,
                *frameNewWorkAddSkill,
                *frameWorkUndoUpdate,
                *frameNewWorkDeleteSkill,
                *frameNewWorkAdd,
                *frameNewNetAdd,
                *frameNetDelete,
                *frameUsersAdd,
                *frameUsersDelete;
    QRadioButton *frameAccountTypeBasic,
                 *frameAccountTypeExecutive,
                 *frameAccountTypeBusiness;
    QLabel  *frameUsersLabelTitle,
            *label_6,
            *label_7,
            *labelPassword,
            *label_11,
            *frameAccountLabelChangePassword,
            *label_2,
            *label_3,
            *label_4,
            *label_5,
            *frameNetCount,
            *frameProfileLabelContact,
            *frameLanguageCount,
            *frameNewLanguageLabelTitle,
            *frameNewLanguageLabelWritten,
            *frameNewLanguageLabelHeard,
            *frameNewLanguageLabelSpoken,
            *frameNewLanguageLabelName,
            *frameQualificationCount,
            *frameNewQualificationLabelTitle,
            *frameNewQualificationLabelYear,
            *frameNewQualificationLabelName,
            *frameWorkCount,
            *frameNewWorkLabelTitle,
            *frameWorkLabelFrom,
            *frameWorkLabelTo,
            *frameWorkLabelSector,
            *frameWorkLabelJob,
            *frameWorkLabelSkill,
            *frameNewNetLabelTitle,
            *frameUsersLabelFilterUsername,
            *frameUsersCount,
            *frameUsersLabelFilterYear;
    QLineEdit   *frameAccountNewPassword,
                *frameAccountUsername,
                *frameAccountNewPasswordConfirm,
                *frameProfileName,
                *frameProfileSurname,
                *frameProfileNationality,
                *frameProfileBirth,
                *frameProfileNewAddress,
                *frameNewLanguageName,
                *frameNewQualificationName,
                *frameNewQualificationYear,
                *frameNewWorkDateFrom,
                *frameNewWorkSector,
                *frameNewWorkJob,
                *frameNewWorkDateTo,
                *frameNewWorkNewSkill,
                *frameUsersFilterUsername;
    QTabWidget *frameDetails;
    QListWidget *frameUsersList,
                *frameProfileAddressList,
                *frameLanguageList,
                *frameWorkList,
                *frameQualificationList,
                *frameNewWorkListSkill,
                *frameNewNetList,
                *frameNetList;
    QSpinBox    *frameNewLanguageWritten,
                *frameNewLanguageHeard,
                *frameNewLanguageSpoken,
                *frameUsersFilterYear;
    QFrame *frameNewLanguage,
            *frameNewQualification,
            *frameNewWork,
            *frameNewNet;
    QWidget *Account,
            *Profilo,
            *Lingue,
            *Titoli,
            *Lavori_2,
            *Rete;
    QComboBox *frameUsersFilterSector;
};

#endif // CLIENTADMIN_H
