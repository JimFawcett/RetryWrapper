#pragma once
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
*/
template <typename Callable>
bool reTry(
  Callable co,
  std::chrono::duration<size_t, std::milli> duration = std::chrono::milliseconds(100),
  size_t maxCount = 10
)
{
  size_t count = 0;
  while (true)
  {
    try
    {
      co();
      return true;
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
    return false;
  }
}