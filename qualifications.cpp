#include "qualifications.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Qualifications::Qualifications(string n,QDate a) : name(n), year(a)
{
}
Qualifications::~Qualifications()
{
    cout<<"DELETE QUALIFICATION"<<endl;
}
string Qualifications::getName() const
{
    return name;
}
QDate Qualifications::getYear() const
{
    return year;
}
void Qualifications::setName(string n)
{
    name = n;
}

void Qualifications::setYear(QDate d)
{
    year = d;
}
