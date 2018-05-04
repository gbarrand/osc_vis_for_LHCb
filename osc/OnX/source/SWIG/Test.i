%inline {

// To test technicalities (mainly typemaps) :

class OnX_SWIG_Test {
public:
  OnX_SWIG_Test() {
    fVectorString.push_back("Item 1");
    fVectorString.push_back("Item 2");
    fVectorString.push_back("Item 3");

    fVectorDouble.push_back(1.1);
    fVectorDouble.push_back(2.2);
    fVectorDouble.push_back(3.3);

    fVectorInt.push_back(1);
    fVectorInt.push_back(2);
    fVectorInt.push_back(3);
  } 
  virtual ~OnX_SWIG_Test() {}
  // std::vector<std::string> : 
  void arg_const_std_vec_string(const std::vector<std::string>& aVector) {
    printf("OnX_SWIG_Test::arg_const_std_vec_string : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%s\n",aVector[index].c_str());
    }
  }
  void arg_std_vec_string(std::vector<std::string> aVector) {
    printf("OnX_SWIG_Test::arg_std_vec_string : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%s\n",aVector[index].c_str());
    }
  }
  std::vector<std::string> ret_std_vec_string() const { return fVectorString;}
  const std::vector<std::string>& ret_const_std_vec_string() const { 
    return fVectorString;
  }
  // std::vector<double> : 
  void arg_const_std_vec_double(const std::vector<double>& aVector) {
    printf("OnX_SWIG_Test::arg_const_std_vec_double : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%g\n",aVector[index]);
    }
  }
  void arg_std_vec_double(std::vector<double> aVector) {
    printf("OnX_SWIG_Test::arg_std_vec_double : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%g\n",aVector[index]);
    }
  }
  std::vector<double> ret_std_vec_double() const { return fVectorDouble;}
  const std::vector<double>& ret_const_std_vec_double() const { 
    return fVectorDouble;
  }

  // std::vector<int> : 
  void arg_const_std_vec_int(const std::vector<int>& aVector) {
    printf("OnX_SWIG_Test::arg_const_std_vec_int : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%d\n",aVector[index]);
    }
  }
  void arg_std_vec_int(std::vector<int> aVector) {
    printf("OnX_SWIG_Test::arg_std_vec_int : %d\n",
      (int)aVector.size());
    for(unsigned int index=0;index<aVector.size();index++){
      printf("%d\n",aVector[index]);
    }
  }
  std::vector<int> ret_std_vec_int() const { return fVectorInt;}
  const std::vector<int>& ret_const_std_vec_int() const { 
    return fVectorInt;
  }
private:
  std::vector<std::string> fVectorString;
  std::vector<double> fVectorDouble;
  std::vector<int> fVectorInt;
};

}
