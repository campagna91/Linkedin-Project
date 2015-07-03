#include "linkediner.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Linkediner::Linkediner(string u, string p, string i, Profile pr, list<Works*> w, list<Languages*> l, list<Linkediner*> n, list<Qualifications*> q):
    email(u), password(p), photo(i), profile(pr), works(w), net(n), languages(l), qualifications(q)
{
    cout<<"CREATE LINKEDINER"<<endl;
}
Linkediner::~Linkediner()
{
    cout<<"-- DELETE LINKEDINER"<<endl;
}
string Linkediner::getEmail() const
{
     return email;
}
Profile * Linkediner::getProfile()
{
    return &profile;
}
list<Works*> * Linkediner::getWorks()
{
    return &works;
}
list<Qualifications*> * Linkediner::getQualifications()
{
    return &qualifications;
}
list<Linkediner*> * Linkediner::getNet()
{
    return &net;
}
list<Languages*> * Linkediner::getLanguages()
{
    return &languages;
}
string Linkediner::getPassword() const
{
    return password;
}
void Linkediner::setPassword(string n)
{
    password = n;
}
void Linkediner::setPhoto(string p)
{
    if(p.empty()) photo = "./resource/default.png";
    else photo = p;
}
void Linkediner::setEmail(string e)
{
    email = e;
}
void Linkediner::addContact(Linkediner * l)
{
    net.push_front(l);
}
void Linkediner::addLanguage(Languages * l)
{
    languages.push_front(l);
}
string Linkediner::getPhoto() const
{
    return photo;
}
void Linkediner::addQualifications(Qualifications * q)
{
    qualifications.push_front(q);
}
void Linkediner::addWork(Works * w)
{
    works.push_front(w);
}
void Linkediner::deallocate()
{
    list<Works*>::iterator iW = getWorks()->begin();
    for( ; iW != getWorks()->end(); ++iW) delete *iW;
    list<Qualifications*>::iterator iQ = getQualifications()->begin();
    for( ; iQ != getQualifications()->end(); ++iQ) delete *iQ;
    list<Languages*>::iterator iL = getLanguages()->begin();
    for( ; iL != getLanguages()->end(); ++iL) delete *iL;
}
Linkediner* Linkediner::SearchFunctor::operator()(Linkediner * uFound) const
{
    Linkediner * aux = uFound->clone();
    switch(level)
    {
        case(1):{
            list<Languages*>::iterator iL = aux->getLanguages()->begin();
            list<Works*>::iterator iW = aux->getWorks()->begin();
            list<Qualifications*>::iterator iQ = aux->getQualifications()->begin();
            for( ; iL != aux->getLanguages()->end(); ++iL)  { delete *iL; aux->getLanguages()->erase(iL); }
            for( ; iW != aux->getWorks()->end(); ++iW)  { delete *iW; aux->getWorks()->erase(iW);}
            for( ; iQ != aux->getQualifications()->end(); ++iQ)  { delete *iQ; aux->getQualifications()->erase(iQ); }
            aux->getProfile()->getContacts()->clear();
            aux->getNet()->clear();
            break;
        }
        case(2):{
            aux->getNet()->clear();
            break;
        }
        case(3):{
            break;
        }
    }
    return aux;
}
