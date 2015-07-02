#ifndef CLIENTUSER_H
#define CLIENTUSER_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>
// graphic part
#include "clientuserfind.h"
#include "clientstart.h"
// logical part
#include "db.h"
#include "linkediner.h"
#include "linkedinerbasic.h"
#include "linkedinerbusiness.h"
#include "linkedinerexecutive.h"
class ClientUserFind;

class ClientUser : public QWidget
{
    Q_OBJECT

signals:
    void s_save();
    void s_exit();
    void s_notSave();
    void s_find(QString);

public:

    ClientUser(Linkediner *,Db *,QWidget *parent = 0);
    bool (*validate)(int,int,QObject*,QObject*);
    ~ClientUser();
    void loadUserDetails(); //  used to load user information at startup
    void loadCount();   // used to load information counters
    void loadUserAccount(); // used to load account information
    void loadUserProfile(); // used to load profile information
    void loadUserWork();    // used to load work information
    void loadUserLanguage();    // used to load language list
    void loadUserQualification();   // used to load qualification list
    void loadUserNet(); // used to load associated people list
    void clearErrors(); //  used to clean error from gui
    void clearHiddenBottons();  // used to hide same bottom

private slots:

    // general
    void on_exit_clicked(); //  used by user to escape from user gui
    void exitGuiFinder();   //  used by user when escapes from find gui
    void on_find_clicked(); //  used by user to find soma other users

    // account slots
    void on_frameAccountUpdateInformation_clicked();    //  used by user to update account information
    void on_frameAccountOldpassword_textChanged(const QString &arg1);   // used by user to change password

    // profile slots
    void on_frameProfileUndoUpdate_clicked();   // used by user to undo update not still saved
    void on_frameProfileName_textChanged(const QString &arg1);  //  triggered when user change actual name
    void on_frameProfileSurname_textEdited(const QString &arg1);    // triggered when user change actual surname
    void on_frameProfileBirth_textChanged(const QString &arg1); // triggered when user change actual birth
    void on_frameProfileNationality_textChanged(const QString &arg1);   //  triggered when user change actual nationality
    void on_frameProfileUpdateInformation_clicked();    // used by user to update profile information
    void on_frameProfileAddNewAddress_clicked();    // used by user to add a new address to contact-list of profile
    void on_frameProfileDeleteAddress_clicked();    // used by user to delete a new address from contact-list of profile

    // work slots
    void on_frameNewWorkAddSkill_clicked(); // used by user to add new skill to a specific work
    void on_frameWorkList_itemDoubleClicked(QListWidgetItem *item); //  used by user to udpate or remove an existing work
    void on_frameNewWorkDeleteSkill_clicked();  // used by user to delete a skill from specific and existing work
    void on_frameNewWorkAdd_clicked();  // used by user to add new work
    void on_frameWorkUpdate_clicked();  // used by user to update an existing work
    void on_frameWorkDelete_clicked();  // used by user to delete an existing work
    void on_frameWorkUndoUpdate_clicked();   // used by user to undo udpate not still saved

    // language slots
    void on_frameLanguageList_itemDoubleClicked(QListWidgetItem *item); // used by user to remove or udpate an existing language
    void on_frameLanguageDelete_clicked();  // used by user to delete an existing language
    void on_frameLanguageUndoUpdate_clicked();  // used by user to undo udpate not still saved
    void on_frameNewLanguageAdd_clicked();  // used by user to add new language
    void on_frameLanguageUpdate_clicked();  // used by user to udpate language

    // qualification slots
    void on_frameQualificationUpdate_clicked(); // used by user to udpate an existing qualification
    void on_frameQualificationDelete_clicked(); // used by user to delete an existing qualification
    void on_frameQualificationUndoUpdate_clicked(); // used by user to undo update not still saved
    void on_frameNewQualificationAdd_clicked(); // used by user to add new qualification
    void on_frameQualificationList_itemDoubleClicked(QListWidgetItem *item);    // used by user to remove or update an existing qualification

    // net slots
    void on_frameNetDelete_clicked();   // used by user to add a fried to list of associated people
    void on_frameNewNetAdd_clicked();   // used by user to remove a fried from list of associated people

private:

    // graphic setup
    void setup();

    // logical objects
    Db * db;
    Linkediner * actualUser;
    Works * actualWork;
    Qualifications * actualQualification;
    Languages * actualLanguage;
    ClientUserFind * guiFinder;

    // graphic objects
    QLineEdit   *frameAccountUsername,
                *frameAccountOldpassword,
                *frameAccountNewpassword,
                *frameAccountNewpasswordConfirm,
                *frameProfileNewAddress,
                *frameProfileName,
                *frameProfileSurname,
                *frameProfileNationality,
                *frameNewWorkDateFrom,
                *frameNewWorkDateTo,
                *frameNewWorkSector,
                *frameNewWorkJob,
                *frameNewLanguageName,
                *frameNewWorkNewSkill,
                *frameNewQualificationName,
                *frameNewQualificationYear,
                *findPeople,
                *frameProfileBirth;
    QPushButton *frameAccountUpdateInformation,
                *frameProfileAddNewAddress,
                *frameProfileUpdateInformation,
                *frameProfileUndoUpdate,
                *frameProfileDeleteAddress,
                *frameNewWorkAddSkill,
                *frameNewWorkDeleteSkill,
                *frameWorkUpdate,
                *frameWorkDelete,
                *frameWorkUndoUpdate,
                *frameNewLanguageAdd,
                *frameLanguageDelete,
                *frameLanguageUpdate,
                *frameLanguageUndoUpdate,
                *frameNewQualificationAdd,
                *frameQualificationUpdate,
                *frameQualificationDelete,
                *frameQualificationUndoUpdate,
                *frameNetDelete,
                *frameNewNetAdd,
                *exit,
                *find,
                *frameNewWorkAdd;
    QTabWidget *userDetails;
    QLabel *frameAccountLabelUsername,
            *frameAccountLabelOldpassword,
            *frameAccountLabelNewpassword,
            *frameAccountLabelNewpasswordConfirm,
            *frameAccountLabelTitle,
            *frameAccountLabelChangePassword,
            *frameAcountLabelResumeWork,
            *frameAcountLabelResumeQualification,
            *frameAcountLabelResumeLanguage,
            *frameAcountLabelResumePeople,
            *frameAccountCountWork,
            *frameAccountCountQualification,
            *frameAccountCountPeople,
            *frameAccountCountLanguage,
            *frameAccountErrorPassword,
            *frameAccountErrorOldpassword,
            *frameProfileLabelAddress,
            *frameProfileLabelName,
            *frameProfileLabelSurname,
            *frameProfileLabelBirth,
            *frameProfileLabelNationality,
            *frameProfileLabelTitle,
            *frameProfileErrorName,
            *frameProfileErrorSurname,
            *frameProfileErrorBirth,
            *frameProfileErrorNationality,
            *frameWorkLabelTitle,
            *frameNewWorkLabelTitle,
            *frameNewWorkLabelFrom,
            *frameNewWorkLabelTo,
            *frameNewWorkLabelSector,
            *frameNewWorkLabelJob,
            *frameNewWorkLabelSkill,
            *frameLanguageLabelTitle,
            *frameNewLanguageLabelTitle,
            *frameNewLanguageLabelSpoken,
            *frameNewLanguageLabelWritten,
            *frameNewLanguageLabelHeard,
            *frameNewLanguageLabelName,
            *frameQualificationLabelTitle,
            *frameNewQualificationLabelTitle,
            *frameNewQualificationLabelName,
            *frameNewQualificationLabelYear,
            *frameNetLabelTitle,
            *frameNewNetLabelTitle;
    QListWidget *frameProfileAddressList,
                *frameWorkList,
                *frameNewWorkListSkill,
                *frameLanguageList,
                *frameQualificationList,
                *frameNetList,
                *frameNewNetList;
    QFrame *frameWork,
            *frameNewWork,
            *frameLanguage,
            *frameNewLanguage,
            *frameQualification,
            *frameNewQualification,
            *frameNet,
            *frameNewNet,
            *frameAccount,
            *frameProfile;
    QSpinBox    *frameNewLanguageSpoken,
                *frameNewLanguageWritten,
                *frameNewLanguageHeard;
    QWidget *tab_3,
            *tab_4,
            *tab_5,
            *tab_6,
            *tab_2,
            *tab;
};

#endif // CLIENTUSER_H
