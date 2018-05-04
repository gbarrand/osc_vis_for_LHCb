#ifndef OnX_WidgetClass_h
#define OnX_WidgetClass_h

#include <vector>

// Inheritance :
#include <Slash/UI/IWidgetClass.h>

namespace OnX {

class WidgetClass : public virtual Slash::UI::IWidgetClass {
public: //Slash::UI::IWidgetClass
  virtual std::string name() const;
  virtual std::vector<std::string> attributes() const;
  virtual void setAttributes (const std::vector<std::string>&);
  virtual std::vector<std::string> children() const;
  virtual std::vector<std::string> properties() const;
  virtual std::vector<std::string> callbacks() const;
public: 
  /** 
   * Setting parameters of this widget class.
   * @param aClass name of this widget class
   * @param aChildren possible children widget class
   * @param aAttributes attributes of this widget class
   * @param aProperties properties
   * @param aCallbacks callbacks
   */
  WidgetClass(const std::string& aClass,
              const std::vector<std::string>& aChildren,
              const std::vector<std::string>& aAttributes,
              const std::vector<std::string>& aProperties,
              const std::vector<std::string>& aCallbacks);
  WidgetClass(const WidgetClass&);
  virtual ~WidgetClass();
private :
  std::string fName;
  std::vector<std::string> fChildren;
  std::vector<std::string> fAttributes;
  std::vector<std::string> fProperties;
  std::vector<std::string> fCallbacks;
};
}
#endif




