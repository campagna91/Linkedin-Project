#include "linkedinerbasic.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LinkedinerBasic::LinkedinerBasic(string u, string p, string i, Profile pr, list<Works*> w, list<Languages*> l, list<Linkediner*> n, list<Qualifications*> q):
    Linkediner(u,p,i,pr,w,l,n,q)
{
    cout<<"CREATE LINKEDINER-BASIC"<<endl;
}
LinkedinerBasic::~LinkedinerBasic()
{
    deallocate();
    cout<<"-- DELETE LINKEDINER-BASIC"<<endl;
}
Linkediner * LinkedinerBasic::find(Db * db, const string & uToFind)
{
    Linkediner * uFound = db->getUserByEmail(uToFind);
    if(!uFound)
        return 0;
    Linkediner::SearchFunctor search(1);
    return search(uFound);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PRIVATE METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

LinkedinerBasic * LinkedinerBasic::clone()
{
    Profile cloneP = Profile(
                this->getProfile()->getName(),
                this->getProfile()->getSurname(),
                this->getProfile()->getNationality(),
                list<string>(*(this->getProfile()->getContacts()))
                );
    list<Works*> cloneW;
    list<Works*>::iterator iW = this->getWorks()->begin();
    for( ; iW != this->getWorks()->end(); ++iW)
    {
        cloneW.push_back(
                    new Works(
                        (*iW)->getStart(),
                        (*iW)->getEnd(),
                        (*iW)->getSector(),
                        (*iW)->getJob(),
                        list<string>( *(*iW)->getSkills() )
                    )
        );
    }
    list<Qualifications*> cloneQ;
    list<Qualifications*>::iterator iQ = this->getQualifications()->begin();
    for( ; iQ != this->getQualifications()->end(); ++iQ)
    {
        cloneQ.push_back(
                    new Qualifications(
                        (*iQ)->getName(),
                        (*iQ)->getYear()
                    )
        );
    }
    list<Languages*> cloneL;
    list<Languages*>::iterator iL = this->getLanguages()->begin();
    for( ; iL != this->getLanguages()->end(); ++iL)
    {
        cloneL.push_back(
                    new Languages(
                        (*iL)->getName(),
                        (*iL)->getWritten(),
                        (*iL)->getSpoken(),
                        (*iL)->getHeard()
                    )
        );
    }
    list<Linkediner*> cloneN;
    list<Linkediner*>::iterator iN = this->getNet()->begin();
    for( ; iN != this->getNet()->end(); ++iN)
    {
        cloneN.push_back(*iN);
    }
    return new LinkedinerBasic(string(this->getEmail()),"","",cloneP,cloneW,cloneL,cloneN,cloneQ);
}
