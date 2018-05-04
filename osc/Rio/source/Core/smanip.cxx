// this :
#include <Rio/Core/smanip.h>

//////////////////////////////////////////////////////////////////////////////
void Rio::smanip::strip(
 std::string& aString
,what aType
,char aChar
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string::size_type l = aString.length();;
  if(l==0) return;

  std::string retVal = aString;
  std::string::size_type i,j,k;
  switch ( aType ) {
  case leading: 
    for(i=0;i<l;i++) {
      if (aString[i] != aChar) break;
    }
    retVal = aString.substr(i,l-i);
    break;
  case trailing:
    for(j=l-1;;j--) {
      if (aString[j] != aChar) break; 
    }
    retVal = aString.substr(0,j+1);
    break;
  case both:{
    for(i=0;i<l;i++) {
      if (aString[i] != aChar) break;
    }
    std::string tmp(aString.substr(i,l-i));
    for(k=tmp.length()-1;;k--) {
      if (tmp[k] != aChar) break; 
    }
    retVal = tmp.substr(0,k+1);
    }break;
  }
  aString = retVal;
}

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
bool Rio::smanip::toint(
 const std::string& aString
,int& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : notation n[eE][+]m not supported !
  char* s;
  long l = ::strtol(aString.c_str(),&s,10);
  if( (s==aString.c_str()) || (s!=(aString.c_str()+aString.size())) ) {
    aValue = 0;
    return false;
  }
  aValue = (int)l;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::smanip::words(
 const std::string& aString
,const std::string& aLimiter 
,std::vector<std::string>&  aWords
)
//////////////////////////////////////////////////////////////////////////////
//  If aLimiter is for exa "|" ; for "xxx||xxx", {"xxx","xxx"} will 
// be created (and NOT {"xxx","","xxx"}).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aWords.clear();
  if(aString.empty()) return;
  if(aLimiter.empty()) {
    aWords.push_back(aString);
  } else {
    std::string::size_type l = aString.length();
    std::string::size_type llimiter = aLimiter.length();
    std::string::size_type pos = 0;
    while(true) {
      std::string::size_type index = aString.find(aLimiter,pos);
      if(index==std::string::npos){ // Last word.
        if((l-pos)>0) aWords.push_back(aString.substr(pos,l-pos));
        break;
      } else {
        //     abcxxxef
        //     0  3  67
        if((index-pos)>0) aWords.push_back(aString.substr(pos,index-pos));
        pos = index + llimiter;
      }
    }
  }
}
#include <stdio.h>
#include <stdarg.h>
//////////////////////////////////////////////////////////////////////////////
bool Rio::smanip::printf(
 std::string& aString
,int aLength
,const char* aFormat
,...
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aString = "";
  if(aLength<0) return false;
  if(!aFormat) return false;
  char* s = new char[aLength+1];
  if(!s) return false;
  s[aLength] = '\0';
  va_list args;
  va_start (args,aFormat);
  ::vsprintf (s,aFormat,args);
  va_end   (args);
  if(s[aLength]!='\0') {
    delete [] s;
    return false;
  }
  aString = s;
  delete [] s;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::smanip::tostringx(
 unsigned int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s;
  printf(s,32,"%x",aValue);
  return s;
}

#include <string.h>

#define STRDUP(str)  ((str) != NULL ? (::strcpy((char*)::malloc((unsigned)::strlen(str) + 1), str)) : (char*)NULL)
#define STRDEL(str) {if((str)!=NULL) {::free(str);str=NULL;}}

//////////////////////////////////////////////////////////////////////////////
bool Rio::smanip::pathNameSuffix(
 const std::string& aString
,std::string& aPath
,std::string& aName
,std::string& aSuffix
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPath = "";
  aName = "";
  aSuffix = "";
  char* path = STRDUP(aString.c_str());
  if(!path) return false;
  char* pos = ::strrchr(path,'/');
  if(!pos) {
    pos = ::strrchr(path,'\\'); // Windows syntax ?
  }
  if(pos) {
    *pos = 0;
    aPath = path;
    aName = pos+1;
  } else {
    aName = path;
  }
  STRDEL(path);
  // Get suffix :
  char* name = STRDUP(aName.c_str());
  if(!name) return false;
  pos = ::strrchr(name,'.');
  if(pos) {
    aSuffix = pos+1;
  }
  STRDEL(name);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::smanip::match(
 const std::string& aString   
,const std::string& aPattern 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int lpattern = aPattern.length();
  int lstring  = aString.length();
  if ((lpattern==0)&&(lstring==0)) return true;
  if ((lpattern==0)&&(lstring!=0)) return true;
  if ((lpattern!=0)&&(lstring==0)) return false;
  // pattern is * :
  if(aPattern=="*") return true;
  int wcount = 0;
  int count;
  for(count=0;count<lpattern;count++) {
    if(aPattern[count]=='*') wcount++;
  }
  // no wildcard :
  if(wcount==0) {
    return (aPattern==aString ? true : false );
  }

  // complex pattern :
#define PACKET 32
  char** words = (char**)::malloc(PACKET * sizeof(char*));
  if(words==NULL) return false;
  int wmax = PACKET;
  int wnumber= 0;
  char* s = STRDUP(aPattern.c_str());
  char* token = s;
  while(true) { 
    char* pos = ::strstr (token,"*");
    if(pos) {
      *pos = '\0';
      if(*token!='\0') {
        if(wnumber>=wmax) {
          words = (char**)::realloc(words,(wmax+PACKET) * sizeof(char*));
          if(words==NULL) {
            STRDEL(s);
            return false;
          }
          wmax += PACKET;
        }
        words[wnumber] = token;
        wnumber++;
      }
      token = pos + 1;
    } else { // last word :
      if(*token!='\0') {
        if(wnumber>=wmax) {
          words = (char**)::realloc(words,(wmax+PACKET) * sizeof(char*));
          if(words==NULL) {
            STRDEL(s);
            return false;
          }
          wmax += PACKET;
        }
        words[wnumber] = token;
        wnumber++;
      }
      break;
    }
  }
  // check that at least one word is not empty :
  bool ok = false;
  for(count=0;count<wnumber;count++) { 
    if(*(words[count])!='\0') {
      ok = true;
      break;
    }
  }
  if(!ok) { // only wildcards :
    STRDEL(s);
    if(words) ::free(words);
    return true;
  } 

  // loop on words :
  bool match = true;
  token = (char*)aString.c_str();
  for(count=0;count<wnumber;count++) { 
    int   lword;
    lword = ::strlen(words[count]); 
    if(lword>0) { 
      char* pos;
      if(count==0) {
        if(aPattern[0]!='*') {
          // Begin of pattern (words[0]) and aString must match :
          if(::strncmp(token,words[count],lword)!=0) {
            match = false; // Different.
            break;      
          }
          token = token + lword;
          continue;
        }
      }
      pos = ::strstr (token,words[count]);
      if(!pos) {
        match = false;
        break;
      }
      if((count==(wnumber-1)) && (aPattern[lpattern-1]!='*') ) {
        // Last word :
        if(::strcmp(aString.c_str()+lstring-lword,words[count])!=0) 
          match = false; // Compare last word and end of aString.
        break;
      } else {
        token = pos + lword;
      }
    }
  }
  STRDEL(s);
  if(words) ::free(words);
  return match;
}
