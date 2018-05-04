#ifndef BatchLab_HDF5_HDF5O_h
#define BatchLab_HDF5_HDF5O_h

#include <string>
#include <vector>

#include <exlib/hdf5/hdf5_h>

namespace BatchLab {

namespace HDF5 {

  class HDF5O {
  public:
    static bool checkSizes();

    static hid_t string_datatype(unsigned int);

    static hid_t basic_mem_type(hid_t);
    static hid_t compound_mem_type(hid_t);

    // std::string :
    static bool read_string(hid_t,const std::string&,std::string&);
    static bool read_array_string(hid_t,const std::string&,
                                  std::vector<std::string>&);
    static bool put_array_string(const std::vector<std::string>&,char*);
    static bool put_array_string(unsigned int,char*,
                                 std::vector<std::string>&);

    // atbs :
    static bool read_atb(hid_t,const std::string&,
                         std::string&,unsigned int = 100);
    static bool read_atb(hid_t,const std::string&,int&);
    static bool read_atb(hid_t,const std::string&,unsigned int&);

    // Else :
    static void dump(hid_t,const std::string&);
  public:
    HDF5O(int level = 1);
    virtual ~HDF5O();
    bool write_atb(hid_t,const std::string&,const std::string&);
    bool write_atb(hid_t,const std::string&,int);
    bool write_atb(hid_t,const std::string&,unsigned int);
    bool write_object(hid_t,const std::string&,hid_t,unsigned int,char*);
    bool read_object(hid_t,const std::string&,unsigned int&,char*&);
  private:
    int fCompress;
    hid_t fScalar;
    hid_t fSimple;
    hid_t fCPT;
  };

} //HDF5

} //BatchLab

#endif
