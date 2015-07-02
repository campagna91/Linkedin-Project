#ifndef LINKEDINER_H
#define LINKEDINER_H

#include <string>
#include <list>
#include <QString>
#include <iostream>
#include "profile.h"
#include "works.h"
#include "qualifications.h"
#include "languages.h"

using std::string;
using std::list;

class Db;
class Linkediner
{
    friend class SearchFunctor;
public:
    Linkediner(
        string email ="",
        string password ="",
        string photoPath ="",
        Profile=Profile(),
        list<Works*> = list<Works*>(),
        list<Languages*> = list<Languages*>(),
        list<Linkediner*> = list<Linkediner*>(),
        list<Qualifications*> = list<Qualifications*>()
    );
    virtual ~Linkediner() = 0;
    void addWork(Works*);   //  add a work to work list
    void addLanguage(Languages*);   // add a language to language list
    void addContact(Linkediner*);   // add a contact to address list
    void addQualifications(Qualifications*);    // add a qualification to qualification list
    void setEmail(string);  // set the email of user ( used only by db at startup )
    void setPassword(string);   // set a password
    void setPhoto(string);  // set a profile photo
    string getPhoto() const;    // return a photo's path
    string getEmail() const;    // return user's email
    string getPassword() const; // return current password
    Profile * getProfile(); // return user's profile
    list<Linkediner*> * getNet();   // return user's associated peole
    list<Works*> * getWorks();  // return user's work list
    list<Languages*> * getLanguages();  //  return user's language list
    list<Qualifications*> * getQualifications();    // return user's qualification list
    virtual Linkediner * find(Db *, const string &) = 0;    // virtual find
    class SearchFunctor
    {
    public:
        int level;
        SearchFunctor(int x=1) : level(x) {}
        Linkediner * operator()(Linkediner * uFound) const; // operator need to find switching account type
    };
protected:
    void deallocate();  // remove all user's data

private:
    string email;
    string password;
    string photo;
    Profile profile;
    list<Works*> works;
    list<Linkediner*> net;
    list<Languages*> languages;
    list<Qualifications*> qualifications;
    virtual Linkediner * clone() = 0;   //  virtual clone for user
};

#endif // LINKEDINER_H
