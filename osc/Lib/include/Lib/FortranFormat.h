#ifndef Lib_FortranFormat_h
#define Lib_FortranFormat_h 

#include <Lib/FortranFormatLexer.h>

#include <vector>

namespace Lib {

#define DataDescriptor_X 1
#define DataDescriptor_I 2
#define DataDescriptor_F 3
#define DataDescriptor_E 4
#define DataDescriptor_G 5
#define DataDescriptor_A 6
#define DataDescriptor_L 7
#define DataDescriptor_SLASH 8

class DataDescriptorIterator;

class DataDescriptor {
public:
  DataDescriptor();
  virtual ~DataDescriptor();
public:
  void clear();
  void dump(std::ostream&,int = 0) const;
  DataDescriptorIterator* createIterator() const;
  DataDescriptor* firstLeaf();
  DataDescriptor* nextChild(DataDescriptor*);
  void reset(bool = true);
  std::string cformat() const;
public:
  std::vector<DataDescriptor*> fChildren;
  DataDescriptor* fParent;  
  int fIndex;
  // Leaf data :
  unsigned int fMultiplicity;
  unsigned int fType;
  unsigned int fWidth;
  unsigned int fPrecision;
  unsigned int fExponent;
};

class DataDescriptorIterator {
public:
  DataDescriptorIterator(DataDescriptor&);
  virtual ~DataDescriptorIterator();
  DataDescriptor* next();
  void reset();
  bool finished() const;
public:
  DataDescriptor& fTree;
  DataDescriptor* fDesc;
  bool fFinished;
};

class FortranFormat {
public:
  FortranFormat(const std::string&,std::ostream&,int = 0);
  virtual ~FortranFormat();
private:
  FortranFormat(const FortranFormat&);
  FortranFormat& operator=(const FortranFormat&);
public:
  void setScript(const std::string&);
  const std::string& script() const;
  void clear(); 
  bool parse();
  bool isSuccess() const;
  const DataDescriptor& result() const;
public:  // Internal :
  FortranFormatLexer& lexer();
  std::ostream& out() const;
public:
  int fVerboseLevel;
  std::string fString;
  std::ostream& f_out;
  FortranFormatLexer fLexer;
  bool fSuccess;
  DataDescriptor fTree;
  std::vector<DataDescriptor*> fTmpDescs;
};

}

#endif
