#include "works.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Works::Works(QDate i,QDate f,string s,string l,list<string> m):
    start(i),end(f),sector(s),job(l),skills(m)
{
    cout<<"CREATE WORK"<<endl;
}
Works::~Works()
{
    cout<<"-- DELETE WORK"<<endl;
}
QDate Works::getStart() const
{
    return start;
}
QDate Works::getEnd() const
{
    return end;
}
string Works::getSector() const
{
    return sector;
}
string Works::getJob() const
{
    return job;
}
list<string>* Works::getSkills()
{
    return &skills;
}
void Works::setStart(QDate s)
{
    start = s;
}
void Works::setEnd(QDate e)
{
    end = e;
}
void Works::setSector(string s)
{
    sector = s;
}
void Works::setJob(string j)
{
    job = j;
}
void Works::addSkill(string s)
{
    skills.push_back(s);
}
