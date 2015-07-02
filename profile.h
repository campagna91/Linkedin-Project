#ifndef PROFILE_H
#define PROFILE_H

#include <list>
#include <QDate>

#include <string>
#include <iostream>

using std::string;
using std::list;
using std::cout;
using std::endl;


class Profile
{
    string name;
    string surname;
    string nationality;
    QDate birth;
    list<string> contacts;
public:
    Profile(
        string a="",
        string b="",
        string c="",
        list<string> d=list<string>()
    );
    ~Profile();

    string getName() const; // return user's name
    string getSurname() const;  // return user's surname
    string getNationality() const;  //  return user's nationality
    list<string> * getContacts();   //  return user's contact-list
    QDate getBirth() const; // return user's birth

    void setName(string);   // set user's name
    void setSurname(string);    //  set user's surname
    void setNationality(string);    //  set user's nationality
    void setBirth(QDate);   // set user's birth
    void addContact(string);    //  add an address into contact-list
    void removeContact(string c="");    // remove a contact from contact-list

};

#endif // PROFILE_H

