#ifndef BatchLab_Function_h
#define BatchLab_Function_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <AIDA/IFunction.h>
#include <Slash/Meta/IObject.h>

#include <BatchLab/Core/Annotation.h>

#include <inlib/func>

#include <Lib/Functions.h>

namespace Slash {namespace Data {class IFunction;}}

// For compiled :
namespace Slash {namespace Data {class IArray;}}

namespace BatchLab {

class BaseFunction 
:public BaseObject
,public virtual AIDA::IFunction 
//FIXME ,public virtual AIDA::IModelFunction  //UseAIDA example SimpleFit.
,public virtual Slash::Store::IStorable {
public: //AIDA::IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA::IFunction
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual bool isEqual(const IFunction&) const;
  virtual const std::vector<double>& gradient(const std::vector<double>&) const;  
  virtual bool providesGradient() const;
  virtual std::string variableName(int) const;
  virtual std::vector<std::string> variableNames() const;
  virtual std::vector<std::string> parameterNames() const;
  virtual const std::vector<double>& parameters() const;
  virtual int numberOfParameters() const;
  virtual int indexOfParameter(const std::string&) const;
  virtual std::string codeletString() const;
  virtual bool setParameter(const std::string&,double);
  virtual std::string normalizationParameter() const; //AIDA-v3r3p0
  virtual double value(const std::vector<double>&) const;
/*
public: //AIDA::IModelFunction
  virtual bool providesNormalization() const;
  virtual void normalize(bool on);
  virtual bool isNormalized() const;
  virtual const std::vector<double>& parameterGradient(const std::vector<double>&) const;
  virtual bool providesParameterGradient() const;
  virtual IRangeSet & normalizationRange(int iAxis);
  virtual void includeNormalizationAll();
  virtual void excludeNormalizationAll();
*/
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //For OnXLab::PlottableFunction :
  virtual bool value(const std::vector<double>&,double&) const = 0;
protected:
  BaseFunction(std::ostream&,const std::string&,const std::string&);
  BaseFunction(const BaseFunction&);
public:
  virtual ~BaseFunction();
  Annotation& getAnnotation();
  void setAnnotation(const Annotation&);
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const BaseFunction* local = 
      INLIB_CONST_CAST(aObject,BatchLab::BaseFunction);
    if(!local) return false;
    return local->BatchLab::BaseFunction::visit(aVisitor);
  }
protected:
  static const int NotFound = -1;
protected:
  Annotation fAnnotation;
private:
  std::string fTitle;
protected:
  std::string fCodelet;
  std::vector<double> fParams;
  std::vector<std::string> fParamNames;
  std::vector<std::string> fVariableNames;
  std::vector<double> fGradient;
};

namespace Function {

class Gaussian 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Gaussian");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Gaussian(std::ostream&,const std::string& = "");
  Gaussian(std::ostream&,
           const std::string&,
           const std::string&,
           double = 1,double = 0,double = 1);
  Gaussian(const Gaussian&);
  virtual ~Gaussian();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Gaussian* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::Gaussian);
    if(!local) return false;
    return local->BatchLab::Function::Gaussian::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::Gaussian",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  inlib::func::gaussian fFunction;
};


class BreitWigner 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::BreitWigner");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  BreitWigner(std::ostream&,const std::string& = "");
  BreitWigner(std::ostream&,
              const std::string&,
              const std::string&,
              double = 1,double = 0,double = 1);
  BreitWigner(const BreitWigner&);
  virtual ~BreitWigner();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const BreitWigner* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::BreitWigner);
    if(!local) return false;
    return local->BatchLab::Function::BreitWigner::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::BreitWigner",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  inlib::func::bw fFunction;
};


class ExpHBOOK 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::ExpHBOOK");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  ExpHBOOK(std::ostream&,const std::string& = "");
  ExpHBOOK(std::ostream&,
              const std::string&,
              const std::string&,
              double = 0,double = 1);
  ExpHBOOK(const ExpHBOOK&);
  virtual ~ExpHBOOK();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const ExpHBOOK* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::ExpHBOOK);
    if(!local) return false;
    return local->BatchLab::Function::ExpHBOOK::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::ExpHBOOK",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  inlib::func::expo fFunction;
};


class ExpAIDA 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::ExpAIDA");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  ExpAIDA(std::ostream&,const std::string& = "");
  ExpAIDA(std::ostream&,
               const std::string&,
               const std::string&,
               double = 1,double = 1);
  ExpAIDA(const ExpAIDA&);
  virtual ~ExpAIDA();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const ExpAIDA* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::ExpAIDA);
    if(!local) return false;
    return local->BatchLab::Function::ExpAIDA::visit(aVisitor);
  }
private:
  inlib::func::expo fFunction;
};


class Polynomial 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Polynomial");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Polynomial(std::ostream&,const std::string& = "");
  Polynomial(std::ostream&,
             const std::string&,
             const std::string&,
             const std::vector<double>&);
  Polynomial(const Polynomial&);
  virtual ~Polynomial();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Polynomial* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::Polynomial);
    if(!local) return false;
    return local->BatchLab::Function::Polynomial::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::Polynomial",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  inlib::func::polynomial fFunction;
};

class Cauchy 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Cauchy");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Cauchy(std::ostream&,const std::string& = "");
  Cauchy(std::ostream&,
           const std::string&,
           const std::string&,
           double = 1,double = 0,double = 1);
  Cauchy(const Cauchy&);
  virtual ~Cauchy();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Cauchy* local = INLIB_CONST_CAST(aObject,BatchLab::Function::Cauchy);
    if(!local) return false;
    return local->BatchLab::Function::Cauchy::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::Cauchy",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  inlib::func::cauchy fFunction;
};

class Scripted 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Scripted");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Scripted(std::ostream&,const std::string& = "");
  Scripted(std::ostream&,
                   const std::string&,
                   const std::string&,
                   const std::string&);
  Scripted(const Scripted&);
  virtual ~Scripted();
  bool isValid() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Scripted* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::Scripted);
    if(!local) return false;
    return local->BatchLab::Function::Scripted::visit(aVisitor);
  }
private:
  Lib::Function::Scripted fFunction;
};

class Composite 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Composite");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Composite(std::ostream&,const std::string& = "");
  Composite(std::ostream&,
                   const std::string&,
                   const std::string&,
                   const std::string&);
  Composite(const Composite&);
  virtual ~Composite();
  bool isValid() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Composite* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::Composite);
    if(!local) return false;
    return local->BatchLab::Function::Composite::visit(aVisitor);
  }
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Function::Composite",s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
    if(!BaseFunction::visit(aVisitor)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
private:
  Lib::Function::Composite fFunction;
};

class Compiled 
:public BaseFunction
,public virtual Slash::Meta::IObject {
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::Compiled");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const;
  virtual bool setParameter(const std::string&,double);
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  Compiled(std::ostream&,const std::string& = "");
  Compiled(std::ostream&,
           const std::string&,
           const std::string&,
           Slash::Data::IFunction*,
           std::vector<Slash::Data::IArray*> arrays = 
             std::vector<Slash::Data::IArray*>(),
           bool retain_arrays = true);
  Compiled(const Compiled&);
  virtual ~Compiled();
public:
  Slash::Data::IFunction* slashFunction() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Compiled* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::Compiled);
    if(!local) return false;
    return local->BatchLab::Function::Compiled::visit(aVisitor);
  }
private:
  void reset();
private:
  Slash::Data::IFunction* fFunction;
  std::string fModel;
  std::string fFile;
  std::string fSignature;
  std::vector<Slash::Data::IArray*> fArrays;
  bool fRetainArrays;
};

class InsidePolygon 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::InsidePolygon");
    return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const{return 2;}

  virtual std::vector<std::string> parameterNames() const;
  virtual const std::vector<double>& parameters() const;
  virtual int numberOfParameters() const;
  virtual int indexOfParameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;

public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  static std::string codelet();
public:
  InsidePolygon(std::ostream&,const std::string& = "");
  InsidePolygon(std::ostream&,const std::string&,const std::string&);
  InsidePolygon(const InsidePolygon&);
  virtual ~InsidePolygon();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const InsidePolygon* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::InsidePolygon);
    if(!local) return false;
    return local->BatchLab::Function::InsidePolygon::visit(aVisitor);
  }
private:
  typedef inlib::func::in_polygon::point Point;
  inlib::func::in_polygon fFunction;
};

class InsideEllipse 
:public BaseFunction 
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Function::InsideEllipse");
    return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
public: //AIDA/IFunction
  virtual int dimension() const{return 2;}
  virtual bool setParameters(const std::vector<double>&);
  virtual double parameter(const std::string&) const;
  virtual bool setParameter(const std::string&,double);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BaseFunction
  virtual bool value(const std::vector<double>&,double&) const;
public:
  static std::string codelet();
public:
  InsideEllipse(std::ostream&,const std::string& = "");
  InsideEllipse(std::ostream&,const std::string&,const std::string&);
  InsideEllipse(const InsideEllipse&);
  virtual ~InsideEllipse();
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const InsideEllipse* local = 
      INLIB_CONST_CAST(aObject,BatchLab::Function::InsideEllipse);
    if(!local) return false;
    return local->BatchLab::Function::InsideEllipse::visit(aVisitor);
  }
private:
  inlib::func::in_ellipse fFunction;
};

} //Function

} //BatchLab

#include "Function.icc"

#endif
