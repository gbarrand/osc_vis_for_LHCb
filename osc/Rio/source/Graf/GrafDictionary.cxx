// this :
#include <Rio/Graf/GrafDictionary.h>

//////////////////////////////////////////////////////////////////////////////
Rio::GrafDictionary::GrafDictionary(std::ostream& aOut) 
:CoreDictionary(aOut)
,f_opt_AttLine_Class(0)
,f_opt_AttFill_Class(0)
,f_opt_AttMarker_Class(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::GrafDictionary::~GrafDictionary(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
const Rio::IClass& Rio::GrafDictionary::opt_AttLine_Class(
) const { return *f_opt_AttLine_Class;}
const Rio::IClass& Rio::GrafDictionary::opt_AttFill_Class(
) const { return *f_opt_AttFill_Class;}
const Rio::IClass& Rio::GrafDictionary::opt_AttMarker_Class(
) const { return *f_opt_AttMarker_Class;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Rio/Core/BaseClass.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>

#include <Rio/Graf/AttLine.h>
#include <Rio/Graf/AttFill.h>
#include <Rio/Graf/AttMarker.h>

#include "GrafClasses.icxx"

//////////////////////////////////////////////////////////////////////////////
bool Rio::GrafDictionary::initialize(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!CoreDictionary::initialize()) return false;

  ///////////////////////////////////////////////////////////////////////
  // Classes :
  ///////////////////////////////////////////////////////////////////////

  //---------------------------------------------------------------------
  {
    IClass* iclass = new AttLine_Class();
    f_opt_AttLine_Class = iclass;
    addClass(iclass);

    //sizeof(Font_t) = 2
    //sizeof(Style_t) = 2
    //sizeof(Marker_t) = 2
    //sizeof(Width_t) = 2
    //sizeof(Size_t) = 4

    // Beurk ; but the ROOT TTree compells indirectly the below.
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    //4 (virtual ~)
    streamerInfo->add(new StreamerBasicType(*this,"fLineColor","line color",4,Streamer_Info::SHORT,"Color_t"));//2
    streamerInfo->add(new StreamerBasicType(*this,"fLineStyle","line style",6,Streamer_Info::SHORT,"Style_t"));//2
    streamerInfo->add(new StreamerBasicType(*this,"fLineWidth","line width",8,Streamer_Info::SHORT,"Width_t"));//2
    //2 (alignement ???)

    //12
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new AttFill_Class();
    f_opt_AttFill_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    //4 (virtual ~)
    streamerInfo->add(new StreamerBasicType(*this,"fFillColor","fill area color",4,Streamer_Info::SHORT,"Color_t"));//2
    streamerInfo->add(new StreamerBasicType(*this,"fFillStyle","fill area style",6,Streamer_Info::SHORT,"Style_t"));//2

    //8
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new AttMarker_Class();
    f_opt_AttMarker_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    //4 (virtual ~)
    streamerInfo->add(new StreamerBasicType(*this,"fMarkerColor","Marker color index",0,Streamer_Info::SHORT,"Color_t"));//2
    streamerInfo->add(new StreamerBasicType(*this,"fMarkerStyle","Marker style",4,Streamer_Info::SHORT,"Style_t"));//2
    streamerInfo->add(new StreamerBasicType(*this,"fMarkerSize","Marker size",6,Streamer_Info::FLOAT,"Size_t"));//4

    //12
  }

  return true;
}
