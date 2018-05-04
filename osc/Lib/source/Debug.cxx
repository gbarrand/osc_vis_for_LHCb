
#include <Lib/Debug.h>

#include <list>

#include <cstring> //strcmp

static bool sCheckByClass = false;
static int sCount = 0;
static std::list< std::pair<std::string,int> >* sList = 0;

void Lib::Debug::checkByClass(bool aEnable){sCheckByClass = aEnable;}
bool Lib::Debug::checkByClass(){return sCheckByClass;}
void Lib::Debug::increment(){sCount++;}
void Lib::Debug::decrement(){sCount--;}
void Lib::Debug::increment(const char* aClass){
  sCount++;
  if(sCheckByClass) {
    if(!sList) sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=sList->begin();it!=sList->end();++it) {
      if(!::strcmp((*it).first.c_str(),aClass)) {
        (*it).second++;
        return;
      }
    }
    sList->push_back(std::pair<std::string,int>(std::string(aClass),1));
  }
}
void Lib::Debug::decrement(const char* aClass){
  sCount--;
  if(sCheckByClass) {
    if(!sList) sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=sList->begin();it!=sList->end();++it) {
      if(!::strcmp((*it).first.c_str(),aClass)) {
        (*it).second--;
        return;
      }
    }
    sList->push_back(std::pair<std::string,int>(std::string(aClass),-1));
  }
}
void Lib::Debug::increment(const std::string& aClass){
  sCount++;
  if(sCheckByClass) {
    if(!sList) sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=sList->begin();it!=sList->end();++it) {
      if((*it).first==aClass) {
        (*it).second++;
        return;
      }
    }
    sList->push_back(std::pair<std::string,int>(aClass,1));
  }
}
void Lib::Debug::decrement(const std::string& aClass){
  sCount--;
  if(sCheckByClass) {
    if(!sList) sList = new std::list< std::pair<std::string,int> >;
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=sList->begin();it!=sList->end();++it) {
      if((*it).first==aClass) {
        (*it).second--;
        return;
      }
    }
    sList->push_back(std::pair<std::string,int>(aClass,-1));
  }
}

Lib::Debug::Debug(){increment("Lib::Debug");}
Lib::Debug::~Debug(){decrement("Lib::Debug");}

void Lib::Debug::balance(std::ostream& a_out) {
  if(sCount) {
    a_out << "Lib::Debug::check :"
          << " bad global object balance : " << sCount << std::endl;
    if(sCheckByClass) {
      a_out << "Lib::Debug::check :"
            << " check by class was enabled." << std::endl;
    } else {
      a_out << "Lib::Debug::check :"
            << " check by class was disabled." << std::endl;
    }
  }
  if(sList) {
    std::list< std::pair<std::string,int> >::iterator it;
    for(it=sList->begin();it!=sList->end();++it) {
      if((*it).second) {
        a_out << "Lib::Debug::check :"
              << " for class \"" << (*it).first << "\""
              << ", bad object balance : " << (*it).second
              << std::endl;
      }
    }
    delete sList;
    sList = 0;
  }
}
