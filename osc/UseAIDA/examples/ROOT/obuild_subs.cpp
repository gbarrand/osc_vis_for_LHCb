
//
//  This program is and should be standalone so that :
//     UNIX> c++ obuild_subs.cxx 
//      DOS> cl.exe obuild_subs.cxx 
// builds at first shoot.
//

#include <string>
#include <cstdlib>
#include <cstdio>

// From Lib :
static bool Lib_smanip_replace(std::string&,
                               const std::string&,
                               const std::string&);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char** aArgv
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aArgc!=4) {
    fprintf(stderr,"obuild_subs : three arguments expected.\n");
    printf("\n");
    return EXIT_FAILURE;
  }

  std::string s(aArgv[1]);
  std::string o(aArgv[2]);
  std::string n(aArgv[3]);

  Lib_smanip_replace(s,o,n);

  printf("%s\n",s.c_str());

  return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool Lib_smanip_replace(
 std::string& aString
,const std::string& aOld     
,const std::string& aNew     
)
//////////////////////////////////////////////////////////////////////////////
// return true : some replacement done.
// return false : nothing replaced.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aOld.empty()) return false;
  std::string snew;
  std::string::size_type lold = aOld.length();
  bool status = false;
  std::string stmp = aString;
  while(true) {
    std::string::size_type pos = stmp.find(aOld);
    if(pos==std::string::npos){
      snew += stmp;
      break;
    } else {
      snew += stmp.substr(0,pos);
      snew += aNew;
      stmp = stmp.substr(pos+lold,stmp.length()-(pos+lold));
      status = true;
    }
  }
  aString = snew;
  return status;
}
