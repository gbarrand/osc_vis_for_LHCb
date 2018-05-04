
#include <Lib/BaseType.h> //to check compilation.

// To test the type manager :

#include <Slash/Data/IIterator.h>

#include <Slash/Tools/BaseAccessor.h>

#include <Lib/AccessorManager.h>
#include <Lib/Value.h>
#include <Lib/Debug.h>

#include <inlib/sprintf>
#include <inlib/pointer>

#include <iostream>

#define DATA_NUMBER 5

namespace User {

class Data {
public:
  Data(){}
  virtual ~Data() {}
public:
  double fEnergy;
};


class Shape {
public:
  Shape():fType("Cube"),fSize(1),fX(0),fY(0),fZ(0){}
  virtual ~Shape() {}
public:
  void set(const std::string& aType,
           double aSize,double aX,double aY,double aZ,
           const std::string& aData)
  {
    fType = aType;
    fSize = aSize;
    fX = aX;
    fY = aY;
    fZ = aZ;
    fData = aData;
  }
  const std::string& type() const { return fType;}
  double size() const { return fSize;}
  double x() const { return fX;}
  double y() const { return fY;}
  double z() const { return fZ;}
  const std::string& data() const { return fData;}
private:
  std::string fType;
  double fSize;
  double fX;
  double fY;
  double fZ;
  std::string fData;
};

}

///////////////////////////////////////////////////
/// Accessors /////////////////////////////////////
///////////////////////////////////////////////////

class DataAccessor : public Slash::BaseAccessor {
public: //Slash::Data::IAccessor
  virtual std::string name() const {return fName;} 
  virtual Slash::Data::IIterator* iterator() {

    class Iterator : public virtual Slash::Data::IIterator {
    public: //Slash::Data::IIterator
      virtual Slash::Data::IAccessor::Data object() {
        if(fIndex>=fNumber) return 0;
        return fList+fIndex;
      }
      virtual void* tag() { return 0;}
      virtual void next() { fIndex++;}
    public:
      Iterator(User::Data* aList,unsigned int aNumber)
      :fIndex(0),fList(aList),fNumber(aNumber){}
      virtual ~Iterator(){}
    private:
      unsigned int fIndex;
      User::Data* fList;
      unsigned int fNumber;
    };
    
    return new Iterator(fData,DATA_NUMBER);
  }
  virtual Slash::Core::IValue* findValue(
    Slash::Data::IAccessor::Data aData,
    const std::string& aName,void*){
    User::Data* obj = (User::Data*)aData;
    if(aName=="id") {
      return new Lib::Value((void*)obj);
    } else if(aName=="energy") {
      return new Lib::Value(obj->fEnergy);
    } else {
      return new Lib::Value();
    }
  }
public:
  DataAccessor(std::ostream& a_out,User::Data* aData)
  : Slash::BaseAccessor(a_out),fName("User::Data"),fData(aData){
    addProperty("id",Slash::Data::IProperty::POINTER);
    addProperty("energy",Slash::Data::IProperty::DOUBLE);
  }
  virtual ~DataAccessor() {}
private:
  std::string fName;
  User::Data* fData;
};

class ShapeAccessor : public Slash::BaseAccessor {
public: //Slash::Data::IAccessor
  virtual std::string name() const {return fName;} 
  virtual Slash::Data::IIterator* iterator() {

    class Iterator : public virtual Slash::Data::IIterator {
    public: //Slash::Data::IIterator
      virtual Slash::Data::IAccessor::Data object() {
        if(fIndex>=fNumber) return 0;
        return fList+fIndex;
      }
      virtual void* tag() { return 0;}
      virtual void next() { fIndex++;}
    public:
      Iterator(User::Shape* aList,unsigned int aNumber)
      :fIndex(0),fList(aList),fNumber(aNumber) {
      }
      virtual ~Iterator() {}
    private:
      unsigned int fIndex;
      User::Shape* fList;
      unsigned int fNumber;
    };

    return new Iterator(fShapes,fNumber);
  }
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data aData,
                              const std::string& aName,void*){
    User::Shape* obj = (User::Shape*)aData;
    if(aName=="id") {
      return new Lib::Value((void*)obj);
    } else if(aName=="type") {
      return new Lib::Value(obj->type());
    } else if(aName=="size") {
      return new Lib::Value(obj->size());
    } else if(aName=="x") {
      return new Lib::Value(obj->x());
    } else if(aName=="y") {
      return new Lib::Value(obj->y());
    } else if(aName=="z") {
      return new Lib::Value(obj->z());
    } else if(aName=="data") {
      return new Lib::Value(obj->data());
    } else {
      return new Lib::Value();
    }
  }
public:
  ShapeAccessor(std::ostream& a_out,User::Data* aData)
    : Slash::BaseAccessor(a_out),fName("User::Shape"),fShapes(0),fNumber(0){
    fNumber = 15;
    fShapes = new User::Shape[fNumber];
    std::string sdata01;
    sdata01 += "User::Data/";
    sdata01 += inlib::p2s(aData+0);
    sdata01 += "\n";
    sdata01 += "User::Data/";
    sdata01 += inlib::p2s(aData+1);
    std::string sdata34;
    sdata34 += "User::Data/";
    sdata34 += inlib::p2s(aData+3);
    sdata34 += "\n";
    sdata34 += "User::Data/";
    sdata34 += inlib::p2sx(aData+4);
    fShapes[0].set("Cube",1, 10, 10,0,sdata01);
    fShapes[1].set("Cube",2,-10, 10,0,sdata34);
    fShapes[2].set("Cube",3,-10,-10,0,sdata01);
    fShapes[3].set("Cube",4, 10,-10,0,sdata34);
    fShapes[4].set("Sphere",1,0, 10, 10,sdata01);
    fShapes[5].set("Sphere",2,0,-10, 10,sdata01);
    fShapes[6].set("Sphere",3,0,-10,-10,sdata01);
    fShapes[7].set("Sphere",4,0, 10,-10,sdata01);
    fShapes[8].set("Cylinder",1, 10,0, 10,sdata34);
    fShapes[9].set("Cylinder",2,-10,0, 10,sdata34);
    fShapes[10].set("Cylinder",3,-10,0,-10,sdata34);
    fShapes[11].set("Cylinder",5, 10,0,-10,sdata34);
    fShapes[12].set("Cone",1,0,0,0,sdata01);
    fShapes[13].set("Hole",1,10,10,10,sdata34);
    fShapes[14].set("Hole",1,-10,-10,-10,sdata01);
    
    addProperty("id",Slash::Data::IProperty::POINTER);
    addProperty("type",Slash::Data::IProperty::STRING,8);
    addProperty("size",Slash::Data::IProperty::DOUBLE,4);
    addProperty("x",Slash::Data::IProperty::DOUBLE,4," (m)");
    addProperty("y",Slash::Data::IProperty::DOUBLE,4," (m)");
    addProperty("z",Slash::Data::IProperty::DOUBLE,4," (m)");
    addProperty("data",Slash::Data::IProperty::STRING);    
  }
  virtual ~ShapeAccessor() {delete [] fShapes;}
private:
  std::string fName;
  User::Shape* fShapes;
  unsigned int fNumber;
};

int main(int,char**) {

  for(int i=0;i<100;i++)
 {// Create some Data :
  User::Data* data = new User::Data[DATA_NUMBER];
  data[0].fEnergy = 0.5; 
  data[1].fEnergy = 0.75; 
  data[2].fEnergy = 1; 
  data[3].fEnergy = 1.5; 
  data[4].fEnergy = 1.75; 

  Lib::AccessorManager typeManager(std::cout);
  typeManager.addAction(new Lib::CollectAction(typeManager));
  typeManager.addAction(new Lib::FilterAction(typeManager));
  typeManager.addAction(new Lib::DumpAction(typeManager));
  typeManager.addAction(new Lib::NumberAction(typeManager));
  typeManager.addAction(new Lib::GetValuesAction(typeManager));
  typeManager.addAction(new Lib::DestroyAction(typeManager));
  typeManager.addAction(new Lib::SetAction(typeManager));
  typeManager.addAction(new Lib::CalculateAction(typeManager));

  typeManager.addAccessor(new DataAccessor(std::cout,data));
  typeManager.addAccessor(new ShapeAccessor(std::cout,data));

  std::vector<std::string> args;
  args.push_back("User::Shape");
  args.push_back("type==\"Cube\"");
  typeManager.execute("collect",args);
  
  args.clear();
  typeManager.execute("dump",args);
  
  args.clear();
  args.push_back("data");
  typeManager.execute("filter",args);
  args.clear();
  typeManager.execute("dump",args);

  args.clear();
  args.push_back("-");
  args.push_back("energy>=1");
  typeManager.execute("collect",args);
  args.clear();
  typeManager.execute("dump",args);

  args.clear();
  args.push_back("-u");
  args.push_back("energy>=1");
  typeManager.execute("collect",args);
  args.clear();
  typeManager.execute("dump",args);

  typeManager.eraseHandlers();

  delete [] data;
  }
  
  Lib::Debug::balance(std::cout);

  return 0;
}
