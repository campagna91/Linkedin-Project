#ifndef QUALIFICATIONS_H
#define QUALIFICATIONS_H

#include <string>
#include <QDate>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class Qualifications
{
private:
    string name;
    QDate year;
public:
    Qualifications(string="",QDate=QDate());
    ~Qualifications();
    string getName() const; //  return qualification's name
    QDate getYear() const;  // return qualification's year
    void setName(string);   // set qualification's name
    void setYear(QDate);    //  set qualification's year
};

#endif // QUALIFICATIONS_H
