/*
* CppUnit test class: test_Package_X_test1.cpp 
*
* Use the HelperMacros.h header file to establish in a easy 
* way the test suite in which you can place all the tests 
* concerning the tested class or component.
*/
#include <cppunit/extensions/HelperMacros.h>

class MyClass_test : public CppUnit::TestFixture{
/// \brief  Definition of the unit test suite "MyClass_test ",
/// don't forget to add all those tests("test1",
/// "test2"... ) you would like to run. 
  CPPUNIT_TEST_SUITE( MyClass_test );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST( test2 );
  CPPUNIT_TEST_SUITE_END();

private:

  int alpha;
  int beta;

public:
/// \brief Override setUp() to initialize the 
///        variables you will use for test
  void setUp () {

  }
/// Write now your own tests, this is the first one
   void test1() {

     int a = 10;
     int b(10);
     CPPUNIT_ASSERT( a == b );
   }
/// Write now your own tests, this is the first one
   void test2() {

     float tol = 0.05;
     float a = 10.;
     float b = a + tol/10.;
     CPPUNIT_ASSERT_DOUBLES_EQUAL( a, b, tol );
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
CPPUNIT_TEST_SUITE_REGISTRATION(MyClass_test);

// CppUnit test-driver common for all the cppunit test classes 
#include <CppUnit_testdriver.cpp>

