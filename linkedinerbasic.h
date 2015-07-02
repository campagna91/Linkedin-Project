#ifndef LINKEDINERBASIC_H
#define LINKEDINERBASIC_H

#include "db.h"
#include "linkediner.h"
#include <iostream>
using std::cout;
using std::endl;

class Db;
class LinkedinerBasic : public Linkediner
{
public:
    LinkedinerBasic(
        string="",
        string="",
        string="",
        Profile=Profile(),
        list<Works*> = list<Works*>(),
        list<Languages*> = list<Languages*>(),
        list<Linkediner*> = list<Linkediner*>(),
        list<Qualifications*> = list<Qualifications*>()
    );
    ~LinkedinerBasic();
    Linkediner * find(Db *, const string &);    //  implementation of virtual find
private:
    LinkedinerBasic * clone();  // implementation of virtual clone
};

#endif // LINKEDINERBASIC_H
