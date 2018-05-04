// this :
#include <BatchLab/Rio/Annotation.h>

// Rio :
//#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

// BatchLab :
#include <BatchLab/Core/Annotation.h>

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Annotation::write(
 ::Rio::IBuffer& aBuffer
,const BatchLab::Annotation& aAnnotation
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  const std::vector<BatchLab::Item>& items = aAnnotation.items();

  int number = items.size();
  if(!aBuffer.write(number)) return false;
  for(int index=0;index<number;index++) {
    const BatchLab::Item& item = items[index];
    if(!aBuffer.write(item.fKey)) return false;
    if(!aBuffer.write(item.fValue)) return false;
    if(!aBuffer.write(item.fSticky)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Annotation::read(
 ::Rio::IBuffer& aBuffer
,BatchLab::Annotation& aAnnotation
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aAnnotation.clear(); //reset() does not remove sticky items.
  int number;
  if(!aBuffer.read(number)) return false;
  for(int index=0;index<number;index++) {
    std::string key;
    if(!aBuffer.read(key)) return false;
    std::string value;
    if(!aBuffer.read(value)) return false;
    bool sticky;
    if(!aBuffer.read(sticky)) return false;
    if(!aAnnotation.addItem(key,value,sticky)) return false;
  }
  return true;
}
