/*
  An element is :
    <tag atb1="" atb2="" ...>
      <property atb1="" atb2="" ...> value </property>
      <property atb1="" atb2="" ...> value </property>
      <property atb1="" atb2="" ...> value </property>
      ...
    </tag>          

   element.attributeValue("",<name>,s) 
    retrieve the value of atb <name> of a <tag>
   element.attributeValue(<property>,<name>,s) 
    retrieve the value of an atb <name> of a <propery> of a <tag>

 */

// this :
#include <Lib/BaseML.h>

// Slash :
#include <Slash/Core/IFileGetter.h>

// Lib :
#include <Lib/Out.h>
#include <Lib/sout.h>
#include <Lib/fmanip.h>
#include <Lib/smanip.h>
#include <Lib/FileReader.h>
//#include <Lib/GZipReader.h>
#include <Lib/ItemML.h>
////#include <Lib/Debug.h>

#include <stdio.h>
#include <ctype.h> //iscntrl

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

//////////////////////////////////////////////////////////////////////////////
Lib::BaseML::BaseML(
 ItemMLFactory& aItemMLFactory 
,int aVerboseLevel
)
:fItemMLFactory(aItemMLFactory)
,fVerboseLevel(aVerboseLevel)
,fTop(0) // Used to cleanup in case XML parsing failed.
,fCurrent(0)
,fFileGetter(0)
,fPrinter(0)
,fDepth(0)
,fAbort(false)
,fErrors(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::increment("Lib::BaseML");
}
//////////////////////////////////////////////////////////////////////////////
Lib::BaseML::~BaseML(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  ////Lib::Debug::decrement("Lib::BaseML");
}

//////////////////////////////////////////////////////////////////////////////
Lib::BaseML::BaseML(
 const BaseML& aFrom
)
:fItemMLFactory(aFrom.fItemMLFactory)
,fVerboseLevel(0)
,fTop(0) // Used to cleanup in case XML parsing failed.
,fCurrent(0)
,fFileGetter(0)
,fPrinter(0)
,fDepth(0)
,fAbort(false)
,fErrors(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::increment("Lib::BaseML");
}
Lib::BaseML& Lib::BaseML::operator=(const BaseML&){return *this;}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::setFileGetter(
 Slash::Core::IFileGetter* aGetter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fFileGetter = aGetter;
}
//////////////////////////////////////////////////////////////////////////////
int Lib::BaseML::errors(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fErrors;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::visitEndElement(
 ItemML&
,bool& aKeep
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aKeep = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::setTags(
 const std::vector<std::string>& aTags
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTags = aTags;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::loadFile(
 const std::string& aFile
,bool aCompressed
,Slash::Core::IWriter& aPrinter
,bool a_push //true
,bool a_getit //true
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //::printf("debug : Lib::BaseML::loadFile : \"%s\"\n",aFile.c_str());

  if(a_push) { //Done on aFile and not on the below file.
    fDirs.push(Lib::smanip::dirname(aFile));
  }

  std::string file;
  if(a_getit && fFileGetter) { //Case of client/server.
    Slash::Core::IFileGetter::Arguments args;
    if(!fFileGetter->fetch(aFile,args,file)) {
      if(a_push) fDirs.pop();
      return false;
    }
  } else {
    file = fmanip::name(aFile);
  }

  if(!readFile(file,
               (XML_StartElementHandler)startElement,
               (XML_EndElementHandler)endElement,
               this,aCompressed,aPrinter)) {
    if(a_push) fDirs.pop();
    return false;
  } else {
    if(fCurrent) {
      if(!solveReferences(*fCurrent,aCompressed,aPrinter)) {
        clear();
        if(a_push) fDirs.pop();
        return false;
      } else {
        fCurrent->setFile(aFile);
      }
    }
  }
  if(a_push) fDirs.pop();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::loadString(
 const std::string& aString
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  if(!parseBuffer(aString.size(),aString.c_str(),
                  (XML_StartElementHandler)startElement,
                  (XML_EndElementHandler)endElement,
                  this,aPrinter)) {
    clear();
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::loadBuffer(
 unsigned int aSize
,const char* aBuffer
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  if(!parseBuffer(aSize,aBuffer,
                  (XML_StartElementHandler)startElement,
                  (XML_EndElementHandler)endElement,
                  this,aPrinter)) {
    clear();
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::clear(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // In case of problem, deleting fCurrent is not sufficient.
  delete fTop; 
  fTop = 0;
  fCurrent = 0;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::solveReferences(
 Lib::ItemML& aItemML                      
,bool aCompressed
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aItemML.tagName()=="reference") {
    std::string sref;
    if(aItemML.attributeValue("","file",sref)) {
      //::printf("debug : Lib::BaseML::solveReferences : \"%s\"\n",
      //    sref.c_str());

      if(aItemML.isAttribute("package")) {
      } else {
        std::string path = Lib::smanip::dirname(sref);
        if(path.empty() ||  //file name only.
           ((path[0]!='$')&&(path[0]!='/')) ){ //path without env and relative.
          if(fDirs.size() && fDirs.back().size()) {
            sref = fDirs.back() + "/" + sref;
          }
        }
      }

     {std::string svalue;
      bool optional;
      if(aItemML.attributeValue("","optional",svalue) 
         && smanip::tobool(svalue,optional) ) {
        if(optional) {
          if(fFileGetter) { //Case of client/server.
            bool b;
            if(!fFileGetter->exists(sref,b)) return false;
            if(!b) return true;
          } else {
            if(!fmanip::exists(sref)) return true;
          }
        }
      }}

      if(aItemML.children().size()<=0) { // Not done.
        BaseML baseML(fItemMLFactory,fVerboseLevel);
        baseML.setTags(fTags);
        if(fFileGetter) { 
          //Case of client/server.
          std::string newName;
          if(!fFileGetter->fetch(sref,aItemML.attributes(),newName)) 
            return false;
          //::printf("debug : Lib::BaseML::solveReferences : fetch sref \"%s\" newName \"%s\"\n",sref.c_str(),newName.c_str());
          baseML.setFileGetter(fFileGetter);
          bool stat;
          std::string d = Lib::smanip::dirname(newName);
          if(d.empty()) {
            //client/server
            //newName is local. Have to push dir of sref.
            baseML.fDirs.push(Lib::smanip::dirname(sref));
            stat = baseML.loadFile(newName,aCompressed,aPrinter,false,false);
          } else {
            //obuild : <reference package=""/>
            baseML.fDirs.push(d);
            stat = baseML.loadFile(newName,aCompressed,aPrinter,false);
          }
          baseML.fDirs.pop();
          if(!stat) return false;
        } else {
          if(!baseML.loadFile(sref,aCompressed,aPrinter)) return false;
        }
        Lib::ItemML* top = baseML.topItem();
        baseML.empty();
        if(!top) return false; //FIXME : return true ? The file could be empty.
        // Put things in the Reference node.
        aItemML.addChild(top);
        top->setParent(&aItemML);
      }
    }
  }
 
  // Solve children :
  const std::list<Lib::ItemML*>& list = aItemML.children();
  std::list<Lib::ItemML*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!solveReferences(*(*it),aCompressed,aPrinter)) return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::readFile(
 const std::string& aFile
,XML_StartElementHandler aStartElement
,XML_EndElementHandler aEndElement
,void* aTag
,bool aCompressed
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  if(!parseFile(aFile,aStartElement,aEndElement,aTag,aCompressed,aPrinter)) {
    clear();
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
const Lib::ItemML* Lib::BaseML::topItem(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCurrent;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::BaseML::topItem(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCurrent;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::empty(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTop = 0;
  fCurrent = 0;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::isTag(
 const std::string& aString
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = fTags.size();
  for(unsigned int index=0;index<number;index++) {
    if(aString==fTags[index]) return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::characterDataHandler(
 void* aUserData
,const XML_Char* aString
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s;
  s.resize(aLength);
  unsigned int count = 0;
  char* p = (char*)aString;
  for (int i = 0; i < aLength; i++, p++) {
    if(!iscntrl(*p)) {
      s[count] = *p;
      count++;
    }
  }  
  if(count) {
    s.resize(count);
    Lib::BaseML* This = (Lib::BaseML*)aUserData;
    This->fValue += s;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::parseFile(
 const std::string& aFile
,XML_StartElementHandler aStartElement
,XML_EndElementHandler aEndElement
,void* aTag
,bool aCompressed
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fVerboseLevel) {
    Lib::Out out(aPrinter);
    out << "Lib::BaseML::parseFile :" 
        << " parse file " << Lib::sout(aFile) << "..." << Lib::endl;
  }
  fErrors = 0;
  Slash::Core::IReader* reader = 0;
  if(aCompressed) {
    /*
    if(!Lib::GZipReader::available()) {
      Lib::Out out(aPrinter);
      out << "Lib::BaseML::parseFile :"
          << " compression not available. " 
          << " check installation of Lib package "
          << " and Lib::GZipReader in particular. "
          << Lib::endl;
      return false;
    }
    if(fVerboseLevel) {
      Lib::Out out(aPrinter);
      out << "Lib::BaseML::parseFile :" 
          << " file " << Lib::sout(aFile) << " is compressed." << Lib::endl;
    }
    reader = new Lib::GZipReader(aFile,aPrinter);
    */
    Lib::Out out(aPrinter);
    out << "Lib::BaseML::parseFile :" 
        << " osc_17 : compression not restored for " << aFile 
        << Lib::endl;
  } else {
    /*
    // May be compressed anyway :
    if(Lib::GZipReader::available()) {
      reader = new Lib::GZipReader(aFile,aPrinter);
      if(reader && reader->enabled()) {
        // Seems to be compressed.
      } else {
        delete reader;
        reader = new Lib::FileReader(aFile,aPrinter);
      }
    } else {
    */
      reader = new Lib::FileReader(aFile,aPrinter);
      //}
  }
  if(!reader || !reader->enabled()) {
    Lib::Out out(aPrinter);
    out << "Lib::BaseML::parseFile :" 
        << " can't open file " << aFile << Lib::endl;
    delete reader;
    return false;
  }
  fPrinter = &aPrinter;
  fDepth = 0;
  fAbort = false;
  //char buf[1024 * BUFSIZ];
  char buf[BUFSIZ];
  XML_Parser parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser,aTag);
  XML_SetElementHandler(parser,aStartElement,aEndElement);
  XML_SetCharacterDataHandler(parser,
              (XML_CharacterDataHandler)Lib::BaseML::characterDataHandler);
  //XML_SetProcessingInstructionHandler(parser,
  //      processingInstructionHandler);
  int done = 0;
  do {
    int len;
    if(!reader->read(buf,sizeof(buf),len)) {
      XML_ParserFree(parser);
      delete reader;
      return false;
    }
    done = len < (int)sizeof(buf) ? 1 : 0;
    if (!XML_Parse(parser, buf, len, done)) {
      Lib::Out out(aPrinter);
      out << "Lib::BaseML::parseFile :" 
          << " in file " << Lib::sout(aFile)
          << " " << XML_ErrorString(XML_GetErrorCode(parser)) 
          << " at line " << XML_GetCurrentLineNumber(parser) << Lib::endl;
      XML_ParserFree(parser);
      delete reader;
      return false;
    }
    if(fAbort) {
      XML_ParserFree(parser);
      delete reader;
      return false;
    }
  } while (!done);
  XML_ParserFree(parser);
  delete reader;
  if(fVerboseLevel) {
    Lib::Out out(aPrinter);
    out << "Lib::BaseML::parseFile :" 
        << " parse file " << Lib::sout(aFile) << " done." << Lib::endl;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::BaseML::parseBuffer(
 unsigned int aSize
,const char* aBuffer
,XML_StartElementHandler aStartElement
,XML_EndElementHandler aEndElement
,void* aTag
,Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fErrors = 0;
  if(!aSize) return true; //nothing to do.
  fPrinter = &aPrinter;
  fDepth = 0;
  fAbort = false;
  XML_Parser parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser,aTag);
  XML_SetElementHandler(parser,aStartElement,aEndElement);
  XML_SetCharacterDataHandler(parser,
              (XML_CharacterDataHandler)Lib::BaseML::characterDataHandler);
  //XML_SetProcessingInstructionHandler(parser,
  //      processingInstructionHandler);
  char* buf = (char*)aBuffer;
  unsigned int l = aSize;
  int done = 0;
  do {
    size_t len = MINIMUM(l,BUFSIZ);
    done = len < BUFSIZ ? 1 : 0;
    if (!XML_Parse(parser, buf, len, done)) {
      Lib::Out out(aPrinter);
      out << "Lib::BaseML::parseBuffer :" 
          << " " << XML_ErrorString(XML_GetErrorCode(parser)) 
          << " at line " << XML_GetCurrentLineNumber(parser) 
          << " at byte index " << (int)XML_GetCurrentByteIndex(parser)
          << Lib::endl; 
     {int pos = XML_GetCurrentByteIndex(parser);
      int pmn = MAXIMUM(pos-10,0);
      int pmx = MINIMUM(pos+10,aSize-1);
      std::string c = " ";
     {for(int p=pmn;p<=pmx;p++) {c[0] = *(aBuffer+p);out << c;}
      out << Lib::endl;}
     {for(int p=pmn;p<pos;p++) out << " ";
      out << "^" << Lib::endl;}}
      XML_ParserFree(parser);
      return false;
    }
    if(fAbort) {
      XML_ParserFree(parser);
      return false;
    }
    buf += len;
    l -= len;
  } while (!done);
  XML_ParserFree(parser);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::startElement(
 void* aUserData
,const XML_Char* aName
,const XML_Char** aAtts
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::BaseML* This = (Lib::BaseML*)aUserData;
  if(This->fAbort) return; //Do nothing.

  This->fDepth++;
  This->fValue = "";

  std::string name = aName; //Can't be empty
  if(This->isTag(name)) {

    if(!This->fCurrent) {
      if(This->fDepth==1) {
        // Ok. Head.
      } else {
        Lib::Out out(*(This->fPrinter));
        out << "Lib::BaseML::startElement :" 
            << " no tag with a depth of " << This->fDepth
            << Lib::endl; 
        This->fAbort = true;
        return;
      }
    } else {
      int delta = This->fCurrent->depth() - This->fDepth;
      if(delta>=1) {
        Lib::Out out(*(This->fPrinter));
        out << "Lib::BaseML::startElement :" 
            << " for element " << Lib::sout(name) 
            << " tag with a delta depth of " << delta
            << Lib::endl; 
        This->fAbort = true;
        return;
      }
    }

    std::vector<Lib::ItemML::Attribute> atbs;
   {const XML_Char** a_atts = aAtts;
    while((*a_atts)&&(*(a_atts+1))) {
      atbs.push_back(Lib::ItemML::Attribute(*a_atts,*(a_atts+1)));
      a_atts+=2;
    }}

    ItemML* parent = This->fCurrent;
    Lib::ItemML* element = 
      This->fItemMLFactory.create(name,atbs,parent);
    if(!element) {
      Lib::Out out(*(This->fPrinter));
      out << "Lib::BaseML::startElement :" 
          << " can't create a Lib::ItemML for tag " << Lib::sout(name) 
          << Lib::endl; 
      This->fAbort = true;
      return;
    }

    //Lib::Out out(*(This->fPrinter));
    //out << "Lib::BaseML::startElement :" << Lib::endl;
    //element->printXML(*(This->fPrinter),"debug : ");

    if(parent) parent->addChild(element);

    This->fCurrent = element;
    element->setDepth(This->fDepth); // Internal only.

    if(!This->fTop) This->fTop = element;

  } else {

    if(!This->fCurrent) {

      // Can't be in a non-tag without a tag !
      Lib::Out out(*(This->fPrinter));
      out << "Lib::BaseML::startElement :" 
          << " for element " << Lib::sout(name) 
          << " non-tag without some parent tag."
          << Lib::endl; 
      This->fAbort = true;
      return;

    } else {

      int delta =  This->fDepth - This->fCurrent->depth();
      if(delta>1) {

        Lib::Out out(*(This->fPrinter));
        out << "Lib::BaseML::startElement :" 
            << " for element " << Lib::sout(name) 
            << " grand child of a tag."
            << Lib::endl; 
        This->fAbort = true;
        return;

      } else if(delta==1) { //ok

        This->fAtbs.clear();
       {const XML_Char** a_atts = aAtts;
        while((*a_atts)&&(*(a_atts+1))) {
          This->fAtbs.push_back(Lib::ItemML::Attribute(*a_atts,*(a_atts+1)));
          a_atts+=2;
        }}

      } else {

        Lib::Out out(*(This->fPrinter));
        out << "Lib::BaseML::startElement :" 
            << " for element " << Lib::sout(name) 
            << " non-tag with a delta depth of " << delta
            << Lib::endl; 
        This->fAbort = true;
        return;

      }
    }

  }
}
//////////////////////////////////////////////////////////////////////////////
void Lib::BaseML::endElement(
 void* aUserData
,const XML_Char* aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::BaseML* This = (Lib::BaseML*)aUserData;
  if(This->fAbort) return; //Do nothing.

  if(This->fCurrent) {

    ItemML* element = This->fCurrent;
    int delta = This->fDepth - element->depth();
    if(delta==0) { //Back to a tag :
      
      ItemML* parent = element->parent();

      bool keep = false;
      bool cont = This->visitEndElement(*element,keep);
      if(keep) {
        if(parent) {
          This->fCurrent = parent;
        }
      } else {
        //FIXME : the top could be recreated !
        if(This->fTop==element) This->fTop = 0;

        if(parent) {
          parent->removeChild(element); //delete the element
        } else {
          delete element;
        }

        This->fCurrent = parent; //parent could be 0 : ok.
      }

      if(!cont) This->fAbort = true;

    } else if(delta==1) { //Back to a child of tag :

      //FIXME : correct fValue ? (Can we pick the one of a sub item ?)
      element->addProperty(std::string(aName),This->fAtbs,This->fValue); 
      //This->fValue = "";

    } else {

      Lib::Out out(*(This->fPrinter));
      out << "Lib::BaseML::endElement :" 
          << " problem for element " << Lib::sout(std::string(aName)) 
          << " : delta depth of " << delta
          << Lib::endl; 
      This->fAbort = true;

    }

  }


  This->fDepth--;
}
