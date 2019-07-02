///////////////////////////////////////////////////////////////////////
// MT3Q3.h - ReTry for callable object                               //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2018 //
///////////////////////////////////////////////////////////////////////

#include <chrono>
#include <thread>
#include <iostream>

//----< retry execution >--------------------------------------------//
/*
* - invokes callable object co
* - if exception is thrown, retries maxCount times
* - sleeps duration milliseconds between each try
* - tricky part is returning error value, which we do with ErrorPolicy
* - this is a great demonstration of why we may need templae policies
*/
template <typename Ret, typename Callable, typename Arg, typename ErrorPolicy>
Ret reTry(
  Callable co, 
  Arg arg, 
  ErrorPolicy ep,
  std::chrono::duration<size_t, std::milli> duration = std::chrono::milliseconds(100), 
  size_t maxCount = 10
)
{
  size_t count = 0;
  while (true)
  {
    try
    {
      Ret ret = co(arg);
      std::cout << "\n  operation successful";
      return ret;
    }
    catch (...)
    {
      if (++count < maxCount)
      {
        std::cout << "\n  operation failed - retrying";
        std::this_thread::sleep_for(duration);
        continue;
      }
    }
    std::cout << "\n  operation failed";
    Ret ret;
    ep.setErrorState(ret);

    ////////////////////////////////////////////////////////
    // This is what setErrorState does for streams:
    //   std::ios* pStream = dynamic_cast<std::ios*>(&ret);
    //   if (pStream != nullptr)
    //     pStream->setstate(std::ios::failbit);
    ////////////////////////////////////////////////////////

    return ret;
  }
}