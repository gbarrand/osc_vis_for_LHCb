#ifndef BatchLab_Rio_ROOT_h
#define BatchLab_Rio_ROOT_h

#include <inlib/histo/histo_data>

namespace inlib {
namespace histo {
  class h1d;
  class h2d;
  class p1d;
}}

#include <Rio/Core/Named.h>

namespace BatchLab {

namespace Rio {

class TH : public ::Rio::Named {
public:
  TH(const ::Rio::IDictionary&);
  virtual ~TH(){}
public:
  const std::string& annotation() const;
protected:
  bool read1D(::Rio::IBuffer&,inlib::histo::histo_data<double,unsigned int,double>&);
  bool write1D(::Rio::IBuffer&,const inlib::histo::histo_data<double,unsigned int,double>&);

  bool read2D(::Rio::IBuffer&,inlib::histo::histo_data<double,unsigned int,double>&);
  bool write2D(::Rio::IBuffer&,const inlib::histo::histo_data<double,unsigned int,double>&);
protected:
  std::string fAnnotation;
  double fEntries;
  double fSw;
};

class TH1F : public TH {
  RIO_META_HEADER(TH1F)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TH1F(const ::Rio::IDictionary&);
  TH1F(const ::Rio::IDictionary&,inlib::histo::h1d*,const std::string&);
  virtual ~TH1F(){}
public:
  inline inlib::histo::h1d* hclHistogram() const {return fHistogram;}
private:
  inlib::histo::h1d* fHistogram;
};

class TH1D : public TH {
  RIO_META_HEADER(TH1D)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TH1D(const ::Rio::IDictionary&);
  TH1D(const ::Rio::IDictionary&,inlib::histo::h1d*,const std::string&,
       const std::string&);
  virtual ~TH1D(){}
public:
  inline inlib::histo::h1d* hclHistogram() const {return fHistogram;}
protected: //because of TProfile.
  bool stream_it(::Rio::IBuffer&,bool profile);
protected: //because of TProfile.
  inlib::histo::h1d* fHistogram;
};

class TH2F : public TH {
  RIO_META_HEADER(TH2F)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TH2F(const ::Rio::IDictionary&);
  TH2F(const ::Rio::IDictionary&,inlib::histo::h2d*,const std::string&);
  virtual ~TH2F(){}
public:
  inline inlib::histo::h2d* hclHistogram() const {return fHistogram;}
private:
  inlib::histo::h2d* fHistogram;
};

class TH2D : public TH {
  RIO_META_HEADER(TH2D)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TH2D(const ::Rio::IDictionary&);
  TH2D(const ::Rio::IDictionary&,
       inlib::histo::h2d*,
       const std::string&,
       const std::string&);
  virtual ~TH2D(){}
public:
  inline inlib::histo::h2d* hclHistogram() const {return fHistogram;}
private:
  inlib::histo::h2d* fHistogram;
};

//NOTE : in CERN-ROOT TProfile inherits TH1D
//       but inlib::histo::p1d does not inherit inlib::histo::h1d.

class TProfile : public TH1D {
  RIO_META_HEADER(TProfile)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TProfile(const ::Rio::IDictionary&);
  TProfile(const ::Rio::IDictionary&,
           inlib::histo::p1d*,
           const std::string&,
           const std::string&);
  virtual ~TProfile(){}
public:
  inline inlib::histo::p1d* hclHistogram() const {return fProfile;}
private:
  inlib::histo::p1d* fProfile;
};

class TVectorD : public ::Rio::Object {
  RIO_META_HEADER(TVectorD)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TVectorD(const ::Rio::IDictionary&);
  virtual ~TVectorD(){}
};

class TMatrix : public ::Rio::Object {
  RIO_META_HEADER(TMatrix)
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  TMatrix(const ::Rio::IDictionary&);
  virtual ~TMatrix(){}
};

} //Rio

} //BatchLab

#endif
