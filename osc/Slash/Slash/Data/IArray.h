#ifndef Slash_Data_IArray_h
#define Slash_Data_IArray_h

#include <string>
#include <vector>

//namespace AIDA {class IAnnotation;}

namespace Slash {

namespace Data {

class IArray {
public: 
    virtual ~IArray() {}
public: 
    typedef std::vector< std::pair<unsigned int,unsigned int> > Cut;
public: 
    //virtual AIDA::IAnnotation & annotation() = 0;
    //virtual const AIDA::IAnnotation & annotation() const = 0;

    virtual std::string title() const = 0;
    virtual bool setTitle(const std::string & title) = 0;

    virtual unsigned int dimension() const = 0;
    virtual unsigned int size() const = 0;

    virtual const std::vector<unsigned int>& orders() const = 0;

    virtual bool setValue(const std::vector<unsigned int>&,double) = 0;
    virtual bool value(const std::vector<unsigned int>&,double&) const = 0;

    virtual bool fill(const std::vector<double>&,Cut* = 0) = 0;

    virtual bool copy(const IArray& aFrom) = 0;
    virtual bool add(const IArray& aArray,Cut* = 0) = 0;
    virtual bool subtract(const IArray& aArray) = 0;
    virtual bool multiply(const IArray& aArray) = 0;
    virtual bool divide(const IArray& aArray) = 0;
    virtual bool add(double,Cut* = 0) = 0;
    virtual bool multiply(double) = 0;
    virtual bool invert() = 0;
    virtual bool indices(unsigned int,std::vector<unsigned int>&) const = 0;
    virtual const std::vector<double>& vector() const = 0;
    virtual std::vector<double>& vector() = 0;
    virtual bool accept(unsigned int,const Cut&) const = 0;

    virtual void* cast(const std::string& className) const = 0;

};

} //Data

} //Slash

#endif
