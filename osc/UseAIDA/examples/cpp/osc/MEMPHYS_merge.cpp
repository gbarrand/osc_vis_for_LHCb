
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

bool process_tracks(AIDA::ITuple& aParent,AIDA::ITuple& a_tracks) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(8);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    int pId = tuple->getInt(0);
    int parent = tuple->getInt(1);
    float timeStart = tuple->getFloat(2);

    if(!a_tracks.fill(0,pId)) return false;
    if(!a_tracks.fill(1,parent)) return false;
    if(!a_tracks.fill(2,timeStart)) return false;

    double dx,dy,dz;
    if(!get_XYZ(*tuple,3,dx,dy,dz)) return false;
    AIDA::ITuple* direction = a_tracks.getTuple(3);
    if(!direction->fill(0,dx)) return false;
    if(!direction->fill(1,dy)) return false;
    if(!direction->fill(2,dz)) return false;
    if(!direction->addRow()) return false;

    /*
    std::cout << " track :"
              << " pId " << pId
              << " timeStart " << timeStart
              << " dx " << dx
              << " dy " << dy
              << " dz " << dz
              << std::endl;
    */

   {double mass = tuple->getDouble(4);
    double pTot = tuple->getDouble(5);
    double eTot = tuple->getDouble(6);

    if(!a_tracks.fill(4,mass)) return false;
    if(!a_tracks.fill(5,pTot)) return false;
    if(!a_tracks.fill(6,eTot)) return false;
    }

   {double px,py,pz;
    if(!get_XYZ(*tuple,7,px,py,pz)) return false;
    AIDA::ITuple* momentum = a_tracks.getTuple(7);
    if(!momentum->fill(0,px)) return false;
    if(!momentum->fill(1,py)) return false;
    if(!momentum->fill(2,pz)) return false;
    if(!momentum->addRow()) return false;
    }

   {double start_x,start_y,start_z;
    if(!get_XYZ(*tuple,8,start_x,start_y,start_z)) return false;
    AIDA::ITuple* startPos = a_tracks.getTuple(8);
    if(!startPos->fill(0,start_x)) return false;
    if(!startPos->fill(1,start_y)) return false;
    if(!startPos->fill(2,start_z)) return false;
    if(!startPos->addRow()) return false;
    }

   {double stop_x,stop_y,stop_z;
    if(!get_XYZ(*tuple,9,stop_x,stop_y,stop_z)) return false;
    AIDA::ITuple* stopPos = a_tracks.getTuple(9);
    if(!stopPos->fill(0,stop_x)) return false;
    if(!stopPos->fill(1,stop_y)) return false;
    if(!stopPos->fill(2,stop_z)) return false;
    if(!stopPos->addRow()) return false;
    }

   {int startVol = tuple->getInt(10);
    int stopVol = tuple->getInt(11); 

    if(!a_tracks.fill(10,startVol)) return false;
    if(!a_tracks.fill(11,stopVol)) return false;
    }

    if(!a_tracks.addRow()) return false;

  }
  return true;
}

bool process_hit_times(AIDA::ITuple& aParent,AIDA::ITuple& a_times) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(5);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {
    float time = tuple->getFloat(0);
    if(!a_times.fill(0,time)) return false;
    if(!a_times.addRow()) return false;
  }

  return true;
}

bool process_hits(AIDA::ITuple& aParent,AIDA::ITuple& a_hits) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(10);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;
  
  tuple->start();
  while(tuple->next()) {

   {int tubeId = tuple->getInt(0);
    float tube_x = tuple->getFloat(1);
    float tube_y = tuple->getFloat(2);
    float tube_z = tuple->getFloat(3);   
    int totalPE = tuple->getInt(4); 
    
    if(!a_hits.fill(0,tubeId)) return false;
    if(!a_hits.fill(1,tube_x)) return false;
    if(!a_hits.fill(2,tube_y)) return false;
    if(!a_hits.fill(3,tube_z)) return false;
    if(!a_hits.fill(4,totalPE)) return false;
    }

    AIDA::ITuple* times = a_hits.getTuple(5);
    if(!process_hit_times(*tuple,*times)) return false;

    if(!a_hits.addRow()) return false;
  }

  return true;
}

bool process_digits(AIDA::ITuple& aParent,AIDA::ITuple& a_digits) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(13);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

   {int tubeId = tuple->getInt(0);
    double pe = tuple->getDouble(1);
    double time = tuple->getDouble(2);

    if(!a_digits.fill(0,tubeId)) return false;
    if(!a_digits.fill(1,pe)) return false;
    if(!a_digits.fill(2,time)) return false;
    }

    if(!a_digits.addRow()) return false;
  }
  return true;
}

#include <iostream>

bool process_file(AIDA::IAnalysisFactory& aAIDA,const std::string& a_file,AIDA::ITuple& a_event) {
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

  bool verbose = false;
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

  tuple->start();
  while(tuple->next()) {
    
    int eventId = tuple->getInt(0);
    int inputEvtId = tuple->getInt(1);
    int interMode = tuple->getInt(2);
    int vtxVol = tuple->getInt(3);

    if(!a_event.fill(0,eventId)) {delete tree;return false;}
    if(!a_event.fill(1,inputEvtId)) {delete tree;return false;}
    if(!a_event.fill(2,interMode)) {delete tree;return false;}
    if(!a_event.fill(3,vtxVol)) {delete tree;return false;}

    double x,y,z;
    if(!get_XYZ(*tuple,4,x,y,z)) break;

    AIDA::ITuple* vtxPos = a_event.getTuple(4);
    if(!vtxPos->fill(0,x)) {delete tree;return false;}
    if(!vtxPos->fill(1,y)) {delete tree;return false;}
    if(!vtxPos->fill(2,z)) {delete tree;return false;}
    if(!vtxPos->addRow()) {delete tree;return false;}
  
    int nPart = tuple->getInt(5);
    int leptonIndex = tuple->getInt(6);
    int protonIndex = tuple->getInt(7); 

    if(!a_event.fill(5,nPart)) {delete tree;return false;}
    if(!a_event.fill(6,leptonIndex)) {delete tree;return false;}
    if(!a_event.fill(7,protonIndex)) {delete tree;return false;}

    /*
    std::cout << "event :"
              << " eventId " << eventId
              << " vtx x " << x
              << " vtx y " << y
              << " vtx z " << z
              << " nPart " << nPart
              << std::endl;
    */

    AIDA::ITuple* tracks = a_event.getTuple(8);
    if(!process_tracks(*tuple,*tracks)) {delete tree;return false;}

   {int nHits = tuple->getInt(9); 
    if(!a_event.fill(9,nHits)) {delete tree;return false;}
    AIDA::ITuple* hits = a_event.getTuple(10);
    if(!process_hits(*tuple,*hits)) {delete tree;return false;}}

   {int nDigits = tuple->getInt(11);
    double sumPE = tuple->getDouble(12);

    if(!a_event.fill(11,nDigits)) {delete tree;return false;}
    if(!a_event.fill(12,sumPE)) {delete tree;return false;}}

    AIDA::ITuple* digits = a_event.getTuple(13);
    if(!process_digits(*tuple,*digits)) {delete tree;return false;}

    if(!a_event.addRow()) {delete tree;return false;}
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

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << "main : can't create an AIDA::ITreeFactory." << std::endl;
    delete aida;
    return 1;
  }
  AIDA::ITree* tree = treeFactory->create("out.root","root",false,true);
  delete treeFactory;
  if(!tree) {
    std::cout << "main : can't create an AIDA::ITree." << std::endl;
    delete aida;
    return 1;
  }

  AIDA::ITupleFactory* tf = aida->createTupleFactory(*tree);
  if(!tf) {
    std::cout << "main : can't create an AIDA::ITupleFactory." << std::endl;
    delete aida;
    return 1;
  }

  std::string booking;
  booking += "int eventId, inputEvtId, interMode, vtxVol, "; 
  booking += "ITuple vtxPos = { double x, y, z }, ";
  booking += "int nPart, leptonIndex, protonIndex, ";                
  booking += "ITuple track = { int pId, parent, float timeStart, ";        
  booking +=                  "ITuple direction = { double dx, dy, dz }, ";
  booking +=                  "double mass, pTot, ETot, ";
  booking +=                  "ITuple momentum  = { double px, py, pz }, ";
  booking +=                  "ITuple startPos  = { double  x,  y,  z }, ";
  booking +=                  "ITuple stopPos   = { double  x,  y,  z }, ";
  booking +=                  "int startVol, stopVol ";
  booking +=                "}, ";
  booking += "int nHits,";
  booking += "ITuple hit = { int tubeId, float x, y, z, int totalPE, ITuple pe = { float time } }, ";      
  booking += "int nDigits, double sumPE, ";
  booking += "ITuple digit = { int tubeId, double pe, time }";

  AIDA::ITuple* tuple = tf->create("Event","MEMPHYS WC Event",booking,"");
  delete tf;
  if(!tuple) {
    std::cout << "main : can't create an AIDA::ITuple." << std::endl;
    delete aida;
    return 1;
  }

  //std::string directory = "/sps/dchooz/tonazzo/Memphys/rootfiles";
  std::string directory = "../data";

  //NOTE : problem with 107_341, 107_496.
  // Rio::File::initialize : file "../data/MEMPHYS_107_341.root"
  //   probably not closed, trying to recover

  //for(unsigned int ifile=1;ifile<=499;ifile++) {
  for(unsigned int ifile=1;ifile<=3;ifile++) {
    std::string file = directory+"/MEMPHYS_107_";  
    file += to(ifile);  
    file += ".root";  
    std::cout << "main : process " << file << std::endl;
    if(!process_file(*aida,file,*tuple)){
      std::cout << "main :"
                << " read_file " << file << " failed."
                << " Continue anyway."
                << std::endl;
    }
  }

  tree->commit();
  delete tree;

  delete aida;

  std::cout << "main : exit..." << std::endl;

  return 1;
}
