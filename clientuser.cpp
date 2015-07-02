#include "clientuser.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHODS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ClientUser::ClientUser(Linkediner * user, Db * link, QWidget *parent) :
    QWidget(parent),
    db(link),
    actualUser(user),
    actualWork(0),
    actualQualification(0),
    actualLanguage(0)
{
    setup();
    connect(this,SIGNAL(s_save()),this->parent(),SLOT(saveRequest()));
    connect(this,SIGNAL(s_notSave()),this->parent(),SLOT(undoUpdate()));
    connect(this,SIGNAL(s_exit()),this->parent(),SLOT(exitUserGui()));
    loadUserDetails();
    loadCount();
    validate = &(dynamic_cast<ClientStart*>(this->parent()))->isValid;
}
ClientUser::~ClientUser()
{

}
void ClientUser::loadUserAccount()
{
    frameAccountUsername->setText(QString::fromStdString(actualUser->getEmail()));
}
void ClientUser::loadUserProfile()
{
    frameProfileName->setText(QString::fromStdString(actualUser->getProfile()->getName()));
    frameProfileSurname->setText(QString::fromStdString(actualUser->getProfile()->getSurname()));
    frameProfileBirth->setText(actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd"));
    frameProfileNationality->setText(QString::fromStdString(actualUser->getProfile()->getNationality()));
    list<string>::iterator i = actualUser->getProfile()->getContacts()->begin();
    for( ; i != actualUser->getProfile()->getContacts()->end(); ++i)
        QListWidgetItem * it = new QListWidgetItem(QPixmap("://resource/contact.png"),frameProfileNewAddress->text(),frameProfileAddressList);
}
void ClientUser::loadUserWork()
{
    loadCount();
    clearErrors();
    clearHiddenBottons();
    frameWorkList->clear();
    frameNewWorkDateTo->clear();
    frameNewWorkDateFrom->clear();
    frameNewWorkSector->clear();
    frameNewWorkJob->clear();
    frameNewWorkNewSkill->clear();
    frameNewWorkListSkill->clear();
    frameNewWorkLabelTitle->setText("NUOVO LAVORO");
    list<Works*>::iterator work = actualUser->getWorks()->begin();
    for( ; work != actualUser->getWorks()->end(); ++work)
    {
        QString aux;
        aux.append(QString::fromStdString((*work)->getJob())+"\n"+
                   ((*work)->getStart().toString("dd'-'MM'-'yyyy")) +" - "+
                   ((*work)->getEnd().toString("dd'-'MM'-'yyyy")) +"\n"+
                   "Settore: "+QString::fromStdString((*work)->getSector())
                   );
        QListWidgetItem * i = new QListWidgetItem(QPixmap("://resource/works.png"),aux,frameWorkList);
        i->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    frameWorkList->setIconSize(QSize(32,32));
}
void ClientUser::loadUserLanguage()
{
    loadCount();
    clearErrors();
    clearHiddenBottons();
    frameLanguageList->clear();
    frameNewLanguageName->clear();
    frameNewLanguageSpoken->setValue(0);
    frameNewLanguageWritten->setValue(0);
    frameNewLanguageHeard->setValue(0);
    frameNewLanguageLabelTitle->setText("NUOVA LINGUA");
    list<Languages*>::iterator lang = actualUser->getLanguages()->begin();
    for( ; lang != actualUser->getLanguages()->end(); ++lang)
    {
        QString aux;
        aux.append(
            QString::fromStdString((*lang)->getName())+"\n"
            +"Parlato:" + QString::number((*lang)->getSpoken())
            +" Scritto:" + QString::number((*lang)->getWritten())
            +" Ascolto:" + QString::number((*lang)->getHeard())
        );
        QListWidgetItem * i = new QListWidgetItem(QPixmap("://resource/language.png"),aux,frameLanguageList);
        i->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    frameLanguageList->setIconSize(QSize(32,32));
}
void ClientUser::loadUserQualification()
{
    loadCount();
    clearErrors();
    clearHiddenBottons();
    frameQualificationList->clear();
    frameNewQualificationName->clear();
    frameNewQualificationYear->clear();
    list<Qualifications*>::iterator qualif = actualUser->getQualifications()->begin();
    for( ; qualif != actualUser->getQualifications()->end(); ++qualif)
    {
        QString aux;
        aux.append((*qualif)->getYear().toString("yyyy")+"\n"+QString::fromStdString((*qualif)->getName()));
        QListWidgetItem * i = new QListWidgetItem(QPixmap("://resource/qualifications.png"),aux,frameQualificationList);
        i->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    frameQualificationList->setIconSize(QSize(32,32));

}
void ClientUser::loadUserNet()
{
    frameNetList->clear();
    frameNewNetList->clear();
    list<Linkediner*>::iterator net = actualUser->getNet()->begin();
    for( ; net != actualUser->getNet()->end(); ++net)
    {
        QString aux;
        aux.append(QString::fromStdString((*net)->getEmail()));
        QListWidgetItem * i = new QListWidgetItem(QPixmap("://resource/net.jpeg"),aux,frameNetList);
    }
    frameNetList->setIconSize(QSize(32,32));

    list<Linkediner*>::iterator i = db->getDb()->begin();
    for( ; i != db->getDb()->end(); ++i)
    {
        bool presente = false;
        list<Linkediner*>::iterator contacts = actualUser->getNet()->begin();
        for( ; contacts != actualUser->getNet()->end() && !presente; ++contacts)
        {
            if( (*contacts)->getEmail() == (*i)->getEmail())
            {
                presente = true;
                break;
            }
        }
        if(!presente && (*i)->getEmail() != actualUser->getEmail() )
            QListWidgetItem * item = new QListWidgetItem(QPixmap("://resource/net.jpeg"),QString::fromStdString( (*i)->getEmail() ),frameNewNetList);
    }
}
void ClientUser::loadUserDetails()
{
    loadUserAccount();
    loadUserProfile();
    loadUserLanguage();
    loadUserQualification();
    loadUserWork();
    loadUserNet();
}
void ClientUser::loadCount()
{
    int l,w,p,q;
    l = actualUser->getLanguages()->size();
    w = actualUser->getWorks()->size();
    p = actualUser->getNet()->size();
    q = actualUser->getQualifications()->size();
    if(l>99) frameAccountCountLanguage->setText("+99");
    else frameAccountCountLanguage->setText(QString::number(l));
    if(w>99) frameAccountCountWork->setText("+99");
    else frameAccountCountWork->setText(QString::number(w));
    if(p>99) frameAccountCountPeople->setText("+99");
    else frameAccountCountPeople->setText(QString::number(p));
    if(q>99) frameAccountCountQualification->setText("+99");
    else frameAccountCountQualification->setText(QString::number(q));
}
void ClientUser::clearErrors()
{
    frameAccountErrorOldpassword->hide();
    frameAccountErrorPassword->hide();
    frameProfileErrorName->hide();
    frameProfileErrorSurname->hide();
    frameProfileErrorBirth->hide();
    frameProfileErrorNationality->hide();
    frameNewWorkDateFrom->setStyleSheet(frameNewWorkDateFrom->styleSheet().replace("border:1px solid red","border:none"));
    frameNewWorkDateTo->setStyleSheet(frameNewWorkDateTo->styleSheet().replace("border:1px solid red","border:none"));
    frameNewWorkSector->setStyleSheet(frameNewWorkSector->styleSheet().replace("border:1px solid red","border:none"));
    frameNewWorkJob->setStyleSheet(frameNewWorkJob->styleSheet().replace("border:1px solid red","border:none"));
    frameNewWorkNewSkill->setStyleSheet(frameNewWorkNewSkill->styleSheet().replace("border:1px solid red","border:none"));
    frameNewLanguageName->setStyleSheet(frameNewLanguageName->styleSheet().replace("border:1px solid red","border:none;"));
    frameNewQualificationName->setStyleSheet(frameNewQualificationName->styleSheet().replace("border:1px solid red","border:none;"));
    frameNewQualificationYear->setStyleSheet(frameNewQualificationYear->styleSheet().replace("border:1px solid red","border:none;"));
}
void ClientUser::clearHiddenBottons()
{
    frameAccountUpdateInformation->hide();
    frameProfileUpdateInformation->hide();
    frameProfileUndoUpdate->hide();
    frameWorkUpdate->hide();
    frameWorkDelete->hide();
    frameWorkUndoUpdate->hide();
    frameLanguageUpdate->hide();
    frameLanguageDelete->hide();
    frameLanguageUndoUpdate->hide();
    frameQualificationUpdate->hide();
    frameQualificationDelete->hide();
    frameQualificationUndoUpdate->hide();

    frameNewWorkAdd->show();
    frameNewLanguageAdd->show();
    frameNewQualificationAdd->show();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE SLOTS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   GENERAL
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::exitGuiFinder()
{
    guiFinder->hide();
    delete guiFinder;
    this->show();
    loadCount();
    loadUserNet();
}
void ClientUser::on_exit_clicked()
{
    QMessageBox::StandardButton q = QMessageBox::question(this, "Salva", "Salvare le modifiche effettuate ?",QMessageBox::Yes|QMessageBox::No);
    if(q == QMessageBox::Yes)
        emit s_save();
    else
        emit s_notSave();
    emit s_exit();
}
void ClientUser::on_find_clicked()
{
    if( findPeople->text().toStdString() != actualUser->getEmail() & validate(3,0,findPeople,0))
    {
        guiFinder= new ClientUserFind(db,actualUser,this);
        guiFinder->setWindowFlags(Qt::Window);
        guiFinder->show();
        emit(s_find(findPeople->text()));
        this->hide();
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   ACCOUNT
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameAccountUpdateInformation_clicked()
{
    if(frameAccountOldpassword->text().toStdString() == actualUser->getPassword() )
    {
        clearErrors();
        if(frameAccountNewpassword->text() == frameAccountNewpasswordConfirm->text())
        {
            clearErrors();
            actualUser->setPassword(frameAccountNewpassword->text().toStdString());
            frameAccountNewpassword->clear();
            frameAccountNewpasswordConfirm->clear();
            frameAccountOldpassword->clear();
            frameAccountUpdateInformation->hide();
        } else {
            frameAccountNewpassword->clear();
            frameAccountNewpasswordConfirm->clear();
            frameAccountErrorPassword->show();
        }
    } else {
        frameAccountOldpassword->clear();
        frameAccountErrorOldpassword->show();
    }
}
void ClientUser::on_frameAccountOldpassword_textChanged(const QString &arg1)
{
    frameAccountUpdateInformation->show();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   PROFILE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameProfileUndoUpdate_clicked()
{
    frameProfileName->setText(QString::fromStdString(actualUser->getProfile()->getName()));
    frameProfileSurname->setText(QString::fromStdString(actualUser->getProfile()->getSurname()));
    frameProfileBirth->setText(actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd"));
    frameProfileNationality->setText(QString::fromStdString(actualUser->getProfile()->getNationality()));
    frameProfileUpdateInformation->hide();
    frameProfileUndoUpdate->hide();
    clearErrors();
}
void ClientUser::on_frameProfileName_textChanged(const QString &arg1)
{
    if(arg1.toStdString() != actualUser->getProfile()->getName() )
    {
        frameProfileUpdateInformation->show();
        frameProfileUndoUpdate->show();
    }
}
void ClientUser::on_frameProfileSurname_textEdited(const QString &arg1)
{
    if(arg1.toStdString() != actualUser->getProfile()->getSurname() )
    {
        frameProfileUpdateInformation->show();
        frameProfileUndoUpdate->show();
    }
}
void ClientUser::on_frameProfileBirth_textChanged(const QString &arg1)
{
    if(arg1 != actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd") )
    {
        frameProfileUpdateInformation->show();
        frameProfileUndoUpdate->show();
    }
}
void ClientUser::on_frameProfileNationality_textChanged(const QString &arg1)
{
    if(arg1.toStdString() != actualUser->getProfile()->getNationality())
    {
        frameProfileUpdateInformation->show();
        frameProfileUndoUpdate->show();
    }
}
void ClientUser::on_frameProfileUpdateInformation_clicked()
{
    if(validate(1,0,frameProfileName,0) &
       validate(1,0,frameProfileSurname,0) &
       validate(1,0,frameProfileNationality,0) &
       validate(0,0,frameProfileBirth,0)
    ){
        actualUser->getProfile()->setName(frameProfileName->text().toStdString());
        actualUser->getProfile()->setSurname(frameProfileSurname->text().toStdString());
        actualUser->getProfile()->setNationality(frameProfileNationality->text().toStdString());
        actualUser->getProfile()->setBirth(QDate::fromString(frameProfileBirth->text(),"yyyy'-'MM'-'dd"));
        frameProfileUpdateInformation->hide();
        frameProfileUndoUpdate->hide();
    }
}
void ClientUser::on_frameProfileAddNewAddress_clicked()
{
    if(validate(3,0,frameProfileNewAddress,0))
    {
        frameProfileNewAddress->setStyleSheet(frameProfileNewAddress->styleSheet().replace("border:1px solid red;","border:none;"));
        actualUser->getProfile()->getContacts()->push_back(frameProfileNewAddress->text().toStdString());
        QListWidgetItem * newContact = new QListWidgetItem(QPixmap("://resource/contact.png"),frameProfileNewAddress->text(),frameProfileAddressList);
        frameProfileNewAddress->setText("");
    }
}
void ClientUser::on_frameProfileDeleteAddress_clicked()
{
    if(frameProfileAddressList->currentItem())
    {
        actualUser->getProfile()->getContacts()->remove(frameProfileNewAddress->text().toStdString());
        delete frameProfileAddressList->currentItem();
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   WORK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameWorkList_itemDoubleClicked(QListWidgetItem *item)
{
    QStringList data = item->text().split("\n");
    int start = QDate::fromString((data[1].split(" - "))[0],"dd'-'MM'-'yyyy").year();
    list<Works*>::iterator i = actualUser->getWorks()->begin();
    for( ; i != actualUser->getWorks()->end(); ++i)
    {
        if( (*i)->getJob() == data[0].toStdString() && start == (*i)->getStart().year())
        {
            actualWork = *i;
            break;
        }
    }
    frameNewWorkListSkill->clear();
    frameNewWorkLabelTitle->setText(QString::fromStdString(actualWork->getJob()));
    frameWorkUpdate->show();
    frameWorkDelete->show();
    frameWorkUndoUpdate->show();
    frameNewWorkAdd->hide();
    frameNewWorkDateFrom->setText(actualWork->getStart().toString("yyyy'-'MM'-'dd"));
    frameNewWorkDateTo->setText(actualWork->getEnd().toString("yyyy'-'MM'-'dd"));
    frameNewWorkSector->setText(QString::fromStdString(actualWork->getSector()));
    frameNewWorkJob->setText(QString::fromStdString(actualWork->getJob()));
    list<string>::iterator sk = actualWork->getSkills()->begin();
    for( ; sk != actualWork->getSkills()->end(); ++sk)
    {
        QListWidgetItem * nuova = new QListWidgetItem(QPixmap("://resource/skills.jpg"),QString::fromStdString(*sk),frameNewWorkListSkill);
    }
}
void ClientUser::on_frameNewWorkAddSkill_clicked()
{
    if(frameNewWorkNewSkill->text() != "")
        QListWidgetItem * nuova = new QListWidgetItem(QPixmap("://resource/skills.jpg"),frameNewWorkNewSkill->text(),frameNewWorkListSkill);
    frameNewWorkNewSkill->clear();
}
void ClientUser::on_frameNewWorkDeleteSkill_clicked()
{
    if(frameNewWorkListSkill->currentItem())
        delete frameNewWorkListSkill->currentItem();
}
void ClientUser::on_frameNewWorkAdd_clicked()
{
    clearErrors();
    if(validate(0,1,frameNewWorkDateFrom,frameProfileBirth) &
       validate(0,2,frameNewWorkDateTo,frameNewWorkDateFrom) &
       validate(1,1,frameNewWorkSector,0) &
       validate(1,1,frameNewWorkJob,0)
    ){
        list<string> skills;
        for(int i = 0; i < frameNewWorkListSkill->count(); ++i)
        skills.push_back(frameNewWorkListSkill->item(i)->text().toStdString());
        Works * nuovo = new Works
        (
            QDate::fromString(frameNewWorkDateFrom->text(),"yyyy'-'MM'-'dd"),
            QDate::fromString(frameNewWorkDateTo->text(),"yyyy'-'MM'-'dd"),
            frameNewWorkSector->text().toStdString(),
            frameNewWorkJob->text().toStdString(),
            skills
        );
        actualUser->addWork(nuovo);
        loadUserWork();
    }
}
void ClientUser::on_frameWorkUpdate_clicked()
{
    clearErrors();
    if(validate(0,1,frameNewWorkDateFrom,frameProfileBirth) &
       validate(0,2,frameNewWorkDateTo,frameNewWorkDateFrom) &
       validate(1,1,frameNewWorkSector,0) &
       validate(1,1,frameNewWorkJob,0)
    ){
        actualWork->getSkills()->clear();
        for(int i = 0; i < frameNewWorkListSkill->count(); ++i)
        actualWork->getSkills()->push_back(frameNewWorkListSkill->item(i)->text().toStdString());
        actualWork->setStart(QDate::fromString(frameNewWorkDateFrom->text(),"yyyy'-'MM'-'dd"));
        actualWork->setEnd(QDate::fromString(frameNewWorkDateTo->text(),"yyyy'-'MM'-'dd"));
        actualWork->setSector(frameNewWorkSector->text().toStdString());
        actualWork->setJob(frameNewWorkJob->text().toStdString());
        actualWork = 0;
        loadUserWork();
     }
}
void ClientUser::on_frameWorkDelete_clicked()
{
    list<Works*>::iterator i = actualUser->getWorks()->begin();
    for( ; i != actualUser->getWorks()->end(); ++i)
    {
        if( *i == actualWork)
        {
            delete *i;
            break;
        }
    }
    actualUser->getWorks()->remove(actualWork);
    actualWork = 0;
    loadUserWork();
}
void ClientUser::on_frameWorkUndoUpdate_clicked()
{
    frameNewWorkDateFrom->setText(actualWork->getStart().toString("yyyy'-'MM'-'dd"));
    frameNewWorkDateTo->setText(actualWork->getEnd().toString("yyyy'-'MM'-'dd"));
    frameNewWorkSector->setText(QString::fromStdString(actualWork->getSector()));
    frameNewWorkJob->setText(QString::fromStdString(actualWork->getJob()));
    list<string>::iterator i = actualWork->getSkills()->begin();
    frameNewWorkListSkill->clear();
    for( ; i != actualWork->getSkills()->end(); ++i)
        QListWidgetItem * it = new QListWidgetItem(QPixmap("://resource/skills.jpg"),QString::fromStdString(*i),frameNewWorkListSkill);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   LANGUAGE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameLanguageList_itemDoubleClicked(QListWidgetItem *item)
{
    QStringList data = item->text().split("\n");
    list<Languages*>::iterator i = actualUser->getLanguages()->begin();
    for( ; i != actualUser->getLanguages()->end(); ++i)
    {
        if(data[0].toStdString() == (*i)->getName() ) {
            actualLanguage = *i;
            break;
        }
    }
    frameLanguageUpdate->show();
    frameLanguageDelete->show();
    frameLanguageUndoUpdate->show();
    frameNewLanguageAdd->hide();
    frameNewLanguageLabelTitle->setText(QString::fromStdString(actualLanguage->getName()));
    frameNewLanguageName->setText(QString::fromStdString(actualLanguage->getName()));
    frameNewLanguageSpoken->setValue(actualLanguage->getSpoken());
    frameNewLanguageWritten->setValue(actualLanguage->getWritten());
    frameNewLanguageHeard->setValue(actualLanguage->getHeard());
}
void ClientUser::on_frameLanguageUpdate_clicked()
{
    if(validate(1,0,frameNewLanguageName,0))
    {
        actualLanguage->setName(frameNewLanguageName->text().toStdString());
        actualLanguage->setHeard(frameNewLanguageHeard->value());
        actualLanguage->setWritten(frameNewLanguageWritten->value());
        actualLanguage->setSpoken(frameNewLanguageSpoken->value());
        actualLanguage = 0;
        loadUserLanguage();
    }
}
void ClientUser::on_frameLanguageDelete_clicked()
{
    list<Languages*>::iterator i = actualUser->getLanguages()->begin();
    for( ; i != actualUser->getLanguages()->end(); ++i)
    {
        if( *i == actualLanguage)
        {
            delete *i;
            break;
        }
    }
    actualUser->getLanguages()->remove(actualLanguage);
    actualLanguage = 0;
    loadUserLanguage();
    clearErrors();
}
void ClientUser::on_frameLanguageUndoUpdate_clicked()
{
    frameNewLanguageName->setText(QString::fromStdString(actualLanguage->getName()));
    frameNewLanguageHeard->setValue(actualLanguage->getHeard());
    frameNewLanguageWritten->setValue(actualLanguage->getWritten());
    frameNewLanguageSpoken->setValue(actualLanguage->getSpoken());
    clearErrors();
}
void ClientUser::on_frameNewLanguageAdd_clicked()
{
    if(validate(1,0,frameNewLanguageName,0))
    {
        Languages * nuova = new Languages(
                    frameNewLanguageName->text().toStdString(),
                    frameNewLanguageHeard->value(),
                    frameNewLanguageSpoken->value(),
                    frameNewLanguageWritten->value()
        );
        actualUser->addLanguage(nuova);
        loadUserLanguage();
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   QUALIFICATION
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameQualificationUpdate_clicked()
{
    if(validate(0,3,frameNewQualificationYear,frameProfileBirth) & validate(1,1,frameNewQualificationName,0))
    {
        actualQualification->setName(frameNewQualificationName->text().toStdString());
        actualQualification->setYear(QDate::fromString(frameNewQualificationYear->text(),"yyyy"));
        actualQualification = 0;
        loadUserQualification();
    }
}
void ClientUser::on_frameQualificationDelete_clicked()
{
    list<Qualifications*>::iterator i = actualUser->getQualifications()->begin();
    for( ; i != actualUser->getQualifications()->end(); ++i)
    {
        if( *i == actualQualification)
        {
            delete *i;
            break;
        }
    }
    actualUser->getQualifications()->remove(actualQualification);
    actualQualification = 0;
    loadUserQualification();
}
void ClientUser::on_frameQualificationUndoUpdate_clicked()
{
    clearErrors();
    frameNewQualificationName->setText(QString::fromStdString(actualQualification->getName()));
    frameNewQualificationYear->setText(actualQualification->getYear().toString("yyyy"));
}
void ClientUser::on_frameNewQualificationAdd_clicked()
{
    clearErrors();
    if(validate(0,3,frameNewQualificationYear,frameProfileBirth) & validate(1,1,frameNewQualificationName,0))
    {
        Qualifications * nuova = new Qualifications(
            frameNewQualificationName->text().toStdString(),
            QDate::fromString(frameNewQualificationYear->text(),"yyyy")
        );
        actualUser->addQualifications(nuova);
        loadUserQualification();
    }
}
void ClientUser::on_frameQualificationList_itemDoubleClicked(QListWidgetItem *item)
{
    QStringList data = item->text().split("\n");
    list<Qualifications*>::iterator i = actualUser->getQualifications()->begin();
    for( ; i != actualUser->getQualifications()->end(); ++i)
    {
        if( (*i)->getName() == data[1].toStdString()) {
            actualQualification = *i;
            break;
        }
    }
    frameNewQualificationName->setText(QString::fromStdString(actualQualification->getName()));
    frameNewQualificationYear->setText(actualQualification->getYear().toString("yyyy"));
    frameQualificationDelete->show();
    frameQualificationUpdate->show();
    frameQualificationUndoUpdate->show();
    frameNewQualificationAdd->hide();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   NET
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::on_frameNetDelete_clicked()
{
    if(frameNetList->currentItem())
    {
        actualUser->getNet()->remove(db->getUserByEmail(frameNetList->currentItem()->text().toStdString()));
        QListWidgetItem * ni = new QListWidgetItem(QPixmap("://resource/net.jpeg"),frameNetList->currentItem()->text(),frameNewNetList);
        delete frameNetList->currentItem();
        loadCount();
    }
}
void ClientUser::on_frameNewNetAdd_clicked()
{
    if(frameNewNetList->currentItem())
    {
        actualUser->addContact(db->getUserByEmail(frameNewNetList->currentItem()->text().toStdString()));
        QListWidgetItem * ni = new QListWidgetItem(QPixmap("://resource/net.jpeg"),frameNewNetList->currentItem()->text(),frameNetList);
        delete frameNewNetList->currentItem();
        loadCount();
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE METHODS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUser::setup()
{
    setObjectName(QString("ClientUser"));
    resize(431, 799);
    setStyleSheet("#ClientUser{\n"
    "	background:#006699;\n"
    "}");
    userDetails = new QTabWidget(this);
    userDetails->setObjectName("userDetails");
    userDetails->setGeometry(QRect(10, 10, 411, 711));
    userDetails->setStyleSheet("#userDetails::pane{\n"
                               "	background:rgba(0,0,0,0.5);\n"
                               "	border:none;\n"
                           "}");
    tab = new QWidget();
    tab->setObjectName(QString("tab"));
    frameAccount = new QFrame(tab);
    frameAccount->setObjectName(QString("frameAccount"));
    frameAccount->setGeometry(QRect(0, 0, 411, 681));
    frameAccount->setStyleSheet("#frameAccount{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "}");
    frameAccount->setFrameShape(QFrame::StyledPanel);
    frameAccount->setFrameShadow(QFrame::Raised);
    frameAccountUsername = new QLineEdit(frameAccount);
    frameAccountUsername->setObjectName(QString("frameAccountUsername"));
    frameAccountUsername->setEnabled(false);
    frameAccountUsername->setGeometry(QRect(130, 70, 241, 31));
    frameAccountUsername->setStyleSheet("#frameAccountUsername{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:grey;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountUsername->setAlignment(Qt::AlignCenter);
    frameAccountOldpassword = new QLineEdit(frameAccount);
    frameAccountOldpassword->setObjectName(QString("frameAccountOldpassword"));
    frameAccountOldpassword->setGeometry(QRect(130, 230, 241, 31));
    frameAccountOldpassword->setStyleSheet("#frameAccountOldpassword{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountOldpassword->setEchoMode(QLineEdit::Password);
    frameAccountOldpassword->setAlignment(Qt::AlignCenter);
    frameAccountLabelUsername = new QLabel(frameAccount);
    frameAccountLabelUsername->setObjectName(QString("frameAccountLabelUsername"));
    frameAccountLabelUsername->setGeometry(QRect(20, 60, 371, 51));
    frameAccountLabelUsername->setStyleSheet("#frameAccountLabelUsername{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameAccountLabelUsername->setIndent(20);
    frameAccountLabelOldpassword = new QLabel(frameAccount);
    frameAccountLabelOldpassword->setObjectName(QString("frameAccountLabelOldpassword"));
    frameAccountLabelOldpassword->setGeometry(QRect(20, 220, 371, 51));
    frameAccountLabelOldpassword->setStyleSheet("#frameAccountLabelOldpassword{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameAccountLabelOldpassword->setIndent(20);
    frameAccountNewpassword = new QLineEdit(frameAccount);
    frameAccountNewpassword->setObjectName(QString("frameAccountNewpassword"));
    frameAccountNewpassword->setGeometry(QRect(130, 310, 241, 31));
    frameAccountNewpassword->setStyleSheet("#frameAccountNewpassword{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountNewpassword->setEchoMode(QLineEdit::Password);
    frameAccountNewpassword->setAlignment(Qt::AlignCenter);
    frameAccountLabelNewpassword = new QLabel(frameAccount);
    frameAccountLabelNewpassword->setObjectName(QString("frameAccountLabelNewpassword"));
    frameAccountLabelNewpassword->setGeometry(QRect(20, 300, 371, 51));
    frameAccountLabelNewpassword->setStyleSheet("#frameAccountLabelNewpassword{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameAccountLabelNewpassword->setIndent(20);
    frameAccountNewpasswordConfirm = new QLineEdit(frameAccount);
    frameAccountNewpasswordConfirm->setObjectName(QString("frameAccountNewpasswordConfirm"));
    frameAccountNewpasswordConfirm->setGeometry(QRect(130, 370, 241, 31));
    frameAccountNewpasswordConfirm->setStyleSheet("#frameAccountNewpasswordConfirm{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountNewpasswordConfirm->setEchoMode(QLineEdit::Password);
    frameAccountNewpasswordConfirm->setAlignment(Qt::AlignCenter);
    frameAccountLabelNewpasswordConfirm = new QLabel(frameAccount);
    frameAccountLabelNewpasswordConfirm->setObjectName(QString("frameAccountLabelNewpasswordConfirm"));
    frameAccountLabelNewpasswordConfirm->setGeometry(QRect(20, 360, 371, 51));
    frameAccountLabelNewpasswordConfirm->setStyleSheet("#frameAccountLabelNewpasswordConfirm{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameAccountLabelNewpasswordConfirm->setIndent(20);
    frameAccountLabelTitle = new QLabel(frameAccount);
    frameAccountLabelTitle->setObjectName(QString("frameAccountLabelTitle"));
    frameAccountLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameAccountLabelTitle->setStyleSheet("#frameAccountLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameAccountLabelTitle->setAlignment(Qt::AlignCenter);
    frameAccountLabelTitle->setMargin(5);
    frameAccountUpdateInformation = new QPushButton(frameAccount);
    frameAccountUpdateInformation->setObjectName(QString("frameAccountUpdateInformation"));
    frameAccountUpdateInformation->setGeometry(QRect(150, 460, 111, 41));
    frameAccountUpdateInformation->setStyleSheet("#frameAccountUpdateInformation{\n"
    "	background:rgba(0,255,30,0.5);\n"
    "	border-radius:20px;\n"
    "	color:white;\n"
    "	border:2px solid white;\n"
    "}\n"
    "#frameAccountUpdateInformation:hover{\n"
    "	background:rgba(0,255,30,0.8);\n"
    "}");
    frameAccountUpdateInformation->setFlat(false);
    frameAccountLabelChangePassword = new QLabel(frameAccount);
    frameAccountLabelChangePassword->setObjectName(QString("frameAccountLabelChangePassword"));
    frameAccountLabelChangePassword->setGeometry(QRect(0, 150, 411, 41));
    frameAccountLabelChangePassword->setStyleSheet("#frameAccountLabelChangePassword{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameAccountLabelChangePassword->setAlignment(Qt::AlignCenter);
    frameAccountLabelChangePassword->setMargin(5);
    frameAcountLabelResumeWork = new QLabel(frameAccount);
    frameAcountLabelResumeWork->setObjectName(QString("frameAcountLabelResumeWork"));
    frameAcountLabelResumeWork->setGeometry(QRect(0, 530, 181, 41));
    frameAcountLabelResumeWork->setStyleSheet("#frameAcountLabelResumeWork{\n"
    "	color:white;\n"
    "	background:rgba(255,255,0,0.5);\n"
    "}");
    frameAcountLabelResumeWork->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameAcountLabelResumeWork->setMargin(5);
    frameAcountLabelResumeWork->setIndent(10);
    frameAcountLabelResumeQualification = new QLabel(frameAccount);
    frameAcountLabelResumeQualification->setObjectName(QString("frameAcountLabelResumeQualification"));
    frameAcountLabelResumeQualification->setGeometry(QRect(0, 590, 181, 41));
    frameAcountLabelResumeQualification->setStyleSheet("#frameAcountLabelResumeQualification{\n"
    "	color:white;\n"
    "	background:rgba(255,255,0,0.5);\n"
    "}");
    frameAcountLabelResumeQualification->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameAcountLabelResumeQualification->setMargin(5);
    frameAcountLabelResumeQualification->setIndent(10);
    frameAcountLabelResumeLanguage = new QLabel(frameAccount);
    frameAcountLabelResumeLanguage->setObjectName(QString("frameAcountLabelResumeLanguage"));
    frameAcountLabelResumeLanguage->setGeometry(QRect(230, 530, 181, 41));
    frameAcountLabelResumeLanguage->setStyleSheet("#frameAcountLabelResumeLanguage{\n"
    "	color:white;\n"
    "	background:rgba(255,255,0,0.5);\n"
    "}");
    frameAcountLabelResumeLanguage->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    frameAcountLabelResumeLanguage->setMargin(5);
    frameAcountLabelResumeLanguage->setIndent(10);
    frameAcountLabelResumePeople = new QLabel(frameAccount);
    frameAcountLabelResumePeople->setObjectName(QString("frameAcountLabelResumePeople"));
    frameAcountLabelResumePeople->setGeometry(QRect(230, 590, 181, 41));
    frameAcountLabelResumePeople->setStyleSheet("#frameAcountLabelResumePeople{\n"
    "	color:white;\n"
    "	background:rgba(255,255,0,0.5);\n"
    "}");
    frameAcountLabelResumePeople->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    frameAcountLabelResumePeople->setMargin(5);
    frameAcountLabelResumePeople->setIndent(10);
    frameAccountCountWork = new QLabel(frameAccount);
    frameAccountCountWork->setObjectName(QString("frameAccountCountWork"));
    frameAccountCountWork->setGeometry(QRect(7, 536, 51, 31));
    frameAccountCountWork->setStyleSheet("#frameAccountCountWork{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountCountWork->setAlignment(Qt::AlignCenter);
    frameAccountCountQualification = new QLabel(frameAccount);
    frameAccountCountQualification->setObjectName(QString("frameAccountCountQualification"));
    frameAccountCountQualification->setGeometry(QRect(7, 596, 51, 31));
    frameAccountCountQualification->setStyleSheet("#frameAccountCountQualification{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountCountQualification->setAlignment(Qt::AlignCenter);
    frameAccountCountPeople = new QLabel(frameAccount);
    frameAccountCountPeople->setObjectName(QString("frameAccountCountPeople"));
    frameAccountCountPeople->setGeometry(QRect(350, 595, 51, 31));
    frameAccountCountPeople->setStyleSheet("#frameAccountCountPeople{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountCountPeople->setAlignment(Qt::AlignCenter);
    frameAccountCountLanguage = new QLabel(frameAccount);
    frameAccountCountLanguage->setObjectName(QString("frameAccountCountLanguage"));
    frameAccountCountLanguage->setGeometry(QRect(350, 535, 51, 31));
    frameAccountCountLanguage->setStyleSheet("#frameAccountCountLanguage{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameAccountCountLanguage->setAlignment(Qt::AlignCenter);
    frameAccountErrorPassword = new QLabel(frameAccount);
    frameAccountErrorPassword->setObjectName(QString("frameAccountErrorPassword"));
    frameAccountErrorPassword->setGeometry(QRect(35, 410, 341, 21));
    frameAccountErrorPassword->setStyleSheet("#frameAccountErrorPassword{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameAccountErrorPassword->setAlignment(Qt::AlignCenter);
    frameAccountErrorPassword->setMargin(5);
    frameAccountErrorOldpassword = new QLabel(frameAccount);
    frameAccountErrorOldpassword->setObjectName(QString("frameAccountErrorOldpassword"));
    frameAccountErrorOldpassword->setGeometry(QRect(35, 269, 341, 21));
    frameAccountErrorOldpassword->setStyleSheet("#frameAccountErrorOldpassword{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameAccountErrorOldpassword->setAlignment(Qt::AlignCenter);
    frameAccountErrorOldpassword->setMargin(5);
    frameAccountLabelUsername->raise();
    frameAccountLabelOldpassword->raise();
    frameAccountLabelNewpasswordConfirm->raise();
    frameAccountLabelNewpassword->raise();
    frameAccountUsername->raise();
    frameAccountOldpassword->raise();
    frameAccountNewpassword->raise();
    frameAccountNewpasswordConfirm->raise();
    frameAccountLabelTitle->raise();
    frameAccountUpdateInformation->raise();
    frameAccountLabelChangePassword->raise();
    frameAcountLabelResumeWork->raise();
    frameAcountLabelResumeQualification->raise();
    frameAcountLabelResumeLanguage->raise();
    frameAcountLabelResumePeople->raise();
    frameAccountCountWork->raise();
    frameAccountCountQualification->raise();
    frameAccountCountPeople->raise();
    frameAccountCountLanguage->raise();
    frameAccountErrorPassword->raise();
    frameAccountErrorOldpassword->raise();
    userDetails->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString("tab_2"));
    frameProfile = new QFrame(tab_2);
    frameProfile->setObjectName(QString("frameProfile"));
    frameProfile->setGeometry(QRect(0, 0, 411, 681));
    frameProfile->setStyleSheet("#frameProfile{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "}");
    frameProfile->setFrameShape(QFrame::StyledPanel);
    frameProfile->setFrameShadow(QFrame::Raised);
    frameProfileLabelAddress = new QLabel(frameProfile);
    frameProfileLabelAddress->setObjectName(QString("frameProfileLabelAddress"));
    frameProfileLabelAddress->setGeometry(QRect(30, 440, 341, 81));
    frameProfileLabelAddress->setStyleSheet("#frameProfileLabelAddress{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.3);\n"
    "}");
    frameProfileLabelAddress->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    frameProfileLabelAddress->setMargin(10);
    frameProfileNewAddress = new QLineEdit(frameProfile);
    frameProfileNewAddress->setObjectName(QString("frameProfileNewAddress"));
    frameProfileNewAddress->setGeometry(QRect(60, 480, 221, 31));
    frameProfileNewAddress->setStyleSheet("#frameProfileNewAddress{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameProfileAddNewAddress = new QPushButton(frameProfile);
    frameProfileAddNewAddress->setObjectName(QString("frameProfileAddNewAddress"));
    frameProfileAddNewAddress->setGeometry(QRect(300, 485, 21, 21));
    frameProfileAddNewAddress->setStyleSheet("#frameProfileAddNewAddress{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameProfileAddNewAddress:hover{\n"
    "	color:white;\n"
    "}");
    frameProfileName = new QLineEdit(frameProfile);
    frameProfileName->setObjectName(QString("frameProfileName"));
    frameProfileName->setGeometry(QRect(130, 70, 241, 31));
    frameProfileName->setStyleSheet("#frameProfileName{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameProfileName->setAlignment(Qt::AlignCenter);
    frameProfileSurname = new QLineEdit(frameProfile);
    frameProfileSurname->setObjectName(QString("frameProfileSurname"));
    frameProfileSurname->setGeometry(QRect(130, 150, 241, 31));
    frameProfileSurname->setStyleSheet("#frameProfileSurname{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameProfileSurname->setAlignment(Qt::AlignCenter);
    frameProfileLabelName = new QLabel(frameProfile);
    frameProfileLabelName->setObjectName(QString("frameProfileLabelName"));
    frameProfileLabelName->setGeometry(QRect(20, 60, 371, 51));
    frameProfileLabelName->setStyleSheet("#frameProfileLabelName{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameProfileLabelName->setIndent(20);
    frameProfileLabelSurname = new QLabel(frameProfile);
    frameProfileLabelSurname->setObjectName(QString("frameProfileLabelSurname"));
    frameProfileLabelSurname->setGeometry(QRect(20, 140, 371, 51));
    frameProfileLabelSurname->setStyleSheet("#frameProfileLabelSurname{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameProfileLabelSurname->setIndent(20);
    frameProfileBirth = new QLineEdit(frameProfile);
    frameProfileBirth->setObjectName(QString("frameProfileBirth"));
    frameProfileBirth->setGeometry(QRect(130, 230, 241, 31));
    frameProfileBirth->setStyleSheet("#frameProfileBirth{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameProfileBirth->setAlignment(Qt::AlignCenter);
    frameProfileLabelBirth = new QLabel(frameProfile);
    frameProfileLabelBirth->setObjectName(QString("frameProfileLabelBirth"));
    frameProfileLabelBirth->setGeometry(QRect(20, 220, 371, 51));
    frameProfileLabelBirth->setStyleSheet("#frameProfileLabelBirth{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameProfileLabelBirth->setIndent(20);
    frameProfileNationality = new QLineEdit(frameProfile);
    frameProfileNationality->setObjectName(QString("frameProfileNationality"));
    frameProfileNationality->setGeometry(QRect(130, 310, 241, 31));
    frameProfileNationality->setStyleSheet("#frameProfileNationality{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameProfileNationality->setAlignment(Qt::AlignCenter);
    frameProfileLabelNationality = new QLabel(frameProfile);
    frameProfileLabelNationality->setObjectName(QString("frameProfileLabelNationality"));
    frameProfileLabelNationality->setGeometry(QRect(20, 300, 371, 51));
    frameProfileLabelNationality->setStyleSheet("#frameProfileLabelNationality{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.7);\n"
    "}");
    frameProfileLabelNationality->setIndent(20);
    frameProfileLabelTitle = new QLabel(frameProfile);
    frameProfileLabelTitle->setObjectName(QString("frameProfileLabelTitle"));
    frameProfileLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameProfileLabelTitle->setStyleSheet("#frameProfileLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameProfileLabelTitle->setAlignment(Qt::AlignCenter);
    frameProfileLabelTitle->setMargin(5);
    frameProfileUpdateInformation = new QPushButton(frameProfile);
    frameProfileUpdateInformation->setObjectName(QString("frameProfileUpdateInformation"));
    frameProfileUpdateInformation->setGeometry(QRect(220, 386, 111, 41));
    frameProfileUpdateInformation->setStyleSheet("#frameProfileUpdateInformation{\n"
    "	background:rgba(0,255,30,0.5);\n"
    "	border-radius:20px;\n"
    "	color:white;\n"
    "	border:2px solid white;\n"
    "}\n"
    "#frameProfileUpdateInformation:hover{\n"
    "	background:rgba(0,255,30,0.8);\n"
    "}");
    frameProfileUpdateInformation->setFlat(false);
    frameProfileUndoUpdate = new QPushButton(frameProfile);
    frameProfileUndoUpdate->setObjectName(QString("frameProfileUndoUpdate"));
    frameProfileUndoUpdate->setGeometry(QRect(80, 386, 111, 41));
    frameProfileUndoUpdate->setStyleSheet("#frameProfileUndoUpdate{\n"
    "	background:rgba(255,0,30,0.5);\n"
    "	color:white;\n"
    "	border-radius:20px;\n"
    "	border:2px solid white;\n"
    "}\n"
    "#frameProfileUndoUpdate:hover{\n"
    "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameProfileUndoUpdate->setFlat(false);
    frameProfileDeleteAddress = new QPushButton(frameProfile);
    frameProfileDeleteAddress->setObjectName(QString("frameProfileDeleteAddress"));
    frameProfileDeleteAddress->setGeometry(QRect(330, 485, 21, 21));
    frameProfileDeleteAddress->setStyleSheet("#frameProfileDeleteAddress{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;	\n"
    "	color:black;\n"
    "}\n"
    "#frameProfileDeleteAddress:hover{\n"
    "	color:white;\n"
    "}");
    frameProfileErrorName = new QLabel(frameProfile);
    frameProfileErrorName->setObjectName(QString("frameProfileErrorName"));
    frameProfileErrorName->setGeometry(QRect(40, 110, 331, 21));
    frameProfileErrorName->setStyleSheet("#frameProfileErrorName{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameProfileErrorName->setAlignment(Qt::AlignCenter);
    frameProfileErrorName->setMargin(5);
    frameProfileErrorSurname = new QLabel(frameProfile);
    frameProfileErrorSurname->setObjectName(QString("frameProfileErrorSurname"));
    frameProfileErrorSurname->setGeometry(QRect(40, 190, 331, 21));
    frameProfileErrorSurname->setStyleSheet("#frameProfileErrorSurname{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameProfileErrorSurname->setAlignment(Qt::AlignCenter);
    frameProfileErrorSurname->setMargin(5);
    frameProfileErrorBirth = new QLabel(frameProfile);
    frameProfileErrorBirth->setObjectName(QString("frameProfileErrorBirth"));
    frameProfileErrorBirth->setGeometry(QRect(40, 270, 331, 21));
    frameProfileErrorBirth->setStyleSheet("#frameProfileErrorBirth{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameProfileErrorBirth->setAlignment(Qt::AlignCenter);
    frameProfileErrorBirth->setMargin(5);
    frameProfileErrorNationality = new QLabel(frameProfile);
    frameProfileErrorNationality->setObjectName(QString("frameProfileErrorNationality"));
    frameProfileErrorNationality->setGeometry(QRect(40, 350, 331, 21));
    frameProfileErrorNationality->setStyleSheet("#frameProfileErrorNationality{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameProfileErrorNationality->setAlignment(Qt::AlignCenter);
    frameProfileErrorNationality->setMargin(5);
    frameProfileAddressList = new QListWidget(frameProfile);
    frameProfileAddressList->setObjectName(QString("frameProfileAddressList"));
    frameProfileAddressList->setGeometry(QRect(30, 521, 341, 161));
    frameProfileAddressList->setStyleSheet("#frameProfileAddressList{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	padding:10px;\n"
    "	color:white;\n"
    "}");
    frameProfileLabelAddress->raise();
    frameProfileLabelNationality->raise();
    frameProfileLabelBirth->raise();
    frameProfileLabelSurname->raise();
    frameProfileLabelName->raise();
    frameProfileNewAddress->raise();
    frameProfileName->raise();
    frameProfileSurname->raise();
    frameProfileBirth->raise();
    frameProfileNationality->raise();
    frameProfileLabelTitle->raise();
    frameProfileUpdateInformation->raise();
    frameProfileUndoUpdate->raise();
    frameProfileAddNewAddress->raise();
    frameProfileDeleteAddress->raise();
    frameProfileErrorName->raise();
    frameProfileErrorSurname->raise();
    frameProfileErrorBirth->raise();
    frameProfileErrorNationality->raise();
    frameProfileAddressList->raise();
    userDetails->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString("tab_3"));
    frameWork = new QFrame(tab_3);
    frameWork->setObjectName(QString("frameWork"));
    frameWork->setGeometry(QRect(0, 0, 411, 371));
    frameWork->setStyleSheet("#frameWork{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameWork->setFrameShape(QFrame::StyledPanel);
    frameWork->setFrameShadow(QFrame::Raised);
    frameWorkLabelTitle = new QLabel(frameWork);
    frameWorkLabelTitle->setObjectName(QString("frameWorkLabelTitle"));
    frameWorkLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameWorkLabelTitle->setStyleSheet("#frameWorkLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameWorkLabelTitle->setAlignment(Qt::AlignCenter);
    frameWorkLabelTitle->setMargin(5);
    frameWorkList = new QListWidget(frameWork);
    frameWorkList->setObjectName(QString("frameWorkList"));
    frameWorkList->setGeometry(QRect(0, 40, 401, 331));
    frameWorkList->setStyleSheet("#frameWorkList{\n"
    "	background:rgba(0,0,0,0,5);\n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewWork = new QFrame(tab_3);
    frameNewWork->setObjectName(QString("frameNewWork"));
    frameNewWork->setGeometry(QRect(0, 370, 411, 311));
    frameNewWork->setStyleSheet("#frameNewWork{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameNewWork->setFrameShape(QFrame::StyledPanel);
    frameNewWork->setFrameShadow(QFrame::Raised);
    frameNewWorkLabelTitle = new QLabel(frameNewWork);
    frameNewWorkLabelTitle->setObjectName(QString("frameNewWorkLabelTitle"));
    frameNewWorkLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameNewWorkLabelTitle->setStyleSheet("#frameNewWorkLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewWorkLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewWorkLabelTitle->setMargin(5);
    frameNewWorkAdd = new QPushButton(frameNewWork);
    frameNewWorkAdd->setObjectName(QString("frameNewWorkAdd"));
    frameNewWorkAdd->setGeometry(QRect(370, 10, 21, 21));
    frameNewWorkAdd->setStyleSheet("#frameNewWorkAdd{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewWorkAdd:hover{\n"
    "	color:white;\n"
    "}");
    frameNewWorkDateFrom = new QLineEdit(frameNewWork);
    frameNewWorkDateFrom->setObjectName(QString("frameNewWorkDateFrom"));
    frameNewWorkDateFrom->setGeometry(QRect(20, 80, 151, 31));
    frameNewWorkDateFrom->setStyleSheet("#frameNewWorkDateFrom{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    frameNewWorkDateFrom->setAlignment(Qt::AlignCenter);
    frameNewWorkDateTo = new QLineEdit(frameNewWork);
    frameNewWorkDateTo->setObjectName(QString("frameNewWorkDateTo"));
    frameNewWorkDateTo->setGeometry(QRect(20, 140, 151, 31));
    frameNewWorkDateTo->setStyleSheet("#frameNewWorkDateTo{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;	\n"
    "	border:none;\n"
    "}");
    frameNewWorkDateTo->setAlignment(Qt::AlignCenter);
    frameNewWorkSector = new QLineEdit(frameNewWork);
    frameNewWorkSector->setObjectName(QString("frameNewWorkSector"));
    frameNewWorkSector->setGeometry(QRect(20, 200, 151, 31));
    frameNewWorkSector->setStyleSheet("#frameNewWorkSector{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;	\n"
    "	border:none;\n"
    "}");
    frameNewWorkJob = new QLineEdit(frameNewWork);
    frameNewWorkJob->setObjectName(QString("frameNewWorkJob"));
    frameNewWorkJob->setGeometry(QRect(20, 260, 151, 31));
    frameNewWorkJob->setStyleSheet("#frameNewWorkJob{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;	\n"
    "	border:none;\n"
    "}");
    frameNewWorkLabelFrom = new QLabel(frameNewWork);
    frameNewWorkLabelFrom->setObjectName(QString("frameNewWorkLabelFrom"));
    frameNewWorkLabelFrom->setGeometry(QRect(20, 60, 111, 16));
    frameNewWorkLabelFrom->setStyleSheet("#frameNewWorkLabelFrom{\n"
    "	color:white;\n"
    "}");
    frameNewWorkLabelFrom->setAlignment(Qt::AlignCenter);
    frameNewWorkLabelTo = new QLabel(frameNewWork);
    frameNewWorkLabelTo->setObjectName(QString("frameNewWorkLabelTo"));
    frameNewWorkLabelTo->setGeometry(QRect(20, 120, 111, 16));
    frameNewWorkLabelTo->setStyleSheet("#frameNewWorkLabelTo{\n"
    "	color:white;\n"
    "}");
    frameNewWorkLabelTo->setAlignment(Qt::AlignCenter);
    frameNewWorkLabelSector = new QLabel(frameNewWork);
    frameNewWorkLabelSector->setObjectName(QString("frameNewWorkLabelSector"));
    frameNewWorkLabelSector->setGeometry(QRect(30, 180, 111, 16));
    frameNewWorkLabelSector->setStyleSheet("#frameNewWorkLabelSector{\n"
    "	color:white;\n"
    "}");
    frameNewWorkLabelSector->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    frameNewWorkLabelJob = new QLabel(frameNewWork);
    frameNewWorkLabelJob->setObjectName(QString("frameNewWorkLabelJob"));
    frameNewWorkLabelJob->setGeometry(QRect(30, 240, 111, 16));
    frameNewWorkLabelJob->setStyleSheet("#frameNewWorkLabelJob{\n"
    "	color:white;\n"
    "}");
    frameNewWorkLabelJob->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    frameNewWorkListSkill = new QListWidget(frameNewWork);
    frameNewWorkListSkill->setObjectName(QString("frameNewWorkListSkill"));
    frameNewWorkListSkill->setGeometry(QRect(188, 135, 211, 161));
    frameNewWorkListSkill->setStyleSheet("#frameNewWorkListSkill{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:10px;\n"
    "	padding:10px;\n"
    "}");
    frameNewWorkLabelSkill = new QLabel(frameNewWork);
    frameNewWorkLabelSkill->setObjectName(QString("frameNewWorkLabelSkill"));
    frameNewWorkLabelSkill->setGeometry(QRect(183, 50, 221, 251));
    frameNewWorkLabelSkill->setStyleSheet("#frameNewWorkLabelSkill{\n"
    "	background:rgba(150,0,30,0.5);\n"
    "	color:white;\n"
    "}");
    frameNewWorkLabelSkill->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    frameNewWorkLabelSkill->setMargin(8);
    frameNewWorkNewSkill = new QLineEdit(frameNewWork);
    frameNewWorkNewSkill->setObjectName(QString("frameNewWorkNewSkill"));
    frameNewWorkNewSkill->setGeometry(QRect(190, 80, 181, 31));
    frameNewWorkNewSkill->setStyleSheet("#frameNewWorkNewSkill{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-radius:15px;\n"
    "	padding:5px;\n"
    "}");
    frameNewWorkAddSkill = new QPushButton(frameNewWork);
    frameNewWorkAddSkill->setObjectName(QString("frameNewWorkAddSkill"));
    frameNewWorkAddSkill->setGeometry(QRect(376, 84, 21, 21));
    frameNewWorkAddSkill->setStyleSheet("#frameNewWorkAddSkill{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewWorkAddSkilll:hover{\n"
    "	color:white;\n"
    "}");
    frameNewWorkDeleteSkill = new QPushButton(frameNewWork);
    frameNewWorkDeleteSkill->setObjectName(QString("frameNewWorkDeleteSkill"));
    frameNewWorkDeleteSkill->setGeometry(QRect(376, 111, 21, 21));
    frameNewWorkDeleteSkill->setStyleSheet("#frameNewWorkDeleteSkill{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewWorkDeleteSkill:hover{\n"
    "	color:white;\n"
    "}");
    frameWorkUpdate = new QPushButton(frameNewWork);
    frameWorkUpdate->setObjectName(QString("frameWorkUpdate"));
    frameWorkUpdate->setGeometry(QRect(10, 10, 51, 21));
    frameWorkUpdate->setStyleSheet("#frameWorkUpdate{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameWorkUpdate:hover{\n"
    "	color:white;\n"
    "}");
    frameWorkDelete = new QPushButton(frameNewWork);
    frameWorkDelete->setObjectName(QString("frameWorkDelete"));
    frameWorkDelete->setGeometry(QRect(70, 10, 61, 21));
    frameWorkDelete->setStyleSheet("#frameWorkDelete{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameWorkDelete:hover{\n"
    "	color:white;\n"
    "}");
    frameWorkUndoUpdate = new QPushButton(frameNewWork);
    frameWorkUndoUpdate->setObjectName(QString("frameWorkUndoUpdate"));
    frameWorkUndoUpdate->setGeometry(QRect(310, 10, 81, 20));
    frameWorkUndoUpdate->setStyleSheet("#frameWorkUndoUpdate{\n"
    "	background:rgba(255,0,30,0.5);\n"
    "	color:white;\n"
    "	border-radius:10px;\n"
    "	border:1px solid white;\n"
    "}\n"
    "#frameWorkUndoUpdate:hover{\n"
    "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameNewWorkLabelTitle->raise();
    frameWorkUndoUpdate->raise();
    frameNewWorkLabelSkill->raise();
    frameNewWorkAdd->raise();
    frameNewWorkDateFrom->raise();
    frameNewWorkDateTo->raise();
    frameNewWorkSector->raise();
    frameNewWorkJob->raise();
    frameNewWorkLabelFrom->raise();
    frameNewWorkLabelTo->raise();
    frameNewWorkLabelSector->raise();
    frameNewWorkLabelJob->raise();
    frameNewWorkListSkill->raise();
    frameNewWorkNewSkill->raise();
    frameNewWorkAddSkill->raise();
    frameNewWorkDeleteSkill->raise();
    frameWorkUpdate->raise();
    frameWorkDelete->raise();
    userDetails->addTab(tab_3, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString("tab_4"));
    frameLanguage = new QFrame(tab_4);
    frameLanguage->setObjectName(QString("frameLanguage"));
    frameLanguage->setGeometry(QRect(0, 0, 411, 481));
    frameLanguage->setStyleSheet("#frameLanguage{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameLanguage->setFrameShape(QFrame::StyledPanel);
    frameLanguage->setFrameShadow(QFrame::Raised);
    frameLanguageLabelTitle = new QLabel(frameLanguage);
    frameLanguageLabelTitle->setObjectName(QString("frameLanguageLabelTitle"));
    frameLanguageLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameLanguageLabelTitle->setStyleSheet("#frameLanguageLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameLanguageLabelTitle->setAlignment(Qt::AlignCenter);
    frameLanguageLabelTitle->setMargin(5);
    frameLanguageList = new QListWidget(frameLanguage);
    frameLanguageList->setObjectName(QString("frameLanguageList"));
    frameLanguageList->setGeometry(QRect(0, 40, 411, 441));
    frameLanguageList->setStyleSheet("#frameLanguageList{\n"
    "	background:rgba(0,0,0,0,5);\n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewLanguage = new QFrame(tab_4);
    frameNewLanguage->setObjectName(QString("frameNewLanguage"));
    frameNewLanguage->setGeometry(QRect(0, 480, 411, 201));
    frameNewLanguage->setStyleSheet("#frameNewLanguage{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameNewLanguage->setFrameShape(QFrame::StyledPanel);
    frameNewLanguage->setFrameShadow(QFrame::Raised);
    frameNewLanguageLabelTitle = new QLabel(frameNewLanguage);
    frameNewLanguageLabelTitle->setObjectName(QString("frameNewLanguageLabelTitle"));
    frameNewLanguageLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameNewLanguageLabelTitle->setStyleSheet("#frameNewLanguageLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewLanguageLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelTitle->setMargin(5);
    frameNewLanguageAdd = new QPushButton(frameNewLanguage);
    frameNewLanguageAdd->setObjectName(QString("frameNewLanguageAdd"));
    frameNewLanguageAdd->setGeometry(QRect(370, 10, 21, 21));
    frameNewLanguageAdd->setStyleSheet("#frameNewLanguageAdd{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewLanguageAdd:hover{\n"
    "	color:white;\n"
    "}");
    frameNewLanguageName = new QLineEdit(frameNewLanguage);
    frameNewLanguageName->setObjectName(QString("frameNewLanguageName"));
    frameNewLanguageName->setGeometry(QRect(80, 80, 251, 41));
    frameNewLanguageName->setStyleSheet("#frameNewLanguageName{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:20px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "	border:none;\n"
    "}");
    frameNewLanguageName->setAlignment(Qt::AlignCenter);
    frameNewLanguageSpoken = new QSpinBox(frameNewLanguage);
    frameNewLanguageSpoken->setObjectName(QString("frameNewLanguageSpoken"));
    frameNewLanguageSpoken->setGeometry(QRect(80, 150, 61, 31));
    frameNewLanguageSpoken->setStyleSheet("#frameNewLanguageSpoken{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:15px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewLanguageWritten = new QSpinBox(frameNewLanguage);
    frameNewLanguageWritten->setObjectName(QString("frameNewLanguageWritten"));
    frameNewLanguageWritten->setGeometry(QRect(180, 150, 61, 31));
    frameNewLanguageWritten->setStyleSheet("#frameNewLanguageWritten{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:15px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewLanguageHeard = new QSpinBox(frameNewLanguage);
    frameNewLanguageHeard->setObjectName(QString("frameNewLanguageHeard"));
    frameNewLanguageHeard->setGeometry(QRect(280, 150, 61, 31));
    frameNewLanguageHeard->setStyleSheet("#frameNewLanguageHeard{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:15px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewLanguageLabelSpoken = new QLabel(frameNewLanguage);
    frameNewLanguageLabelSpoken->setObjectName(QString("frameNewLanguageLabelSpoken"));
    frameNewLanguageLabelSpoken->setGeometry(QRect(80, 130, 59, 16));
    frameNewLanguageLabelSpoken->setStyleSheet("#frameNewLanguageLabelSpoken{\n"
    "	color:white;\n"
    "}");
    frameNewLanguageLabelSpoken->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelWritten = new QLabel(frameNewLanguage);
    frameNewLanguageLabelWritten->setObjectName(QString("frameNewLanguageLabelWritten"));
    frameNewLanguageLabelWritten->setGeometry(QRect(180, 130, 59, 16));
    frameNewLanguageLabelWritten->setStyleSheet("#frameNewLanguageLabelWritten{\n"
    "	color:white;\n"
    "}");
    frameNewLanguageLabelWritten->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelHeard = new QLabel(frameNewLanguage);
    frameNewLanguageLabelHeard->setObjectName(QString("frameNewLanguageLabelHeard"));
    frameNewLanguageLabelHeard->setGeometry(QRect(280, 130, 59, 16));
    frameNewLanguageLabelHeard->setStyleSheet("#frameNewLanguageLabelHeard{\n"
    "	color:white;\n"
    "}");
    frameNewLanguageLabelHeard->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelName = new QLabel(frameNewLanguage);
    frameNewLanguageLabelName->setObjectName(QString("frameNewLanguageLabelName"));
    frameNewLanguageLabelName->setGeometry(QRect(180, 60, 59, 16));
    frameNewLanguageLabelName->setStyleSheet("#frameNewLanguageLabelName{\n"
    "	color:white;\n"
    "}");
    frameNewLanguageLabelName->setAlignment(Qt::AlignCenter);
    frameLanguageDelete = new QPushButton(frameNewLanguage);
    frameLanguageDelete->setObjectName(QString("frameLanguageDelete"));
    frameLanguageDelete->setGeometry(QRect(80, 10, 61, 21));
    frameLanguageDelete->setStyleSheet("#frameLanguageDelete{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameLanguageDelete:hover{\n"
    "	color:white;\n"
    "}");
    frameLanguageUpdate = new QPushButton(frameNewLanguage);
    frameLanguageUpdate->setObjectName(QString("frameLanguageUpdate"));
    frameLanguageUpdate->setGeometry(QRect(10, 10, 61, 21));
    frameLanguageUpdate->setStyleSheet("#frameLanguageUpdate{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameLanguageUpdate:hover{\n"
    "	color:white;\n"
    "}");
    frameLanguageUndoUpdate = new QPushButton(frameNewLanguage);
    frameLanguageUndoUpdate->setObjectName(QString("frameLanguageUndoUpdate"));
    frameLanguageUndoUpdate->setGeometry(QRect(310, 10, 81, 20));
    frameLanguageUndoUpdate->setStyleSheet("#frameLanguageUndoUpdate{\n"
    "	background:rgba(255,0,30,0.5);\n"
    "	color:white;\n"
    "	border-radius:10px;\n"
    "	border:1px solid white;\n"
    "}\n"
    "#frameLanguageUndoUpdate:hover{\n"
    "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameNewLanguageLabelTitle->raise();
    frameLanguageUndoUpdate->raise();
    frameNewLanguageAdd->raise();
    frameNewLanguageName->raise();
    frameNewLanguageSpoken->raise();
    frameNewLanguageWritten->raise();
    frameNewLanguageHeard->raise();
    frameNewLanguageLabelSpoken->raise();
    frameNewLanguageLabelWritten->raise();
    frameNewLanguageLabelHeard->raise();
    frameNewLanguageLabelName->raise();
    frameLanguageDelete->raise();
    frameLanguageUpdate->raise();
    userDetails->addTab(tab_4, QString());
    tab_5 = new QWidget();
    tab_5->setObjectName(QString("tab_5"));
    frameQualification = new QFrame(tab_5);
    frameQualification->setObjectName(QString("frameQualification"));
    frameQualification->setGeometry(QRect(0, 0, 411, 411));
    frameQualification->setStyleSheet("#frameQualification{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameQualification->setFrameShape(QFrame::StyledPanel);
    frameQualification->setFrameShadow(QFrame::Raised);
    frameQualificationLabelTitle = new QLabel(frameQualification);
    frameQualificationLabelTitle->setObjectName(QString("frameQualificationLabelTitle"));
    frameQualificationLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameQualificationLabelTitle->setStyleSheet("#frameQualificationLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameQualificationLabelTitle->setAlignment(Qt::AlignCenter);
    frameQualificationLabelTitle->setMargin(5);
    frameQualificationList = new QListWidget(frameQualification);
    frameQualificationList->setObjectName(QString("frameQualificationList"));
    frameQualificationList->setGeometry(QRect(0, 40, 411, 371));
    frameQualificationList->setStyleSheet("#frameQualificationList{\n"
    "	background:rgba(0,0,0,0,5);\n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewQualification = new QFrame(tab_5);
    frameNewQualification->setObjectName(QString("frameNewQualification"));
    frameNewQualification->setGeometry(QRect(0, 410, 411, 271));
    frameNewQualification->setStyleSheet("#frameNewQualification{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameNewQualification->setFrameShape(QFrame::StyledPanel);
    frameNewQualification->setFrameShadow(QFrame::Raised);
    frameNewQualificationLabelTitle = new QLabel(frameNewQualification);
    frameNewQualificationLabelTitle->setObjectName(QString("frameNewQualificationLabelTitle"));
    frameNewQualificationLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameNewQualificationLabelTitle->setStyleSheet("#frameNewQualificationLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewQualificationLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelTitle->setMargin(5);
    frameNewQualificationAdd = new QPushButton(frameNewQualification);
    frameNewQualificationAdd->setObjectName(QString("frameNewQualificationAdd"));
    frameNewQualificationAdd->setGeometry(QRect(370, 10, 21, 21));
    frameNewQualificationAdd->setStyleSheet("#frameNewQualificationAdd{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewQualificationAdd:hover{\n"
    "	color:white;\n"
    "}");
    frameNewQualificationName = new QLineEdit(frameNewQualification);
    frameNewQualificationName->setObjectName(QString("frameNewQualificationName"));
    frameNewQualificationName->setGeometry(QRect(80, 90, 261, 41));
    frameNewQualificationName->setStyleSheet("#frameNewQualificationName{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:20px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "	border:none;\n"
    "}");
    frameNewQualificationYear = new QLineEdit(frameNewQualification);
    frameNewQualificationYear->setObjectName(QString("frameNewQualificationYear"));
    frameNewQualificationYear->setGeometry(QRect(110, 180, 201, 41));
    frameNewQualificationYear->setStyleSheet("#frameNewQualificationYear{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	border-radius:20px; \n"
    "	color:white;\n"
    "	padding:10px;\n"
    "	border:none;\n"
    "}");
    frameNewQualificationYear->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelName = new QLabel(frameNewQualification);
    frameNewQualificationLabelName->setObjectName(QString("frameNewQualificationLabelName"));
    frameNewQualificationLabelName->setGeometry(QRect(159, 70, 111, 16));
    frameNewQualificationLabelName->setStyleSheet("#frameNewQualificationLabelName {\n"
    "	color:white;\n"
    "}");
    frameNewQualificationLabelName->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelYear = new QLabel(frameNewQualification);
    frameNewQualificationLabelYear->setObjectName(QString("frameNewQualificationLabelYear"));
    frameNewQualificationLabelYear->setGeometry(QRect(120, 160, 181, 16));
    frameNewQualificationLabelYear->setStyleSheet("#frameNewQualificationLabelYear{\n"
    "	color:white;\n"
    "}");
    frameNewQualificationLabelYear->setAlignment(Qt::AlignCenter);
    frameQualificationUpdate = new QPushButton(frameNewQualification);
    frameQualificationUpdate->setObjectName(QString("frameQualificationUpdate"));
    frameQualificationUpdate->setGeometry(QRect(10, 10, 61, 21));
    frameQualificationUpdate->setStyleSheet("#frameQualificationUpdate{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameQualificationUpdate:hover{\n"
    "	color:white;\n"
    "}");
    frameQualificationDelete = new QPushButton(frameNewQualification);
    frameQualificationDelete->setObjectName(QString("frameQualificationDelete"));
    frameQualificationDelete->setGeometry(QRect(80, 10, 61, 21));
    frameQualificationDelete->setStyleSheet("#frameQualificationDelete{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameQualificationDelete:hover{\n"
    "	color:white;\n"
    "}\n"
    "}");
    frameQualificationUndoUpdate = new QPushButton(frameNewQualification);
    frameQualificationUndoUpdate->setObjectName(QString("frameQualificationUndoUpdate"));
    frameQualificationUndoUpdate->setGeometry(QRect(310, 10, 81, 20));
    frameQualificationUndoUpdate->setStyleSheet("#frameQualificationUndoUpdate{\n"
    "	background:rgba(255,0,30,0.5);\n"
    "	color:white;\n"
    "	border-radius:10px;\n"
    "	border:1px solid white;\n"
    "}\n"
    "#frameQualificationUndoUpdate:hover{\n"
    "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameNewQualificationLabelTitle->raise();
    frameQualificationUndoUpdate->raise();
    frameNewQualificationAdd->raise();
    frameNewQualificationName->raise();
    frameNewQualificationYear->raise();
    frameNewQualificationLabelName->raise();
    frameNewQualificationLabelYear->raise();
    frameQualificationUpdate->raise();
    frameQualificationDelete->raise();
    userDetails->addTab(tab_5, QString());
    tab_6 = new QWidget();
    tab_6->setObjectName(QString("tab_6"));
    frameNet = new QFrame(tab_6);
    frameNet->setObjectName(QString("frameNet"));
    frameNet->setGeometry(QRect(0, 0, 411, 411));
    frameNet->setStyleSheet("#frameNet{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameNet->setFrameShape(QFrame::StyledPanel);
    frameNet->setFrameShadow(QFrame::Raised);
    frameNetLabelTitle = new QLabel(frameNet);
    frameNetLabelTitle->setObjectName(QString("frameNetLabelTitle"));
    frameNetLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameNetLabelTitle->setStyleSheet("#frameNetLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(255,0,0,0.5);\n"
    "}");
    frameNetLabelTitle->setAlignment(Qt::AlignCenter);
    frameNetLabelTitle->setMargin(5);
    frameNetDelete = new QPushButton(frameNet);
    frameNetDelete->setObjectName(QString("frameNetDelete"));
    frameNetDelete->setGeometry(QRect(20, 10, 81, 21));
    frameNetDelete->setStyleSheet("#frameNetDelete{\n"
    "	background:rgba(255,0,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNetDelete:hover{\n"
    "	color:white;\n"
    "}");
    frameNetList = new QListWidget(frameNet);
    frameNetList->setObjectName(QString("frameNetList"));
    frameNetList->setGeometry(QRect(0, 40, 411, 371));
    frameNetList->setStyleSheet("#frameNetList{\n"
    "	background:rgba(0,0,0,0,5);\n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    frameNewNet = new QFrame(tab_6);
    frameNewNet->setObjectName(QString("frameNewNet"));
    frameNewNet->setGeometry(QRect(0, 410, 411, 271));
    frameNewNet->setStyleSheet("#frameNewNet{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "}");
    frameNewNet->setFrameShape(QFrame::StyledPanel);
    frameNewNet->setFrameShadow(QFrame::Raised);
    frameNewNetLabelTitle = new QLabel(frameNewNet);
    frameNewNetLabelTitle->setObjectName(QString("frameNewNetLabelTitle"));
    frameNewNetLabelTitle->setGeometry(QRect(0, 0, 411, 41));
    frameNewNetLabelTitle->setStyleSheet("#frameNewNetLabelTitle{\n"
    "	color:white;\n"
    "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewNetLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewNetLabelTitle->setMargin(5);
    frameNewNetAdd = new QPushButton(frameNewNet);
    frameNewNetAdd->setObjectName(QString("frameNewNetAdd"));
    frameNewNetAdd->setGeometry(QRect(370, 10, 21, 21));
    frameNewNetAdd->setStyleSheet("#frameNewNetAdd{\n"
    "	background:rgba(0,255,30,0.7);\n"
    "	border:1px solid rgba(0,0.0.0.2);	\n"
    "	border-radius:10px;\n"
    "	color:black;\n"
    "}\n"
    "#frameNewNetAdd:hover{\n"
    "	color:white;\n"
    "}");
    frameNewNetList = new QListWidget(frameNewNet);
    frameNewNetList->setObjectName(QString("frameNewNetList"));
    frameNewNetList->setGeometry(QRect(0, 40, 411, 231));
    frameNewNetList->setStyleSheet("#frameNewNetList{\n"
    "	background:rgba(0,0,0,0,5);\n"
    "	color:white;\n"
    "	padding:10px;\n"
    "}");
    userDetails->addTab(tab_6, QString());
    exit = new QPushButton(this);
    exit->setObjectName(QString("exit"));
    exit->setGeometry(QRect(10, 730, 71, 51));
    exit->setStyleSheet("#exit{\n"
    "	background:rgba(0,0,0,0.9);\n"
    "	border-radius:25px;\n"
    "	color:white;\n"
    "}\n"
    "#exit:hover{\n"
    "	color:orange;\n"
    "	border:1px solid white;\n"
    "}");
    find = new QPushButton(this);
    find->setObjectName(QString("find"));
    find->setGeometry(QRect(340, 740, 81, 31));
    find->setStyleSheet("#find{\n"
    "	background:rgba(0,0,0,0.9);\n"
    "	border-top-right-radius:15px;\n"
    "	border-bottom-right-radius:15px;\n"
    "	border:1px solid white;\n"
    "	color:white;\n"
    "}\n"
    "#find:hover{\n"
    "	color:orange;\n"
    "	border:1px solid orange;\n"
    "}");
    findPeople = new QLineEdit(this);
    findPeople->setObjectName(QString("findPeople"));
    findPeople->setGeometry(QRect(100, 740, 241, 31));
    findPeople->setStyleSheet("#findPeople{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	border-top-left-radius:15px;\n"
    "	border-bottom-left-radius:15px;\n"
    "	padding:5px;\n"
    "	border:none;\n"
    "}");
    QMetaObject::connectSlotsByName(this);
    setWindowTitle(QString("Form"));
    frameAccountOldpassword->setText(QString());
    frameAccountLabelUsername->setText(QString("Username"));
    frameAccountLabelOldpassword->setText(QString("Vecchia"));
    frameAccountLabelNewpassword->setText(QString("Nuova "));
    frameAccountLabelNewpasswordConfirm->setText(QString("Conferma"));
    frameAccountLabelTitle->setText(QString("ACCOUNT"));
    frameAccountUpdateInformation->setText(QString("Modifica"));
    frameAccountLabelChangePassword->setText(QString("CAMBIO CREDENZIALI"));
    frameAcountLabelResumeWork->setText(QString("Totale lavori"));
    frameAcountLabelResumeQualification->setText(QString("Titoli conseguiti"));
    frameAcountLabelResumeLanguage->setText(QString("Lingue parlate"));
    frameAcountLabelResumePeople->setText(QString("Contatti"));
    frameAccountCountWork->setText(QString());
    frameAccountCountQualification->setText(QString());
    frameAccountCountPeople->setText(QString());
    frameAccountCountLanguage->setText(QString());
    frameAccountErrorPassword->setText(QString("Password non corrispondenti"));
    frameAccountErrorOldpassword->setText(QString("Password attuale errata"));
    userDetails->setTabText(userDetails->indexOf(tab), QString("Account"));
    frameProfileLabelAddress->setText(QString("Contatti personali"));
    frameProfileAddNewAddress->setText(QString("+"));
    frameProfileLabelName->setText(QString("Name"));
    frameProfileLabelSurname->setText(QString("Surname"));
    frameProfileLabelBirth->setText(QString("Birth"));
    frameProfileLabelNationality->setText(QString("Nationality"));
    frameProfileLabelTitle->setText(QString("PROFILO"));
    frameProfileUpdateInformation->setText(QString("Modifica"));
    frameProfileUndoUpdate->setText(QString("Ripristina"));
    frameProfileDeleteAddress->setText(QString("X"));
    frameProfileErrorName->setText(QString("Nome non valido"));
    frameProfileErrorSurname->setText(QString("Cognome non valido"));
    frameProfileErrorBirth->setText(QString("Data non valida"));
    frameProfileErrorNationality->setText(QString("Nazionalit\303\240 non valida"));
    userDetails->setTabText(userDetails->indexOf(tab_2), QString("Profilo"));
    frameWorkLabelTitle->setText(QString("LAVORI"));
    frameNewWorkLabelTitle->setText(QString("NUOVO LAVORO"));
    frameNewWorkAdd->setText(QString("+"));
    frameNewWorkLabelFrom->setText(QString("Data assunzione"));
    frameNewWorkLabelTo->setText(QString("Data di termine"));
    frameNewWorkLabelSector->setText(QString("Settore"));
    frameNewWorkLabelJob->setText(QString("Posizione"));
    frameNewWorkLabelSkill->setText(QString("Competenze acquisite"));
    frameNewWorkAddSkill->setText(QString("+"));
    frameNewWorkDeleteSkill->setText(QString("X"));
    frameWorkUpdate->setText(QString("Salva"));
    frameWorkDelete->setText(QString("Elimina"));
    frameWorkUndoUpdate->setText(QString("Ripristina"));
    userDetails->setTabText(userDetails->indexOf(tab_3), QString("Lavori"));
    frameLanguageLabelTitle->setText(QString("LINGUE"));
    frameNewLanguageLabelTitle->setText(QString("NUOVA LINGUA"));
    frameNewLanguageAdd->setText(QString("+"));
    frameNewLanguageLabelSpoken->setText(QString("Parlato"));
    frameNewLanguageLabelWritten->setText(QString("Scritto"));
    frameNewLanguageLabelHeard->setText(QString("Ascolto"));
    frameNewLanguageLabelName->setText(QString("Nome"));
    frameLanguageDelete->setText(QString("Elimina"));
    frameLanguageUpdate->setText(QString("Salva"));
    frameLanguageUndoUpdate->setText(QString("Ripristina"));
    userDetails->setTabText(userDetails->indexOf(tab_4), QString("Lingue"));
    frameQualificationLabelTitle->setText(QString("TITOLI CONSEGUITI"));
    frameNewQualificationLabelTitle->setText(QString("NUOVO TITOLO"));
    frameNewQualificationAdd->setText(QString("+"));
    frameNewQualificationLabelName->setText(QString("Nome qualifica"));
    frameNewQualificationLabelYear->setText(QString("Anno di conseguimento"));
    frameQualificationUpdate->setText(QString("Salva"));
    frameQualificationDelete->setText(QString("Elimina"));
    frameQualificationUndoUpdate->setText(QString("Ripristina"));
    userDetails->setTabText(userDetails->indexOf(tab_5), QString("Titoli"));
    frameNetLabelTitle->setText(QString("LISTA CONTATTI"));
    frameNetDelete->setText(QString("Elimina"));
    frameNewNetLabelTitle->setText(QString("AGGIUNGI CONTATTO "));
    frameNewNetAdd->setText(QString("+"));
    userDetails->setTabText(userDetails->indexOf(tab_6), QString("Persone"));
    exit->setText(QString("Esci"));
    find->setText(QString("CERCA"));
}
