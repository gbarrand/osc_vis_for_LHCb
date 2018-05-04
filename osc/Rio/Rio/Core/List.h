#ifndef Rio_List_h
#define Rio_List_h

#include <Rio/Core/Object.h>

#include <list>

namespace Rio {

class List : public Object, public std::list<IObject*> {
  RIO_META_HEADER(List)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  List(const IDictionary&);
  List(const List&);
  virtual ~List();
  List& operator=(const List&);
private:
  void reset(); 
private:
  std::string fName;
};

}

#endif
