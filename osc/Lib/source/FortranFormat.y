%{

// this :
#include <Lib/FortranFormat.h>

#include <inlib/sprintf>
#include <inlib/cast>

#include <Lib/FortranFormatLexer.h>
#include <Lib/Debug.h>

extern int yylex(Lib::FortranFormatLexer&);
static int yyparse(void);
static void yyerror(Lib::FortranFormat&,const char*); 

//#define LIB_DEBUG

%}

%union {
  unsigned long fInteger;
  Lib::DataDescriptor* fDesc;
  unsigned int fType;
}

%token <fInteger> NUMBER_INTEGER NUMBER_EXPONENT

%token T_X T_I T_F T_E T_G T_A T_L T_SLASH

%type <fDesc> data_desc_tree
%type <fDesc> data_desc_list
%type <fDesc> data_desc_leaf
%type <fDesc> data_desc_one
%type <fType> data_type

%left '-' '+'
%left '/' '*'
%nonassoc UMINUS

%%

result 
: error {
  // To reset lex and yacc in case of error.
} | data_desc_tree {
  aParser.fTree.fChildren.push_back($1);
  $1->fParent = &(aParser.fTree);
  aParser.fTmpDescs.pop_back();
};

data_desc_tree 
: '(' data_desc_list ')' {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_tree : (data_desc_list)" 
        << std::endl;
  }
  $2->fMultiplicity = 1;
  $$ = $2;
} | NUMBER_INTEGER '(' data_desc_list ')' {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_tree : integer(data_desc_list)" 
        << std::endl;
  }
  $3->fMultiplicity = $1;
  $$ = $3;
};

data_desc_list 
: data_desc_leaf {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_leaf" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back($1);
  $1->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);//To cleanup in case of error.
  $$ = list;
} | data_desc_tree {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_tree" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back($1);
  $1->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);//To cleanup in case of error.
  $$ = list;
} | data_desc_list ',' data_desc_list {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_tree,data_desc_tree" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back($1);
  $1->fParent = list;
  list->fChildren.push_back($3);
  $3->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);//To cleanup in case of error.
  $$ = list;
};

data_desc_leaf 
: data_desc_one {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_leaf : data_desc_one" 
        << std::endl;
  }
  $1->fMultiplicity = 1;
  $$ = $1;
} | NUMBER_INTEGER data_desc_one {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_leaf : integer data_desc_one" 
        << std::endl;
  }
  $2->fMultiplicity = $1;
  $$ = $2;
};

data_desc_one
: data_type {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);//To cleanup in case of error.
  leaf->fType = $1;
  $$ = leaf;
} | data_type NUMBER_INTEGER {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);//To cleanup in case of error.
  leaf->fType = $1;
  leaf->fWidth = $2;
  $$ = leaf;
} | data_type NUMBER_INTEGER '.' NUMBER_INTEGER {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer.integer" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);//To cleanup in case of error.
  leaf->fType = $1;
  leaf->fWidth = $2;
  leaf->fPrecision = $4;
  $$ = leaf;
} | data_type NUMBER_INTEGER '.' NUMBER_INTEGER NUMBER_EXPONENT {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer.integerEinteger" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);//To cleanup in case of error.
  leaf->fType = $1;
  leaf->fWidth = $2;
  leaf->fPrecision = $4;
  leaf->fExponent = $5;
  $$ = leaf;
};

data_type  
: T_X { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_X" 
        << std::endl;
  }
  $$ = DataDescriptor_X;
} | T_I { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_I" 
          << std::endl;
  }
  $$ = DataDescriptor_I;
} | T_F { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_F" 
        << std::endl;
  }
  $$ = DataDescriptor_F;
} | T_E {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_E" 
        << std::endl;
  }
  $$ = DataDescriptor_E;
} | T_G {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_G" 
        << std::endl;
  }
  $$ = DataDescriptor_G;
} | T_A {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_A" 
          << std::endl;
  }
  $$ = DataDescriptor_A;
} | T_L {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_L" 
          << std::endl;
  }
  $$ = DataDescriptor_L;
} | T_SLASH {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_SLASH" 
        << std::endl;
  }
  $$ = DataDescriptor_SLASH;
}
;

%%

//////////////////////////////////////////////////////////////////////////////
Lib::FortranFormat::FortranFormat(
 const std::string& aString
,std::ostream& a_out
,int aVerboseLevel
) 
:fVerboseLevel(aVerboseLevel)
,fString(aString)
,f_out(a_out)
,fLexer(a_out,aVerboseLevel)
,fSuccess(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Lib::FortranFormat::~FortranFormat(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
}

// private.
Lib::FortranFormat::FortranFormat(const FortranFormat& aFrom) 
:fVerboseLevel(aFrom.fVerboseLevel)
,fString(aFrom.fString)
,f_out(aFrom.f_out)
,fLexer(aFrom.f_out,fVerboseLevel)
,fSuccess(false)
{}

// private.
Lib::FortranFormat& Lib::FortranFormat::operator=(const FortranFormat&) {return *this;}

void Lib::FortranFormat::setScript(const std::string& aString) {fString = aString;}

const std::string& Lib::FortranFormat::script() const {return fString;}

bool Lib::FortranFormat::parse() {
  clear();
  if(fString.empty()) return false;
  fLexer.set(fString);
  fSuccess = true;
  yyparse(*this);
  if(!fSuccess) clear();
  return fSuccess;
}

const Lib::DataDescriptor& Lib::FortranFormat::result() const {return fTree;}

void Lib::FortranFormat::clear() {
  fTree.clear();

  // In case of error :
 {std::vector<DataDescriptor*>::iterator it;
  for(it=fTmpDescs.begin();it!=fTmpDescs.end();++it) delete (*it);
  fTmpDescs.clear();}
}

bool Lib::FortranFormat::isSuccess() const {return fSuccess;}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

std::ostream& Lib::FortranFormat::out() const {return f_out;}

Lib::FortranFormatLexer& Lib::FortranFormat::lexer() {return fLexer;}

static void yyerror(Lib::FortranFormat& aParser,const char* aString) {
  aParser.out() << "FortranFormat : " << aString << std::endl;
  aParser.fSuccess = false;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor::DataDescriptor(
)
:fParent(0)
,fIndex(0)
,fMultiplicity(1)
,fType(0) //Unknown by default
,fWidth(0)
,fPrecision(0)
,fExponent(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("Lib::DataDescriptor");
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor::~DataDescriptor(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  Lib::Debug::decrement("Lib::DataDescriptor");
}

void Lib::DataDescriptor::clear() {
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) delete *it;
  fChildren.clear();
}

void Lib::DataDescriptor::reset(bool aDoThis){
  // Descent the tree to reset all the fIndex to 0.
  if(aDoThis) fIndex = 0;
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) (*it)->reset();
}

Lib::DataDescriptorIterator* Lib::DataDescriptor::createIterator() const {
  INLIB_SELF(DataDescriptor);
  return new Lib::DataDescriptorIterator(self);
}

Lib::DataDescriptor* Lib::DataDescriptor::firstLeaf() {
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    Lib::DataDescriptor* leaf = (*it)->firstLeaf();
    if(leaf) return leaf;
  }
  if(!fChildren.size()) return 0;
  return fChildren[0];
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor* Lib::DataDescriptor::nextChild(
 Lib::DataDescriptor* aChild
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = fChildren.size(); 
  if(!aChild && number) return fChildren[0];
  for(unsigned int index=0;index<number;index++) {
    if(aChild==fChildren[index]) {
      if((index+1)<number) {
        return fChildren[index+1];
      } else { 
        return 0;
      }
    }
  }
  return 0; //Should never pass here.
}
//////////////////////////////////////////////////////////////////////////////
void Lib::DataDescriptor::dump(
 std::ostream& a_out
,int aIndent
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string sindent(aIndent,' ');

  if(!fChildren.size()) { // Leaf :
    a_out << sindent;
    a_out << fMultiplicity;
    if(fType==DataDescriptor_X) a_out << "X";
    else if(fType==DataDescriptor_I) a_out << "I";
    else if(fType==DataDescriptor_F) a_out << "F";
    else if(fType==DataDescriptor_E) a_out << "E";
    else if(fType==DataDescriptor_G) a_out << "G";
    else if(fType==DataDescriptor_A) a_out << "A";
    else if(fType==DataDescriptor_SLASH) a_out << "/";
    else a_out << "?";
    a_out << fWidth;
    a_out << ".";
    a_out << fPrecision;
    a_out << "E";
    a_out << fExponent;
    a_out << std::endl;    
  } else {

    a_out << sindent;
    a_out << fMultiplicity;
    a_out << std::endl;    

   {std::vector<DataDescriptor*>::const_iterator it;
    for(it=fChildren.begin();it!=fChildren.end();++it) {
      (*it)->dump(a_out,aIndent+2);
    }}
  }

}

std::string Lib::DataDescriptor::cformat() const {
  INLIB_SELF(DataDescriptor);

       if(fType==DataDescriptor_X) {
    return "";
  } else if(fType==DataDescriptor_I) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dd",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_F) {
    // # : to force having a decimal point, even if precision is 0.
    std::string cformat;
    inlib::sprintf(cformat,64,"%%#%d.%df",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_E) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dE",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_G) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dG",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_A) {
    return "%s";
  } else if(fType==DataDescriptor_SLASH) {
    return "";
  } else {
    return "";  
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptorIterator::DataDescriptorIterator(
 Lib::DataDescriptor& aTree
)
:fTree(aTree)
,fDesc(0)
,fFinished(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("Lib::DataDescriptorIterator");
  fTree.reset();
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptorIterator::~DataDescriptorIterator(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::decrement("Lib::DataDescriptorIterator");
}
//////////////////////////////////////////////////////////////////////////////
void Lib::DataDescriptorIterator::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTree.reset();
  fDesc = 0;
  fFinished = false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::DataDescriptorIterator::finished(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFinished;
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor* Lib::DataDescriptorIterator::next(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFinished) return 0;
  if(!fDesc) { //Begin of iteration.
    fDesc = &fTree;
    fDesc->fIndex = 0;
  } else { // Repeat again ?
    if((fDesc->fIndex+1)<(int)fDesc->fMultiplicity) {
      fDesc->fIndex++;
      fDesc->reset(false);
      return fDesc; // The same.
    }
  }
  // New desc :
  while(true) {
    Lib::DataDescriptor* leaf = fDesc->firstLeaf();
    if(leaf) {
      fDesc = leaf;
      fDesc->fIndex = 0;
   /*{Lib::DataDescriptor* parent = fDesc->fParent;
      while(parent) {
        parent->fIndex = 0;
        parent = parent->fParent;          
      }}*/
      return leaf;
    }
    // Back to parent and after current fDesc :
    Lib::DataDescriptor* parent = fDesc->fParent;
    while(1) {
      if(!parent) { //Tree finished.      
        fDesc = 0;
        fFinished = true;
        return 0;
      }
      fDesc = parent->nextChild(fDesc);
      if(fDesc) break;
      // parent children exhausted.
      // Repeat again ?
      if((parent->fIndex+1)<(int)parent->fMultiplicity) {
        parent->fIndex++;
        parent->reset(false);
        fDesc = parent;
        break;
      } else {
        fDesc = parent;
        parent = parent->fParent;          
      }
    }
  }
}
