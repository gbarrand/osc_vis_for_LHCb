// this :
#include <BatchLab/HDF5/ReadContext.h>

BatchLab::HDF5::ReadContext::ReadContext(
 int aVerboseLevel
,std::ostream& a_out
,hid_t aID
,const std::string& aName
,int aVersion
,HDF5O& aHDF5O
)
:fVerboseLevel(aVerboseLevel)
,f_out(a_out)
,fID(aID)
,fName(aName)
,fVersion(aVersion)
,fHDF5O(aHDF5O)
{}

BatchLab::HDF5::ReadContext::~ReadContext(){}

int BatchLab::HDF5::ReadContext::verboseLevel() const{return fVerboseLevel;}

std::ostream& BatchLab::HDF5::ReadContext::out() const {return f_out;}

hid_t BatchLab::HDF5::ReadContext::id() const {return fID;}

std::string BatchLab::HDF5::ReadContext::name() const {return fName;}

int BatchLab::HDF5::ReadContext::version() const {return fVersion;}
BatchLab::HDF5::HDF5O& BatchLab::HDF5::ReadContext::helper() const {return fHDF5O;}
