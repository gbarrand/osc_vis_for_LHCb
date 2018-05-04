#ifndef Rio_IDictionary_h
#define Rio_IDictionary_h

#include <string>
#include <list>

namespace Rio {

class IClass;
class IObject;

/**
 * @class IDictionary
 *
 *  IDictionary is the interface to handle class dictionary informations.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IDictionary {
public:
  virtual ~IDictionary() {}
public:
  /** Initialize / fill the dictionary.
   */
  virtual bool initialize() = 0;

  virtual bool isClass(const std::string& aName) const = 0;

  /** Find a class describer given its name.
   * @param aName : The name of the class to find.
   * @return The class describer (an IClass). 
   *         It returns 0 in case of failure. */
  virtual IClass* findClass(const std::string& aName) const = 0;

  /** Find a class describer given its name in file.
   * @param aName : The name of the class to find.
   * @return The class describer (an IClass). 
   *         It returns 0 in case of failure. */
  virtual IClass* findInStoreClass(const std::string& aName) const = 0;

  /** Add a class describer to the dictionary.
   * @param aClass : The class describer. */
  virtual void addClass(IClass* aClass) = 0;

  /** @return The streamer informations. */
  virtual const std::list<IObject*>& streamerInfos() const = 0;

  virtual const IClass& noneClass() const = 0;

  //optimization :
  //Core :
  virtual const IClass& opt_File_Class() const = 0;
  virtual const IClass& opt_Directory_Class() const = 0;
  virtual const IClass& opt_Object_Class() const = 0;
  virtual const IClass& opt_Named_Class() const = 0;
  virtual const IClass& opt_Array_Class() const = 0;
  virtual const IClass& opt_Array_int_Class() const = 0;
  virtual const IClass& opt_Array_double_Class() const = 0;
  virtual const IClass& opt_Array_float_Class() const = 0;
  virtual const IClass& opt_List_Class() const = 0;
  virtual const IClass& opt_ObjArray_Class() const = 0;

  //Graf :
  virtual const IClass& opt_AttLine_Class() const = 0;
  virtual const IClass& opt_AttFill_Class() const = 0;
  virtual const IClass& opt_AttMarker_Class() const = 0;
  
  //Tree :
  virtual const IClass& opt_Basket_Class() const = 0;
  virtual const IClass& opt_BaseLeaf_Class() const = 0;
  virtual const IClass& opt_Branch_Class() const = 0;
  virtual const IClass& opt_Tree_Class() const = 0;
  virtual const IClass& opt_BranchObject_Class() const = 0;
  virtual const IClass& opt_BranchElement_Class() const = 0;

  virtual const IClass& opt_LeafFloat_Class() const = 0;
  virtual const IClass& opt_LeafDouble_Class() const = 0;
  virtual const IClass& opt_LeafShort_Class() const = 0;
  virtual const IClass& opt_LeafInteger_Class() const = 0;
  virtual const IClass& opt_LeafCharacter_Class() const = 0;
  virtual const IClass& opt_LeafElement_Class() const = 0;
  virtual const IClass& opt_LeafObject_Class() const = 0;
  virtual const IClass& opt_LeafString_Class() const = 0;
};

}

#endif
