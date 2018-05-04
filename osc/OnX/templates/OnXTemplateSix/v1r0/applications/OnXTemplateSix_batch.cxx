
// User :
#include <User/Framework.h>
#include <User/PrintProcessor.h>

int main() {

 {User::Framework userFramework;

  userFramework.addProcessor(new User::PrintProcessor(userFramework));

  userFramework.mainLoop(10);}

  return 0;
}
