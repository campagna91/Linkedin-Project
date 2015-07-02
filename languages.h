#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class Languages
{
    string name;
    int written;
    int spoken;
    int heard;

public:

    Languages(string="",int=0,int=0,int=0);
    ~Languages();
    void setHeard(int); //  set head property
    void setSpoken(int);    // set spoken property
    int getHeard() const;   // return heard property
    void setWritten(int);   // set written property
    void setName(string);   // set name of language
    int getSpoken() const;  //return spoken property
    string getName() const; // get name of language
    int getWritten() const; // return written property
};

#endif // LANGUAGES_H
