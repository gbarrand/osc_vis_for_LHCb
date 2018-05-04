
#include <cstdio>

#include <iostream>

/*
Lib_extern std::ostream Lib_cout;
#include <vector>
Lib_extern double Lib_double;
Lib_extern std::vector<std::string> Lib_vector;

#include <list>

class B {
public:
  B(){printf("B\n");}
  virtual ~B() {printf("debug : ~B\n");}
  B(const B&) {
    printf("B copy constructor\n");
  }
  B& operator=(const B&) {
    printf("B operator =\n");
    return *this;
  }
};

class A {
public:
  A(){printf("A\n");}
  virtual ~A() {printf("debug : ~A\n");}
  A(const A&) {
    printf("A copy constructor\n");
  }
  A& operator=(const A&) {
    printf("A operator =\n");
    return *this;
  }
  void xxx() {printf("debug : xxx :%ld\n",(unsigned long)this);}
  B yyy() {return fB;}
  void zzz(const B&) {printf("debug : zzz\n");}
private:
  B fB;
};

class C : public A {
public:
  C(){printf("C\n");}
  virtual ~C() {printf("debug : ~C\n");}
  C(const C& aC):A(aC) {
    printf("C copy constructor\n");
  }
  C& operator=(const C&) {
    printf("C operator =\n");
    return *this;
  }
};

A returnA () {
  return A();
}

*/
#include <typeinfo>
namespace NM1 {
class NM1_X {
};
}

/*
#include <algorithm>

int BinarySearch(const std::vector<double>& aArray,double aValue);
*/

#include <map>

int main() {

  const std::type_info& tid = typeid(NM1::NM1_X);
  // name() return a const char*.
  std::cout << "typeinfo A : " << std::string(tid.name()) << std::endl;

/*
  Lib_cout << "My cout" << std::endl;

  Lib_double = 0;
  Lib_vector.push_back("Hello");

  const std::type_info& tid = typeid(NM1::NM1_X);
  // name() return a const char*.
  out << "typeinfo A : " << std::string(tid.name()) << std::endl;

  std::string s= "hello";
  Lib::smanip::strip(s);
  out << "Should be \"hello\" : \"" << s << "\"" << std::endl;

  s = "  hello   ";
  Lib::smanip::strip(s);
  out << "Should be \"hello\" : \"" << s << "\"" << std::endl;

  s = "  hello   ";
  Lib::smanip::strip(s,Lib::smanip::leading);
  out << "Should be \"hello   \" : \"" << s << "\"" << std::endl;

  s = "  hello   ";
  Lib::smanip::strip(s,Lib::smanip::trailing);
  out << "Should be \"  hello\" : \"" << s << "\"" << std::endl;

  s = "  helloxxx";
  Lib::smanip::strip(s,Lib::smanip::trailing,'x');
  out << "Should be \"  hello\" : \"" << s << "\"" << std::endl;

  std::string s2(5,'x');
  out << "Should be \"Slash::Core::ILibraryManagerx\" : \"" << s2 << "\"" << std::endl;

  std::string s3("Shape/12345678");
  std::string::size_type pos = s3.find("/");
  if(pos!=std::string::npos) {
    out << "Should be 5 : " << (unsigned int)pos << std::endl;
    std::string stype = s3.substr(0,pos);
    out << "Should be \"Shape\" : \"" << stype << "\"" << std::endl;
    std::string sid = s3.substr(pos+1,s3.length()-(pos+1));
    //std::string sid = s3.substr(pos+1,2);
    out << "Should be \"12345678\" : \"" << sid << "\"" << std::endl;
  } else {
    out << "char not found" << std::endl;
  }

  std::string s4 = "abc$xxx";
  if(!Lib::smanip::getenv(s4)) out << "Some env variables not found" << std::endl;
  out << "\"" << s4 << "\"" << std::endl;

  std::string s5 = "abc$xxx/def/$xxx/ghi";
  //std::string s5 = "abc$xxx/def/$xxx";
  if(!Lib::smanip::getenv(s5)) out << "Some env variables not found." << std::endl;
  out << "\"" << s5 << "\"" << std::endl;

  std::string s6 = "abcxxxdefxxxghixxx";
  Lib::smanip::replace(s6,"xxx","yyyy");
  out << "Should be \"abcyyyydefyyyyghiyyyy\" : \"" << s6 << "\"" << std::endl;

  std::vector<std::string> words;
  Lib::smanip::words("abc\n\ndef\n\nghi","\n",words);
  Lib::smanip::write(words,printer);

  Lib::smanip::words("abc\\n\\ndef\\n\\nghi","\\n",words);
  Lib::smanip::write(words,printer);

  std::string s7("Slash::Core::ILibraryManageryyyzzz");
  pos = s7.find_last_of("yyy");
  out << "Should be 6 : " << (unsigned int)pos 
      << " and Slash::Core::ILibraryManager \"" << s7.substr(0,pos-3+1) << "\"" << std::endl;

 {std::list<A*> list,list1;
  A a1,a2,a3;
  list.push_back(&a1);
  list.push_back(&a1);
  list.push_back(&a2);
  list.push_back(&a2);
  list.push_back(&a3);
  list.push_back(&a1);
  std::list<A*>::iterator it; 
  it = list.begin();
  it++;
  //it++;it--;
  list.insert(it,&a3);

  for(it=list.begin();it!=list.end();it++) (*it)->xxx();
  list.remove(&a2);
  printf("debug : ----\n");
  for(it=list.begin();it!=list.end();it++) (*it)->xxx();
  list1 = list;
  list.clear();
  printf("debug : ----+ %d\n",(int)list.size());
  for(it=list1.begin();it!=list1.end();it++) (*it)->xxx();
  for(it=list1.begin();it!=list1.end();) {
    if((*it)==&a1) it = list1.erase(it);
    else ++it;
  }
  printf("debug : ----++ %d\n",(int)list1.size());
  for(it=list1.begin();it!=list1.end();it++) (*it)->xxx();



  //int count;
  //for(count=0;count<list.size();count++) 
  //printf("debug :%ld\n",list[count]);
  }


 s = "a\\n";
 s.resize(s.size()-2);
 printf("debug :%s|\n",s.c_str());

{ printf("debug : std::vector & ~\n");
 std::vector<A> va;
 A a1,a2,a3;
 va.push_back(a1);
 va.push_back(a2);
 va.push_back(a3);

 printf("debug : clear : begin\n");
 va.clear();
 printf("debug : clear : end\n");
}

 {
 printf("debug : return A : begin\n");
  A a = returnA();
 printf("debug : return A : end\n");

 printf("debug : return B : begin\n");
  A aa;
 printf("debug : return B : 000\n");
  B b = aa.yyy();
 printf("debug : return B : 001\n");
  aa.zzz(aa.yyy());
 printf("debug : return B : end\n");


 }

 {std::string s= " ";
  std::vector<std::string> words;
  Lib::smanip::words(s," ",words);
  out << "Should be 0 : " << (unsigned int)words.size() << std::endl;
 }

 {
   std::vector<std::string> text;
   text.push_back("hello");
   text.push_back("hello2");
   text.push_back("hello");
   text.push_back("hello2");
   text.push_back("hello3");
   Lib::smanip::unique(text);
   for(unsigned int count=0;count<text.size();count++)
     out << count << " " << text[count] << std::endl;
 }

 {
  printf("debug : test copy constructor :\n");
   C c;
   C c2(c);
 }

 {
   printf("debug : test vector set element :\n");
   std::vector<int> vd;
   vd.push_back(100);
   vd.push_back(200);
   vd[1] = 300;
   printf("debug :  should have 300 : %d\n",vd[1]);

 }

 {
   printf("debug : test binary search :\n");
   std::vector<double> vd;
   vd.push_back(1);
   vd.push_back(2.1);
   vd.push_back(3);
   vd.push_back(3);
   vd.push_back(4.2);
   std::vector<double>::const_iterator vdi =
     std::lower_bound(vd.begin(),vd.end(),10);
   printf("debug :  binary search : +++ : %g\n",*vdi);
#define NotFound (-1)
   int i = BinarySearch(vd,4.2);
   if(i==NotFound)
     printf("debug :  binary search : not found\n");
   else 
     printf("debug :  binary search : == : %d %g\n",i,vd[i]);
 }
*/

   /*
 {typedef std::map<std::string,std::string> Map;
  Map mp;
  mp["a*"] = "item 1";
  mp["aa"] = "item 2";
  mp["/dd/Geometry/DownstreamRegion/Muon/M*R*Cham/lvM*R*Cham"] = "item 3";

  {Map::const_iterator it;
   for(it=mp.begin();it!=mp.end();it++) {
    ::printf("debug :  \"%s\" \"%s\" \n",it->first.c_str(),it->second.c_str());
   }
  }  

  //std::string search("aax");
  //std::string search("/dd/Geometry/DownstreamRegion/Muon/M1R1Cham/lvM1R1Cham");
  std::string search("/dd/Geometry/DownstreamRegion/Muon/M5R4Cham/lvM5R4Cham");

  ::printf("debug :  search %s...\n",search.c_str());
  Map::const_iterator it = mp.find(search);
  if(it!=mp.end()) {
    ::printf("debug :  found %s : %s\n",search.c_str(),it->second.c_str());
  } else {
    ::printf("debug :  not found %s\n",search.c_str());
  }

  {Map::const_iterator it;
   bool found = false;
   for(it=mp.begin();it!=mp.end();it++) {
     if(inlib::match(search,it->first)) {
       ::printf("debug :  found %s : %s\n",search.c_str(),it->second.c_str());
       found = true;
     }
   }
   if(!found) {
     ::printf("debug :  not found %s\n",search.c_str());
   }
  }  

 }
   */

  return 0;
}
/*
int BinarySearch(const std::vector<double>& aArray,double aValue){
  printf("debug : Binary2 : begin +\n");
  int nabove = aArray.size() + 1;
  int nbelow = 0;
  while(nabove-nbelow > 1) {
     int middle = (nabove+nbelow)/2;
     if (aValue == aArray[middle-1]) return middle-1;
     if (aValue  < aArray[middle-1]) nabove = middle;
     else nbelow = middle;
  }
  printf("debug : Binary2 : end %d\n",nbelow-1);
  return nbelow-1;
}
*/
