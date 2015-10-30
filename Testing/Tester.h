#pragma once

#include "Utility/UtilityInterface.h"

namespace WickedSick
{
  class UnitTest;
  typedef bool(*testerFn)(void);
  class Tester
  {
  public:

    Tester();

    void AddTest(UnitTest* newTest);

    void AddTest(testerFn test,
                 const std::string& category);

    bool RunTest(int testNumber);

    bool RunTestCategory(const std::string& category);

    bool RunAllTests();
  private:
    
    std::vector<UnitTest*> tests_;
    bool mem_check_;
    bool time_check_;
    bool silent_;

    MemoryManager<UnitTest> testManager;
  };
}
