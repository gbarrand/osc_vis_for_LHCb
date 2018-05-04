/*
* CppUnit test class: Quad4FCppUnit.cpp
*
* adapted from test_Package_X_test1.cpp
*
* Use the HelperMacros.h header file to establish in a easy 
* way the test suite in which you can place all the tests 
* concerning the tested class or component.
*/

#include <cppunit/extensions/HelperMacros.h>

#include "Quad4F.h"
#include "Minuit/FunctionMinimum.h"
#include "Minuit/MnMigrad.h"
#include "Minuit/MnUserParameters.h"
#include "Minuit/MnPrint.h"

double sum_of_elements(const LASymMatrix&);

class Quad4FCppUnit : public CppUnit::TestFixture{
/// \brief  Definition of the unit test suite "MyClass_test ",
/// don't forget to add all those tests("test1",
/// "test2"... ) you would like to run. 
  CPPUNIT_TEST_SUITE( Quad4FCppUnit );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST( test2 );
  CPPUNIT_TEST_SUITE_END();

private:

  int theNFcn;
  double theSumOfEl;

public:
/// \brief Override setUp() to initialize the 
///        variables you will use for test
  void setUp () {

    Quad4F fcn;
    
    MnUserParameters upar;
    upar.add("x", 1., 0.1);
    upar.add("y", 1., 0.1);
    upar.add("z", 1., 0.1);
    upar.add("w", 1., 0.1);
    
    MnMigrad migrad(fcn, upar);
    FunctionMinimum min = migrad();
    std::cout<<"minimum: "<<min<<std::endl;

    theNFcn = migrad.numOfCalls();
    theSumOfEl = sum_of_elements(min.states().back().error().matrix());
  }

/// Write now your own tests, this is the first one
   void test1() {

     CPPUNIT_ASSERT( theNFcn == 74 );
   }
/// Write now your own tests, this is the first one
   void test2() {

     CPPUNIT_ASSERT_DOUBLES_EQUAL( theSumOfEl, 22., 1.e-8 );
   }

// Final clean up
//
  void tearDown() {

  }

};

// Class registration on cppunit framework
/// \brief You have to register the test suite "ComplexNumberTest". In this 
/// it will be recognized by the may test program which drives the 
/// different tests in a specific package test directory.
CPPUNIT_TEST_SUITE_REGISTRATION(Quad4FCppUnit);

// CppUnit test-driver common for all the cppunit test classes 
#include <CppUnit_testdriver.cpp>

