// this :
#include <User/Track.h>

User::Track::Track(double aPX,double aPY,double aPZ)
:fPX(aPX),fPY(aPY),fPZ(aPZ){}

User::Track::Track(const Track& aFrom)
:fPX(aFrom.fPX),fPY(aFrom.fPY),fPZ(aFrom.fPZ){}

User::Track::~Track() {}

User::Track& User::Track::operator=(const Track& aFrom) {
  fPX = aFrom.fPX;
  fPY = aFrom.fPY;
  fPZ = aFrom.fPZ;
  return *this;
}
void User::Track::setImpulse(double aPX,double aPY,double aPZ) {
  fPX = aPX;
  fPY = aPY;
  fPZ = aPZ;
}
double User::Track::px() const { return fPX;}
double User::Track::py() const { return fPY;}
double User::Track::pz() const { return fPZ;}

