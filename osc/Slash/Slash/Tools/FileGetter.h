#ifndef Slash_Tools_FileGetter_h
#define Slash_Tools_FileGetter_h

// Inheritance :
#include <exlib/xml/parser> //file_getter

#include <inlib/net/base_socket>
#include <inlib/sys/file>

//#include <Lib/Debug.h>
#include "../Net/protocol"

#include <algorithm>

namespace Slash {

class FileGetter : public virtual exlib::xml::file_getter {
public: //exlib::xml::file_getter:
  virtual bool fetch(const std::string& aFileName,
                     const exlib::xml::file_getter::args&,
                     std::string& aNewName) {

    //::printf("debug : Slash::FileGetter::fetch : want \"%s\"\n",
    //         aFileName.c_str());

    // Ask to the client the file :

    // The fact that the file is local is important.
    // If changing that, someone has to reconsider
    // the logic in exlib::xml::parser::[load_file,solve_references]

    if(!inlib::tmpname(".","Slash_FileGetter_","",aNewName)) {
      f_out << "Slash::FileGetter::fetch : "
            << " can't get tmp file name."
            << std::endl;
      return false;
    }

    //::printf("debug : Slash::FileGetter::fetch : local \"%s\"\n",
    //         aNewName.c_str());

    if(!fSocket.send_string(inlib_net_protocol_file)) return false;
    if(!fSocket.send_string(aFileName)) return false;
    if(!fSocket.fetch_file(aNewName)) return false;
    fFiles.push_back(aNewName);
    return true;
  }  
  virtual bool exists(const std::string& aFileName,bool& aValue) {
    if(!fSocket.send_string(inlib_net_protocol_file_exists)) {
      aValue = false;
      return false;
    }
    if(!fSocket.send_string(aFileName)) {
      aValue = false;
      return false;
    }
    std::string s;
    if(!fSocket.fetch_string(s)) {aValue = false;return false;}
    if(!inlib::to(s,aValue)) {
      f_out << "Slash::FileGetter::exists : "
          << " received value not a boolean " << inlib::sout(s) 
          << std::endl;
      return false;
    }
    return true;  
  }
public:
  FileGetter(std::ostream& a_out,
             inlib::net::base_socket& aSocket)
  :f_out(a_out),fSocket(aSocket) {}
  virtual ~FileGetter() {
    std::for_each(fFiles.begin(),fFiles.end(),file_remove);
  }
private:
  FileGetter(const FileGetter& aFrom)
  :f_out(aFrom.f_out),fSocket(aFrom.fSocket){}
  FileGetter& operator=(const FileGetter&){return *this;}
private:
  static inline bool file_remove(const std::string& a_file) {
    return (::remove(a_file.c_str()) ==0 ? true : false);
  }
private:
  std::ostream& f_out;
  inlib::net::base_socket& fSocket;
  std::vector<std::string> fFiles;
};

}

#endif




