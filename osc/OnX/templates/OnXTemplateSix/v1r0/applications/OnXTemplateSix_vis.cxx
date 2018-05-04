
// User :
#include <User/Framework.h>
#include <User/PrintProcessor.h>

// this :
#include <OnXTemplateSix/VisProcessor.h>

int main() {

 {User::Framework userFramework;

  userFramework.addProcessor(new User::PrintProcessor(userFramework));

  std::string GUI = "../scripts/OnX/main.onx";

  OnXTemplateSix::VisProcessor* visProcessor =  
    new OnXTemplateSix::VisProcessor(userFramework,GUI);
  if(!visProcessor->isValid()) return 0;

  userFramework.addProcessor(visProcessor);

  userFramework.mainLoop(10);}

  return 0;
}
