/*
 * Store a tuple-in-tuple in a file at various format.
 * Usage :
 *     OS> AIDA_example_jec [-read,-write] [-root,-xml,-compress,-dump]
 * Example :
 *     OS> AIDA_example_jec       # write jec.aida at the AIDA xml format.
 *     OS> AIDA_example_jec -read # read the jec.aida.
 * To work at the ROOT format :
 *     OS> AIDA_example_jec -root
 *     OS> AIDA_example_jec -read -root -dump
 */

//JEC AIDA :
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITuple.h>

// std::
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define CAST(aObject,aClass) (aClass*)(aObject)->cast(#aClass)

static bool write(AIDA::IAnalysisFactory&);
static bool read(AIDA::IAnalysisFactory&);

// Options :
static bool write_file = true;
static std::string store = "xml";
static bool compress = false;
static bool dump = false;
static bool verb = false;
static bool do_dir = true;

static int col_eventId = 0;
static int col_nPart = 1;
static int col_track = 4;

static int col_pId = 0;
static int col_parent = 1;
static int col_direction = 2;
static int col_ETot = 3;

static int col_dx = 0;
static int col_dy = 1;
static int col_dz = 2;

int main(int aArgc, char** aArgv) {

 {for(int index=1;index<aArgc;index++) {
   std::string arg(aArgv[index]);
   if(arg=="-h") {
      std::cout << "<program> [-write,-read,-root,-xml,-compress,-dump]" 
                << std::endl;
      std::cout << "<program>" << std::endl;
      std::cout << "  write/read a tuple-in-tuple in a file." << std::endl;
      return 0;
   }
   if(arg=="-write") write_file = true;
   if(arg=="-read") write_file = false;
   if(arg=="-root") store = "root";
   if(arg=="-xml") store = "xml";
   if(arg=="-compress") compress = true;
   if(arg=="-dump") dump = true;
   if(arg=="-verb") verb = true;
   if(arg=="-nodir") do_dir = false;
  }}

  //AIDA Analysis factory
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "AIDA not found." << std::endl;
    return 0;
  }

  if(!do_dir) col_ETot = 2;

  if(write_file) {
    if(!write(*aida)) {
      std::cout << "write failed." << std::endl;
    }
  } else {
    if(!read(*aida)) {
      std::cout << "read failed." << std::endl;
    }
  }

  delete aida;

}//main

bool write(AIDA::IAnalysisFactory& aAIDA) {
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();

  std::string opts;

  if(compress) opts += "compress=yes";
  else opts += "compress=no";

  if(verb) opts += ";verbose=yes";
  else opts += ";verbose=no";

  std::string ext = store;
  if(store=="xml") ext = "aida";
  AIDA::ITree* fTree = treeFactory->create("jec."+ext,store,false,true,opts);

  delete treeFactory;

  if(!fTree) {
    std::cout << "write :"
              << " tree not created." << std::endl;
    return false;
  }

  //Booking Tuple
  AIDA::ITupleFactory* tf = aAIDA.createTupleFactory(*fTree);

  //JEC 10/11/05: translated from the jhfNtuple structure
  std::string column = "int eventId, ";
  column += "nPart, positiveIndex, negativeIndex, ";
  column += "ITuple track = { int pId, parent, ";
  if(do_dir) {
    column +=                  "ITuple direction = { float dx, dy, dz }, ";
  }
  column +=                  "float ETot ";
  column +=                "} ";

  tf->create("jec","Water Cerenkov Analysis",column,"");

  //End of User Part
  delete tf;

  AIDA::IManagedObject* obj; //generic Object managed by a Tree

  obj = fTree->find("jec");
  if(!obj) {
    std::cout << "write : no tuple jec" << std::endl;
    fTree->ls();
    return false;
  }

  AIDA::ITuple* m_tuple =  CAST(obj,AIDA::ITuple);
  if (!m_tuple) {
    std::cout << "write : tuple not a Tuple" << std::endl;
    fTree->ls();
    return false;
  }

  //Filling
  int nPart, positiveIndex, negativeIndex;
  int pId, parent;
  float dx = 0;
  float dy = 0;
  float dz = 0;
  float modulus;
  float ETot;

  int tot_nPart = 0;

  //int nevent = 2;
  int nevent = 100;
  //int nevent = 100000;
  //int nevent = 1000;
  //int nevent = 10000;
  for (int ievt=1; ievt<=nevent; ievt++) {
    m_tuple->fill(col_eventId, ievt);

    AIDA::ITuple* track = m_tuple->getTuple(col_track);
    if(!track) {
      std::cout << "write : can't get sub tuple." << std::endl;
      return false;
    }

    //1ere track = beam
    pId = 0;
    track->fill(col_pId, pId);

    parent = 0;
    track->fill(col_parent, parent);

    if(do_dir) {
    AIDA::ITuple* direction = track->getTuple( col_direction );
    if(!direction) {
      std::cout << "write : can't get sub tuple." << std::endl;
      return false;
    }
    dx=0.;
    dy=0.;
    dz=1.;
    direction->fill(col_dx, dx);
    direction->fill(col_dy, dy);
    direction->fill(col_dz, dz);
    if(!direction->addRow()) {
      std::cout << "write : can't addRow to sub tuple." << std::endl;
      return false;
    }}

    ETot = 10.;
    track->fill(col_ETot, ETot);
    //add the beam
    if(!track->addRow()) {
      std::cout << "write : can't addRow to sub tuple." << std::endl;
      return false;
    }

    //2nd track = target
    pId = 1;
    track->fill(col_pId, pId);

    parent = 0;
    track->fill(col_parent, parent);

    if(do_dir) {
    AIDA::ITuple* direction = track->getTuple( col_direction );
    if(!direction) {
      std::cout << "write : can't get sub tuple." << std::endl;
      return false;
    }
    dx=0.;
    dy=0.;
    dz=-1.;
    direction->fill(col_dx, dx);
    direction->fill(col_dy, dy);
    direction->fill(col_dz, dz);
    if(!direction->addRow()) {
      std::cout << "write : can't addRow to sub tuple." << std::endl;
      return false;
    }}

    ETot = 20.;
    track->fill(col_ETot, ETot);
    //add the target
    if(!track->addRow()) {
      std::cout << "write : can't addRow to sub tuple." << std::endl;
      return false;
    }

    //Loop on outgoing particles
    nPart = 1+(int) (10.0*rand()/(RAND_MAX+1.0)); //[1,10]

    if(dump) {
      std::cout << "event " << ievt << " outgoing particles number : " << nPart
                << std::endl;
    }

    for (int ipart=1; ipart<=nPart; ++ipart) {
      pId = -10+(int) (20.0*rand()/(RAND_MAX+1.0)); //[-10,10]
      track->fill(col_pId, pId);

      parent = 1+(int) (ipart*rand()/(RAND_MAX+1.0)); //[1,ipart+1]
      track->fill(col_parent, parent);

      if(do_dir) {
      AIDA::ITuple* direction = track->getTuple( col_direction );
      if(!direction) {
        std::cout << "write : can't get sub tuple." << std::endl;
        return false;
      }
      dx = float(-1+ (2.0*rand()/(RAND_MAX+1.0))); //[-1,1];
      dy = float(-1+ (2.0*rand()/(RAND_MAX+1.0))); //[-1,1];
      dz = float(-1+ (2.0*rand()/(RAND_MAX+1.0))); //[-1,1];
      modulus = (float)::sqrt(dx*dx+dy*dy+dz*dz);
      dx = dx/modulus;
      dy = dy/modulus;
      dz = dz/modulus;
      direction->fill(col_dx, dx);
      direction->fill(col_dy, dy);
      direction->fill(col_dz, dz);
      if(!direction->addRow()) {
        std::cout << "write : can't addRow to sub tuple." << std::endl;
        return false;
      }}

      ETot =  float(30.0*rand()/(RAND_MAX+1.0)); //[0,30.0]
      if(dump) {
        std::cout << " pId " << pId
                  << " ETot " << ETot 
                  << " dx " << dx
                  << " dy " << dy
                  << " dz " << dz
                  << std::endl;
      }
      track->fill(col_ETot, ETot);
      //add the particle
      if(!track->addRow()) {
        std::cout << "write : can't addRow to sub tuple." << std::endl;
        return false;
      }
    }//eo loop on outgoing part

    nPart +=2; //add the "beam" and the "target" pseudo track
    m_tuple->fill(col_nPart, nPart);
    tot_nPart += nPart;

    positiveIndex = negativeIndex = -1; //not yet defined
    if(!m_tuple->addRow()) {
      std::cout << "write : can't addRow to tuple." << std::endl;
      return false;
    }

  }//eo evt

  fTree->commit();

  delete fTree; //Will close the file.

  if(dump) {
    std::cout << "number of particles " << tot_nPart << std::endl;
  }

  return true;
}
#include <AIDA/ITupleEntry.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IPlotterFactory.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>
bool read(AIDA::IAnalysisFactory& aAIDA) {
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();

  std::string ext = store;
  if(store=="xml") ext = "aida";
  AIDA::ITree* fTree = treeFactory->create("jec."+ext,store,true,false);
  delete treeFactory;
  if(!fTree) {
    std::cout << "read :"
              << " tree not created." << std::endl;
    return false;
  }

  AIDA::IManagedObject* obj = fTree->find("jec");
  if(!obj) {
    std::cout << "read: FATAL: no tuple jec." << std::endl;
    fTree->ls();
    return false;
  }

  AIDA::ITuple* m_tuple =  CAST(obj,AIDA::ITuple);
  if (!m_tuple) {
    std::cout << "read: FATAL: tuple not a Tuple" << std::endl;
    fTree->ls();
    return false;
  }

  if(dump) {
    std::cout << " tuple rows " << m_tuple->rows() << std::endl;
  }

  AIDA::ITreeFactory* trf = aAIDA.createTreeFactory();
  if(!trf) {
    std::cout << "read: FATAL: can't create a tree factory." << std::endl;
    return false;
  }
  AIDA::ITree* memTree = trf->create();
  delete trf;
  if(!memTree) {
    std::cout << "read: FATAL: can't create a tree." << std::endl;
    return false;
  }

  AIDA::IHistogramFactory* hf = aAIDA.createHistogramFactory(*memTree);
  if(!hf) {
    std::cout << "read: FATAL: can't create an histogram factory." 
              << std::endl;
    return false;
  }
  AIDA::IHistogram1D* h_ETot = hf->createHistogram1D("pxHist",100,0,30);
  delete hf;
  if(!h_ETot) {
    std::cout << "read: FATAL: can't create pxHist histogram." 
              << std::endl;
    return false;
  }    
  
  m_tuple->start();
  int i = 0;
  while ( m_tuple->next() ) {

    int nPart = m_tuple->getInt(col_nPart);

    if(dump) {
      std::cout << "jec : ievt " << i << " nPart " << nPart << std::endl;
    }

    AIDA::ITupleEntry* tracksEntry = 
      (AIDA::ITupleEntry*)m_tuple->getObject(col_track);
    if(!tracksEntry) {
      std::cout << "read: FATAL: can't tuple object." 
                << std::endl;
      return false;
    }
    AIDA::ITuple* tracks = dynamic_cast<AIDA::ITuple*>(tracksEntry);
    if(!tracks) {
      std::cout << "read: FATAL: tuple object not a AIDA::ITuple." 
                << std::endl;
      return false;
    }

    if(nPart!=tracks->rows()) {
      std::cout << "read: nPart / tracks mismatch " 
                << nPart << " " << tracks->rows() << std::endl;
      return false;
    }

    tracks->start();
    while ( tracks->next() ) {
      float dx = 0;
      float dy = 0;
      float dz = 0;
      if(do_dir) {
      AIDA::ITupleEntry* dirEntry = 
        (AIDA::ITupleEntry*)tracks->getObject(col_direction);
      if(!dirEntry) {
        std::cout << "read: FATAL: can't tuple object." 
                  << std::endl;
        return false;
      }
      AIDA::ITuple* dir = dynamic_cast<AIDA::ITuple*>(dirEntry);
      if(!dir) {
        std::cout << "read: FATAL: tuple object not a AIDA::ITuple." 
                  << std::endl;
        return false;
      }

      dir->start();
      dir->next();
      dx = dir->getFloat(col_dx);
      dy = dir->getFloat(col_dy);
      dz = dir->getFloat(col_dz);
      }

      float ETot = tracks->getFloat(col_ETot);

      if(dump) {
        std::cout << " ETot " << ETot 
                  << " dx " << dx
                  << " dy " << dy
                  << " dz " << dz
                  << std::endl;
      }

      h_ETot->fill( ETot);
    }

    i++;
  }
    
  AIDA::IPlotterFactory* pf = aAIDA.createPlotterFactory(0,0);
  if(pf) {
    AIDA::IPlotter* plotter = pf->create();
    delete pf;
    if(plotter) {
      plotter->createRegions(1,1,0);
      plotter->region(0)->plot(*h_ETot);
      plotter->show();
      plotter->interact();
      delete plotter;
    }
  }

  delete fTree;

  return true;
}
