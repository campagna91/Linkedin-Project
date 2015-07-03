#include "profile.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Profile::Profile(string n, string c, string na,list<string> cn):name(n),surname(c),nationality(na),contacts(cn)
{
    cout<<"CREATE PROFILE"<<endl;
}
Profile::~Profile()
{
    cout<<"-- DELETE PROFILE"<<endl;
}

string Profile::getName() const
{
    return name;
}

string Profile::getSurname() const
{
    return surname;
}

string Profile::getNationality() const
{
    return nationality;
}

list<string> * Profile::getContacts()
{
    return &contacts;
}

QDate Profile::getBirth() const
{
    return birth;
}

void Profile::setBirth(QDate d)
{
    birth = d;
}

void Profile::setName(string n)
{
    name = n;
}

void Profile::setSurname(string c)
{
    surname = c;
}

void Profile::setNationality(string n)
{
    nationality = n;
}

void Profile::addContact(string c)
{
    contacts.insert(contacts.begin(),c);
}

void Profile::removeContact(string c)
{
    contacts.remove(c);
}
