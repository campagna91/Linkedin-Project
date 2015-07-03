#include "clientadmin.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHODS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


ClientAdmin::ClientAdmin(Db * link,QWidget *parent) :
    QWidget(parent),
    db(link)
{
    setup();
    connect(this,SIGNAL(s_save()),this->parent(),SLOT(saveRequest()));
    connect(this,SIGNAL(s_notSave()), this->parent(), SLOT(undoUpdate()));
    connect(this,SIGNAL(s_exit()),this->parent(),SLOT(exitAdminGui()));
    this->setGeometry(this->x(),this->y(),351,this->height());
    frameUsersFilterLoad();
    frameUsersLoad();
    actualWork = 0;
    actualLanguage = 0;
    actualQualification = 0;
    actualUser = 0;
    validate = &(dynamic_cast<ClientStart*>(this->parent()))->isValid;
}
ClientAdmin::~ClientAdmin()
{
    cout<<"CREATE CLIENTADMIN"<<endl;
}
void ClientAdmin::loadCount()
{
    if(actualUser)
    {
        frameLanguageCount->setText(QString::number(actualUser->getLanguages()->size()) + " lingue parlate");
        frameNetCount->setText(QString::number(actualUser->getNet()->size()) + " contatti in rubrica");
        frameQualificationCount->setText(QString::number(actualUser->getQualifications()->size()) + " titoli conseguiti");
        frameWorkCount->setText(QString::number(actualUser->getWorks()->size()) + " lavori svolti");
    }
    frameUsersCount->setText(QString::number(db->getDb()->size()) + " utenti trovati");
}
void ClientAdmin::frameUsersFilterLoad()
{
    frameUsersFilterSector->addItem("Filtra per settore");
    list<Linkediner*>::iterator i = db->getDb()->begin();
    for( ; i != db->getDb()->end(); ++i)
    {
        list<Works*>::iterator w = (*i)->getWorks()->begin();
        for( ; w != (*i)->getWorks()->end(); ++w)
            if(frameUsersFilterSector->findText(QString::fromStdString((*w)->getSector())) == -1)
                    frameUsersFilterSector->addItem(QString::fromStdString((*w)->getSector()));
    }
}
void ClientAdmin::frameUsersLoad()
{
   frameUsersList->clear();
   QPixmap icon = QPixmap("://resource/user.png");
   list<Linkediner*>::iterator i = db->getDb()->begin();
   for( ; i!= db->getDb()->end(); ++i){
       QString username = QString::fromStdString((*i)->getEmail());
       QListWidgetItem * u = new QListWidgetItem(username,frameUsersList);
       u->setIcon(icon);
       u->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
   }
   frameUsersList->setIconSize(QSize(32,32));
   QString count = QString::number(db->size());
   count.append(" utenti trovati ");
}
void ClientAdmin::frameDetailsLoad()
{
    frameAccountLoad();
    frameProfileLoad();
    frameLanguageLoad();
    frameQualificationLoad();
    frameWorkLoad();
    frameNetLoad();
    if(typeid(*actualUser) == typeid(LinkedinerBasic)) frameAccountTypeBasic->setChecked(true);
    if(typeid(*actualUser) == typeid(LinkedinerBusiness)) frameAccountTypeBusiness->setChecked(true);
    if(typeid(*actualUser) == typeid(LinkedinerExecutive)) frameAccountTypeExecutive->setChecked(true);
}
void ClientAdmin::frameAccountLoad()
{
    frameAccountUpdate->hide();
    frameAccountUsername->setText(QString::fromStdString(actualUser->getEmail()));
}
void ClientAdmin::frameProfileLoad()
{
    frameProfileAddressList->clear();
    frameProfileName->setText(QString::fromStdString(actualUser->getProfile()->getName()));
    frameProfileSurname->setText(QString::fromStdString(actualUser->getProfile()->getSurname()));
    frameProfileBirth->setText(actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd"));
    frameProfileNationality->setText(QString::fromStdString(actualUser->getProfile()->getNationality()));
    list<string>::iterator i = actualUser->getProfile()->getContacts()->begin();
    for( ; i != actualUser->getProfile()->getContacts()->end(); ++i)
        QListWidgetItem * it = new QListWidgetItem(QPixmap("://resource/contact.png"),QString::fromStdString(*i),frameProfileAddressList);
}
void ClientAdmin::frameLanguageLoad()
{
    loadCount();
    clearHiddenBottons();
    frameNewLanguageAdd->show();
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
void ClientAdmin::frameWorkLoad()
{
    loadCount();
    clearHiddenBottons();
    frameNewWorkAdd->show();
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
void ClientAdmin::frameQualificationLoad()
{
    loadCount();
    clearHiddenBottons();
    frameNewQualificationAdd->show();
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
void ClientAdmin::frameNetLoad()
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
void ClientAdmin::filter(string email,string sector,int year)
{
    frameUsersLoad();
    Linkediner * u;
    QListWidgetItem * actualItem;
    for(int i = 0; i < frameUsersList->count(); ++i)
    {
        bool take = false;
        actualItem = frameUsersList->item(i);
        u = db->getUserByEmail(actualItem->text().toStdString());
        if(email != "")
        {
            if(u->getEmail().substr(0,email.length()) != email || u->getEmail().length() < email.length())
            {
                delete actualItem;
                i--;
                take = true;
            }
        }
        if(!take && u->getProfile()->getBirth().year() < year)
        {
            take = true;
            delete actualItem;
            i--;
        }
        if(!take && sector != "Filtra per settore")
        {
            bool find = false;
            list<Works*>::iterator it = u->getWorks()->begin();
            for( ; it != u->getWorks()->end() && !find; ++it)
                if( (*it)->getSector() == sector )
                    find = true;
            if(!find)
            {
                delete actualItem;
                i--;
            }
        }
    }
}
void ClientAdmin::clearHiddenBottons(){
    frameAccountUpdate->hide();
    frameProfileUpdate->hide();
    frameProfileUndoUpdate->hide();
    frameLanguageUpdate->hide();
    frameLanguageDelete->hide();
    frameLanguageUndoUpdate->hide();
    frameWorkDelete->hide();
    frameWorkUpdate->hide();
    frameWorkUndoUpdate->hide();
    frameQualificationDelete->hide();
    frameQualificationUpdate->hide();
    frameQualificationUndoUpdate->hide();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE SLOTS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   GENERAL
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_exit_clicked()
{
    QMessageBox::StandardButton q = QMessageBox::question(this, "Salva", "Salvare le modifiche effettuate ?",QMessageBox::Yes|QMessageBox::No);
    if(q == QMessageBox::Yes)
        emit s_save();
    else
        emit s_notSave();
    emit s_exit();
}
void ClientAdmin::exitDialogNewUser()
{
    dialogNewUser->hide();
    delete dialogNewUser;
    loadCount();
    frameUsersLoad();
    this->show();
}
void ClientAdmin::on_frameUsersAdd_clicked()
{
    dialogNewUser = new ClientAdminNewUser(db,this);
    connect(dialogNewUser,SIGNAL(s_exit()),this,SLOT(exitDialogNewUser()));
    dialogNewUser->setModal(true);
    dialogNewUser->show();
    this->hide();
}
void ClientAdmin::on_frameUsersDelete_clicked()
{
    if(frameUsersList->currentItem())
    {
        Linkediner * l = db->getUserByEmail(frameUsersList->currentItem()->text().toStdString());
        db->removeUser(l);
        list<Linkediner*>::iterator i = db->getDb()->begin();
        for ( ; i != db->getDb()->end(); ++i)
        {
            (*i)->getNet()->remove(l);
        }
        delete frameUsersList->currentItem();
    }
    this->setGeometry(this->x(),this->y(),351,this->height());
}
void ClientAdmin::on_frameUsersFilterUsername_textChanged(const QString &arg1)
{
    filter(arg1.toStdString(),frameUsersFilterSector->currentText().toStdString(),frameUsersFilterYear->text().toInt());
}
void ClientAdmin::on_frameUsersFilterYear_valueChanged(int arg1)
{
    filter(frameUsersFilterUsername->text().toStdString(),frameUsersFilterSector->currentText().toStdString(),arg1);
}
void ClientAdmin::on_frameUsersFilterSector_currentTextChanged(const QString &arg1)
{
    filter(frameUsersFilterUsername->text().toStdString(),arg1.toStdString(),frameUsersFilterYear->text().toInt());
}
void ClientAdmin::on_frameUsersList_itemDoubleClicked(QListWidgetItem *item)
{
    actualUser = db->getUserByEmail(item->text().toStdString());
    frameDetailsLoad();
    this->setGeometry(this->x(),this->y(),762,this->height());
    frameDetails->show();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   ACCOUNT
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameAccountChangetype_clicked()
{
    int toType = -1;
    if(frameAccountTypeBasic->isChecked())
    {
        if(typeid(*actualUser) != typeid(LinkedinerBasic)){
            toType = 0;
        }
    } else if(frameAccountTypeBusiness->isChecked())
    {
        if(typeid(*actualUser) != typeid(LinkedinerBusiness))
            toType = 1;
    } else {
        if(typeid(*actualUser) != typeid(LinkedinerExecutive))
            toType = 2;
    }
    Linkediner * oldUser, * newUser;
    if(toType != -1)
    {
        list<Linkediner*>::iterator i = db->getDb()->begin();
        for( ; i != db->getDb()->end(); ++i)
        {
            if( (*i)->getEmail() == frameAccountUsername->text().toStdString() )
            {
                switch(toType)
                {
                case(0):
                    newUser = new LinkedinerBasic(
                                actualUser->getEmail(),
                                actualUser->getPassword(),
                                actualUser->getPhoto(),
                                *actualUser->getProfile(),
                                *actualUser->getWorks(),
                                *actualUser->getLanguages(),
                                *actualUser->getNet(),
                                *actualUser->getQualifications()
                                );
                    break;
                case(1):
                    newUser = new LinkedinerBusiness(
                                actualUser->getEmail(),
                                actualUser->getPassword(),
                                actualUser->getPhoto(),
                                *actualUser->getProfile(),
                                *actualUser->getWorks(),
                                *actualUser->getLanguages(),
                                *actualUser->getNet(),
                                *actualUser->getQualifications()
                                );
                    break;
                case(2):
                    newUser = new LinkedinerExecutive(
                                actualUser->getEmail(),
                                actualUser->getPassword(),
                                actualUser->getPhoto(),
                                *actualUser->getProfile(),
                                *actualUser->getWorks(),
                                *actualUser->getLanguages(),
                                *actualUser->getNet(),
                                *actualUser->getQualifications()
                                );
                    break;
                }
                db->getDb()->erase(i);
                db->addUser(newUser);
                break;
            }
        }
        actualUser = db->getUserByEmail(newUser->getEmail());
     }
}
void ClientAdmin::on_frameAccountUpdate_clicked()
{

    if(frameAccountNewPassword->text() == frameAccountNewPasswordConfirm->text())
    {
        if(validate(2,0,frameAccountNewPassword,0))
        {
            frameAccountLabelChangePassword->setText("Password aggiornata con succcesso");
            actualUser->setPassword(frameAccountNewPassword->text().toStdString());
        } else frameAccountLabelChangePassword->setText("La password deve contenere Maiuscole minuscole e numeri");
    } else frameAccountLabelChangePassword->setText("La password deve contenere Maiuscole minuscole e numeri");
    frameAccountNewPassword->setText("");
    frameAccountNewPasswordConfirm->setText("");
}
void ClientAdmin::on_frameAccountNewPassword_textChanged(const QString &arg1)
{
    string newPass = frameAccountNewPassword->text().toStdString();
    string newPassConfirm = frameAccountNewPasswordConfirm->text().toStdString();
    if(newPass != "" && newPass != actualUser->getPassword())
       frameAccountUpdate->show();
    else frameAccountUpdate->hide();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   PROFILE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameProfileName_textChanged(const QString &arg1)
{
    if(frameProfileName->text().toStdString() != actualUser->getProfile()->getName() & validate(1,0,frameProfileName,0))
    {
        frameProfileUpdate->show();
        frameProfileUndoUpdate->show();
    } else clearHiddenBottons();
}
void ClientAdmin::on_frameProfileSurname_textChanged(const QString &arg1)
{
    if(frameProfileSurname->text().toStdString() != actualUser->getProfile()->getSurname() & validate(1,0,frameProfileSurname,0))
    {
        frameProfileUpdate->show();
        frameProfileUndoUpdate->show();
    } else clearHiddenBottons();
}
void ClientAdmin::on_frameProfileNationality_textChanged(const QString &arg1)
{
    if(frameProfileNationality->text().toStdString() != actualUser->getProfile()->getNationality() & validate(1,0,frameProfileNationality,0))
    {
        frameProfileUpdate->show();
        frameProfileUndoUpdate->show();
    } else clearHiddenBottons();
}
void ClientAdmin::on_frameProfileBirth_textChanged(const QString &arg1)
{
    if(frameProfileBirth->text() != actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd") & validate(0,0,frameProfileBirth,0))
    {
        frameProfileUpdate->show();
        frameProfileUndoUpdate->show();
    } else clearHiddenBottons();
}
void ClientAdmin::on_frameProfileAddNewAddress_clicked()
{
    if(validate(3,0,frameProfileNewAddress,0))
    {
        frameProfileNewAddress->setStyleSheet(frameProfileNewAddress->styleSheet().replace("border:1px solid red;","border:none;"));
        actualUser->getProfile()->getContacts()->push_back(frameProfileNewAddress->text().toStdString());
        QListWidgetItem * newContact = new QListWidgetItem(QPixmap("://resource/contact.png"),frameProfileNewAddress->text(),frameProfileAddressList);
        frameProfileNewAddress->setText("");
    }
}
void ClientAdmin::on_frameProfileUpdate_clicked()
{
    actualUser->getProfile()->setBirth(QDate::fromString(frameProfileBirth->text(),"yyyy'-'MM'-'dd"));
    actualUser->getProfile()->setName(frameProfileName->text().toStdString());
    actualUser->getProfile()->setSurname(frameProfileSurname->text().toStdString());
    actualUser->getProfile()->setNationality(frameProfileNationality->text().toStdString());
    clearHiddenBottons();
}
void ClientAdmin::on_frameProfileUndoUpdate_clicked()
{
    frameProfileName->setText(QString::fromStdString(actualUser->getProfile()->getName()));
    frameProfileSurname->setText(QString::fromStdString(actualUser->getProfile()->getSurname()));
    frameProfileNationality->setText(QString::fromStdString(actualUser->getProfile()->getNationality()));
    frameProfileBirth->setText(actualUser->getProfile()->getBirth().toString("yyyy'-'MM'-'dd"));
    clearHiddenBottons();
}
void ClientAdmin::on_frameProfileDeleteAddress_clicked()
{
    if(frameProfileAddressList->currentItem())
        actualUser->getProfile()->getContacts()->remove(frameProfileAddressList->currentItem()->text().toStdString());
    frameProfileLoad();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   WORK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameWorkList_itemDoubleClicked(QListWidgetItem *item)
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
void ClientAdmin::on_frameNewWorkAddSkill_clicked()
{
    if(frameNewWorkNewSkill->text() != "")
        QListWidgetItem * nuova = new QListWidgetItem(QPixmap("://resource/skills.jpg"),frameNewWorkNewSkill->text(),frameNewWorkListSkill);
    frameNewWorkNewSkill->clear();
}
void ClientAdmin::on_frameNewWorkDeleteSkill_clicked()
{
    if(frameNewWorkListSkill->currentItem())
        delete frameNewWorkListSkill->currentItem();
}
void ClientAdmin::on_frameNewWorkAdd_clicked()
{
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
        frameWorkLoad();
    }
}
void ClientAdmin::on_frameWorkUpdate_clicked()
{
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
        frameWorkLoad();
     }
}
void ClientAdmin::on_frameWorkDelete_clicked()
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
    frameWorkLoad();
}
void ClientAdmin::on_frameWorkUndoUpdate_clicked()
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
 *   QUALIFICATION
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameQualificationUpdate_clicked()
{
    if(validate(0,3,frameNewQualificationYear,frameProfileBirth) & validate(1,1,frameNewQualificationName,0))
    {
        actualQualification->setName(frameNewQualificationName->text().toStdString());
        actualQualification->setYear(QDate::fromString(frameNewQualificationYear->text(),"yyyy"));
        actualQualification = 0;
        frameQualificationLoad();
    }
}
void ClientAdmin::on_frameQualificationDelete_clicked()
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
    frameQualificationLoad();
}
void ClientAdmin::on_frameQualificationUndoUpdate_clicked()
{
    frameNewQualificationName->setText(QString::fromStdString(actualQualification->getName()));
    frameNewQualificationYear->setText(actualQualification->getYear().toString("yyyy"));
}
void ClientAdmin::on_frameNewQualificationAdd_clicked()
{
    if(validate(0,3,frameNewQualificationYear,frameProfileBirth) & validate(1,1,frameNewQualificationName,0))
    {
        Qualifications * nuova = new Qualifications(
            frameNewQualificationName->text().toStdString(),
            QDate::fromString(frameNewQualificationYear->text(),"yyyy")
        );
        actualUser->addQualifications(nuova);
        frameQualificationLoad();
    }
}
void ClientAdmin::on_frameQualificationList_itemDoubleClicked(QListWidgetItem *item)
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
 *   LANGUAGE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameLanguageList_itemDoubleClicked(QListWidgetItem *item)
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
void ClientAdmin::on_frameLanguageUpdate_clicked()
{
    if(validate(1,0,frameNewLanguageName,0))
    {
        actualLanguage->setName(frameNewLanguageName->text().toStdString());
        actualLanguage->setHeard(frameNewLanguageHeard->value());
        actualLanguage->setWritten(frameNewLanguageWritten->value());
        actualLanguage->setSpoken(frameNewLanguageSpoken->value());
        actualLanguage = 0;
        frameLanguageLoad();
    }
}
void ClientAdmin::on_frameLanguageDelete_clicked()
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
    frameLanguageLoad();
    clearHiddenBottons();
}
void ClientAdmin::on_frameLanguageUndoUpdate_clicked()
{
    frameNewLanguageName->setText(QString::fromStdString(actualLanguage->getName()));
    frameNewLanguageHeard->setValue(actualLanguage->getHeard());
    frameNewLanguageWritten->setValue(actualLanguage->getWritten());
    frameNewLanguageSpoken->setValue(actualLanguage->getSpoken());
}
void ClientAdmin::on_frameNewLanguageAdd_clicked()
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
        frameLanguageLoad();
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   NET
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::on_frameNetDelete_clicked()
{
    if(frameNetList->currentItem())
    {
        actualUser->getNet()->remove(db->getUserByEmail(frameNetList->currentItem()->text().toStdString()));
        QListWidgetItem * ni = new QListWidgetItem(QPixmap("://resource/net.jpeg"),frameNetList->currentItem()->text(),frameNewNetList);
        delete frameNetList->currentItem();
        loadCount();
    }
}
void ClientAdmin::on_frameNewNetAdd_clicked()
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
 *   PRIVATE METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   GRAPHIC SETUP
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdmin::setup()
{
    setObjectName("ClientAdmin");
    resize(762, 581);
    setMinimumSize(QSize(351, 581));
    setMaximumSize(QSize(762, 581));
    setStyleSheet("#ClientAdmin{\n"
        "background:#006699;\n"
    "}\n");
    frameUsersList = new QListWidget(this);
    frameUsersList->setObjectName("frameUsersList");
    frameUsersList->setGeometry(QRect(0, 212, 351, 331));
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frameUsersList->sizePolicy().hasHeightForWidth());
    frameUsersList->setSizePolicy(sizePolicy);
    frameUsersList->setMinimumSize(QSize(291, 0));
    frameUsersList->setMaximumSize(QSize(500, 500));
    frameUsersList->setStyleSheet("#frameUsersList{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    exit = new QPushButton(this);
    exit->setObjectName("exit");
    exit->setGeometry(QRect(32, 546, 51, 31));
    exit->setStyleSheet("#exit{\n"
        "	background:rgba(0,0,0,0.9);\n"
        "	border-radius:10px;\n"
        "	color:white;\n"
        "}\n"
        "#exit:hover{\n"
        "	color:orange;\n"
        "	border:1px solid white;\n"
    "}");
    frameUsersLabelTitle = new QLabel(this);
    frameUsersLabelTitle->setObjectName("frameUsersLabelTitle");
    frameUsersLabelTitle->setGeometry(QRect(0, 0, 351, 51));
    frameUsersLabelTitle->setStyleSheet("#frameUsersLabelTitle{\n"
        "	background:rgba(0,0,0,0.8);\n"
        "	color:white;\n"
    "}");
    frameUsersLabelTitle->setAlignment(Qt::AlignCenter);
    frameUsersLabelTitle->setIndent(12);
    frameDetails = new QTabWidget(this);
    frameDetails->setObjectName("frameDetails");
    frameDetails->setEnabled(true);
    frameDetails->setGeometry(QRect(350, -2, 411, 587));
    frameDetails->setStyleSheet("#frameDetails::pane{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border:none;\n"
    "}");
    Account = new QWidget();
    Account->setObjectName("Account");
    label_6 = new QLabel(Account);
    label_6->setObjectName("label_6");
    label_6->setGeometry(QRect(160, 20, 71, 31));
    label_6->setStyleSheet("#label_6{\n"
        "	color:white;\n"
    "}");
    label_7 = new QLabel(Account);
    label_7->setObjectName("label_7");
    label_7->setGeometry(QRect(0, 140, 411, 41));
    label_7->setStyleSheet("#label_7{\n"
        "	color:white;\n"
        "	background:rgba(0,0,0,0.4);\n"
    "}");
    label_7->setAlignment(Qt::AlignCenter);
    label_7->setIndent(9);
    frameAccountUpdate = new QPushButton(Account);
    frameAccountUpdate->setObjectName("frameAccountUpdate");
    frameAccountUpdate->setGeometry(QRect(140, 340, 141, 41));
    frameAccountUpdate->setStyleSheet("#frameAccountUpdate{\n"
        "	background:rgba(0,255,30,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	border:2px solid white;\n"
        "}\n"
        "#frameAccountUpdate:hover{\n"
        "	background:rgba(0,255,30,0.8);\n"
    "}");
    frameAccountNewPassword = new QLineEdit(Account);
    frameAccountNewPassword->setObjectName("frameAccountNewPassword");
    frameAccountNewPassword->setGeometry(QRect(110, 200, 251, 41));
    frameAccountNewPassword->setStyleSheet("#frameAccountNewPassword{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	border-radius:20px;\n"
    "}");
    frameAccountNewPassword->setEchoMode(QLineEdit::Password);
    frameAccountNewPassword->setAlignment(Qt::AlignCenter);
    frameAccountUsername = new QLineEdit(Account);
    frameAccountUsername->setObjectName("frameAccountUsername");
    frameAccountUsername->setEnabled(false);
    frameAccountUsername->setGeometry(QRect(40, 60, 331, 41));
    frameAccountUsername->setStyleSheet("#frameAccountUsername{\n"
        "	background:rgba(0,0,0,0.2);\n"
        "	color:white;\n"
        "	font-weight:bolder;\n"
        "	border-radius:20px;\n"
    "}");
    frameAccountUsername->setAlignment(Qt::AlignCenter);
    frameAccountNewPasswordConfirm = new QLineEdit(Account);
    frameAccountNewPasswordConfirm->setObjectName("frameAccountNewPasswordConfirm");
    frameAccountNewPasswordConfirm->setGeometry(QRect(110, 250, 251, 41));
    frameAccountNewPasswordConfirm->setStyleSheet("#frameAccountNewPasswordConfirm{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;	\n"
        "	border-radius:20px;\n"
    "}");
    frameAccountNewPasswordConfirm->setEchoMode(QLineEdit::Password);
    frameAccountNewPasswordConfirm->setAlignment(Qt::AlignCenter);
    labelPassword = new QLabel(Account);
    labelPassword->setObjectName("labelPassword");
    labelPassword->setGeometry(QRect(36, 204, 141, 31));
    labelPassword->setStyleSheet("#labelPassword{\n"
        "	color:white;\n"
    "}");
    label_11 = new QLabel(Account);
    label_11->setObjectName("label_11");
    label_11->setGeometry(QRect(35, 255, 141, 31));
    label_11->setStyleSheet("#label_11{\n"
        "	color:white;\n"
    "}");
    frameAccountTypeBasic = new QRadioButton(Account);
    frameAccountTypeBasic->setObjectName("frameAccountTypeBasic");
    frameAccountTypeBasic->setGeometry(QRect(40, 420, 102, 22));
    frameAccountTypeBasic->setStyleSheet("#frameAccountTypeBasic{\n"
        "	color:white;\n"
    "}");
    frameAccountTypeExecutive = new QRadioButton(Account);
    frameAccountTypeExecutive->setObjectName("frameAccountTypeExecutive");
    frameAccountTypeExecutive->setGeometry(QRect(260, 420, 102, 22));
    frameAccountTypeExecutive->setStyleSheet("#frameAccountTypeExecutive{\n"
        "	color:white;\n"
    "}");
    frameAccountTypeBusiness = new QRadioButton(Account);
    frameAccountTypeBusiness->setObjectName("frameAccountTypeBusiness");
    frameAccountTypeBusiness->setGeometry(QRect(150, 420, 102, 22));
    frameAccountTypeBusiness->setStyleSheet("#frameAccountTypeBusiness{\n"
        "	color:white;\n"
    "}");
    frameAccountChangetype = new QPushButton(Account);
    frameAccountChangetype->setObjectName("frameAccountChangetype");
    frameAccountChangetype->setGeometry(QRect(150, 470, 101, 41));
    frameAccountChangetype->setStyleSheet("#frameAccountChangetype{\n"
        "	background:rgba(250,100,0,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:20px;\n"
        "	color:black;\n"
        "}\n"
        "#frameAccountChangetype:hover{\n"
        "	color:white;\n"
    "}");
    frameAccountLabelChangePassword = new QLabel(Account);
    frameAccountLabelChangePassword->setObjectName("frameAccountLabelChangePassword");
    frameAccountLabelChangePassword->setGeometry(QRect(40, 300, 321, 31));
    frameAccountLabelChangePassword->setStyleSheet("#frameAccountLabelChangePassword{\n"
        "	background:rgba(0,0,255,0.5);\n"
        "	color:white;\n"
    "}");
    frameAccountLabelChangePassword->setAlignment(Qt::AlignCenter);
    frameDetails->addTab(Account, QString());
    Profilo = new QWidget();
    Profilo->setObjectName("Profilo");
    label_2 = new QLabel(Profilo);
    label_2->setObjectName("label_2");
    label_2->setGeometry(QRect(20, 20, 81, 21));
    label_2->setStyleSheet("#label_2{\n"
        "	color:white;\n"
    "}");
    label_3 = new QLabel(Profilo);
    label_3->setObjectName("label_3");
    label_3->setGeometry(QRect(20, 70, 81, 21));
    label_3->setStyleSheet("#label_3{\n"
        "	color:white;\n"
    "}");
    label_4 = new QLabel(Profilo);
    label_4->setObjectName("label_4");
    label_4->setGeometry(QRect(20, 170, 81, 21));
    label_4->setStyleSheet("#label_4{\n"
        "	color:white;\n"
    "}");
    label_5 = new QLabel(Profilo);
    label_5->setObjectName("label_5");
    label_5->setGeometry(QRect(20, 120, 91, 21));
    label_5->setStyleSheet("#label_5{\n"
        "	color:white;\n"
    "}");
    frameProfileName = new QLineEdit(Profilo);
    frameProfileName->setObjectName("frameProfileName");
    frameProfileName->setGeometry(QRect(130, 10, 241, 41));
    frameProfileName->setStyleSheet("#frameProfileName{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;	\n"
        "	border-radius:20px;\n"
        "	border:none;\n"
    "}");
    frameProfileName->setAlignment(Qt::AlignCenter);
    frameProfileSurname = new QLineEdit(Profilo);
    frameProfileSurname->setObjectName("frameProfileSurname");
    frameProfileSurname->setGeometry(QRect(130, 60, 241, 41));
    frameProfileSurname->setStyleSheet("#frameProfileSurname{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;	\n"
        "	border-radius:20px;\n"
        "	border:none;\n"
    "}");
    frameProfileSurname->setAlignment(Qt::AlignCenter);
    frameProfileNationality = new QLineEdit(Profilo);
    frameProfileNationality->setObjectName("frameProfileNationality");
    frameProfileNationality->setGeometry(QRect(130, 160, 241, 41));
    frameProfileNationality->setStyleSheet("#frameProfileNationality{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;	\n"
        "	border-radius:20px;\n"
        "	border:none;\n"
    "}");
    frameProfileNationality->setAlignment(Qt::AlignCenter);
    frameProfileBirth = new QLineEdit(Profilo);
    frameProfileBirth->setObjectName("frameProfileBirth");
    frameProfileBirth->setGeometry(QRect(130, 110, 241, 41));
    frameProfileBirth->setStyleSheet("#frameProfileBirth{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;	\n"
        "	border-radius:20px;\n"
        "	border:none;\n"
    "}");
    frameProfileBirth->setAlignment(Qt::AlignCenter);
    frameProfileUpdate = new QPushButton(Profilo);
    frameProfileUpdate->setObjectName("frameProfileUpdate");
    frameProfileUpdate->setGeometry(QRect(270, 220, 101, 41));
    frameProfileUpdate->setStyleSheet("#frameProfileUpdate{\n"
        "	background:rgba(0,255,30,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	border:2px solid white;\n"
    "}\n"
    "#frameProfileUpdate:hover{\n"
    "	background:rgba(0,255,30,0.8);\n"
    "}");
    frameProfileUndoUpdate = new QPushButton(Profilo);
    frameProfileUndoUpdate->setObjectName("frameProfileUndoUpdate");
    frameProfileUndoUpdate->setGeometry(QRect(40, 220, 101, 41));
    frameProfileUndoUpdate->setStyleSheet("#frameProfileUndoUpdate{\n"
        "	background:rgba(255,0,30,0.5);\n"
        "	color:white;\n"
        "	border-radius:20px;\n"
        "	border:2px solid white;\n"
        "}\n"
        "#frameProfileUndoUpdate:hover{\n"
        "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameProfileAddressList = new QListWidget(Profilo);
    frameProfileAddressList->setObjectName("frameProfileAddressList");
    frameProfileAddressList->setGeometry(QRect(0, 350, 411, 211));
    frameProfileAddressList->setStyleSheet("#frameProfileAddressList{\n"
        "	color:white;\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	padding:10px;\n"
    "}");
    frameProfileNewAddress = new QLineEdit(Profilo);
    frameProfileNewAddress->setObjectName("frameProfileNewAddress");
    frameProfileNewAddress->setGeometry(QRect(10, 310, 351, 31));
    frameProfileNewAddress->setStyleSheet("#frameProfileNewAddress{\n"
        "	border-radius:15px;\n"
        "	background:rgba(0,30,30,0.6);\n"
        "	border:none;\n"
        "	color:white;\n"
        "	padding:5px;\n"
    "}");
    frameProfileLabelContact = new QLabel(Profilo);
    frameProfileLabelContact->setObjectName("frameProfileLabelContact");
    frameProfileLabelContact->setGeometry(QRect(0, 280, 411, 71));
    frameProfileLabelContact->setStyleSheet("#frameProfileLabelContact{\n"
        "	color:white;\n"
        "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameProfileLabelContact->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    frameProfileLabelContact->setMargin(7);
    frameProfileAddNewAddress = new QPushButton(Profilo);
    frameProfileAddNewAddress->setObjectName("frameProfileAddNewAddress");
    frameProfileAddNewAddress->setGeometry(QRect(372, 325, 21, 21));
    frameProfileAddNewAddress->setStyleSheet("#frameProfileAddNewAddress{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameProfileAddNewAddress:hover{\n"
        "	color:white;\n"
    "}");
    frameProfileDeleteAddress = new QPushButton(Profilo);
    frameProfileDeleteAddress->setObjectName("frameProfileDeleteAddress");
    frameProfileDeleteAddress->setGeometry(QRect(372, 302, 21, 21));
    frameProfileDeleteAddress->setStyleSheet("#frameProfileDeleteAddress{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameProfileDeleteAddress:hover{\n"
        "	color:white;\n"
    "}");
    frameDetails->addTab(Profilo, QString());
    frameProfileLabelContact->raise();
    label_2->raise();
    label_3->raise();
    label_4->raise();
    label_5->raise();
    frameProfileName->raise();
    frameProfileSurname->raise();
    frameProfileNationality->raise();
    frameProfileBirth->raise();
    frameProfileUpdate->raise();
    frameProfileUndoUpdate->raise();
    frameProfileAddressList->raise();
    frameProfileNewAddress->raise();
    frameProfileAddNewAddress->raise();
    frameProfileDeleteAddress->raise();
    Lingue = new QWidget();
    Lingue->setObjectName("Lingue");
    frameLanguageList = new QListWidget(Lingue);
    frameLanguageList->setObjectName("frameLanguageList");
    frameLanguageList->setGeometry(QRect(0, 30, 411, 321));
    frameLanguageList->setStyleSheet("#frameLanguageList{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    frameLanguageCount = new QLabel(Lingue);
    frameLanguageCount->setObjectName("frameLanguageCount");
    frameLanguageCount->setGeometry(QRect(0, 0, 411, 31));
    frameLanguageCount->setStyleSheet("#frameLanguageCount{\n"
        "	background:rgba(0,0,0,0.3);\n"
        "	color:white;\n"
    "}");
    frameLanguageCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameLanguageCount->setIndent(17);
    frameNewLanguage = new QFrame(Lingue);
    frameNewLanguage->setObjectName("frameNewLanguage");
    frameNewLanguage->setGeometry(QRect(0, 351, 421, 211));
    frameNewLanguage->setStyleSheet("#frameNewLanguage{\n"
        "	background:rgba(150,0,50,0.3);\n"
    "}");
    frameNewLanguage->setFrameShape(QFrame::StyledPanel);
    frameNewLanguage->setFrameShadow(QFrame::Raised);
    frameNewLanguageName = new QLineEdit(frameNewLanguage);
    frameNewLanguageName->setObjectName("frameNewLanguageName");
    frameNewLanguageName->setGeometry(QRect(70, 70, 291, 41));
    frameNewLanguageName->setStyleSheet("#frameNewLanguageName{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	padding:10px;\n"
        "	border:none;\n"
    "}");
    frameNewLanguageName->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelTitle = new QLabel(frameNewLanguage);
    frameNewLanguageLabelTitle->setObjectName("frameNewLanguageLabelTitle");
    frameNewLanguageLabelTitle->setGeometry(QRect(-2, -5, 411, 41));
    frameNewLanguageLabelTitle->setStyleSheet("#frameNewLanguageLabelTitle{\n"
        "	color:white;\n"
        "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewLanguageLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewLanguageWritten = new QSpinBox(frameNewLanguage);
    frameNewLanguageWritten->setObjectName("frameNewLanguageWritten");
    frameNewLanguageWritten->setGeometry(QRect(60, 130, 47, 25));
    frameNewLanguageWritten->setStyleSheet("#frameNewLanguageWritten{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
    "}");
    frameNewLanguageWritten->setAlignment(Qt::AlignCenter);
    frameNewLanguageWritten->setMinimum(0);
    frameNewLanguageWritten->setMaximum(5);
    frameNewLanguageWritten->setValue(0);
    frameNewLanguageHeard = new QSpinBox(frameNewLanguage);
    frameNewLanguageHeard->setObjectName("frameNewLanguageHeard");
    frameNewLanguageHeard->setGeometry(QRect(192, 130, 47, 25));
    frameNewLanguageHeard->setStyleSheet("#frameNewLanguageHeard{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
    "}");
    frameNewLanguageHeard->setAlignment(Qt::AlignCenter);
    frameNewLanguageHeard->setMinimum(0);
    frameNewLanguageHeard->setMaximum(5);
    frameNewLanguageHeard->setValue(0);
    frameNewLanguageSpoken = new QSpinBox(frameNewLanguage);
    frameNewLanguageSpoken->setObjectName("frameNewLanguageSpoken");
    frameNewLanguageSpoken->setGeometry(QRect(320, 130, 47, 25));
    frameNewLanguageSpoken->setStyleSheet("#frameNewLanguageSpoken{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
    "}");
    frameNewLanguageSpoken->setAlignment(Qt::AlignCenter);
    frameNewLanguageSpoken->setMaximum(5);
    frameNewLanguageLabelWritten = new QLabel(frameNewLanguage);
    frameNewLanguageLabelWritten->setObjectName("frameNewLanguageLabelWritten");
    frameNewLanguageLabelWritten->setGeometry(QRect(53, 160, 59, 16));
    frameNewLanguageLabelWritten->setStyleSheet("#frameNewLanguageLabelWritten{\n"
        "	color:white;\n"
    "}");
    frameNewLanguageLabelWritten->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelHeard = new QLabel(frameNewLanguage);
    frameNewLanguageLabelHeard->setObjectName("frameNewLanguageLabelHeard");
    frameNewLanguageLabelHeard->setGeometry(QRect(185, 160, 59, 16));
    frameNewLanguageLabelHeard->setStyleSheet(" #frameNewLanguageLabelHeard{\n"
        "	color:white;\n"
    "}");
    frameNewLanguageLabelHeard->setAlignment(Qt::AlignCenter);
    frameNewLanguageLabelSpoken = new QLabel(frameNewLanguage);
    frameNewLanguageLabelSpoken->setObjectName("frameNewLanguageLabelSpoken");
    frameNewLanguageLabelSpoken->setGeometry(QRect(315, 160, 59, 16));
    frameNewLanguageLabelSpoken->setStyleSheet("#frameNewLanguageLabelSpoken{\n"
        "	color:white;\n"
    "}");
    frameNewLanguageLabelSpoken->setAlignment(Qt::AlignCenter);
    frameLanguageUndoUpdate = new QPushButton(frameNewLanguage);
    frameLanguageUndoUpdate->setObjectName("frameLanguageUndoUpdate");
    frameLanguageUndoUpdate->setGeometry(QRect(320, 6, 71, 24));
    frameLanguageUndoUpdate->setStyleSheet("#frameLanguageUndoUpdate{\n"
        "	background:rgba(255,0,30,0.5);\n"
        "	color:white;\n"
        "	border-radius:10px;\n"
        "	border:1px solid white;\n"
        "}\n"
        "#frameLanguageUndoUpdate:hover{\n"
        "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameLanguageDelete = new QPushButton(frameNewLanguage);
    frameLanguageDelete->setObjectName("frameLanguageDelete");
    frameLanguageDelete->setGeometry(QRect(80, 8, 71, 21));
    frameLanguageDelete->setStyleSheet("#frameLanguageDelete{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameLanguageDelete:hover{\n"
        "	color:white;\n"
    "}");
    frameNewLanguageLabelName = new QLabel(frameNewLanguage);
    frameNewLanguageLabelName->setObjectName("frameNewLanguageLabelName");
    frameNewLanguageLabelName->setGeometry(QRect(180, 50, 59, 16));
    frameNewLanguageLabelName->setStyleSheet("#frameNewLanguageLabelName{\n"
        "	color:white;\n"
    "}");
    frameNewLanguageLabelName->setAlignment(Qt::AlignCenter);
    frameNewLanguageAdd = new QPushButton(frameNewLanguage);
    frameNewLanguageAdd->setObjectName("frameNewLanguageAdd");
    frameNewLanguageAdd->setGeometry(QRect(371, 7, 20, 21));
    frameNewLanguageAdd->setStyleSheet("#frameNewLanguageAdd{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "##frameNewLanguageAdd:hover{\n"
        "	color:white;\n"
    "}");
    frameLanguageUpdate = new QPushButton(frameNewLanguage);
    frameLanguageUpdate->setObjectName("frameLanguageUpdate");
    frameLanguageUpdate->setGeometry(QRect(10, 8, 61, 21));
    frameLanguageUpdate->setStyleSheet("#frameLanguageUpdate{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameLanguageUpdate:hover{\n"
        "	color:white;\n"
    "}");
    frameDetails->addTab(Lingue, QString());
    Titoli = new QWidget();
    Titoli->setObjectName("Titoli");
    frameQualificationList = new QListWidget(Titoli);
    frameQualificationList->setObjectName("frameQualificationList");
    frameQualificationList->setGeometry(QRect(0, 30, 411, 361));
    frameQualificationList->setStyleSheet("#frameQualificationList{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	padding:10px;\n"
        "}\n"
    "}");
    frameQualificationCount = new QLabel(Titoli);
    frameQualificationCount->setObjectName("frameQualificationCount");
    frameQualificationCount->setGeometry(QRect(-3, 0, 411, 31));
    frameQualificationCount->setStyleSheet("#frameQualificationCount{\n"
        "	background:rgba(0,0,0,0.3);\n"
        "	color:white;\n"
    "}");
    frameQualificationCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameQualificationCount->setIndent(17);
    frameNewQualification = new QFrame(Titoli);
    frameNewQualification->setObjectName("frameNewQualification");
    frameNewQualification->setGeometry(QRect(0, 390, 411, 171));
    frameNewQualification->setStyleSheet("#frameNewQualification{\n"
    "		background:rgba(150,0,50,0.3);\n"
    "}");
    frameNewQualification->setFrameShape(QFrame::StyledPanel);
    frameNewQualification->setFrameShadow(QFrame::Raised);
    frameQualificationDelete = new QPushButton(frameNewQualification);
    frameQualificationDelete->setObjectName("frameQualificationDelete");
    frameQualificationDelete->setGeometry(QRect(80, 10, 61, 21));
    frameQualificationDelete->setStyleSheet("#frameQualificationDelete{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameQualificationDelete:hover{\n"
        "	color:white;\n"
    "}");
    frameQualificationUpdate = new QPushButton(frameNewQualification);
    frameQualificationUpdate->setObjectName("frameQualificationUpdate");
    frameQualificationUpdate->setGeometry(QRect(10, 9, 61, 21));
    frameQualificationUpdate->setStyleSheet("#frameQualificationUpdate{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameQualificationUpdate:hover{\n"
        "	color:white;\n"
    "}");
    frameNewQualificationName = new QLineEdit(frameNewQualification);
    frameNewQualificationName->setObjectName("frameNewQualificationName");
    frameNewQualificationName->setGeometry(QRect(140, 80, 231, 41));
    frameNewQualificationName->setStyleSheet("#frameNewQualificationName{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	padding:10px;\n"
        "	border:none;\n"
    "}");
    frameNewQualificationName->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelTitle = new QLabel(frameNewQualification);
    frameNewQualificationLabelTitle->setObjectName("frameNewQualificationLabelTitle");
    frameNewQualificationLabelTitle->setGeometry(QRect(-2, 0, 411, 41));
    frameNewQualificationLabelTitle->setStyleSheet("#frameNewQualificationLabelTitle{\n"
        "	color:white;\n"
        "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewQualificationLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewQualificationYear = new QLineEdit(frameNewQualification);
    frameNewQualificationYear->setObjectName("frameNewQualificationYear");
    frameNewQualificationYear->setGeometry(QRect(30, 80, 91, 41));
    frameNewQualificationYear->setStyleSheet("#frameNewQualificationYear{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	padding:10px;\n"
        "	border:none;\n"
    "}");
    frameNewQualificationYear->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelYear = new QLabel(frameNewQualification);
    frameNewQualificationLabelYear->setObjectName("frameNewQualificationLabelYear");
    frameNewQualificationLabelYear->setGeometry(QRect(40, 60, 71, 16));
    frameNewQualificationLabelYear->setStyleSheet("#frameNewQualificationLabelYear{\n"
        "	color:white;\n"
    "}");
    frameNewQualificationLabelYear->setAlignment(Qt::AlignCenter);
    frameNewQualificationLabelName = new QLabel(frameNewQualification);
    frameNewQualificationLabelName->setObjectName("frameNewQualificationLabelName");
    frameNewQualificationLabelName->setGeometry(QRect(190, 60, 121, 16));
    frameNewQualificationLabelName->setStyleSheet("#frameNewQualificationLabelName{\n"
        "	color:white;\n"
    "}");
    frameNewQualificationLabelName->setAlignment(Qt::AlignCenter);
    frameQualificationUndoUpdate = new QPushButton(frameNewQualification);
    frameQualificationUndoUpdate->setObjectName("frameQualificationUndoUpdate");
    frameQualificationUndoUpdate->setGeometry(QRect(330, 10, 71, 24));
    frameQualificationUndoUpdate->setStyleSheet("#frameQualificationUndoUpdate{\n"
        "	background:rgba(255,0,30,0.5);\n"
        "	color:white;\n"
        "	border-radius:10px;\n"
        "	border:1px solid white;\n"
        "}\n"
        "#frameQualificationUndoUpdate:hover{\n"
        "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameNewQualificationAdd = new QPushButton(frameNewQualification);
    frameNewQualificationAdd->setObjectName("frameNewQualificationAdd");
    frameNewQualificationAdd->setGeometry(QRect(376, 11, 21, 21));
    frameNewQualificationAdd->setStyleSheet("#frameNewQualificationAdd{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNewQualificationAdd:hover{\n"
        "	color:white;\n"
    "}");
    frameNewQualificationLabelTitle->raise();
    frameQualificationDelete->raise();
    frameQualificationUpdate->raise();
    frameNewQualificationName->raise();
    frameNewQualificationYear->raise();
    frameNewQualificationLabelYear->raise();
    frameNewQualificationLabelName->raise();
    frameQualificationUndoUpdate->raise();
    frameNewQualificationAdd->raise();
    frameDetails->addTab(Titoli, QString());
    Lavori_2 = new QWidget();
    Lavori_2->setObjectName("Lavori_2");
    frameWorkCount = new QLabel(Lavori_2);
    frameWorkCount->setObjectName("frameWorkCount");
    frameWorkCount->setGeometry(QRect(0, 0, 411, 31));
    frameWorkCount->setStyleSheet("#frameWorkCount{\n"
        "	background:rgba(0,0,0,0.3);\n"
        "	color:white;\n"
    "}");
    frameWorkCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameWorkCount->setIndent(17);
    frameWorkList = new QListWidget(Lavori_2);
    frameWorkList->setObjectName("frameWorkList");
    frameWorkList->setGeometry(QRect(0, 30, 411, 251));
    frameWorkList->setStyleSheet("#frameWorkList{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    frameNewWork = new QFrame(Lavori_2);
    frameNewWork->setObjectName("frameNewWork");
    frameNewWork->setGeometry(QRect(0, 280, 411, 281));
    frameNewWork->setStyleSheet("#frameNewWork{\n"
        "		background:rgba(150,0,50,0.3);\n"
    "}");
    frameNewWork->setFrameShape(QFrame::StyledPanel);
    frameNewWork->setFrameShadow(QFrame::Raised);
    frameWorkDelete = new QPushButton(frameNewWork);
    frameWorkDelete->setObjectName("frameWorkDelete");
    frameWorkDelete->setGeometry(QRect(70, 7, 61, 21));
    frameWorkDelete->setStyleSheet("#frameWorkDelete{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameWorkDelete:hover{\n"
        "	color:white;\n"
    "}");
    frameWorkUpdate = new QPushButton(frameNewWork);
    frameWorkUpdate->setObjectName("frameWorkUpdate");
    frameWorkUpdate->setGeometry(QRect(9, 7, 51, 20));
    frameWorkUpdate->setStyleSheet("#frameWorkUpdate{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameWorkUpdate:hover{\n"
        "	color:white;\n"
    "}");
    frameNewWorkLabelTitle = new QLabel(frameNewWork);
    frameNewWorkLabelTitle->setObjectName("frameNewWorkLabelTitle");
    frameNewWorkLabelTitle->setGeometry(QRect(-2, -5, 411, 41));
    frameNewWorkLabelTitle->setStyleSheet("#frameNewWorkLabelTitle{\n"
        "	color:white;\n"
        "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewWorkLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewWorkDateFrom = new QLineEdit(frameNewWork);
    frameNewWorkDateFrom->setObjectName("frameNewWorkDateFrom");
    frameNewWorkDateFrom->setGeometry(QRect(10, 60, 151, 31));
    frameNewWorkDateFrom->setStyleSheet("#frameNewWorkDateFrom{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	border:none;\n"
    "}");
    frameNewWorkDateFrom->setAlignment(Qt::AlignCenter);
    frameNewWorkSector = new QLineEdit(frameNewWork);
    frameNewWorkSector->setObjectName("frameNewWorkSector");
    frameNewWorkSector->setGeometry(QRect(10, 170, 151, 31));
    frameNewWorkSector->setStyleSheet("#frameNewWorkSector{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	padding:5px;\n"
        "	border:none;\n"
    "}");
    frameNewWorkJob = new QLineEdit(frameNewWork);
    frameNewWorkJob->setObjectName("frameNewWorkJob");
    frameNewWorkJob->setGeometry(QRect(10, 230, 151, 31));
    frameNewWorkJob->setStyleSheet("#frameNewWorkJob{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	padding:5px;\n"
        "	border:none;\n"
    "}");
    frameNewWorkDateTo = new QLineEdit(frameNewWork);
    frameNewWorkDateTo->setObjectName("frameNewWorkDateTo");
    frameNewWorkDateTo->setGeometry(QRect(10, 110, 151, 31));
    frameNewWorkDateTo->setStyleSheet("#frameNewWorkDateTo{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	border:none;\n"
    "}");
    frameNewWorkDateTo->setAlignment(Qt::AlignCenter);
    frameWorkLabelFrom = new QLabel(frameNewWork);
    frameWorkLabelFrom->setObjectName("frameWorkLabelFrom");
    frameWorkLabelFrom->setGeometry(QRect(50, 40, 59, 16));
    frameWorkLabelFrom->setStyleSheet("#frameWorkLabelFrom{\n"
        "	color:white;\n"
    "}");
    frameWorkLabelFrom->setAlignment(Qt::AlignCenter);
    frameWorkLabelTo = new QLabel(frameNewWork);
    frameWorkLabelTo->setObjectName("frameWorkLabelTo");
    frameWorkLabelTo->setGeometry(QRect(50, 90, 59, 16));
    frameWorkLabelTo->setStyleSheet("#frameWorkLabelTo{\n"
        "	color:white;\n"
    "}");
    frameWorkLabelTo->setAlignment(Qt::AlignCenter);
    frameWorkLabelSector = new QLabel(frameNewWork);
    frameWorkLabelSector->setObjectName("frameWorkLabelSector");
    frameWorkLabelSector->setGeometry(QRect(50, 150, 59, 16));
    frameWorkLabelSector->setStyleSheet("#frameWorkLabelSector{\n"
        "	color:white;\n"
    "}");
    frameWorkLabelSector->setAlignment(Qt::AlignCenter);
    frameWorkLabelJob = new QLabel(frameNewWork);
    frameWorkLabelJob->setObjectName("frameWorkLabelJob");
    frameWorkLabelJob->setGeometry(QRect(50, 210, 59, 16));
    frameWorkLabelJob->setStyleSheet("#frameWorkLabelJob{\n"
        "	color:white;\n"
    "}");
    frameWorkLabelJob->setAlignment(Qt::AlignCenter);
    frameNewWorkListSkill = new QListWidget(frameNewWork);
    frameNewWorkListSkill->setObjectName("frameNewWorkListSkill");
    frameNewWorkListSkill->setGeometry(QRect(180, 110, 211, 141));
    frameNewWorkListSkill->setStyleSheet("#frameNewWorkListSkill{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    frameWorkLabelSkill = new QLabel(frameNewWork);
    frameWorkLabelSkill->setObjectName("frameWorkLabelSkill");
    frameWorkLabelSkill->setGeometry(QRect(160, 50, 241, 31));
    frameWorkLabelSkill->setStyleSheet("#frameWorkLabelSkill{\n"
        "	background:rgba(255,0,0,0,3);\n"
        "	color:white;\n"
    "}");
    frameWorkLabelSkill->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    frameNewWorkNewSkill = new QLineEdit(frameNewWork);
    frameNewWorkNewSkill->setObjectName("frameNewWorkNewSkill");
    frameNewWorkNewSkill->setGeometry(QRect(180, 70, 171, 31));
    frameNewWorkNewSkill->setStyleSheet("#frameNewWorkNewSkill{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	padding:5px;\n"
        "	border:none;\n"
    "}");
    frameNewWorkAddSkill = new QPushButton(frameNewWork);
    frameNewWorkAddSkill->setObjectName("frameNewWorkAddSkill");
    frameNewWorkAddSkill->setGeometry(QRect(356, 74, 21, 21));
    frameNewWorkAddSkill->setStyleSheet("#frameNewWorkAddSkill{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNewWorkAddSkill:hover{\n"
        "	color:white;\n"
    "}");
    frameWorkUndoUpdate = new QPushButton(frameNewWork);
    frameWorkUndoUpdate->setObjectName("frameWorkUndoUpdate");
    frameWorkUndoUpdate->setGeometry(QRect(330, 6, 71, 24));
    frameWorkUndoUpdate->setStyleSheet("#frameWorkUndoUpdate{\n"
        "	background:rgba(255,0,30,0.5);\n"
        "	color:white;\n"
        "	border-radius:10px;\n"
        "	border:1px solid white;\n"
        "}\n"
        "#frameWorkUndoUpdate:hover{\n"
        "	background:rgba(255,0,30,0.8);\n"
    "}");
    frameNewWorkDeleteSkill = new QPushButton(frameNewWork);
    frameNewWorkDeleteSkill->setObjectName("frameNewWorkDeleteSkill");
    frameNewWorkDeleteSkill->setGeometry(QRect(381, 74, 21, 21));
    frameNewWorkDeleteSkill->setStyleSheet("#frameNewWorkDeleteSkill{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNewWorkDeleteSkill:hover{\n"
        "	color:white;\n"
    "}");
    frameNewWorkLabelTitle->raise();
    frameWorkDelete->raise();
    frameWorkUpdate->raise();
    frameNewWorkDateFrom->raise();
    frameNewWorkSector->raise();
    frameNewWorkJob->raise();
    frameNewWorkDateTo->raise();
    frameWorkLabelFrom->raise();
    frameWorkLabelTo->raise();
    frameWorkLabelSector->raise();
    frameWorkLabelJob->raise();
    frameNewWorkListSkill->raise();
    frameWorkLabelSkill->raise();
    frameNewWorkNewSkill->raise();
    frameNewWorkAddSkill->raise();
    frameWorkUndoUpdate->raise();
    frameNewWorkDeleteSkill->raise();
    frameNewWorkAdd = new QPushButton(Lavori_2);
    frameNewWorkAdd->setObjectName("frameNewWorkAdd");
    frameNewWorkAdd->setGeometry(QRect(377, 289, 21, 20));
    frameNewWorkAdd->setStyleSheet("#frameNewWorkAdd{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNewWorkAdd:hover{\n"
        "	color:white;\n"
    "}");
    frameDetails->addTab(Lavori_2, QString());
    Rete = new QWidget();
    Rete->setObjectName("Rete");
    frameNetList = new QListWidget(Rete);
    frameNetList->setObjectName("frameNetList");
    frameNetList->setGeometry(QRect(0, 31, 411, 281));
    frameNetList->setStyleSheet("#frameNetList{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    frameNetCount = new QLabel(Rete);
    frameNetCount->setObjectName("frameNetCount");
    frameNetCount->setGeometry(QRect(-2, 0, 411, 31));
    frameNetCount->setStyleSheet("#frameNetCount{\n"
        "	background:rgba(0,0,0,0.3);\n"
        "	color:white;\n"
    "}");
    frameNetCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameNetCount->setIndent(17);
    frameNetDelete = new QPushButton(Rete);
    frameNetDelete->setObjectName("frameNetDelete");
    frameNetDelete->setGeometry(QRect(10, 3, 131, 21));
    frameNetDelete->setStyleSheet("#frameNetDelete{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNetDelete:hover{\n"
        "	color:white;\n"
    "}");
    frameNewNet = new QFrame(Rete);
    frameNewNet->setObjectName("frameNewNet");
    frameNewNet->setGeometry(QRect(0, 310, 411, 251));
    frameNewNet->setStyleSheet("#frameNewNet{\n"
        "		background:rgba(150,0,50,0.3);\n"
    "}");
    frameNewNet->setFrameShape(QFrame::StyledPanel);
    frameNewNet->setFrameShadow(QFrame::Raised);
    frameNewNetAdd = new QPushButton(frameNewNet);
    frameNewNetAdd->setObjectName("frameNewNetAdd");
    frameNewNetAdd->setGeometry(QRect(380, 7, 21, 21));
    frameNewNetAdd->setStyleSheet("#frameNewNetAdd{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:10px;\n"
        "	color:black;\n"
        "}\n"
        "#frameNewNetAdd:hover{\n"
        "	color:white;\n"
    "}");
    frameNewNetLabelTitle = new QLabel(frameNewNet);
    frameNewNetLabelTitle->setObjectName("frameNewNetLabelTitle");
    frameNewNetLabelTitle->setGeometry(QRect(-2, -5, 411, 41));
    frameNewNetLabelTitle->setStyleSheet("#frameNewNetLabelTitle{\n"
        "	color:white;\n"
        "	background:rgba(150,0,50,0.5);\n"
    "}");
    frameNewNetLabelTitle->setAlignment(Qt::AlignCenter);
    frameNewNetList = new QListWidget(frameNewNet);
    frameNewNetList->setObjectName("frameNewNetList");
    frameNewNetList->setGeometry(QRect(8, 42, 391, 191));
    frameNewNetList->setStyleSheet("#frameNewNetList{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
        "	padding:10px;\n"
    "}");
    frameNewNetLabelTitle->raise();
    frameNewNetAdd->raise();
    frameNewNetList->raise();
    frameDetails->addTab(Rete, QString());
    frameUsersAdd = new QPushButton(this);
    frameUsersAdd->setObjectName("frameUsersAdd");
    frameUsersAdd->setGeometry(QRect(16, 173, 121, 31));
    QFont font;
    font.setPointSize(14);
    frameUsersAdd->setFont(font);
    frameUsersAdd->setStyleSheet("#frameUsersAdd{\n"
        "	background:rgba(0,255,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:15px;\n"
        "	color:black;\n"
        "}\n"
        "#frameUsersAdd:hover{\n"
        "	color:white;\n"
    "}");
    frameUsersAdd->setFlat(true);
    frameUsersFilterUsername = new QLineEdit(this);
    frameUsersFilterUsername->setObjectName("frameUsersFilterUsername");
    frameUsersFilterUsername->setGeometry(QRect(70, 60, 261, 21));
    frameUsersFilterUsername->setStyleSheet("#frameUsersFilterUsername{\n"
        "	border-radius:10px;\n"
        "	background:rgba(0,0,0,0.3);\n"
        "	color:white;\n"
    "}");
    frameUsersFilterUsername->setAlignment(Qt::AlignCenter);
    frameUsersLabelFilterUsername = new QLabel(this);
    frameUsersLabelFilterUsername->setObjectName("frameUsersLabelFilterUsername");
    frameUsersLabelFilterUsername->setGeometry(QRect(0, 50, 351, 41));
    frameUsersLabelFilterUsername->setStyleSheet("#frameUsersLabelFilterUsername{\n"
        "	background:rgba(0,0,0,0.4);\n"
        "	color:white;\n"
    "}");
    frameUsersLabelFilterUsername->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    frameUsersLabelFilterUsername->setIndent(17);
    frameUsersCount = new QLabel(this);
    frameUsersCount->setObjectName("frameUsersCount");
    frameUsersCount->setGeometry(QRect(-1, 543, 351, 42));
    frameUsersCount->setStyleSheet("#frameUsersCount{\n"
        "	background:rgba(0,0,0,0.4);\n"
        "	color:white;\n"
    "}");
    frameUsersCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    frameUsersCount->setIndent(17);
    frameUsersDelete = new QPushButton(this);
    frameUsersDelete->setObjectName("frameUsersDelete");
    frameUsersDelete->setGeometry(QRect(190, 174, 141, 31));
    frameUsersDelete->setStyleSheet("#frameUsersDelete{\n"
        "	background:rgba(255,0,30,0.7);\n"
        "	border:1px solid rgba(0,0.0.0.2);	\n"
        "	border-radius:15px;\n"
        "	color:black;\n"
        "}\n"
        "#frameUsersDelete:hover{\n"
        "	color:white;\n"
    "}");
    frameUsersDelete->setFlat(false);
    frameUsersLabelFilterYear = new QLabel(this);
    frameUsersLabelFilterYear->setObjectName("frameUsersLabelFilterYear");
    frameUsersLabelFilterYear->setGeometry(QRect(0, 91, 351, 121));
    frameUsersLabelFilterYear->setStyleSheet("#frameUsersLabelFilterYear{\n"
        "	background:rgba(200,0,0,0.4);\n"
        "	color:white;\n"
    "}");
    frameUsersLabelFilterYear->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    frameUsersLabelFilterYear->setMargin(9);
    frameUsersLabelFilterYear->setIndent(10);
    frameUsersFilterYear = new QSpinBox(this);
    frameUsersFilterYear->setObjectName("frameUsersFilterYear");
    frameUsersFilterYear->setGeometry(QRect(120, 104, 161, 25));
    frameUsersFilterYear->setStyleSheet("#filterUserYear{\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	color:white;\n"
        "	border-radius:5px;\n"
    "}");
    frameUsersFilterYear->setAlignment(Qt::AlignCenter);
    frameUsersFilterYear->setMinimum(1900);
    frameUsersFilterYear->setMaximum(2020);
    frameUsersFilterSector = new QComboBox(this);
    frameUsersFilterSector->setObjectName("frameUsersFilterSector");
    frameUsersFilterSector->setGeometry(QRect(30, 140, 281, 24));
    frameUsersFilterSector->setStyleSheet("#filterUserSector{\n"
        "	color:black;\n"
        "	border-radius:10px;\n"
        "	border:1px solid white;\n"
    "}");
    frameUsersLabelFilterYear->raise();
    frameUsersLabelTitle->raise();
    frameUsersCount->raise();
    frameUsersLabelFilterUsername->raise();
    frameUsersList->raise();
    exit->raise();
    frameDetails->raise();
    frameUsersFilterUsername->raise();
    frameUsersAdd->raise();
    frameUsersDelete->raise();
    frameUsersFilterYear->raise();
    frameUsersFilterSector->raise();
    QWidget::setTabOrder(frameUsersFilterUsername, frameUsersFilterYear);
    QWidget::setTabOrder(frameUsersFilterYear, frameUsersFilterSector);
    QWidget::setTabOrder(frameUsersFilterSector, frameUsersAdd);
    QWidget::setTabOrder(frameUsersAdd, frameUsersList);
    QWidget::setTabOrder(frameUsersList, frameUsersDelete);
    QWidget::setTabOrder(frameUsersDelete, frameDetails);
    QWidget::setTabOrder(frameDetails, frameAccountUsername);
    QWidget::setTabOrder(frameAccountUsername, frameAccountNewPassword);
    QWidget::setTabOrder(frameAccountNewPassword, frameAccountNewPasswordConfirm);
    QWidget::setTabOrder(frameAccountNewPasswordConfirm, frameAccountUpdate);
    QWidget::setTabOrder(frameAccountUpdate, frameAccountTypeBasic);
    QWidget::setTabOrder(frameAccountTypeBasic, frameAccountTypeBusiness);
    QWidget::setTabOrder(frameAccountTypeBusiness, frameAccountTypeExecutive);
    QWidget::setTabOrder(frameAccountTypeExecutive, frameAccountChangetype);
    QWidget::setTabOrder(frameAccountChangetype, frameProfileName);
    QWidget::setTabOrder(frameProfileName, frameProfileSurname);
    QWidget::setTabOrder(frameProfileSurname, frameProfileBirth);
    QWidget::setTabOrder(frameProfileBirth, frameProfileNationality);
    QWidget::setTabOrder(frameProfileNationality, frameProfileUndoUpdate);
    QWidget::setTabOrder(frameProfileUndoUpdate, frameProfileUpdate);
    QWidget::setTabOrder(frameProfileUpdate, frameProfileNewAddress);
    QWidget::setTabOrder(frameProfileNewAddress, frameProfileAddNewAddress);
    QWidget::setTabOrder(frameProfileAddNewAddress, frameProfileAddressList);
    QWidget::setTabOrder(frameProfileAddressList, frameLanguageList);
    QWidget::setTabOrder(frameLanguageList, frameQualificationList);
    QWidget::setTabOrder(frameQualificationList, frameWorkList);
    QWidget::setTabOrder(frameWorkList, frameNetList);
    QWidget::setTabOrder(frameNetList, frameNetDelete);
    QWidget::setTabOrder(frameNetDelete, frameWorkDelete);
    QWidget::setTabOrder(frameWorkDelete, frameNewWorkDateFrom);
    QWidget::setTabOrder(frameNewWorkDateFrom, frameNewWorkDateTo);
    QWidget::setTabOrder(frameNewWorkDateTo, frameNewWorkSector);
    QWidget::setTabOrder(frameNewWorkSector, frameNewWorkJob);
    QWidget::setTabOrder(frameNewWorkJob, frameNewWorkNewSkill);
    QWidget::setTabOrder(frameNewWorkNewSkill, frameNewWorkAddSkill);
    QWidget::setTabOrder(frameNewWorkAddSkill, frameNewWorkListSkill);
    QWidget::setTabOrder(frameNewWorkListSkill, frameWorkUndoUpdate);
    QWidget::setTabOrder(frameWorkUndoUpdate, frameWorkUpdate);
    QWidget::setTabOrder(frameWorkUpdate, frameProfileDeleteAddress);
    QWidget::setTabOrder(frameProfileDeleteAddress, frameQualificationDelete);
    QWidget::setTabOrder(frameQualificationDelete, frameNewQualificationName);
    QWidget::setTabOrder(frameNewQualificationName, frameNewQualificationYear);
    QWidget::setTabOrder(frameNewQualificationYear, frameQualificationUndoUpdate);
    QWidget::setTabOrder(frameQualificationUndoUpdate, frameQualificationUpdate);
    QWidget::setTabOrder(frameQualificationUpdate, frameLanguageDelete);
    QWidget::setTabOrder(frameLanguageDelete, frameNewLanguageName);
    QWidget::setTabOrder(frameNewLanguageName, frameNewLanguageWritten);
    QWidget::setTabOrder(frameNewLanguageWritten, frameNewLanguageHeard);
    QWidget::setTabOrder(frameNewLanguageHeard, frameNewLanguageSpoken);
    QWidget::setTabOrder(frameNewLanguageSpoken, frameLanguageUndoUpdate);
    QWidget::setTabOrder(frameLanguageUndoUpdate, frameNewLanguageAdd);
    QWidget::setTabOrder(frameNewLanguageAdd, frameNewNetList);
    QWidget::setTabOrder(frameNewNetList, frameNewNetAdd);
    QWidget::setTabOrder(frameNewNetAdd, exit);
    frameDetails->setCurrentIndex(0);
    setWindowTitle(QString("Form"));
    exit->setText(QString("Esci"));
    frameUsersLabelTitle->setText(QString("UTENTI"));
    frameUsersLabelFilterUsername->setText("Cerca");
    label_6->setText(QString("Username"));
    label_7->setText(QString("Cambia credenziali d'accesso"));
    frameAccountUpdate->setText(QString("Cambia password"));
    labelPassword->setText(QString("Nuova"));
    label_11->setText(QString("Conferma "));
    frameAccountTypeBasic->setText(QString("Basic"));
    frameAccountTypeExecutive->setText(QString("Executive"));
    frameAccountTypeBusiness->setText(QString("Business"));
    frameAccountChangetype->setText(QString("Cambia tipo"));
    frameAccountLabelChangePassword->setText(QString());
    frameDetails->setTabText(frameDetails->indexOf(Account), QString("Account"));
    label_2->setText(QString("Nome"));
    label_3->setText(QString("Cognome"));
    label_4->setText(QString("Nazionalit\303\240"));
    label_5->setText(QString("Anno nascita"));
    frameProfileUpdate->setText(QString("Aggiorna"));
    frameProfileUndoUpdate->setText(QString("Ripristina"));
    frameProfileLabelContact->setText(QString("Contatti personali"));
    frameProfileAddNewAddress->setText(QString("+"));
    frameProfileDeleteAddress->setText(QString("-"));
    frameDetails->setTabText(frameDetails->indexOf(Profilo), QString("Profilo"));
    frameNewLanguageLabelTitle->setText(QString("LINGUA"));
    frameNewLanguageLabelWritten->setText(QString("Scritto"));
    frameNewLanguageLabelHeard->setText(QString("Ascolto"));
    frameNewLanguageLabelSpoken->setText(QString("Parlato"));
    frameLanguageUndoUpdate->setText(QString("Ripristina"));
    frameLanguageDelete->setText(QString("Elimina"));
    frameNewLanguageLabelName->setText(QString("Nome"));
    frameNewLanguageAdd->setText(QString("+"));
    frameLanguageUpdate->setText(QString("Salva"));
    frameDetails->setTabText(frameDetails->indexOf(Lingue), QString("Lingue"));
    frameQualificationCount->setText(QString());
    frameQualificationDelete->setText(QString("Elimina"));
    frameQualificationUpdate->setText(QString("Salva"));
    frameNewQualificationLabelTitle->setText(QString("TITOLO"));
    frameNewQualificationLabelYear->setText(QString("Anno"));
    frameNewQualificationLabelName->setText(QString("Titolo"));
    frameQualificationUndoUpdate->setText(QString("Ripristina"));
    frameNewQualificationAdd->setText(QString("+"));
    frameDetails->setTabText(frameDetails->indexOf(Titoli), QString("Titoli"));
    frameWorkDelete->setText(QString("Elimina"));
    frameWorkUpdate->setText(QString("Salva"));
    frameNewWorkLabelTitle->setText(QString("LAVORO"));
    frameWorkLabelFrom->setText(QString("Da"));
    frameWorkLabelTo->setText(QString("A"));
    frameWorkLabelSector->setText(QString("Settore"));
    frameWorkLabelJob->setText(QString("Mansione"));
    frameWorkLabelSkill->setText(QString("Competenza"));
    frameNewWorkAddSkill->setText(QString("+"));
    frameWorkUndoUpdate->setText(QString("Ripristina"));
    frameNewWorkDeleteSkill->setText(QString("-"));
    frameNewWorkAdd->setText(QString("+"));
    frameDetails->setTabText(frameDetails->indexOf(Lavori_2), QString("Lavori"));
    frameNetDelete->setText(QString("Elimina selezionato"));
    frameNewNetAdd->setText(QString("+"));
    frameNewNetLabelTitle->setText(QString("AGGIUNGI CONTATTI"));
    frameDetails->setTabText(frameDetails->indexOf(Rete), QString("Rete"));
    frameUsersDelete->setText(QString("Elimina selezionato"));
    frameUsersAdd->setText("Aggiungi utente");
    frameUsersLabelFilterYear->setText(QString("Nato dopo il"));
    QMetaObject::connectSlotsByName(this);
    frameDetails->hide();
    frameAccountLabelChangePassword->hide();
}
