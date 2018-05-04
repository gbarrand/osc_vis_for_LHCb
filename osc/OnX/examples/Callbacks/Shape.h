#ifndef OnX_Shape_h
#define OnX_Shape_h

#include <string>

//#include <Lib/Debug.h>

namespace OnX {

class Shape {
public:
  Shape(const std::string& aType,
        double aSize,
        double aX,double aY,double aZ)
  :fType(aType),fSize(aSize),fX(aX),fY(aY),fZ(aZ){
    //Lib::Debug::increment("OnX::Shape");
  }
  virtual ~Shape() {
    //Lib::Debug::decrement("OnX::Shape");
  }
  Shape(const Shape& aFrom)
  :fType(aFrom.fType)
  ,fSize(aFrom.fSize)
  ,fX(aFrom.fX)
  ,fY(aFrom.fY)
  ,fZ(aFrom.fZ)
  {}
  Shape& operator=(const Shape& aFrom) {
    fType = aFrom.fType;
    fSize = aFrom.fSize;
    fX = aFrom.fX;
    fY = aFrom.fY;
    fZ = aFrom.fZ;
    return *this;
  }
public:
  inline const std::string& type() const { return fType;}
  inline double size() const { return fSize;}
  inline void setSize(double aX,double aY,double aZ) { 
    fX = aX;fY = aY;fZ = aZ;
  }
  inline double x() const { return fX;}
  inline double y() const { return fY;}
  inline double z() const { return fZ;}
private:
  std::string fType;
  double fSize;
  double fX;
  double fY;
  double fZ;
};

}

#endif
