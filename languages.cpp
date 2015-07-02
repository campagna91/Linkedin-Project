#include "languages.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   PUBLIC METHOD
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Languages::Languages(string n,int s,int p, int a)
{
    if(s > 5 || s < 0 || p > 5 || p < 0 || a > 5 || a < 0 ) {
        name=""; written = spoken = heard = 0;
    }
    else { name = n; written = s; spoken = p; heard = a; }
}
Languages::~Languages()
{
    cout<<"DELETE LANGUAGE"<<endl;
}
string Languages::getName() const
{
    return name;
}
int Languages::getSpoken() const
{
    return spoken;
}
int Languages::getWritten() const
{
    return written;
}
int Languages::getHeard() const
{
    return heard;
}
void Languages::setName(string n)
{
    name = n;
}
void Languages::setWritten(int l)
{
    written = l;
}
void Languages::setSpoken(int l)
{
    spoken = l;
}
void Languages::setHeard(int l)
{
    heard = l;
}

