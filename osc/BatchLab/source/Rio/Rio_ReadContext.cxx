// this :
#include <BatchLab/Rio/ReadContext.h>

BatchLab::Rio::ReadContext::ReadContext(
 std::ostream& a_out
,::Rio::IObject& aObject
,const std::string& aSID
,int aVerboseLevel
)
:f_out(a_out)
,fObject(aObject)
,fSID(aSID)
,fVerboseLevel(aVerboseLevel)
{}

BatchLab::Rio::ReadContext::~ReadContext(){}

std::ostream& BatchLab::Rio::ReadContext::out() const {return f_out;}

::Rio::IObject& BatchLab::Rio::ReadContext::object() const {return fObject;}

const std::string& BatchLab::Rio::ReadContext::SID() const {return fSID;}

int BatchLab::Rio::ReadContext::verboseLevel() const {return fVerboseLevel;}
