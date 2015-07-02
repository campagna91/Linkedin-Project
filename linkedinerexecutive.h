#ifndef LINKEDINEREXECUTIVE_H
#define LINKEDINEREXECUTIVE_H

#include "db.h"
#include "linkediner.h"
#include <iostream>
using std::cout;
using std::endl;

class Db;
class LinkedinerExecutive : public Linkediner
{
public:
    LinkedinerExecutive(
        string="",
        string="",
        string="",
        Profile=Profile(),
        list<Works*> = list<Works*>(),
        list<Languages*> = list<Languages*>(),
        list<Linkediner*> = list<Linkediner*>(),
        list<Qualifications*> = list<Qualifications*>()
    );
    ~LinkedinerExecutive();
    Linkediner * find(Db *, const string &);    // implementation of virtual find
private:
    LinkedinerExecutive * clone();  // implementation of virtual clone
};

#endif // LINKEDINEREXECUTIVE_H
