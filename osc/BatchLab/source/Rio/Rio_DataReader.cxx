// this :
#include <BatchLab/Rio/DataReader.h>

// Slash :
#include <Slash/Store/IStorable.h>

#include <amanip/annotation.h>

// Rio :
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IFile.h>
#include <Rio/Tree/Tree.h>

// BatchLab :
#include <BatchLab/Rio/ReadContext.h>

#include <BatchLab/Core/Histogram.h>

#include <BatchLab/Rio/ROOT.h>

#include <BatchLab/Rio/Tuple.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_TH1F(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  Rio::TH1F* rioHisto = ::Rio::cast<Rio::TH1F>(aContext.object());
  if(!rioHisto) return 0;

  inlib::histo::h1d* hclHisto = rioHisto->hclHistogram();
  if(!hclHisto) return 0; //Reading could fail.

  BatchLab::BaseHistogram* baseHistogram = 0;
  const std::string& aSID = aContext.SID();
  int dim = hclHisto->get_dimension();
  if(dim!=1) {
    aContext.out() << "BatchLab::Rio::read_TH1F :"
        << " dimension mismatch for " << inlib::sout(aSID)
        << " dimension " << dim << " (1 expected)."
        << std::endl;
  } else {
    baseHistogram = 
      new BatchLab::Histogram1D(aContext.out(),aSID,*hclHisto);
    Annotation a;
    amanip::from(rioHisto->annotation(),a);
    baseHistogram->setAnnotation(a);
  }
  delete hclHisto;

  return baseHistogram;
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_TH2F(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  Rio::TH2F* rioHisto = ::Rio::cast<Rio::TH2F>(aContext.object());
  if(!rioHisto) return 0;

  inlib::histo::h2d* hclHisto = rioHisto->hclHistogram();
  if(!hclHisto) return 0; //Reading could fail.

  BatchLab::BaseHistogram* baseHistogram = 0;
  const std::string& aSID = aContext.SID();
  int dim = hclHisto->get_dimension();
  if(dim!=2) {
    aContext.out() << "BatchLab::Rio::read_TH2F :"
        << " dimension mismatch for " << inlib::sout(aSID)
        << " dimension " << dim << " (1 expected)."
        << std::endl;
  } else {
    baseHistogram = 
      new BatchLab::Histogram2D(aContext.out(),aSID,*hclHisto);
    Annotation a;
    amanip::from(rioHisto->annotation(),a);
    baseHistogram->setAnnotation(a);
  }
  delete hclHisto;

  return baseHistogram;
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_TH1D(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  Rio::TH1D* rioHisto = ::Rio::cast<Rio::TH1D>(aContext.object());
  if(!rioHisto) return 0;

  inlib::histo::h1d* hclHisto = rioHisto->hclHistogram();
  if(!hclHisto) return 0; //Reading could fail.

  BatchLab::BaseHistogram* baseHistogram = 0;
  const std::string& aSID = aContext.SID();
  int dim = hclHisto->get_dimension();
  if(dim!=1) {
    aContext.out() << "BatchLab::Rio::read_TH1D :"
        << " dimension mismatch for " << inlib::sout(aSID)
        << " dimension " << dim << " (1 expected)." 
        << std::endl;
  } else {
    baseHistogram = 
      new BatchLab::Histogram1D(aContext.out(),aSID,*hclHisto);
    Annotation a;
    amanip::from(rioHisto->annotation(),a);
    baseHistogram->setAnnotation(a);
  }
  delete hclHisto;

  return baseHistogram;
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_TH2D(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  Rio::TH2D* rioHisto = ::Rio::cast<Rio::TH2D>(aContext.object());
  if(!rioHisto) return 0;

  inlib::histo::h2d* hclHisto = rioHisto->hclHistogram();
  if(!hclHisto) return 0; //Reading could fail.

  BatchLab::BaseHistogram* baseHistogram = 0;
  const std::string& aSID = aContext.SID();
  int dim = hclHisto->get_dimension();
  if(dim!=2) {
    aContext.out() << "BatchLab::Rio::read_TH2D :"
        << " dimension mismatch for " << inlib::sout(aSID)
        << " dimension " << dim << " (2 expected)."
        << std::endl;
  } else {
    baseHistogram = 
      new BatchLab::Histogram2D(aContext.out(),aSID,*hclHisto);
    Annotation a;
    amanip::from(rioHisto->annotation(),a);
    baseHistogram->setAnnotation(a);
  }
  delete hclHisto;

  return baseHistogram;
}

//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_TProfile(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  Rio::TProfile* rioHisto = ::Rio::cast<Rio::TProfile>(aContext.object());
  if(!rioHisto) return 0;

  inlib::histo::p1d* hclHisto = rioHisto->hclHistogram();
  if(!hclHisto) return 0; //Reading could fail.

  BatchLab::BaseHistogram* baseHistogram = 0;
  const std::string& aSID = aContext.SID();
  int dim = hclHisto->get_dimension();
  if(dim!=1) {
    aContext.out() << "BatchLab::Rio::read_TProfile :"
        << " dimension mismatch for " << inlib::sout(aSID)
        << " dimension " << dim << " (1 expected)." 
        << std::endl;
  } else {
    baseHistogram = 
      new BatchLab::Profile1D(aContext.out(),aSID,*hclHisto);
    Annotation a;
    amanip::from(rioHisto->annotation(),a);
    baseHistogram->setAnnotation(a);
  }
  delete hclHisto;

  return baseHistogram;
}


//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::read_Tree(
 Rio::ReadContext& aContext
,bool& aDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aDelete = true;
  ::Rio::Tree* rioTree = ::Rio::cast< ::Rio::Tree>(aContext.object());
  if(!rioTree) return 0;
  aDelete = false;
  const std::string& aSID = aContext.SID();
  return new BatchLab::Rio::Tuple
           (aContext.out(),aSID,rioTree,aContext.verboseLevel());
}
