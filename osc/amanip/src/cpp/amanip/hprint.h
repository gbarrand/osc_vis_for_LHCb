#ifndef amanip_hprint_h
#define amanip_hprint_h

#include <AIDA/IAnnotation.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IAxis.h>
#include <AIDA/ITuple.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IFunction.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>
#include <AIDA/IDataPointSet.h>

#include "inlib.h"
#include "cast.h"

namespace amanip {

inline void hprint(const AIDA::IAnnotation& a_annot,std::ostream& a_out){
  int number = a_annot.size();
  a_out << " * ANNOTATIONS :" << std::endl;
  for(int index=0;index<number;index++) {
    //out << " * (" << index << ") " 
    a_out << " *  "
          << a_annot.key(index)
          << " = " << a_annot.value(index)
          << std::endl;
  }
}

inline void hprint(const AIDA::IHistogram1D& a_h,const std::string& a_name,std::ostream& a_out){
  // A la HPRINT.
  //
  // From cern/src/packlib/hbook/code/hprst.F :
  // 3000 FORMAT(' * ENTRIES =',I11,6X,'* ALL CHANNELS =',E11.4,6X,
  //     +'* UNDERFLOW =',E11.4,6X,'* OVERFLOW =',E11.4,/,
  //     +' * BIN WID =',E11.4,6X,'* MEAN VALUE   =',E11.4,6X,
  //    +'* R . M . S =',E11.4)
  // 4000 FORMAT(' * ENTRIES =',I11,6X,'* ALL CHANNELS =',E11.4,6X,
  //     +'* UNDERFLOW =',E11.4,6X,'* OVERFLOW =',E11.4,/,
  //     +' * BIN WID =',E11.4,6X,'* MEAN VALUE   =',E11.4,6X,
  //     +'* R . M . S =',E11.4,6X,'* NEQUIVAL =',E11.4)

  int field = 13; //HBOOK had 17.

  if(!a_h.allEntries()) {

    a_out << "****** AIDA   NO ENTRIES FOR HISTOGRAM = " << a_name

        << std::endl;

  } else {

    a_out << a_h.dimension() << "D " << a_h.title() << std::endl;
    a_out << " * NAME = " << a_name << std::endl;

    a_out << " * ENTRIES = " << ain_lib::sjust<int>(a_h.allEntries(),field)
        << " * ALL CHANNELS = " << ain_lib::sjust<double>(a_h.sumBinHeights(),field)
        << " * UNDERFLOW = " << ain_lib::sjust<double>(a_h.binHeight(AIDA::IAxis::UNDERFLOW_BIN),field)
        << " * OVERFLOW = " << ain_lib::sjust<double>(a_h.binHeight(AIDA::IAxis::OVERFLOW_BIN),field)
        << std::endl;
    a_out << " * BIN WID = " << ain_lib::sjust<double>(a_h.axis().binWidth(0),field)
        << " * MEAN VALUE   = " << ain_lib::sjust<double>(a_h.mean(),field)
        << " * R . M . S = " << ain_lib::sjust<double>(a_h.rms(),field)
        << std::endl;
    a_out << " * MIN = " << ain_lib::sjust<double>(a_h.minBinHeight(),field+4)
        << " * MAX = " << ain_lib::sjust<double>(a_h.maxBinHeight(),field)
        << std::endl;
      
  }

  hprint(a_h.annotation(),a_out);

}

inline void hprint(const AIDA::ITuple& a_tuple,const std::string& a_name,std::ostream& a_out){
  int field = 13; //HBOOK had 17.

  a_out << " ********************************************************" 
        << std::endl;

  a_out << " * NTUPLE NAME= " << a_name
        << " ENTRIES= " << a_tuple.rows()
        << " TITLE= " << a_tuple.title()
        << std::endl;

  a_out << " ********************************************************" 
        << std::endl;
  a_out << " *  Var numb  *   Name    *    Lower     *    Upper     *" 
        << std::endl;
  a_out << " ********************************************************" 
        << std::endl;

    
  int coln = a_tuple.columns();
  for(int coli=0;coli<coln;coli++) {
    a_out << " *" 
          << ain_lib::sjust(ain_lib::to<int>(coli+1),12,ain_lib::middle) 
          << "* " 
          << ain_lib::sjust(a_tuple.columnName(coli),10) 
          << "* " 
          << ain_lib::sjust(ain_lib::to<double>(a_tuple.columnMin(coli)),13) 
          << "* " 
          << ain_lib::sjust(ain_lib::to<double>(a_tuple.columnMax(coli)),13) 
          << "*" 
          << std::endl;
  }

  a_out << " ********************************************************" 
      << std::endl;

/*

 ********************************************************
 * NTUPLE ID=   10  ENTRIES=   3354   CERN Population 
 ********************************************************
 *  Var numb  *   Name    *    Lower     *    Upper     *
 ********************************************************
 *      1     * CATEGORY  * 0.102000E+03 * 0.567000E+03 *
 *      2     * DIVISION  * 0.100000E+01 * 0.130000E+02 *
 *      3     * FLAG      * 0.000000E+00 * 0.310000E+02 *
 *      4     * AGE       * 0.210000E+02 * 0.640000E+02 *
 *      5     * SERVICE   * 0.000000E+00 * 0.350000E+02 *
 *      6     * CHILDREN  * 0.000000E+00 * 0.600000E+01 *
 *      7     * GRADE     * 0.300000E+01 * 0.140000E+02 *
 *      8     * STEP      * 0.000000E+00 * 0.150000E+02 *
 *      9     * NATION    * 0.100000E+01 * 0.150000E+02 *
 *     10     * HRWEEK    * 0.200000E+01 * 0.440000E+02 *
 *     11     * COST      * 0.686000E+03 * 0.188530E+05 *
 ********************************************************

*/

  hprint(a_tuple.annotation(),a_out);
}

inline void hprint( const AIDA::IProfile1D& a_prof,const std::string& a_name,std::ostream& a_out){

  int field = 13; //HBOOK had 17.

  if(!a_prof.allEntries()) {

    a_out << "****** AIDA   NO ENTRIES FOR PROFILE = " << a_name
          << std::endl;

  } else {

    a_out << a_prof.dimension() << "D " << a_prof.title() << std::endl;
    a_out << " * NAME = " << a_name << std::endl;

    a_out << " * ENTRIES = " << ain_lib::sjust<int>(a_prof.allEntries(),field)
        << " * ALL CHANNELS = " << ain_lib::sjust<double>(a_prof.sumBinHeights(),field)
        << " * UNDERFLOW = " << ain_lib::sjust<double>(a_prof.binHeight(AIDA::IAxis::UNDERFLOW_BIN),field)
        << " * OVERFLOW = " << ain_lib::sjust<double>(a_prof.binHeight(AIDA::IAxis::OVERFLOW_BIN),field)
        << std::endl;
    a_out << " * BIN WID = " << ain_lib::sjust<double>(a_prof.axis().binWidth(0),field)
        << " * MEAN VALUE   = " << ain_lib::sjust<double>(a_prof.mean(),field)
        << " * R . M . S = " << ain_lib::sjust<double>(a_prof.rms(),field)
        << std::endl;
    a_out << " * MIN = " << ain_lib::sjust<double>(a_prof.minBinHeight(),field+4)
        << " * MAX = " << ain_lib::sjust<double>(a_prof.maxBinHeight(),field)
        << std::endl;
      
  }

  hprint(a_prof.annotation(),a_out);
}

inline void hprint(const AIDA::IHistogram2D& a_h,const std::string& a_name,std::ostream& a_out){

  int field = 13; //HBOOK had 17.

  if(!a_h.allEntries()) {

    a_out << "****** AIDA   NO ENTRIES FOR HISTOGRAM = " << a_name
        << std::endl;

  } else {

    a_out << a_h.dimension() << "D " << a_h.title() << std::endl;
    a_out << " * NAME = " << a_name << std::endl;

    a_out << " * ENTRIES = " << a_h.allEntries() << std::endl;

    //   6 | 7 | 8
    //  -----------
    //   3 | 4 | 5
    //  -----------
    //   0 | 1 | 2

    double height_0 = a_h.binHeight(AIDA::IAxis::UNDERFLOW_BIN,
                                   AIDA::IAxis::UNDERFLOW_BIN);
    double height_2 = a_h.binHeight(AIDA::IAxis::OVERFLOW_BIN,
                                   AIDA::IAxis::UNDERFLOW_BIN);
    double height_6 = a_h.binHeight(AIDA::IAxis::UNDERFLOW_BIN,
                                   AIDA::IAxis::OVERFLOW_BIN);
    double height_8 = a_h.binHeight(AIDA::IAxis::OVERFLOW_BIN,
                                   AIDA::IAxis::OVERFLOW_BIN);
      
    int i,j;
      
    double height_1 = 0;
    double height_7 = 0;
    for(i=0;i<a_h.xAxis().bins();i++){
      height_1 += a_h.binHeight(i,AIDA::IAxis::UNDERFLOW_BIN);
      height_7 += a_h.binHeight(i,AIDA::IAxis::OVERFLOW_BIN);
    }
      
    double height_3 = 0;
    double height_5 = 0;
    for(j=0;j<a_h.yAxis().bins();j++){
      height_3 += a_h.binHeight(AIDA::IAxis::UNDERFLOW_BIN,j);
      height_5 += a_h.binHeight(AIDA::IAxis::OVERFLOW_BIN,j);
    }

    double height_4 = 0;
    for(i=0;i<a_h.xAxis().bins();i++){
      for(j=0;j<a_h.yAxis().bins();j++){
        height_4 += a_h.binHeight(i,j);
      }
    }
      
    std::string bar(32,'-');
    a_out << ain_lib::sjust(height_6,10,ain_lib::middle)
        << "|" << ain_lib::sjust<double>(height_7,10,ain_lib::middle) 
        << "|" << ain_lib::sjust<double>(height_8,10,ain_lib::middle) 
        << std::endl;
    a_out << bar << std::endl;
    a_out << ain_lib::sjust(height_3,10,ain_lib::middle)
        << "|" << ain_lib::sjust<double>(height_4,10,ain_lib::middle)
        << "|" << ain_lib::sjust<double>(height_5,10,ain_lib::middle)
        << std::endl;
    a_out << bar << std::endl;
    a_out << ain_lib::sjust<double>(height_0,10,ain_lib::middle)
        << "|" << ain_lib::sjust<double>(height_1,10,ain_lib::middle)
        << "|" << ain_lib::sjust<double>(height_2,10,ain_lib::middle)
        << std::endl;

    a_out << " * MIN = " << ain_lib::sjust<double>(a_h.minBinHeight(),field)
        << " * MAX = " << ain_lib::sjust<double>(a_h.maxBinHeight(),field)
        << std::endl;
  }

  hprint(a_h.annotation(),a_out);
}

inline void hprint(const AIDA::IFunction& a_func,const std::string& a_name,std::ostream& a_out){
  int field = 13; //HBOOK had 17.

  std::vector<std::string> names = a_func.parameterNames();
  std::vector<double> vals = a_func.parameters();

  a_out << a_func.dimension() << "D " << a_func.title() << std::endl;
  a_out << " * NAME = " << a_name << std::endl;

  unsigned int number = names.size();
  for(unsigned int index=0;index<number;index++){
    a_out << " * PARAM   = " << ain_lib::sjust(names[index],field)
        << " * VALUE = " << ain_lib::sjust<double>(vals[index],field)
        << std::endl;
  }
  a_out << " * CODELET = " << a_func.codeletString() << std::endl;

  hprint(a_func.annotation(),a_out);
}

inline void hprint(const AIDA::ICloud& a_cloud,const std::string& a_name,std::ostream& a_out){
  int field = 13; //HBOOK had 17.

  if(!a_cloud.entries()) {

    a_out << "****** AIDA   NO ENTRIES FOR CLOUD = " << a_name
        << std::endl;

  } else {

    a_out << a_cloud.dimension() << "D " << a_cloud.title() << std::endl;
    a_out << " * NAME = " << a_name << std::endl;

    a_out << " * ENTRIES = " << ain_lib::sjust<int>(a_cloud.entries(),field)
        << std::endl;
      
    a_out << " * SUM OF WEIGHTS = " << ain_lib::sjust<double>(a_cloud.sumOfWeights(),field)
        << std::endl;

    a_out << " * IS CONVERTED TO HISTOGRAM = " 
	  << ain_lib::to(a_cloud.isConverted())
        << std::endl;

    if(const AIDA::ICloud1D* cl1D = 
         cast_const<AIDA::ICloud1D,AIDA::ICloud>(a_cloud,"AIDA::ICloud1D")) {

        a_out << " * MEAN VALUE = " << ain_lib::sjust<double>(cl1D->mean(),field)
            << " * R . M . S = " << ain_lib::sjust<double>(cl1D->rms(),field)
            << std::endl;

        a_out << " * MIN = " << ain_lib::sjust<double>(cl1D->lowerEdge(),field+7)
            << " * MAX = " << ain_lib::sjust<double>(cl1D->upperEdge(),field)
            << std::endl;

    } else if(const AIDA::ICloud2D* cl2D = 
          cast_const<AIDA::ICloud2D,AIDA::ICloud>(a_cloud,"AIDA::ICloud2D")) {

        a_out << " * X MEAN VALUE = " << ain_lib::sjust<double>(cl2D->meanX(),field)
            << " * X R . M . S = " << ain_lib::sjust<double>(cl2D->rmsX(),field)
            << std::endl;

        a_out << " * X MIN = " << ain_lib::sjust<double>(cl2D->lowerEdgeX(),field+7)
            << " * X MAX = " << ain_lib::sjust<double>(cl2D->upperEdgeX(),field)
            << std::endl;

        a_out << " * Y MEAN VALUE = " << ain_lib::sjust<double>(cl2D->meanY(),field)
            << " * Y R . M . S = " << ain_lib::sjust<double>(cl2D->rmsY(),field)
            << std::endl;

        a_out << " * Y MIN = " << ain_lib::sjust<double>(cl2D->lowerEdgeY(),field+7)
            << " * Y MAX = " << ain_lib::sjust<double>(cl2D->upperEdgeY(),field)
            << std::endl;

    } else if(const AIDA::ICloud3D* cl3D = 
          cast_const<AIDA::ICloud3D,AIDA::ICloud>(a_cloud,"AIDA::ICloud3D")) {

        a_out << " * X MEAN VALUE = " << ain_lib::sjust<double>(cl3D->meanX(),field)
            << " * X R . M . S = " << ain_lib::sjust<double>(cl3D->rmsX(),field)
            << std::endl;

        a_out << " * X MIN = " << ain_lib::sjust<double>(cl3D->lowerEdgeX(),field+7)
            << " * X MAX = " << ain_lib::sjust<double>(cl3D->upperEdgeX(),field)
            << std::endl;

        a_out << " * Y MEAN VALUE = " << ain_lib::sjust<double>(cl3D->meanY(),field)
            << " * Y R . M . S = " << ain_lib::sjust<double>(cl3D->rmsY(),field)
            << std::endl;

        a_out << " * Y MIN = " << ain_lib::sjust<double>(cl3D->lowerEdgeY(),field+7)
            << " * Y MAX = " << ain_lib::sjust<double>(cl3D->upperEdgeY(),field)
            << std::endl;

        a_out << " * Z MEAN VALUE = " << ain_lib::sjust<double>(cl3D->meanZ(),field)
            << " * Z R . M . S = " << ain_lib::sjust<double>(cl3D->rmsZ(),field)
            << std::endl;

        a_out << " * Z MIN = " << ain_lib::sjust<double>(cl3D->lowerEdgeZ(),field+7)
            << " * Z MAX = " << ain_lib::sjust<double>(cl3D->upperEdgeZ(),field)
            << std::endl;
    }
  }

  hprint(a_cloud.annotation(),a_out);
}

inline void hprint(const AIDA::IDataPointSet& a_dps,const std::string& a_name,std::ostream& a_out){
  int field = 13; //HBOOK had 17.

  if(!a_dps.size()) {

    a_out << "****** AIDA   NO ENTRIES FOR DATA POINT SET = " << a_name
        << std::endl;

  } else {

    a_out << a_dps.dimension() << "D " << a_dps.title() << std::endl;
    a_out << " * NAME = " << a_name << std::endl;

    a_out << " * ENTRIES = " << ain_lib::sjust<int>(a_dps.size(),field)
        << std::endl;
      
  }

  hprint(a_dps.annotation(),a_out);
}

inline void hprint(const AIDA::IBaseHistogram& a_o,const std::string& a_name,std::ostream& a_out){
  if(const AIDA::IHistogram1D* h1 = 
       cast_const<AIDA::IHistogram1D,AIDA::IBaseHistogram>
         (a_o,"AIDA::IHistogram1D")) {

    hprint(*h1,a_name,a_out);

  } else if(const AIDA::IProfile1D* pf1 = 
       cast_const<AIDA::IProfile1D,AIDA::IBaseHistogram>
         (a_o,"AIDA::IProfile1D")) {

    hprint(*pf1,a_name,a_out);

  } else if(const AIDA::IHistogram2D* h2 = 
       cast_const<AIDA::IHistogram2D,AIDA::IBaseHistogram>
         (a_o,"AIDA::IHistogram2D")) {

    hprint(*h2,a_name,a_out);

  } else if(const AIDA::ICloud* cl = 
       cast_const<AIDA::ICloud,AIDA::IBaseHistogram>(a_o,"AIDA::ICloud")) {

    hprint(*cl,a_name,a_out);

  } else {

    a_out << "****** AIDA NO INFOS FOR OBJECT = " << a_name
        << std::endl;

  }
}

inline void hprint(const AIDA::IManagedObject& a_o,std::ostream& a_out){
  if(const AIDA::IHistogram1D* h1 = 
       cast_const<AIDA::IHistogram1D,AIDA::IManagedObject>
         (a_o,"AIDA::IHistogram1D")) {

    hprint(*h1,a_o.name(),a_out);

  } else if(const AIDA::IProfile1D* pf1 = 
       cast_const<AIDA::IProfile1D,AIDA::IManagedObject>
         (a_o,"AIDA::IProfile1D")) {

    hprint(*pf1,a_o.name(),a_out);

  } else if(const AIDA::IHistogram2D* h2 = 
       cast_const<AIDA::IHistogram2D,AIDA::IManagedObject>
         (a_o,"AIDA::IHistogram2D")) {

    hprint(*h2,a_o.name(),a_out);

  } else if(const AIDA::IFunction* f = 
       cast_const<AIDA::IFunction,AIDA::IManagedObject>
         (a_o,"AIDA::IFunction")) {

    hprint(*f,a_o.name(),a_out);

  } else if(const AIDA::ITuple* tuple = 
       cast_const<AIDA::ITuple,AIDA::IManagedObject>(a_o,"AIDA::ITuple")) {

    hprint(*tuple,a_o.name(),a_out);

  } else if(const AIDA::ICloud* cl = 
       cast_const<AIDA::ICloud,AIDA::IManagedObject>(a_o,"AIDA::ICloud")) {

    hprint(*cl,a_o.name(),a_out);

  } else if(const AIDA::IDataPointSet* dps = 
       cast_const<AIDA::IDataPointSet,AIDA::IManagedObject>
         (a_o,"AIDA::IDataPointSet")) {

    hprint(*dps,a_o.name(),a_out);

  } else {

    a_out << "****** AIDA NO INFOS FOR OBJECT = " << a_o.name()
        << std::endl;

  }
}

}

#endif
