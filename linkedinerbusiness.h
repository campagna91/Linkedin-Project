#ifndef LINKEDINERBUSINESS_H
#define LINKEDINERBUSINESS_H

#include "db.h"
#include "linkediner.h"
#include <iostream>
using std::cout;
using std::endl;

class Db;
class LinkedinerBusiness : public Linkediner
{
public:
    LinkedinerBusiness(
            string="",
            string="",
            string="",
            Profile=Profile(),
            list<Works*> = list<Works*>(),
            list<Languages*> = list<Languages*>(),
            list<Linkediner*> = list<Linkediner*>(),
            list<Qualifications*> = list<Qualifications*>()
        );
    ~LinkedinerBusiness();
    Linkediner * find(Db *, const string &);    // implementation of virtual find
private:
    LinkedinerBusiness * clone();   // implementation of virtual clone
};

#endif // LINKEDINERBUSINESS_H
