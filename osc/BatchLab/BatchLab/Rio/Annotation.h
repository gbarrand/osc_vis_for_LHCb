#ifndef BatchLab_Rio_Annotation_h
#define BatchLab_Rio_Annotation_h

namespace Rio {class IBuffer;}

namespace BatchLab {

class Annotation;

namespace Rio {

namespace Annotation {

  bool write(::Rio::IBuffer&,const BatchLab::Annotation&);
  bool read(::Rio::IBuffer&,BatchLab::Annotation&);

} //Annotation

} //Rio

} //BatchLab

#endif
