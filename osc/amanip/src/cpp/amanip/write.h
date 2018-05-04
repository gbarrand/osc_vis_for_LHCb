#ifndef amanip_write_h
#define amanip_write_h

#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IAxis.h>

#include "inlib.h"
#include "axis.h"

namespace amanip {

// used in BatchLab/FS_Tree. 
// The FS::Tree::ASCII format had not been AIDA discussed.
inline bool write(const AIDA::IHistogram1D& a_o,std::ostream& a_io,bool aHeader,bool /*aTabSep*/){
  if(aHeader) {

    a_io << ain_lib::sout(a_o.title()) << std::endl;

  // <axis> :
 {const AIDA::IAxis& axis = a_o.axis();
  a_io << "x"
       << " " << axis.bins() 
       << " " << ain_lib::d2s(axis.lowerEdge()) 
       << " " << ain_lib::d2s(axis.upperEdge()); 
  if(axis.isFixedBinning()) {
  } else {
    int number = axis.bins()-1;
    for(int index=0;index<number;index++) {
      a_io << " " << ain_lib::d2s(axis.binUpperEdge(index));
    }
  }
  a_io << std::endl;}

  // <statistics> : 
  a_io << a_o.entries() << std::endl;
  a_io << "x"
       << " " << ain_lib::d2s(a_o.mean()) 
       << " " << ain_lib::d2s(a_o.rms()) 
       << std::endl;

  // bins :
#define OUT_BIN_1D(aIndex)\
    a_io << bin_to_string(aIndex)\
         << " " << a_o.binEntries(aIndex)\
         << " " << ain_lib::d2s(a_o.binHeight(aIndex))\
         << " " << ain_lib::d2s(a_o.binError(aIndex))\
         << " " << ain_lib::d2s(a_o.binMean(aIndex));\
  /*a_io << " " << ain_lib::d2s(object->binStdDev(aIndex));*/\
    a_io << std::endl;

  OUT_BIN_1D(AIDA::IAxis::UNDERFLOW_BIN)
  OUT_BIN_1D(AIDA::IAxis::OVERFLOW_BIN)

  } //aHeader

 {int xbins = a_o.axis().bins();
  for(int index=0;index<xbins;index++) {
    OUT_BIN_1D(index)
  }}

#undef OUT_BIN_1D

  return true;  
}

inline bool write(const AIDA::IHistogram2D& a_o,std::ostream& a_io,bool aHeader,bool /*aTabSep*/){
  if(aHeader) {

    a_io << ain_lib::sout(a_o.title()) << std::endl;

  // <axis> :
 {const AIDA::IAxis& axis = a_o.xAxis();
  a_io << "x"
       << " " << axis.bins() 
       << " " << ain_lib::d2s(axis.lowerEdge()) 
       << " " << ain_lib::d2s(axis.upperEdge()); 
  if(axis.isFixedBinning()) {
  } else {
    int number = axis.bins()-1;
    for(int index=0;index<number;index++) {
      a_io << " " << ain_lib::d2s(axis.binUpperEdge(index));
    }
  }
  a_io << std::endl;}

 {const AIDA::IAxis& axis = a_o.yAxis();
  a_io << "y"
       << " " << axis.bins() 
       << " " << ain_lib::d2s(axis.lowerEdge()) 
       << " " << ain_lib::d2s(axis.upperEdge()); 
  if(axis.isFixedBinning()) {
  } else {
    int number = axis.bins()-1;
    for(int index=0;index<number;index++) {
      a_io << " " << ain_lib::d2s(axis.binUpperEdge(index));
    }
  }
  a_io << std::endl;}

  // <statistics> : 
  a_io << a_o.entries() << std::endl;
  a_io << "x"
       << " " << ain_lib::d2s(a_o.meanX()) 
       << " " << ain_lib::d2s(a_o.rmsX()) 
       << std::endl;
  a_io << "y"
       << " " << ain_lib::d2s(a_o.meanY()) 
       << " " << ain_lib::d2s(a_o.rmsY()) 
       << std::endl;

  // bins :
#define OUT_BIN_2D(aIndexX,aIndexY)\
      a_io << bin_to_string(aIndexX)\
           << " " << bin_to_string(aIndexY)\
           << " " << a_o.binEntries(aIndexX,aIndexY)\
           << " " << ain_lib::d2s(a_o.binHeight(aIndexX,aIndexY))\
           << " " << ain_lib::d2s(a_o.binError(aIndexX,aIndexY))\
           << " " << ain_lib::d2s(a_o.binMeanX(aIndexX,aIndexY))\
           << " " << ain_lib::d2s(a_o.binMeanY(aIndexX,aIndexY));\
    /*a_io << " " << ain_lib::d2s(object->binStdDevX(aIndexX,aIndexY));\
      a_io << " " << ain_lib::d2s(object->binStdDevY(aIndexX,aIndexY));*/\
      a_io << std::endl;

  OUT_BIN_2D(AIDA::IAxis::UNDERFLOW_BIN,AIDA::IAxis::UNDERFLOW_BIN)
  OUT_BIN_2D(AIDA::IAxis::UNDERFLOW_BIN,AIDA::IAxis::OVERFLOW_BIN)
  OUT_BIN_2D(AIDA::IAxis::OVERFLOW_BIN,AIDA::IAxis::UNDERFLOW_BIN)
  OUT_BIN_2D(AIDA::IAxis::OVERFLOW_BIN,AIDA::IAxis::OVERFLOW_BIN)

  } //aHeader

 {int xbins = a_o.xAxis().bins();
  int ybins = a_o.yAxis().bins();
  int indexX,indexY;
  for(indexX=0;indexX<xbins;indexX++) {
    for(indexY=0;indexY<ybins;indexY++) {
      OUT_BIN_2D(indexX,indexY)
    }
  }}

#undef OUT_BIN_2D

  return true;  
}

}

#endif
