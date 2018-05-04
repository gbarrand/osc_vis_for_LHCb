#ifndef BatchLab_CHBOOK_h
#define BatchLab_CHBOOK_h

#include <string>
#include <vector>

// Package :
void CHLIMIT(int);
void CHDCOFL();
void CHDELET(int);
void CRZINK(int,int,const std::string&);
void CZITOH(int*,int*,int*);
//std::vector<int> CHIDALL();
//std::vector<int> CHID1();
//std::vector<int> CHID2();
//bool CHEXIST(int);
bool CHISID(int);

// File :
int CHROPEN(int,const std::string&,const std::string&,const std::string&,int);
void CHROUT(int,int,const std::string&);
void CHREND(const std::string&);
void CHCDIR(const std::string&,const std::string&);
void CHLDIR(const std::string&,const std::string&);
std::string CHPWD();
void CHMDIR(const std::string&,const std::string&);
void CHDDIR(const std::string&);
void CHRIN(int,int,int);
std::vector<std::string> CHDIRS(bool);
std::vector<int> CHKEYS();
bool CHEDIR(const std::string&,bool);

// Histogram :
void CHBOOK1(int,const std::string&,int,float,float,float=0);
void CHBOOK2(int,const std::string&,int,float,float,int,float,float,float=0);
bool CHBOOKB(int,const std::string&,const std::vector<float>&,float=0);
void CHBPROF(int,const std::string&,int,float,float,float,float,const std::string&);
void CHGIVE(int,std::string&,int&,float&,float&,int&,float&,float&);
void CHFILL(int,float,float,float);
float CHI(int,int);
float CHIE(int,int);
float CHIJ(int,int,int);
float CHIJE(int,int,int);
float CHIX(int,int);
void CHIJXY(int,int,int,float&,float&);
int CHXI(int,float);
void CHXYIJ(int,float,float,int&,int&);
float CHMIN(int);
float CHMAX(int);
float CHSTATI(int,int,const std::string&,int);
void CHRESET(int,const std::string&);
void CHOPERA(int,const std::string&,int,int,float,float);

// tuple :
void CHBNT(int,const std::string&,const std::string&);
void CHBNAME(int,const std::string&,void*,const std::string&);
void CHBNAM(int,const std::string&,void*,const std::string&,int);
void CHFNT(int);
void CHNTVAR2(int,int,std::string&,std::string&,std::string&,int&,int&,int&,int&);
int CHGIVEN(int,std::string&,std::vector<std::string>&);
int CHNOENT(int);
int CHGNT(int,int);
int CHGNF(int,int,float*);
void CHGNPAR(int,const std::string&);

#endif
