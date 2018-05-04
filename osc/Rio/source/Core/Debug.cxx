//////////////////////////////////////////////////////////////////////////////
// Simple class to do instance counting.
//////////////////////////////////////////////////////////////////////////////


// this :
#include <Rio/Core/Debug.h>

#include <list>

int Rio::Debug::fCount = 0;

static bool Rio_sCheckByClass = false;
static std::list< std::pair<std::string,int> >* Rio_sList = 0;

//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::checkByClass(
 bool aEnable
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_sCheckByClass = aEnable;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::increment(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fCount++;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::decrement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fCount--;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::increment(
 const char* aClass
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(Rio_sCheckByClass) increment(std::string(aClass));
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::decrement(
 const char* aClass
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(Rio_sCheckByClass) decrement(std::string(aClass));
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::increment(
 const std::string& aClass
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(Rio_sCheckByClass) {
    if(!Rio_sList) Rio_sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=Rio_sList->begin();it!=Rio_sList->end();++it) {
      if((*it).first==aClass) {
        (*it).second++;
        return;
      }
    }
    Rio_sList->push_back(std::pair<std::string,int>(aClass,1));
  } else {
    fCount++;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Debug::decrement(
 const std::string& aClass
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(Rio_sCheckByClass) {
    if(!Rio_sList) Rio_sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=Rio_sList->begin();it!=Rio_sList->end();++it) {
      if((*it).first==aClass) {
        (*it).second--;
        return;
      }
    }
    Rio_sList->push_back(std::pair<std::string,int>(aClass,-1));
  } else {
    fCount--;
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::Debug::Debug(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  increment();
}
//////////////////////////////////////////////////////////////////////////////
Rio::Debug::~Debug(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  decrement();
}

void Rio::Debug::check(std::ostream& aOut) {
  if(fCount) {
    aOut << "Rio::Debug::check :"
         << " bad object balance : " << fCount << std::endl;
  }
  if(Rio_sList) {
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=Rio_sList->begin();it!=Rio_sList->end();++it) {
      if((*it).second) {
        aOut << "Lib::Debug::check :"
             << " for class \"" << (*it).first
             << "\", bad object balance : " << (*it).second
             << std::endl;
      }
    }
    delete Rio_sList;
    Rio_sList = 0;
  }
}
