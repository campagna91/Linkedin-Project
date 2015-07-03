#include "clientadminnewuser.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClientAdminNewUser::ClientAdminNewUser(Db * link, QWidget *parent) :
    db(link),
    QDialog(parent)
{
    setup();
    dialogNewUserErrPassword->hide();
    dialogNewUserErrUsername->hide();
    dialogNewUserTypeBasic->setChecked(true);
    validate = &(dynamic_cast<ClientStart*>(parent->parent()))->isValid;
    cout<<"CREATE CLIENTADMINNEWUSER"<<endl;
}
ClientAdminNewUser::~ClientAdminNewUser()
{
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE SLOTS
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdminNewUser::on_dialogNewUserCreate_clicked()
{
    if(dialogNewUserPassword->text() != "" && dialogNewUserPassword->text() == dialogNewUserPasswordConfirm->text())
    {
        int type = 0;
        if(dialogNewUserUsername->text() != "" && validate(3,0,dialogNewUserUsername,0) && !db->getUserByEmail(dialogNewUserUsername->text().toStdString()))
        {            
            string tmp_image = "";
            //"Users/champ/Dropbox/Uni/p2/qt/Linkedin/LInkedin/resource/default.png";
            if(dialogNewUserTypeBasic->isChecked())
                db->addUser( new LinkedinerBasic(dialogNewUserUsername->text().toStdString(),
                                                 dialogNewUserPassword->text().toStdString(),
                                                 tmp_image,
                                                 Profile(),
                                                 list<Works*>(),
                                                 list<Languages*>(),
                                                 list<Linkediner*>(),
                                                 list<Qualifications*>()
                                                 ));
            else if(dialogNewUserTypeBusiness->isChecked())
                db->addUser( new LinkedinerBusiness(dialogNewUserUsername->text().toStdString(),
                                                     dialogNewUserPassword->text().toStdString(),
                                                     tmp_image,
                                                    Profile(),
                                                    list<Works*>(),
                                                    list<Languages*>(),
                                                    list<Linkediner*>(),
                                                    list<Qualifications*>()
                                                    ));
            else
                db->addUser( new LinkedinerExecutive(dialogNewUserUsername->text().toStdString(),
                                                     dialogNewUserPassword->text().toStdString(),
                                                     tmp_image,
                                                     Profile(),
                                                     list<Works*>(),
                                                     list<Languages*>(),
                                                     list<Linkediner*>(),
                                                     list<Qualifications*>()
                                                     ));
            dialogNewUserUsername->setText("");
            dialogNewUserPassword->setText("");
            dialogNewUserPasswordConfirm->setText("");
            dialogNewUserErrPassword->hide();
            dialogNewUserErrUsername->hide();
        } else dialogNewUserErrUsername->show();
    } else dialogNewUserErrPassword->show();
}
void ClientAdminNewUser::on_dialogNewUserBack_clicked()
{
    emit s_exit();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClientAdminNewUser::setup(){
    setObjectName("DialogNewUser");
    resize(400, 441);
    setStyleSheet("#DialogNewUser{\n"
        "	background:#006699;\n"
    "}");
    dialogNewUserPassword = new QLineEdit(this);
    dialogNewUserPassword->setObjectName("dialogNewUserPassword");
    dialogNewUserPassword->setGeometry(QRect(130, 260, 221, 31));
    dialogNewUserPassword->setStyleSheet("#dialogNewUserPassword{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
    "}");
    dialogNewUserPassword->setAlignment(Qt::AlignCenter);
    dialogNewUserPasswordConfirm = new QLineEdit(this);
    dialogNewUserPasswordConfirm->setObjectName("dialogNewUserPasswordConfirm");
    dialogNewUserPasswordConfirm->setGeometry(QRect(130, 300, 221, 31));
    dialogNewUserPasswordConfirm->setStyleSheet("#dialogNewUserPasswordConfirm{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
    "}");
    dialogNewUserPasswordConfirm->setAlignment(Qt::AlignCenter);
    dialogNewUserUsername = new QLineEdit(this);
    dialogNewUserUsername->setObjectName("dialogNewUserUsername");
    dialogNewUserUsername->setGeometry(QRect(60, 70, 301, 31));
    dialogNewUserUsername->setStyleSheet("#dialogNewUserUsername{	\n"
        "	background:rgba(0,0,0,0.5);\n"
        "	border-radius:15px;\n"
        "	color:white;\n"
    "}");
    dialogNewUserUsername->setAlignment(Qt::AlignCenter);
    dialogNewUserLabelPassword_2 = new QLabel(this);
    dialogNewUserLabelPassword_2->setObjectName("dialogNewUserLabelPassword_2");
    dialogNewUserLabelPassword_2->setGeometry(QRect(50, 295, 81, 21));
    dialogNewUserLabelPassword_2->setStyleSheet("#dialogNewUserLabelPassword_2{\n"
        "	color:white; \n"
    "}");
    dialogNewUserLabelPassword = new QLabel(this);
    dialogNewUserLabelPassword->setObjectName("dialogNewUserLabelPassword");
    dialogNewUserLabelPassword->setGeometry(QRect(50, 260, 81, 21));
    dialogNewUserLabelPassword->setStyleSheet("#dialogNewUserLabelPassword{\n"
        "	color:white; \n"
    "}");
    dialogNewUserLabelUsername = new QLabel(this);
    dialogNewUserLabelUsername->setObjectName("dialogNewUserLabelUsername");
    dialogNewUserLabelUsername->setGeometry(QRect(170, 50, 81, 21));
    dialogNewUserLabelUsername->setStyleSheet("#dialogNewUserLabelUsername{\n"
        "	color:white; \n"
    "}");
    dialogNewUserLabelTitle = new QLabel(this);
    dialogNewUserLabelTitle->setObjectName("dialogNewUserLabelTitle");
    dialogNewUserLabelTitle->setGeometry(QRect(-10, 0, 411, 41));
    dialogNewUserLabelTitle->setStyleSheet("#dialogNewUserLabelTitle{\n"
        "	color:white; \n"
        "	background:rgba(0,0,0,0.6);\n"
    "}");
    dialogNewUserLabelTitle->setAlignment(Qt::AlignCenter);
    dialogNewUserBack = new QPushButton(this);
    dialogNewUserBack->setObjectName("dialogNewUserBack");
    dialogNewUserBack->setGeometry(QRect(70, 380, 101, 41));
    dialogNewUserBack->setStyleSheet("#dialogNewUserBack{\n"
        "	background:rgba(250,100,0,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	border:2px solid white;\n"
        "}\n"
        "#dialogNewUserBack:hover{\n"
        "	background:rgba(250,100,0,0.8);\n"
    "}");
    dialogNewUserCreate = new QPushButton(this);
    dialogNewUserCreate->setObjectName("dialogNewUserCreate");
    dialogNewUserCreate->setGeometry(QRect(230, 380, 101, 41));
    dialogNewUserCreate->setStyleSheet("#dialogNewUserCreate{\n"
        "	background:rgba(0,255,30,0.5);\n"
        "	border-radius:20px;\n"
        "	color:white;\n"
        "	border:2px solid white;\n"
        "}\n"
        "#dialogNewUserCreate:hover{\n"
        "	background:rgba(0,255,30,0.8);\n"
    "}");
    dialogNewUserErrPassword = new QLabel(this);
    dialogNewUserErrPassword->setObjectName("dialogNewUserErrPassword");
    dialogNewUserErrPassword->setGeometry(QRect(140, 340, 201, 21));
    dialogNewUserErrPassword->setStyleSheet("#dialogNewUserErrPassword{\n"
        "	color:white;\n"
        "	background:rgba(255,0,0,0.5);\n"
    "}");
    dialogNewUserErrPassword->setAlignment(Qt::AlignCenter);
    dialogNewUserErrUsername = new QLabel(this);
    dialogNewUserErrUsername->setObjectName("dialogNewUserErrUsername");
    dialogNewUserErrUsername->setGeometry(QRect(70, 110, 281, 21));
    dialogNewUserErrUsername->setStyleSheet("#dialogNewUserErrUsername{\n"
        "	color:white; \n"
        "	background:rgba(255,0,0,0.5);\n"
    "}");
    dialogNewUserErrUsername->setAlignment(Qt::AlignCenter);
    dialogNewUserTypeBasic = new QRadioButton(this);
    dialogNewUserTypeBasic->setObjectName("dialogNewUserTypeBasic");
    dialogNewUserTypeBasic->setGeometry(QRect(30, 193, 121, 31));
    dialogNewUserTypeBasic->setStyleSheet("#dialogNewUserTypeBasic{\n"
        "	background:rgba(0,255,0,0.5);\n"
        "border-top-left-radius:5px;\n"
        "border-bottom-left-radius:5px;\n"
        "padding-left:10px;\n"
    "}");
    dialogNewUserTypeBasic->setIconSize(QSize(16, 16));
    dialogNewUserTypeExecutive = new QRadioButton(this);
    dialogNewUserTypeExecutive->setObjectName("dialogNewUserTypeExecutive");
    dialogNewUserTypeExecutive->setGeometry(QRect(261, 193, 111, 31));
    dialogNewUserTypeExecutive->setStyleSheet("#dialogNewUserTypeExecutive{\n"
        "	background:rgba(255,0,0,0.5); \n"
        "	border-top-right-radius:5px;	\n"
        "	border-bottom-right-radius:5px;\n"
        "	padding-left:10px;\n"
    "}");
    dialogNewUserTypeBusiness = new QRadioButton(this);
    dialogNewUserTypeBusiness->setObjectName("dialogNewUserTypeBusiness");
    dialogNewUserTypeBusiness->setGeometry(QRect(150, 193, 111, 31));
    dialogNewUserTypeBusiness->setStyleSheet("#dialogNewUserTypeBusiness{\n"
        "	background:rgba(255,100,180,0.5);\n"
        "	padding-left:10px;\n"
    "}");
    dialogNewUserLabelType = new QLabel(this);
    dialogNewUserLabelType->setObjectName("dialogNewUserLabelType");
    dialogNewUserLabelType->setGeometry(QRect(20, 150, 361, 91));
    dialogNewUserLabelType->setStyleSheet("#dialogNewUserLabelType{\n"
        "	background:rgba(0,0,0,0.5);\n"
        " 	color:white;\n"
        "	border-radius:20px;\n"
    "}");
    dialogNewUserLabelType->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    dialogNewUserLabelType->setMargin(15);
    dialogNewUserLabelType->raise();
    dialogNewUserPassword->raise();
    dialogNewUserPasswordConfirm->raise();
    dialogNewUserUsername->raise();
    dialogNewUserLabelPassword_2->raise();
    dialogNewUserLabelPassword->raise();
    dialogNewUserLabelUsername->raise();
    dialogNewUserLabelTitle->raise();
    dialogNewUserBack->raise();
    dialogNewUserCreate->raise();
    dialogNewUserErrPassword->raise();
    dialogNewUserErrUsername->raise();
    dialogNewUserTypeBasic->raise();
    dialogNewUserTypeBusiness->raise();
    dialogNewUserTypeExecutive->raise();
    setWindowTitle(QString("Dialog"));
    dialogNewUserUsername->setText(QString());
    dialogNewUserLabelPassword_2->setText(QString("Conferma"));
    dialogNewUserLabelPassword->setText(QString("Password"));
    dialogNewUserLabelUsername->setText(QString("Email utente"));
    dialogNewUserLabelTitle->setText(QString("INSERIMENTO NUOVO UTENTE"));
    dialogNewUserBack->setText(QString("Indietro"));
    dialogNewUserCreate->setText(QString("Crea"));
    dialogNewUserErrPassword->setText(QString("Password non corrispondenti"));
    dialogNewUserErrUsername->setText(QString("Username errato o gi\303\240 in uso"));
    dialogNewUserTypeBasic->setText(QString("Basic"));
    dialogNewUserTypeExecutive->setText(QString("Executive"));
    dialogNewUserTypeBusiness->setText(QString("Business"));
    dialogNewUserLabelType->setText(QString("Tipologia utente"));
    QMetaObject::connectSlotsByName(this);
}
