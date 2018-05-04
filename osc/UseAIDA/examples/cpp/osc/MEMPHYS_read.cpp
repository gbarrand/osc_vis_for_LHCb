
//#define VERBOSE

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#include <AIDA/ITupleEntry.h>
#include <AIDA/IManagedObject.h>

AIDA::ITuple* cast_Tuple(AIDA::ITupleEntry* aEntry) {
#ifdef __CINT__
  //CINT can't handle dynamic_cast :
  //AIDA-3.2.1 : ITupleEntry does not have the cast method.
  //return (AIDA::ITuple*)aEntry->cast("AIDA::ITuple");
  return cast_ITuple(aEntry);
#else
  return dynamic_cast<AIDA::ITuple*>(aEntry);
#endif
}

bool get_XYZ(AIDA::ITuple& aParent,int aColumn,
             double& aX,double& aY,double& aZ) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(aColumn);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  if(!tuple->next()) return false;

  aX = tuple->getDouble(0);
  aY = tuple->getDouble(1);
  aZ = tuple->getDouble(2);

  return true;
}

bool process_tracks(AIDA::ITuple& aParent) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(8);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    int pId = tuple->getInt(0);
    int parent = tuple->getInt(1);
    float timeStart = tuple->getFloat(2);

    double dx,dy,dz;
    if(!get_XYZ(*tuple,3,dx,dy,dz)) return false;

   {double mass = tuple->getDouble(4);
    double pTot = tuple->getDouble(5);
    double eTot = tuple->getDouble(6);}

#ifdef VERBOSE
    std::cout << " track :"
              << " pId " << pId
              << " timeStart " << timeStart
              << " dx " << dx
              << " dy " << dy
              << " dz " << dz
              << std::endl;
#endif

   {double px,py,pz;
    if(!get_XYZ(*tuple,7,px,py,pz)) return false;}

   {double start_x,start_y,start_z;
    if(!get_XYZ(*tuple,8,start_x,start_y,start_z)) return false;}

   {double stop_x,stop_y,stop_z;
    if(!get_XYZ(*tuple,9,stop_x,stop_y,stop_z)) return false;}

   {int startVol = tuple->getInt(10);
    int stopVol = tuple->getInt(11);}

  }
  return true;
}

bool process_hit_times(AIDA::ITuple& aParent) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(5);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {
    float time = tuple->getFloat(0);
  }

  return true;
}

bool process_hits(AIDA::ITuple& aParent) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(10);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;
  
  tuple->start();
  while(tuple->next()) {
  
    int tubeId = tuple->getInt(0);
    float tube_x = tuple->getFloat(1);
    float tube_y = tuple->getFloat(2);
    float tube_z = tuple->getFloat(3);   
    int totalPE = tuple->getInt(4);
    /*
    std::cout << " tubeId " << tubeId
              << " tube_x " << tube_x
              << " tube_y " << tube_y
              << " tube_z " << tube_z
              << std::endl;
    */
    if(!process_hit_times(*tuple)) return false;

  }

  return true;
}

bool process_digits(AIDA::ITuple& aParent) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(13);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

   {int tubeId = tuple->getInt(0);
    double pe = tuple->getDouble(1);
    double time = tuple->getDouble(2);}

  }
  return true;
}

#include <iostream>

bool process_file(AIDA::IAnalysisFactory& aAIDA,const std::string& a_file) {
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) {
    std::cout << "process_file : can't get a TreeFactory." << std::endl;
    return false;
  }

  AIDA::ITree* tree = treeFactory->create(a_file,"root",true,false);
  delete treeFactory;
  if(!tree) {
    std::cout << "process_file : can't open data file." << std::endl;
    return false;
  }

  AIDA::IManagedObject* object = tree->find("Event");
  if(!object) {
    std::cout << "process_file :"
              << "object Event not found in tree." 
              << std::endl;
    delete tree;
    return false;
  }

#ifdef __CINT__
  //CINT can't handle dynamic_cast :
  AIDA::ITuple* tuple = (AIDA::ITuple*)object->cast("AIDA::ITuple");
#else
  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
#endif
  if(!tuple) {
    std::cout << "process_file : object not an AIDA::ITuple." << std::endl;
    delete tree;
    return false;
  }

  //bool verbose = false;
  bool verbose = true;
  if(verbose) {
    int coln = tuple->columns();
    for(int index=0;index<coln;index++) {
      std::cout << "process_file :"
                << " icol = " << index
                << ", label = " << tuple->columnName(index) 
                << ", type = " << tuple->columnType(index) 
                << std::endl;
    }
    std::cout << "process_file : rows = " << tuple->rows() << std::endl;
  }

  //unsigned int ievent = 0;
  tuple->start();
  while(tuple->next()) {
    //ievent++;
    //std::cout << "ievent " << ievent << std::endl;
    
    int eventId = tuple->getInt(0);
    int inputEvtId = tuple->getInt(1);
    int interMode = tuple->getInt(2);
    int vtxVol = tuple->getInt(3);

    double x,y,z;
    if(!get_XYZ(*tuple,4,x,y,z)) break;
  
    int nPart = tuple->getInt(5);
    int leptonIndex = tuple->getInt(6);
    int protonIndex = tuple->getInt(7);

#ifdef VERBOSE
    std::cout << "event :"
              << " eventId " << eventId
              << " vtx x " << x
              << " vtx y " << y
              << " vtx z " << z
              << " nPart " << nPart
              << std::endl;
#endif

    if(!process_tracks(*tuple)) break;

   {int nHits = tuple->getInt(9); 
    if(!process_hits(*tuple)) break;}

   {int nDigits = tuple->getInt(11);
    double sumPE = tuple->getDouble(12);}

    if(!process_digits(*tuple)) break;
  }

  delete tree;

  return true;
}

#include <sstream>

template <class T>
inline std::string to(const T& a_v) {
  std::ostringstream strm;
  strm << a_v;
  return strm.str();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int main() {
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "main : AIDA not found." << std::endl;
    return 1;
  }

  std::string file = "out.root";  
  if(!process_file(*aida,file)){
    std::cout << "main : read_file " << file << " failed." << std::endl;
    delete aida;
    return 0;
  }

  delete aida;

  std::cout << "main : exit..." << std::endl;

  return 1;
}
