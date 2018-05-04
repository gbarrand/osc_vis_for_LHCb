#ifndef OnX_Page_h
#define OnX_Page_h

#include <OnX/Interfaces/IPage_deprec.h>

#include <OnX/Inventor/Region_deprec.h>
#include <list>

class SoPage;
class SoRegion;

namespace OnX {

class Page : public virtual IPage {
public: //IPage
  virtual bool createRegions(int,int,int,const std::string& = "");
  virtual bool addRegions(int,int,const std::string& = "");
  virtual IRegion* createRegion(double,double,double,double,const std::string& = "");
  virtual IRegion* createGridRegion(int,int,int,const std::string& = "");
  virtual bool setCurrentRegion(int = 0);
  virtual bool setCurrentRegion(IRegion&);
  virtual bool clear();
  virtual bool next();
  virtual bool previous();
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&) const;
  virtual bool toggleParameter(const std::string&);
  virtual bool refresh();
  virtual void deleteRegion();
  virtual void deleteRegions();
  virtual IRegion* currentRegion() const;
  virtual IRegion* region(int) const;
  virtual int currentRegionIndex() const;
  virtual void currentRegionToPage();
  virtual void currentRegionOnTop();
  virtual int numberOfRegions() const;
  virtual bool setVerboseLevel(int);
  virtual void* nativePage() const;
public:
  Page(SoPage*);
  Page(const Page&);
  Page& operator=(const Page&);
  virtual ~Page();
private:
  IRegion* getIRegion(SoRegion*);
private:
  SoPage* fSoPage;
  std::list<OnX::Region> fRegions;
};

}

#endif
