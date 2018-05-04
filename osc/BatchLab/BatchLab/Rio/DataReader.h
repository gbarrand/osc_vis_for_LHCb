#ifndef BatchLab_Rio_DataReader_h
#define BatchLab_Rio_DataReader_h

namespace Slash {namespace Store {class IObject;}}

namespace BatchLab {namespace Rio {class ReadContext;}}

namespace BatchLab {

namespace Rio {

Slash::Store::IObject* read_TH1F(ReadContext&,bool&);
Slash::Store::IObject* read_TH2F(ReadContext&,bool&);
Slash::Store::IObject* read_TH1D(ReadContext&,bool&);
Slash::Store::IObject* read_TH2D(ReadContext&,bool&);
Slash::Store::IObject* read_TProfile(ReadContext&,bool&);
Slash::Store::IObject* read_Tree(ReadContext&,bool&);

} //Rio

} //BatchLab

#endif
