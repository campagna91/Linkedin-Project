#ifndef WORKS_H
#define WORKS_H

#include <QDate>
#include <string>
#include <list>
#include <iostream>

using std::string;
using std::list;
using std::cout;
using std::endl;

class Works
{
private:
    QDate start;
    QDate end;
    string sector;
    string job;
    list<string> skills;
public:
    Works(QDate i=QDate(),QDate f=QDate(),string s="",string l="",list<string> = list<string>());
    ~Works();
    QDate getStart() const; // return work's start
    QDate getEnd() const;   //  return work's end
    string getSector() const;   // return work's sector
    string getJob() const;  //  return work's job
    void setStart(QDate);   //  set work's start
    void setEnd(QDate); //  set work's end
    void setSector(string); //  set work's sector
    void setJob(string);    //  set work's job
    void addSkill(string);  // add a skill to work-skill-list
    list<string> * getSkills(); // return skill-list
};

#endif // WORKS_H
