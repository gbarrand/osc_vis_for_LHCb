#ifndef OnX_Inventor_h
#define OnX_Inventor_h

namespace Slash { namespace Core {class ISession;}}

namespace OnX {

class Inventor {
public:
  static void initialize(Slash::Core::ISession&);
  static void finalize(Slash::Core::ISession&);
};

}

#endif
