#ifndef Rio_CoreDictionary_h
#define Rio_CoreDictionary_h

#include <Rio/Interfaces/IDictionary.h>

//
#define RIO_CLASSES_MAP

#ifdef RIO_CLASSES_MAP
#include <map>
#endif

#include <ostream>

namespace Rio {

class CoreDictionary : public virtual IDictionary {
public: //Rio::IDictionary
  virtual bool initialize();
  virtual bool isClass(const std::string&) const;
  virtual IClass* findClass(const std::string&) const;
  virtual IClass* findInStoreClass(const std::string&) const;
  virtual const std::list<IObject*>& streamerInfos() const;
  virtual void addClass(IClass*);
  virtual const IClass& noneClass() const;
  //opt:
  virtual const IClass& opt_File_Class() const;
  virtual const IClass& opt_Directory_Class() const;
  virtual const IClass& opt_Object_Class() const;
  virtual const IClass& opt_Named_Class() const;
  virtual const IClass& opt_Array_Class() const;
  virtual const IClass& opt_Array_int_Class() const;
  virtual const IClass& opt_Array_double_Class() const;
  virtual const IClass& opt_Array_float_Class() const;
  virtual const IClass& opt_List_Class() const;
  virtual const IClass& opt_ObjArray_Class() const;
public:
  CoreDictionary(std::ostream&);
  virtual ~CoreDictionary();
private:
  CoreDictionary(const CoreDictionary&);
  CoreDictionary& operator=(const CoreDictionary&);
public:
  virtual void out() const;
protected:
  std::ostream& fOut;
private:
#ifdef RIO_CLASSES_MAP
  std::map<std::string,IClass*> fClasses;
  std::map<std::string,IClass*> fClasses_store;
#else
  std::list<IClass*> fClasses;
#endif
protected:
  std::list<IObject*> fStreamerInfos;
  IClass* fNoneClass;
  IClass* f_opt_File_Class;
  IClass* f_opt_Directory_Class;
  IClass* f_opt_Object_Class;
  IClass* f_opt_Named_Class;
  IClass* f_opt_Array_Class;
  IClass* f_opt_Array_int_Class;
  IClass* f_opt_Array_float_Class;
  IClass* f_opt_Array_double_Class;
  IClass* f_opt_List_Class;
  IClass* f_opt_ObjArray_Class;
};

}

#endif

