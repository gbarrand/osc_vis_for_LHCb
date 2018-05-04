#ifndef Slash_Data_IVisualizer_h
#define Slash_Data_IVisualizer_h

#include <Slash/Data/IAccessor.h> //for Slash::Data::IAccessor::Data

namespace Slash {namespace UI {class IStyle;}}

namespace Slash {
namespace Data {

class IVisualizer {
public:
  virtual ~IVisualizer() {}
public:
  virtual void beginVisualize(const Slash::UI::IStyle&) = 0;
  virtual void beginVisualize() = 0;
  virtual void visualize(Slash::Data::IAccessor::Data,void*) = 0;
  virtual void endVisualize() = 0;
};

}}

#endif
