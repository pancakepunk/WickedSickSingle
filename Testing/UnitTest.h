#pragma once

#include "TestInfo.h"

namespace WickedSick
{
  
  typedef bool(*testerFn)(void);
  class UnitTest
  {
    
  public:
    UnitTest(testerFn test, 
             const std::string& category);
    bool Run(TestInfo& info);

    int test_number_;
    std::string category_;
  private:
    static int total_tests_;
    
    
    testerFn test_;
    
  };
}
