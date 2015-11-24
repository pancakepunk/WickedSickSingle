#include "Precompiled.h"
#include "TestingPrecompiled.h"
#include "Tester.h"

#include "Debugging/DebugInterface.h"

#include "UnitTest.h"
#include "TestInfo.h"

namespace WickedSick
{
  Tester::Tester() :  mem_check_(false),
                      time_check_(false),
                      silent_(false)
  {

  }

  void Tester::AddTest(UnitTest* newTest)
  {
    tests_.push_back(newTest);
  }

  void Tester::AddTest(testerFn test,
                       const std::string& cat)
  {
    UnitTest* newTest = testManager.New(test, cat);
    tests_.push_back(newTest);
  }

  bool Tester::RunTest(int testNumber)
  {
    TestInfo info;
    ConsolePrint("Running Test" + std::to_string(testNumber));
    bool returnVal = tests_[testNumber]->Run(info);
    
    
    if (returnVal && !silent_)
    {
      ConsolePrint("Test" + std::to_string(testNumber) + " Successfully Completed"/**/);/**-/ +/**/
      if (time_check_)
      {
        ConsolePrint( " in " + std::to_string(info.time) + "ms");
      }
      if (mem_check_)
      {
        ConsolePrint( " with " + std::to_string(info.bytesAllocated) + " bytes allocated"
                      " and " + std::to_string(info.bytesLeaked) + " bytes leaked");
      }
      ConsolePrint(".\n");
    }
    else if (!returnVal)
    {
      ConsolePrint("Test" + std::to_string(testNumber) + " Failed"/**/);/**-/ +/**/
      if (time_check_)
      {
        ConsolePrint( " in " + std::to_string(info.time) + "ms");
      }
      if (mem_check_)
      {
        ConsolePrint( " with " + std::to_string(info.bytesAllocated) + " bytes allocated"
                      " and " + std::to_string(info.bytesLeaked) + " bytes leaked");
      }
      ConsolePrint(".\n");
    }


    return returnVal;
  }

  bool Tester::RunTestCategory(const std::string& category)
  {
    ConsolePrint("Running all tests with category " + category + ".\n");
    bool retVal = true;
    for (unsigned i = 0; i < tests_.size(); ++i)
    {
      if (tests_[i]->category_ == category)
      {
        retVal = (RunTest(i) && retVal);
      }
    }
    return retVal;
  }

  bool Tester::RunAllTests()
  {
    ConsolePrint("Running all tests.\n");
    bool retVal = true;
    for (unsigned i = 0; i < tests_.size(); ++i)
    {
      retVal = (RunTest(i) && retVal);
    }
    return retVal;
  }

}
