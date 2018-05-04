// this :
#include <Rio/Tree/TreeDictionary.h>

//////////////////////////////////////////////////////////////////////////////
Rio::TreeDictionary::TreeDictionary(std::ostream& aOut)
:GrafDictionary(aOut)
,f_opt_Basket_Class(0)
,f_opt_BaseLeaf_Class(0)
,f_opt_Branch_Class(0)
,f_opt_Tree_Class(0)
,f_opt_LeafFloat_Class(0)
,f_opt_LeafDouble_Class(0)
,f_opt_LeafShort_Class(0)
,f_opt_LeafInteger_Class(0)
,f_opt_LeafCharacter_Class(0)
,f_opt_LeafElement_Class(0)
,f_opt_LeafObject_Class(0)
,f_opt_LeafString_Class(0)
,f_opt_BranchObject_Class(0)
,f_opt_BranchElement_Class(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::TreeDictionary::~TreeDictionary(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
const Rio::IClass& Rio::TreeDictionary::opt_Basket_Class() const { return *f_opt_Basket_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_BaseLeaf_Class() const { return *f_opt_BaseLeaf_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_Branch_Class() const { return *f_opt_Branch_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_Tree_Class() const { return *f_opt_Tree_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafFloat_Class() const { return *f_opt_LeafFloat_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafDouble_Class() const { return *f_opt_LeafDouble_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafShort_Class() const { return *f_opt_LeafShort_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafInteger_Class() const { return *f_opt_LeafInteger_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafCharacter_Class() const { return *f_opt_LeafCharacter_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafElement_Class() const { return *f_opt_LeafElement_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafObject_Class() const { return *f_opt_LeafObject_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_LeafString_Class() const { return *f_opt_LeafString_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_BranchObject_Class() const { return *f_opt_BranchObject_Class;}
const Rio::IClass& Rio::TreeDictionary::opt_BranchElement_Class() const { return *f_opt_BranchElement_Class;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/BaseClass.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>
#include <Rio/Core/Sizes.h>

//FIXME : should use dico to find version
#include <Rio/Graf/AttLine.h>
#include <Rio/Graf/AttFill.h>
#include <Rio/Graf/AttMarker.h>

#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BranchObject.h>
#include <Rio/Tree/BranchElement.h>
#include <Rio/Tree/Basket.h>
#include <Rio/Tree/Leaf.h>
#include <Rio/Tree/LeafString.h>
#include <Rio/Tree/LeafObject.h>
#include <Rio/Tree/LeafElement.h>

#include "TreeClasses.icxx"
//////////////////////////////////////////////////////////////////////////////
bool Rio::TreeDictionary::initialize(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!GrafDictionary::initialize()) return false;

  ///////////////////////////////////////////////////////////////////////
  // Classes :
  ///////////////////////////////////////////////////////////////////////
  //---------------------------------------------------------------------
  {
    IClass* iclass = new Tree_Class(*this,fOut);
    f_opt_Tree_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;

    streamerInfo->add(new StreamerBase(*this,
      "TNamed",
      "The basis for a named object (name, title)",
      offset,
      Named::is(*this).version()));
    offset += Rio_size_TNamed;

    streamerInfo->add(new StreamerBase(*this,
      "TAttLine",
      "Line attributes",
      offset,
      AttLine::is(*this).version()));
    offset += Rio_size_TAttLine;

    streamerInfo->add(new StreamerBase(*this,
      "TAttFill",
      "Fill area attributes",
      offset,
      AttFill::is(*this).version()));
    offset += Rio_size_TAttFill;

    streamerInfo->add(new StreamerBase(*this,
      "TAttMarker",
      "Marker attributes",
      offset,
      AttMarker::is(*this).version()));
    offset += Rio_size_TAttMarker;

    streamerInfo->add(new StreamerBasicType(*this,
      "fEntries",
      "Number of entries",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fTotBytes",
      "Total number of bytes in all branches before compression",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fZipBytes",
      "Total number of bytes in all branches after compression",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fSavedBytes",
      "Number of autosaved bytes",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fTimerInterval",
      "Timer interval in milliseconds",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fScanField",
      "Number of runs before prompting in Scan",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fUpdate",
      "Update frequency for EntryLoop",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fMaxEntryLoop",
      "Maximum number of entries to process",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fMaxVirtualSize",
      "Maximum total size of buffers kept in memory",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fAutoSave",
      "Autosave tree when fAutoSave bytes produced",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType( *this,
      "fEstimate",
      "Number of entries to estimate histogram limits",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    offset += 5*Rio_size_INT+Rio_size_POINTER, //! //144

    streamerInfo->add(new StreamerObject(*this,
      "fBranches",
      "List of Branches",
      offset,
      "TObjArray"));
    offset += Rio_size_TObjArray;

    streamerInfo->add(new StreamerObject(*this,
      "fLeaves",
      "Direct pointers to individual branch leaves",
      offset,
      "TObjArray"));
    offset += Rio_size_TObjArray;

    //streamerInfo->add(new StreamerObjectPointer( *this,
    //  "fAliases",
    //  "List of aliases for expressions based on the tree branches.",
    //  offset,
    //  "TList*"));
    //offset += Rio_size_OBJECT_POINTER;

    offset += Rio_size_POINTER; //! //228

    streamerInfo->add(new StreamerObjectAny(*this,
      "fIndexValues",
      "Sorted index values",
      offset,
      "TArrayD"));
    offset += Rio_size_TArrayD;

    streamerInfo->add(new StreamerObjectAny(*this,
      "fIndex",
      "Index of sorted values",
      offset,
      "TArrayI"));

    //streamerInfo->add(new StreamerObjectPointer( *this,
    //  "fFriends",
    //  "pointer to list of friend elements",
    //  offset,
    //  "TList*"));
    //offset += Rio_size_OBJECT_POINTER;

    if(offset!=240) {
      fOut << "Rio::TreeDictionary :"
           << " Tree " << offset << " (240 expected.)"
           << std::endl;
    }
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Branch_Class(*this,fOut);
    f_opt_Branch_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;

    streamerInfo->add(new StreamerBase( *this,
      "TNamed",
      "The basis for a named object (name, title)",
      offset,
      Named::is(*this).version()));
    offset += Rio_size_TNamed;

    streamerInfo->add(new StreamerBase(*this,
      "TAttFill",
      "Fill area attributes",
      offset,
      AttFill::is(*this).version()));
    offset += Rio_size_TAttFill;

    streamerInfo->add(new StreamerBasicType(*this,
      "fCompress",
      "(=1 branch is compressed, 0 otherwise)",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fBasketSize",
      "Initial Size of  Basket Buffer",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fEntryOffsetLen",
      "Initial Length of fEntryOffset table in the basket buffers",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fWriteBasket",
      "Last basket number written",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fEntryNumber",
      "Current entry number (last one filled in this branch)",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fOffset",
      "Offset of this branch",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,
      "fMaxBaskets",
      "Maximum number of Baskets so far",
      offset,
      Streamer_Info::COUNTER,"Int_t"));
    offset += Rio_size_COUNTER;

    streamerInfo->add(new StreamerBasicType(*this, //3.03.01
      "fSplitLevel",
      "Branch split level",
      offset,
      Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    offset += 3*Rio_size_INT; //!

    streamerInfo->add(new StreamerBasicType(*this,
      "fEntries",
      "Number of entries",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fTotBytes",
      "Total number of bytes in all leaves before compression",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerBasicType(*this,
      "fZipBytes",
      "Total number of bytes in all leaves after compression",
      offset,
      Streamer_Info::DOUBLE,"Stat_t"));
    offset += Rio_size_DOUBLE;

    streamerInfo->add(new StreamerObject(*this,
      "fBranches",
      "-> List of Branches of this branch",
      offset,
      "TObjArray"));
    offset += Rio_size_TObjArray;

    streamerInfo->add(new StreamerObject(*this,
      "fLeaves", 
      "-> List of leaves of this branch",
      Rio_size_TNamed+6*Rio_size_INT+Rio_size_COUNTER+
      offset,
      "TObjArray"));
    offset += Rio_size_TObjArray;

    streamerInfo->add(new StreamerObject(*this,
      "fBaskets",
      "-> List of baskets of this branch",
      offset,
      "TObjArray"));
    offset += Rio_size_TObjArray;

    offset += Rio_size_INT+Rio_size_POINTER; //!

    streamerInfo->add(new StreamerBasicPointer(*this,
      "fBasketBytes",
      "[fMaxBaskets] Lenght of baskets on file",
      offset,
      Streamer_Info::INT,"fMaxBaskets","TBranch",6,"Int_t*"));
    offset += Rio_size_POINTER;

    streamerInfo->add(new StreamerBasicPointer(*this,
      "fBasketEntry",
      "[fMaxBaskets] Table of first entry in eack basket",
      offset,
      Streamer_Info::INT,"fMaxBaskets","TBranch",6,"Int_t*"));
    offset += Rio_size_POINTER;

    streamerInfo->add(new StreamerBasicPointer( *this,
      "fBasketSeek",
      "[fMaxBaskets] Addresses of baskets on file",
      offset,
      Streamer_Info::INT,"fMaxBaskets","TBranch",6,"Seek_t*"));
    offset += Rio_size_POINTER;

    offset += 3*Rio_size_POINTER; //!

    streamerInfo->add(new StreamerString(*this,
      "fFileName",
      "Name of file where buffers are stored (\"\" if in same file as Tree header)",
      offset));

    if(offset!=256) {
      fOut << "Rio::TreeDictionary :"
           << " Branch " << offset << " (256 expected.)"
           << std::endl;
    }
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new BranchObject_Class(*this,fOut);
    f_opt_BranchObject_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TBranch","Branch descriptor",0,Branch::is(*this).version()));
    streamerInfo->add(new StreamerString(*this,"fClassName","Class name of referenced object",256));
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new BranchElement_Class(*this,fOut);
    f_opt_BranchElement_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;
    streamerInfo->add(new StreamerBase(*this,"TBranch","Branch descriptor",offset,Branch::is(*this).version()));
    offset += Rio_size_TBranch;

    streamerInfo->add(new StreamerString(*this,"fClassName","Class name of referenced object",offset));
    offset += Rio_size_TString;

    streamerInfo->add(new StreamerBasicType(*this,"fClassVersion","Version number of class",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,"fID","element serial number in fInfo",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,"fType","branch type",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,"fStreamerType","branch streamer type",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new BaseLeaf_Class();
    f_opt_BaseLeaf_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TNamed","The basis for a named object (name, title)",0,Named::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fLen","Number of fixed length elements",32,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fLenType","Number of bytes for this data type",36,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fOffset","Offset in ClonesArray object (if one)",40,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fIsRange","(=kTRUE if leaf has a range, kFALSE otherwise)",44,Streamer_Info::UNSIGNED_CHAR,"Bool_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fIsUnsigned","(=kTRUE if unsigned, kFALSE otherwise)",45,Streamer_Info::UNSIGNED_CHAR,"Bool_t"));
    streamerInfo->add(new StreamerObjectPointer(*this,"fLeafCount","Pointer to Leaf count if variable length",48,"TLeaf*"));

  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafShort_Class(fOut);
    f_opt_LeafShort_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::SHORT,"Short_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",58,Streamer_Info::SHORT,"Short_t"));

  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafInteger_Class(fOut);
    f_opt_LeafInteger_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",60,Streamer_Info::INT,"Int_t"));

  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafFloat_Class(fOut);
    f_opt_LeafFloat_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::FLOAT,"Float_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",60,Streamer_Info::FLOAT,"Float_t"));
 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafDouble_Class(fOut);
    f_opt_LeafDouble_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::DOUBLE,"Double_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",64,Streamer_Info::DOUBLE,"Double_t"));
 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafCharacter_Class(fOut);
    f_opt_LeafCharacter_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::CHAR,"Char_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",57,Streamer_Info::CHAR,"Char_t"));
 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafString_Class(fOut);
    f_opt_LeafString_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fMinimum","Minimum value if leaf range is specified",56,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fMaximum","Maximum value if leaf range is specified",60,Streamer_Info::INT,"Int_t"));
 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafObject_Class(fOut);
    f_opt_LeafObject_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",0,BaseLeaf::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fVirtual","Support for Virtuality",64,Streamer_Info::UNSIGNED_CHAR,"Bool_t"));
 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new LeafElement_Class(fOut);
    f_opt_LeafElement_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;
    streamerInfo->add(new StreamerBase(*this,"TLeaf","Leaf: description of a Branch data type",offset,BaseLeaf::is(*this).version()));
    offset += Rio_size_TLeaf;

    offset += Rio_size_POINTER;

    streamerInfo->add(new StreamerBasicType(*this,"fID","element serial number in fInfo",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;

    streamerInfo->add(new StreamerBasicType(*this,"fType","leaf type",offset,Streamer_Info::INT,"Int_t"));
    offset += Rio_size_INT;
 
  }

  //---------------------------------------------------------------------
  {
    IClass* clBasket = new Basket_Class(fOut);
    f_opt_Basket_Class = clBasket;
    addClass(clBasket);
    // Has no streamer info. 
  }


  return true;
}



/* ROOT tag v3-00-06 :
debug : TStreamerInfo : +++++ TTree -1049922452
debug : TStreamerInfo : +++++ TAttLine 1369587346
debug : TStreamerInfo : +++++ TAttFill 1204118360
debug : TStreamerInfo : +++++ TAttMarker -87219836
debug : TStreamerInfo : +++++ TBranch 2056727376
debug : TStreamerInfo : +++++ TLeafI -799765899
debug : TStreamerInfo : +++++ TLeaf 727988519
debug : TStreamerInfo : +++++ TLeafF 1366318032
*/
