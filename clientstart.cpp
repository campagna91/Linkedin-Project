#include "clientstart.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClientStart::ClientStart(QWidget *parent) :
    QMainWindow(parent),
    db(new Db())
{
    setup();
    feed(0,0);
    connect(db,SIGNAL(s_feed(int,int)),this,SLOT(feed(int,int)));
    cout<<"CREATE CLIENTSTART"<<endl;
}
ClientStart::~ClientStart()
{
    delete db;
}
bool ClientStart::isValid(int type, int op,QObject * o0, QObject * o1)
{
    QLineEdit * q0 = dynamic_cast<QLineEdit *>(o0);
    try{
        if(q0)
        {
            QString s0 = q0->text();
            if(s0 == "") throw false;
            switch(type)
            {
                // date validation
                case(0):
                {
                    if(s0.length() != 10 && s0.length() != 4) throw false;
                    if(s0.length() == 4 && ( !QDate::fromString(s0,"yyyy").isValid() || QDate::fromString(s0,"yyyy").year() > QDate::currentDate().year() ) ) return false;
                    if(s0.length() == 10 && !QDate::fromString(s0,"yyyy'-'MM'-'dd").isValid()) throw false;
                    QDate d0 = QDate::fromString(s0,"yyyy'-'MM'-'dd");
                    switch(op)
                    {
                        case(0):
                        {
                            if(d0.year() < 1930 || s0.length() != 10) throw false;
                            throw true;
                        }
                        // start work validation
                        case(1):
                        {
                            QString s1 = (dynamic_cast<QLineEdit *>(o1))->text();
                            QDate d1 = QDate::fromString(s1,"yyyy'-'MM'-'dd");
                            if(d0.year() > d1.year() + 18 && ( d0.year() < QDate::currentDate().year() || (d0.year() < QDate::currentDate().year() && d0.month() < QDate::currentDate().month() ) ) )
                                throw true;
                            throw false;
                        }
                        // end work validation
                        case(2):
                        {
                            QString s1 = (dynamic_cast<QLineEdit *>(o1))->text();
                            QDate d1 = QDate::fromString(s1,"yyyy'-'MM'-'dd");
                            if(d0.year() > d1.year() || (d0.year() == d1.year() && d0.month() > d1.month()))
                                throw true;
                            throw false;
                        }
                        case(3):
                        {
                            QString s1 = (dynamic_cast<QLineEdit *>(o1))->text();
                            QDate d1 = QDate::fromString(s1,"yyyy");
                            if(s0.toInt() > (d1.year() + 12) ) throw true;
                            throw false;
                        }
                    }
                }
                // text validation
            case(1):
                {
                    QRegExp generalText("\\b^[a-zA-Z]{1,}[a-zA-Z0-9 ]{1,}[a-zA-Z0-9]$\\b");
                    if(!generalText.exactMatch(s0)) throw false;
                    switch(op)
                    {
                        case(0):
                        {
                            QRegExp onlyLetter("\\b^[a-zA-Z]{1,}[a-zA-Z ]{1,}[a-zA-Z]$\\b");
                            if(onlyLetter.exactMatch(s0)) throw true;
                            throw false;
                        }
                        case(1):
                        {
                            throw true;
                        }
                    }
                }
                // password validation
            case(2):
                {
                    QRegExp passwordSafe("\\b^[a-zA-Z0-9 ]{7,}[a-zA-Z0-9]$\\b");
                    if(passwordSafe.exactMatch(s0)) throw true;
                    throw false;
                }
                // email validation
            case(3):
                {
                    QRegExp email("\\b^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$\\b");
                    email.setCaseSensitivity(Qt::CaseInsensitive);
                    email.setPatternSyntax(QRegExp::RegExp);
                    if(email.exactMatch(s0)) throw true;
                    throw false;
                }
            }
        }
    }
    catch(bool b){
        if(b)
        {
            q0->setStyleSheet(q0->styleSheet().replace("border:1px solid red;","border:none;"));
            return true;
        } else {
            q0->setStyleSheet(q0->styleSheet().replace("border:none;","border:1px solid red;"));
            return false;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ClientStart::setup()
{
            setObjectName("ClientStart");
            resize(330, 426);
            setMinimumSize(QSize(330, 426));
            setMaximumSize(QSize(330, 426));
            setStyleSheet("#ClientStart{\n"
                "background:#006699;\n"
            "}\n"
            "*{font-size:12px;}");
            actionEsci = new QAction(this);
            actionEsci->setObjectName("actionEsci");
            QIcon icon;
            icon.addFile(":/media/freccia.png", QSize(), QIcon::Normal, QIcon::Off);
            actionEsci->setIcon(icon);
            actionApri = new QAction(this);
            actionApri->setObjectName("actionApri");
            QIcon icon1;
            icon1.addFile(":/media/apri.png", QSize(), QIcon::Normal, QIcon::Off);
            actionApri->setIcon(icon1);
            actionNuovo = new QAction(this);
            actionNuovo->setObjectName("actionNuovo");
            QIcon icon2;
            icon2.addFile(":/media/nuovo.png", QSize(), QIcon::Normal, QIcon::Off);
            actionNuovo->setIcon(icon2);
            actionSalva = new QAction(this);
            actionSalva->setObjectName("actionSalva");
            QIcon icon3;
            icon3.addFile(":/media/salva.png", QSize(), QIcon::Normal, QIcon::Off);
            actionSalva->setIcon(icon3);
            actionLinkedin = new QAction(this);
            actionLinkedin->setObjectName("actionLinkedin");
            QIcon icon4;
            icon4.addFile(":/media/help.png", QSize(), QIcon::Normal, QIcon::Off);
            actionLinkedin->setIcon(icon4);
            centralWidget = new QWidget(this);
            centralWidget->setObjectName("centralWidget");
            username = new QLineEdit(centralWidget);
            username->setObjectName("username");
            username->setGeometry(QRect(40, 60, 241, 41));
            username->setFocusPolicy(Qt::StrongFocus);
            username->setStyleSheet("#username{\n"
                "	background:rgba(0,0,0,0.5);\n"
                "	color:white;\n"
                "	border-radius:20px;\n"
                "	border:none;\n"
                "}\n"
            "");
            username->setAlignment(Qt::AlignCenter);
            password = new QLineEdit(centralWidget);
            password->setObjectName("password");
            password->setGeometry(QRect(40, 110, 241, 41));
            password->setStyleSheet("#password{\n"
                "	background:rgba(0,0,0,0.5);\n"
                "	color:white;\n"
                "	border-radius:20px;\n"
            "}");
            password->setEchoMode(QLineEdit::Password);
            password->setAlignment(Qt::AlignCenter);
            labelSecond = new QLabel(centralWidget);
            labelSecond->setObjectName("labelSecond");
            labelSecond->setGeometry(QRect(35, 250, 131, 41));
            labelSecond->setStyleSheet("#labelSecond{\n"
                "color:white;\n"
            "}");
            adminButton = new QPushButton(centralWidget);
            adminButton->setObjectName("adminButton");
            adminButton->setGeometry(QRect(170, 245, 131, 51));
            adminButton->setStyleSheet("#adminButton{\n"
                "	border:2px solid white;\n"
                "	border-radius:25px;\n"
                "	color:white;\n"
                "}\n"
                "#adminButton:hover{\n"
                "	border:2px solid red;\n"
                "	background:rgba(255,0,0,0.3);\n"
            "}");
            adminButton->setFlat(true);
            loginButton = new QPushButton(centralWidget);
            loginButton->setObjectName("loginButton");
            loginButton->setGeometry(QRect(120, 170, 91, 41));
            loginButton->setStyleSheet("#loginButton{\n"
                "	border:2px solid white;\n"
                "	border-radius:20px;\n"
                "	color:white;\n"
                "}\n"
                "#loginButton:hover{\n"
                "	background:rgba(255,0,0,0.3);\n"
                "	border:2px solid red;\n"
            "}");
            loginButton->setFlat(true);
            labelFirst = new QLabel(centralWidget);
            labelFirst->setObjectName("labelFirst");
            labelFirst->setGeometry(QRect(20, 20, 281, 211));
            labelFirst->setStyleSheet("#labelFirst{\n"
                "	color:white;\n"
                "	background:rgba(0,0,0,0.5);\n"
                "	border-radius:20px;\n"
            "}");
            labelFirst->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            labelFirst->setMargin(11);
            labelThird = new QLabel(centralWidget);
            labelThird->setObjectName("labelThird");
            labelThird->setGeometry(QRect(0, 327, 331, 41));
            labelThird->setStyleSheet("#labelThird{\n"
                "color:white;\n"
                "font-weight:bolder;\n"
                "background:rgba(255,0,0,0.4);\n"
            "}");
            labelThird->setAlignment(Qt::AlignCenter);
            setCentralWidget(centralWidget);
            labelFirst->raise();
            username->raise();
            password->raise();
            labelSecond->raise();
            adminButton->raise();
            labelThird->raise();
            loginButton->raise();
            menuBar = new QMenuBar(this);
            menuBar->setObjectName("menuBar");
            menuBar->setGeometry(QRect(0, 0, 330, 22));
            menuFile = new QMenu(menuBar);
            menuFile->setObjectName("menuFile");
            menuDatabase = new QMenu(menuBar);
            menuDatabase->setObjectName("menuDatabase");
            menu = new QMenu(menuBar);
            menu->setObjectName("menu");
            setMenuBar(menuBar);
            mainToolBar = new QToolBar(this);
            mainToolBar->setObjectName("mainToolBar");
            addToolBar(Qt::TopToolBarArea, mainToolBar);
            statusBar = new QStatusBar(this);
            statusBar->setObjectName("statusBar");
            setStatusBar(statusBar);
            QWidget::setTabOrder(username, password);
            QWidget::setTabOrder(password, loginButton);
            QWidget::setTabOrder(loginButton, adminButton);
            menuBar->addAction(menuFile->menuAction());
            menuBar->addAction(menuDatabase->menuAction());
            menuBar->addAction(menu->menuAction());
            menuFile->addAction(actionEsci);
            menuDatabase->addAction(actionNuovo);
            menuDatabase->addAction(actionApri);
            menuDatabase->addAction(actionSalva);
            menu->addAction(actionLinkedin);
            setWindowTitle("ClientStart");
            actionEsci->setText("Esci");
            actionEsci->setShortcut(QString("Ctrl+W"));
            actionApri->setText("Apri");
            actionApri->setShortcut(QString("Ctrl+O"));
            actionNuovo->setText("Nuovo");
            actionNuovo->setShortcut(QString("Ctrl+N"));
            actionSalva->setText("Salva");
            actionSalva->setShortcut(QString("Ctrl+S"));
            actionLinkedin->setText("Linkedin");
            actionLinkedin->setShortcut(QString("F1"));
            username->setText("username");
            password->setText("password");
            labelSecond->setText("Oppure accedi come ");
            adminButton->setText("amministratore");
            loginButton->setText("Entra");
            labelFirst->setText("Entra in Linkedin");
            labelThird->setText(QString());
            menuFile->setTitle("File");
            menuDatabase->setTitle("Database");
            menu->setTitle("?");
            QMetaObject::connectSlotsByName(this);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE SLOT
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ClientStart::feed(int code, int type)
{
    if(!code)
        labelThird->setStyleSheet("color:white;background-color:rgba(255,0,0,0.6)");
    else
        labelThird->setStyleSheet("color:white;background-color:rgba(0,255,0,0.6)");
    switch(type){
    case(0):    labelThird->setText("Database non selezionato");
        break;
    case(1):    labelThird->setText("Permessi utente insufficienti");
        break;
    case(2):    labelThird->setText("Database corrotto");
        break;
    case(3):    labelThird->setText("Password errata");
        break;
    case(4):    labelThird->setText("Utente non presente");
        break;
    case(5):    labelThird->setText("Database "+QString::fromStdString(db->getName(1))+" caricato correttamente");
        break;
    case(6):    labelThird->setText("L\'errore non è da sottovalutare \n Contattare il tecnico ;) ");
        break;
    case(7):    labelThird->setText("Permessi per la destinazione insufficienti");
        break;
    case(8):    labelThird->setText("Database "+QString::fromStdString(db->getName(1))+" creato ed in uso");
        break;
    }
}
void ClientStart::saveRequest()
{
    db->save();
}
void ClientStart::undoUpdate()
{
    db->load(db->getPath());
}
void ClientStart::exitAdminGui()
{
    guiAdmin->hide();
    delete guiAdmin;
    emit feed(1,5);
    this->show();
    actionNuovo->setDisabled(false);
    actionApri->setDisabled(false);
}
void ClientStart::exitUserGui()
{
    guiUser->hide();
    delete guiUser;
    emit feed(1,5);
    this->show();
    actionNuovo->setDisabled(false);
    actionApri->setDisabled(false);
}
void ClientStart::on_username_returnPressed()
{
    on_loginButton_clicked();
}
void ClientStart::on_password_returnPressed()
{
    on_loginButton_clicked();
}
void ClientStart::on_loginButton_clicked()
{
    if(!db->empty())
    {
        if(isValid(3,0,username))
        {
            Linkediner * user = db->login(username->text().toStdString(),password->text().toStdString());
            if(user)
            {
                guiUser = new ClientUser(user,db,this);
                guiUser->setWindowFlags(Qt::Window);
                guiUser->show();
                actionNuovo->setDisabled(true);
                actionApri->setDisabled(true);
                this->hide();
            }
        }
    } else {
        QMessageBox::information(this,tr("Database assente"),tr("Per effettuare il login selezionare prima un database")); feed(0,0);
    }
}
void ClientStart::on_adminButton_clicked()
{
    if(!db->empty()){
        guiAdmin = new ClientAdmin(db,this);
        guiAdmin->setWindowFlags(Qt::Window);
        guiAdmin->show();
        actionNuovo->setDisabled(true);
        actionApri->setDisabled(true);
        this->hide();
    } else QMessageBox::information(this,tr("Database assente"),tr("Per accedere all'area amministrativa \n è necessario prima selezionare un nuovo \n databaase o aprirne uno già esistente"));
}
void ClientStart::on_actionApri_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Apri database utenti"),"./","UserDB (*.linkeDB)");
    db->load(path.toStdString());
}
void ClientStart::on_actionNuovo_triggered()
{
    if(!db->empty())
        db->clean();
    QString a, b, c, d;
    QString path = QFileDialog::getSaveFileName(0,tr("Salva in "),"./","UserDB (*.linkeDB)");
    QFile file(path);
    if(path.size() > 8 ) {
        if(!file.open( QIODevice::WriteOnly | QIODevice::ReadOnly | QIODevice::Text) )
        {
            feed(0,7);
            db->clean();
        }
        else {
            db->setPath(path.toStdString());
            db->save();
            feed(1,8);
        }
    }
}
void ClientStart::on_actionSalva_triggered()
{
    db->save();
}
void ClientStart::on_actionEsci_triggered()
{
    QApplication::quit();
}
