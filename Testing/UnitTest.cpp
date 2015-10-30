#include "TestingPrecompiled.h"
#include "Debugging/DebugInterface.h"
#include "UnitTest.h"

namespace WickedSick
{
  int UnitTest::total_tests_ = 0;

  UnitTest::UnitTest(testerFn test,
                     const std::string& category)
                      : test_(test),
                        category_(category),
                        test_number_(total_tests_++)
  {
    
  }

  bool UnitTest::Run(TestInfo& info)
  {
    //start mem
    //start timer
    
    info.success = false;
    try
    {
      info.success = test_();
    }
    catch (const std::exception&)
    {
      //info.bytesLeaked = get allocated bytes
      //info.time = get elapsed time
      info.reason = Exception;
      return info.success;
    }
    //end timer
    //end mem

    return info.success;
  }
}
