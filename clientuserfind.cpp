#include "clientuserfind.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHODS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClientUserFind::ClientUserFind(Db * link, Linkediner * u, QWidget *parent) :
    db(link),
    actualUser(u),
    QWidget(parent)
{
    setup();
    finderAdd->hide();
    connect(this,SIGNAL(s_exit()),this->parent(),SLOT(exitGuiFinder()));
    connect(this->parent(),SIGNAL(s_find(QString)),this,SLOT(find(QString)));
}
ClientUserFind::~ClientUserFind()
{
}
void ClientUserFind::showResult(Linkediner * u)
{
    finderDetailsContact->clear();
    finderDetailsContact->clear();
    finderDetailsWork->clear();
    finderDetailsQualification->clear();
    finderDetailsLanguage->clear();
    finderDetailsPeople->clear();
    if(!u)
    {
        finderBasicMessage->setText("UTENTE NON TROVATO");
        finderBasicMessage->setStyleSheet(finderBasicMessage->styleSheet().replace("0,255,0,0.5","255,0,0,0.5"));
        finderAdd->hide();
    } else {
        finderBasicMessage->setText("UTENTE PRESENTE");
        finderBasicMessage->setStyleSheet(finderBasicMessage->styleSheet().replace("255,0,0,0.5","0,255,0,0.5"));
        if(u->getWorks()){
            //carico i lavori
            list<Works*>::iterator iWork = u->getWorks()->begin();
            for( ; iWork != u->getWorks()->end(); ++iWork)
            {
                QString aux = QString::fromStdString((*iWork)->getJob()) + " (" +
                        QString::number((*iWork)->getStart().year()) + " - " +
                        QString::number((*iWork)->getEnd().year()) + " )\n " +
                        QString::fromStdString((*iWork)->getSector()) + "\nCompetenze principali:\n";
                list<string>::iterator iSkill = (*iWork)->getSkills()->begin();
                QString auxS;
                for( ; iSkill != (*iWork)->getSkills()->end(); ++iSkill)
                    auxS += " - " + QString::fromStdString((*iSkill)) + " \n";
                aux.append(auxS);
                QListWidgetItem * itWork = new QListWidgetItem(QPixmap("://resource/works.png"),aux,finderDetailsWork);
            }
        }
        if(u->getLanguages())
        {
            // carico lingue
            list<Languages*>::iterator iLang = u->getLanguages()->begin();
            for( ; iLang != u->getLanguages()->end(); ++iLang)
            {
                QString aux = QString::fromStdString((*iLang)->getName()) + "\n\tParlato: " +
                        QString::number((*iLang)->getSpoken()) + " Scritto: " +
                        QString::number((*iLang)->getWritten()) + " Ascoltato: " +
                        QString::number((*iLang)->getHeard());
                QListWidgetItem * itLang = new QListWidgetItem(QPixmap("://resource/language.png"),aux,finderDetailsLanguage);
            }
        }
        if(u->getQualifications())
        {
            //carico titoli
            list<Qualifications*>::iterator iQualif = u->getQualifications()->begin();
            for( ; iQualif != u->getQualifications()->end(); ++iQualif)
            {
                QString aux = "(" + (*iQualif)->getYear().toString("yyyy") + ") " + QString::fromStdString((*iQualif)->getName());
                QListWidgetItem * itQualif = new QListWidgetItem(QPixmap("://resource/qualifications.png"),aux,finderDetailsQualification);
            }
        }
        if(u->getProfile()->getContacts())
        {
            //carico contatti personali
            list<string>::iterator iContact = u->getProfile()->getContacts()->begin();
            for( ; iContact != u->getProfile()->getContacts()->end(); ++iContact)
                QListWidgetItem * itContact = new QListWidgetItem(QPixmap("://resource/contact.png"),QString::fromStdString(*iContact),finderDetailsContact);

            list<Linkediner*>::iterator iNet = u->getNet()->begin();
            for( ; iNet != u->getNet()->end(); ++iNet)
                QListWidgetItem * itNet = new QListWidgetItem(QPixmap("://resource/net.jpeg"),QString::fromStdString((*iNet)->getEmail()),finderDetailsPeople);
        }
        bool justFriend = false;
        list<Linkediner*>::iterator i = actualUser->getNet()->begin();
        for( ; i != actualUser->getNet()->end(); ++i)
        {
            if( (*i)->getEmail() == u->getEmail() )
            {
                justFriend = true;
                break;
            }
        }
        if(!justFriend) finderAdd->show();
        else finderAdd->hide();
        delete u;
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC SLOTS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUserFind::find(QString u="none")
{
    if(typeid(*(this->sender())) == typeid(ClientUser))
    {
        finderPeople->setText(u);
    }
    if(finderPeople->text().toStdString() == actualUser->getEmail())
        finderAdd->hide();
    else {
        ClientUser * guiUser = dynamic_cast<ClientUser*>(this->parent());
        if(guiUser->validate(3,0,finderPeople,0))
        {
            showResult(actualUser->find(db,finderPeople->text().toStdString()));
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE SLOTS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUserFind::on_finderBack_clicked()
{
    emit s_exit();
}
void ClientUserFind::on_finderFind_clicked()
{
    find();
}
void ClientUserFind::on_finderAdd_clicked()
{
    actualUser->addContact(db->getUserByEmail(finderPeople->text().toStdString()));
    finderAdd->hide();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE METHODS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientUserFind::setup()
{
    setObjectName(QString("Finder"));
    finderPeople = new QLineEdit(this);
    finderFind = new QPushButton(this);
    finderDetails = new QTabWidget(this);
    tab = new QWidget();
    tab_2 = new QWidget();
    tab_3 = new QWidget();
    tab_5 = new QWidget();
    tab_4 = new QWidget();
    finderDetailsWork = new QListWidget(tab);
    finderDetailsQualification = new QListWidget(tab_2);
    finderDetailsLanguage = new QListWidget(tab_3);
    finderDetailsContact = new QListWidget(tab_5);
    finderDetailsPeople = new QListWidget(tab_4);
    finderBack = new QPushButton(this);
    finderAdd = new QPushButton(this);
    finderBasicMessage = new QLabel(this);
    finderPeople->setObjectName(QString("finderPeople"));
    finderFind->setObjectName(QString("finderFind"));
    finderDetails->setObjectName(QString("finderDetails"));
    finderDetailsWork->setObjectName(QString("finderDetailsWork"));
    finderDetailsQualification->setObjectName(QString("finderDetailsQualification"));
    tab->setObjectName(QString("tab"));
    tab_2->setObjectName(QString("tab_2"));
    tab_3->setObjectName(QString("tab_3"));
    tab_4->setObjectName(QString("tab_4"));
    tab_5->setObjectName(QString("tab_5"));
    finderBasicMessage->setObjectName(QString("finderBasicMessage"));
    finderAdd->setObjectName(QString("finderAdd"));
    finderBack->setObjectName(QString("finderBack"));
    finderDetailsLanguage->setObjectName(QString("finderDetailsLanguage"));
    finderDetailsPeople->setObjectName(QString("finderDetailsPeople"));
    finderDetailsContact->setObjectName(QString("finderDetailsContact"));
    finderDetails->setGeometry(QRect(10, 80, 371, 201));
    finderDetailsWork->setGeometry(QRect(0, 0, 361, 171));
    finderDetailsPeople->setGeometry(QRect(0, 0, 361, 171));
    finderDetailsContact->setGeometry(QRect(0, 0, 361, 171));
    finderDetailsLanguage->setGeometry(QRect(0, 0, 361, 171));
    finderDetailsQualification->setGeometry(QRect(0, 0, 361, 171));
    finderPeople->setGeometry(QRect(30, 10, 261, 31));
    finderFind->setGeometry(QRect(290, 10, 81, 31));
    finderBasicMessage->setGeometry(QRect(41, 41, 321, 31));
    finderBack->setGeometry(QRect(0, 299, 71, 31));
    finderAdd->setGeometry(QRect(250, 280, 131, 31));
    finderDetails->addTab(tab, QString());
    finderDetails->addTab(tab_2, QString());
    finderDetails->addTab(tab_3, QString());
    finderDetails->addTab(tab_4, QString());
    finderDetails->addTab(tab_5, QString());
    resize(400, 334);
    setStyleSheet("#Finder{\n"
    "	background:#006699;\n"
    "}");
    finderPeople->setStyleSheet("#finderPeople{\n"
    "	background:rgba(0,0,0,0.5);\n"
    "	color:white;\n"
    "	padding:5px;\n"
    "	border-top-left-radius:15px;\n"
    "	border-bottom-left-radius:15px;\n"
    "	border:none;\n"
    "}");
    finderFind->setStyleSheet("#finderFind{\n"
    "	background:rgba(0,0,0,0.9);\n"
    "	border-top-right-radius:15px;\n"
    "	border-bottom-right-radius:15px;\n"
    "	border:1px solid white;\n"
    "	color:white;\n"
    "}\n"
    "#finderFind:hover{\n"
    "	color:orange;\n"
    "	border:1px solid orange;\n"
    "}");
    finderBack->setStyleSheet("#finderBack{\n"
    "	background:rgba(0,0,0,0.9);\n"
    "	border-top-right-radius:15px;\n"
    "	border-bottom-right-radius:15px;\n"
    "	border:1px solid white;\n"
    "	color:white;\n"
    "}\n"
    "#finderBack:hover{\n"
    "	color:orange;\n"
    "	border:1px solid orange;\n"
    "}");
    finderAdd->setStyleSheet("#finderAdd{\n"
    "	background:rgba(0,255,0,0.5);\n"
    "	border-bottom-left-radius:15px;\n"
    "	border-bottom-right-radius:15px;\n"
    "	border:1px solid white;\n"
    "	color:white;\n"
    "}\n"
    "#finderAdd:hover{\n"
    "	color:orange;\n"
    "	border:1px solid orange;\n"
    "}");
    finderBasicMessage->setStyleSheet("#finderBasicMessage{\n"
    "	background:rgba(255,0,0,0.5);\n"
    "	color:white;\n"
    "	border-bottom-left-radius:10px;\n"
    "	border-bottom-right-radius:10px;\n"
    "}");
    finderBasicMessage->setAlignment(Qt::AlignCenter);
    setWindowTitle("Form");
    finderFind->setText("Cerca");
    finderDetails->setTabText(finderDetails->indexOf(tab), "Lavori");
    finderDetails->setTabText(finderDetails->indexOf(tab_2), "Qualifiche");
    finderDetails->setTabText(finderDetails->indexOf(tab_3), "Lingue");
    finderDetails->setTabText(finderDetails->indexOf(tab_5), "Recapiti");
    finderDetails->setTabText(finderDetails->indexOf(tab_4), "Rete");
    finderBack->setText("Indietro");
    finderAdd->setText("Aggiungi ai contatti");
    finderBasicMessage->setText("CONTATTO");
    QMetaObject::connectSlotsByName(this);
}

