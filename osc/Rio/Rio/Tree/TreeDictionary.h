#ifndef Rio_TreeDictionary_h
#define Rio_TreeDictionary_h

#include <Rio/Graf/GrafDictionary.h>

namespace Rio {

class TreeDictionary : public GrafDictionary {
public: //Rio::IDictionary
  virtual bool initialize();

  virtual const IClass& opt_Basket_Class() const;
  virtual const IClass& opt_BaseLeaf_Class() const;
  virtual const IClass& opt_Branch_Class() const;
  virtual const IClass& opt_Tree_Class() const;
  virtual const IClass& opt_LeafFloat_Class() const;
  virtual const IClass& opt_LeafDouble_Class() const;
  virtual const IClass& opt_LeafShort_Class() const;
  virtual const IClass& opt_LeafInteger_Class() const;
  virtual const IClass& opt_LeafCharacter_Class() const;
  virtual const IClass& opt_LeafElement_Class() const;
  virtual const IClass& opt_LeafObject_Class() const;
  virtual const IClass& opt_LeafString_Class() const;
  virtual const IClass& opt_BranchObject_Class() const;
  virtual const IClass& opt_BranchElement_Class() const;
public:
  TreeDictionary(std::ostream&);
  virtual ~TreeDictionary();
private:
  IClass* f_opt_Basket_Class;
  IClass* f_opt_BaseLeaf_Class;
  IClass* f_opt_Branch_Class;
  IClass* f_opt_Tree_Class;
  IClass* f_opt_LeafFloat_Class;
  IClass* f_opt_LeafDouble_Class;
  IClass* f_opt_LeafShort_Class;
  IClass* f_opt_LeafInteger_Class;
  IClass* f_opt_LeafCharacter_Class;
  IClass* f_opt_LeafElement_Class;
  IClass* f_opt_LeafObject_Class;
  IClass* f_opt_LeafString_Class;
  IClass* f_opt_BranchObject_Class;
  IClass* f_opt_BranchElement_Class;
};

}

#endif

