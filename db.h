#ifndef DB_H
#define DB_H

#include <list>
#include <QFile>
#include <string>
#include <QObject>
#include <iostream>
#include <typeinfo>
#include <QMessageBox>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include "baddb.h"
#include "badpermission.h"
#include "linkediner.h"
#include "linkedinerbasic.h"
#include "linkedinerbusiness.h"
#include "linkedinerexecutive.h"

using std::list;
using std::string;
using std::cout;
using std::endl;
typedef QDomElement El;

class Linkediner;
class Db : public QObject
{
    Q_OBJECT

private:
    string name;
    string path;
    list<Linkediner*> db;
public:
    Db(string name="",string path="", list<Linkediner*> =list<Linkediner*>());
    ~Db();
    int size() const;   //  return number of users
    bool empty() const; //  return if db's path is empty
    void load(string path);  // load db from xml save db
    void save();    // save db to xml file
    void clean();   // clean users and db's path
    void setPath(string path ="");    //  set db's path
    void setName(string name ="");    // set name of db
    void addUser(Linkediner * newUser); // add an user into db
    void removeUser(Linkediner * user);  //  remove an existing user from db
    string getPath() const; //  return db's path
    string getName(int l) const; // return db's name
    Linkediner * find(string email);  //  return if exist and user otherwise 0
    Linkediner * login(string email, string password) const;    //  authenticate a user
    Linkediner * getUserByEmail(string email);  //  return a pointer to user from email
    list<Linkediner*> * getDb();    // return db list as pointer

signals :
    void s_feed(int,int) const; //  emit same message to clientStart gui
};

#endif // DB_H
