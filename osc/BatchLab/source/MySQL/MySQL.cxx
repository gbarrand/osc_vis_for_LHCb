
#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <BatchLab/MySQL/MySQLManager.h>

extern "C" {
void BatchLabMySQLInitialize(Slash::Core::ISession&);
void BatchLabMySQLFinalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
void BatchLabMySQLInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aSession.findManager("MySQLManager")) {
    aSession.addManager(new BatchLab::MySQLManager(aSession));
  }
}
//////////////////////////////////////////////////////////////////////////////
void BatchLabMySQLFinalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("MySQLManager");
}




