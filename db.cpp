#include "db.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Db::Db(string n,string p,list<Linkediner*>u):name(n),path(p),db(u)
{
   emit s_feed(0,0);
}
Db::~Db()
{
    list<Linkediner*>::iterator i = db.begin();
    for( ; i != db.end(); ++i) delete *i;
}
void Db::load(string p)
{
    try{
        if(p.empty()) return;
        if(!empty()) clean();
        QDomDocument doc("dbLinkedin");
        QFile file(QString::fromStdString(p));
        if (!file.open(QIODevice::ReadOnly | QIODevice::WriteOnly)) { throw BadPermission();}
        if (!doc.setContent(&file)) { throw BadPermission(); }
        file.close();
        int n = 0; El point;
        El users = doc.firstChildElement("users").firstChildElement("user");
        while(!users.isNull())
        {
            El pointer;
            Linkediner * l;
            if(users.attributeNode("type").value() == "basic"){
                l = new LinkedinerBasic(
                            "","","",
                            Profile(),
                            list<Works*>(),
                            list<Languages*>(),
                            list<Linkediner*>(),
                            list<Qualifications*>()
                            );
            } else if(users.attributeNode("type").value() == "business"){
                l = new LinkedinerBusiness(
                            "","","",
                            Profile(),
                            list<Works*>(),
                            list<Languages*>(),
                            list<Linkediner*>(),
                            list<Qualifications*>()
                            );
            } else {
                l = new LinkedinerExecutive(
                            "","","",
                            Profile(),
                            list<Works*>(),
                            list<Languages*>(),
                            list<Linkediner*>(),
                            list<Qualifications*>()
                            );
            }
            if(users.firstChildElement("password").isNull()) throw BadDb();
                l->setPassword((users.firstChildElement("password").text()).toStdString());
            if(users.firstChildElement("email").isNull()) throw BadDb();
                l->setEmail((users.firstChildElement("email").text()).toStdString());
            if(users.firstChildElement("image").isNull()) throw BadDb();
                l->setPhoto((users.firstChildElement("image").text()).toStdString());
            if(users.firstChildElement("profile").firstChildElement("name").isNull()) throw BadDb();
                l->getProfile()->setName((users.firstChildElement("profile").firstChildElement("name").text()).toStdString());
            if(users.firstChildElement("profile").firstChildElement("surname").isNull()) throw BadDb();
                l->getProfile()->setSurname((users.firstChildElement("profile").firstChildElement("surname").text()).toStdString());
            if(users.firstChildElement("profile").firstChildElement("birth").isNull()) throw BadDb();
                l->getProfile()->setBirth(QDate::fromString(users.firstChildElement("profile").firstChildElement("birth").text(),"yyyy'-'MM'-'dd"));
            if(users.firstChildElement("profile").firstChildElement("nationality").isNull()) throw BadDb();
                l->getProfile()->setNationality((users.firstChildElement("profile").firstChildElement("nationality").text()).toStdString());
            if(users.firstChildElement("profile").firstChildElement("contacts").isNull()) throw BadDb();
            pointer = users.firstChildElement("profile").firstChildElement("contacts").firstChildElement("contact");
            while(!pointer.isNull()){
                l->getProfile()->addContact((pointer.text()).toStdString());
                pointer = pointer.nextSiblingElement("contact");
            }
            if(users.firstChildElement("works").isNull()) throw BadDb();
            point = users.firstChildElement("works").firstChildElement("work");
            while(!point.isNull()){
                pointer = point.firstChildElement("skills").firstChildElement("skill");
                list<string> sk;
                while(!pointer.isNull()){
                    sk.push_back((pointer.text()).toStdString());
                    pointer = pointer.nextSiblingElement("skill");
                }
                if( point.firstChildElement("start").isNull()  ||
                    point.firstChildElement("end").isNull()    ||
                    point.firstChildElement("sector").isNull() ||
                    point.firstChildElement("job").isNull() ) throw BadDb();
                l->addWork(new Works(
                            QDate::fromString(point.firstChildElement("start").text(),"yyyy'-'MM'-'dd"),
                            QDate::fromString(point.firstChildElement("end").text(),"yyyy'-'MM'-'dd"),
                            (point.firstChildElement("sector").text()).toStdString(),
                            (point.firstChildElement("job").text()).toStdString(),
                            sk
                            )
                );
                point = point.nextSiblingElement("work");
            }
            if(users.firstChildElement("languages").isNull()) throw BadDb();
            pointer = users.firstChildElement("languages").firstChildElement("language");
            while(!pointer.isNull()){
                l->addLanguage(new Languages(
                                (pointer.firstChildElement("name").text()).toStdString(),
                                (pointer.firstChildElement("written").text()).toInt(),
                                (pointer.firstChildElement("spoken").text()).toInt(),
                                (pointer.firstChildElement("heard").text()).toInt()
                                )
                );
                pointer = pointer.nextSiblingElement("language");
            }
            if(users.firstChildElement("qualifications").isNull()) throw BadDb();
            pointer = users.firstChildElement("qualifications").firstChildElement("qualification");
            while(!pointer.isNull()){
                if(pointer.firstChildElement("name").isNull()) throw BadDb();
                if(pointer.firstChildElement("year").isNull()) throw BadDb();
                l->addQualifications(new Qualifications(
                                        (pointer.firstChildElement("name").text()).toStdString(),
                                        QDate::fromString((pointer.firstChildElement("year").text()),"yyyy")
                                     )
                );
                pointer = pointer.nextSiblingElement("qualification");
            }
            db.push_back(l);
            users = users.nextSiblingElement("user");
            n++;
        }
        users = doc.firstChildElement("users").firstChildElement("user");
        list<Linkediner*>::iterator utenti = db.begin();
        for( ; utenti != db.end(); ++utenti)
        {
            bool find = false;
            while(!users.isNull() && !find)
            {
                if(users.firstChildElement("email").text().toStdString() == (*utenti)->getEmail()){
                    find = true;
                    if(users.firstChildElement("net").isNull()) throw BadDb();
                    El net = users.firstChildElement("net").firstChildElement("contact");
                    while(!net.isNull())
                    {
                        (*utenti)->getNet()->push_back(getUserByEmail(net.text().toStdString()));
                        net = net.nextSiblingElement("contact");
                    }
                }
                users = users.nextSiblingElement("user");
            }
        }
        setPath(p);
        emit s_feed(1,5);
    }
    catch(BadDb){
        emit s_feed(0,2);
    }
    catch(BadPermission){
        emit s_feed(0,1);
    }
    catch(...){
        emit s_feed(0,6);
    }
}
void Db::save(){
    try{
        QFile * to = new QFile(QString::fromStdString(path));
        if (!to->open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Truncate)) { throw BadPermission();}
        QXmlStreamWriter * out = new QXmlStreamWriter;
        out->setAutoFormatting(true);
        out->setDevice(to);
        out->writeStartDocument();
        out->writeStartElement("users");
        list<Linkediner*>::iterator user = db.begin();
        for( ; user != db.end(); ++user )
        {
           out->writeStartElement("user");
           if(dynamic_cast<LinkedinerBasic*>(*user))
               out->writeAttribute("type","basic");
           else if(dynamic_cast<LinkedinerBusiness*>(*user))
               out->writeAttribute("type","business");
           else out->writeAttribute("type","executive");
           out->writeTextElement("email",QString::fromStdString((*user)->getEmail()));
           out->writeTextElement("password",QString::fromStdString((*user)->getPassword()));
           out->writeTextElement("image",QString::fromStdString((*user)->getPhoto()));
           out->writeStartElement("profile");
           out->writeTextElement("name",QString::fromStdString((*user)->getProfile()->getName()));
           out->writeTextElement("surname",QString::fromStdString((*user)->getProfile()->getSurname()));
           out->writeTextElement("nationality",QString::fromStdString((*user)->getProfile()->getNationality()));
           out->writeTextElement("birth",(*user)->getProfile()->getBirth().toString("yyyy'-'MM'-'dd"));
           out->writeStartElement("contacts");
           list<string>::iterator contact = (*user)->getProfile()->getContacts()->begin();
           for( ; contact != (*user)->getProfile()->getContacts()->end(); ++contact )
              out->writeTextElement("contact",QString::fromStdString((*contact)));
           out->writeEndElement();
           out->writeEndElement();
           out->writeStartElement("works");
           list<Works*>::iterator work = (*user)->getWorks()->begin();
           for( ; work != (*user)->getWorks()->end(); ++work)
           {
                out->writeStartElement("work");
                out->writeTextElement("start",(*work)->getStart().toString("yyyy'-'MM'-'dd"));
                out->writeTextElement("end",(*work)->getEnd().toString("yyyy'-'MM'-'dd"));
                out->writeTextElement("sector",QString::fromStdString((*work)->getSector()));
                out->writeTextElement("job",QString::fromStdString((*work)->getJob()));

                out->writeStartElement("skills");
                list<string>::iterator skill = (*work)->getSkills()->begin();
                for( ; skill != (*work)->getSkills()->end(); ++skill)
                    out->writeTextElement("skill",QString::fromStdString((*skill)));
                out->writeEndElement();
                out->writeEndElement();
            }
            out->writeEndElement();
            out->writeStartElement("languages");
            list<Languages*>::iterator lang = (*user)->getLanguages()->begin();
            for( ; lang != (*user)->getLanguages()->end(); ++lang)
            {
                out->writeStartElement("language");
                out->writeTextElement("name",QString::fromStdString((*lang)->getName()));
                out->writeTextElement("heard",QString::number((*lang)->getHeard()));
                out->writeTextElement("spoken",QString::number((*lang)->getSpoken()));
                out->writeTextElement("written",QString::number((*lang)->getWritten()));
                out->writeEndElement();
            }
            out->writeEndElement();
            out->writeStartElement("qualifications");
            list<Qualifications*>::iterator qualif = (*user)->getQualifications()->begin();
            for( ; qualif != (*user)->getQualifications()->end(); ++qualif)
            {
                out->writeStartElement("qualification");
                out->writeTextElement("name",QString::fromStdString((*qualif)->getName()));
                out->writeTextElement("year",(*qualif)->getYear().toString("yyyy"));
                out->writeEndElement();
            }
            out->writeEndElement();
            out->writeStartElement("net");
            list<Linkediner*>::iterator ref = (*user)->getNet()->begin();
            for( ; ref != (*user)->getNet()->end(); ++ref)
                out->writeTextElement("contact",QString::fromStdString((*ref)->getEmail()));
            out->writeEndElement();
            out->writeEndElement();
        }
        out->writeEndElement();
        out->writeEndDocument();
        to->close();
    }
    catch(BadPermission){
        emit s_feed(0,1);
        QMessageBox::information(0,"Salvataggio non riuscito","E' possibile che non si disponga più dei\npermessi necessari per scrivere il database");
        return;
    }
    catch(...){
    }
}
void Db::removeUser(Linkediner * l)
{
    list<Linkediner*>::iterator i = db.begin();
    for( ; i != db.end(); ++i)
    {
        if( (*i)->getEmail() == l->getEmail() ) {
            delete *i;
            db.remove(l);
            break;
        }
    }
}
void Db::addUser(Linkediner * l)
{
    db.push_back(l);
}
bool Db::empty() const
{
    return path == "";
}
void Db::clean()
{
    if(db.size()){
        list<Linkediner*>::iterator i=db.begin();        
        for(;i!=db.end();++i){
            delete *i;
        }
        db.clear();
    }
    path = "";
    emit s_feed(0,0);
}
string Db::getPath() const
{
    return path;
}
string Db::getName(int l) const
{
    if(l && name.size() > 15) return name.substr(0,10)+"...";
    else return name;
}
void Db::setPath(string p)
{
    setName(p.substr(p.find_last_of("/")+1,p.size()));
    path = p;

}
void Db::setName(string n)
{
    name = n;
}
Linkediner* Db::login(string u, string p) const
{
    list<Linkediner*>::const_iterator i = db.begin();
    for(; i != db.end(); ++i){
        if((*i)->getEmail() == u){
            if((*i)->getPassword()==p){
                return const_cast<Linkediner*>(*i);
            }
            else { emit s_feed(0,3); return 0; }
        }
    }
    emit s_feed(0,4); return 0;
}
Linkediner * Db::getUserByEmail(string e)
{
    list<Linkediner*>::iterator i = db.begin();
    for( ; i!= db.end(); ++i)
    {
        if((*i)->getEmail() == e) return *i;
    }
    return 0;
}
list<Linkediner*> * Db::getDb(){
    return &db;
}

int Db::size() const
{
    return db.size();
}
