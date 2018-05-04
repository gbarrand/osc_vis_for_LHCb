#ifndef User_Track_h
#define User_Track_h

//
// Example of simple "user" data to be visualized.
//

namespace User {

class Track {
public:
  Track(double aPX,double aPY,double aPZ);
  Track(const Track&);
  virtual ~Track();
  Track& operator=(const Track&);
  void setImpulse(double aPX,double aPY,double aPZ);
  double px() const;
  double py() const;
  double pz() const;
private:
  double fSize;
  double fPX;
  double fPY;
  double fPZ;
};

}

#endif
