#ifndef BatchLab_Rio_Dictionary_h
#define BatchLab_Rio_Dictionary_h

// Inheritance :
#include <Rio/Tree/TreeDictionary.h>

namespace BatchLab {
namespace Rio {

class Dictionary : public ::Rio::TreeDictionary {
public: //Rio::IDictionary
  virtual bool initialize();
public:
  Dictionary(std::ostream&);
  virtual ~Dictionary();
};

} //Rio
} //BatchLab

#endif
