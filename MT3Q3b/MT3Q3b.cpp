///////////////////////////////////////////////////////////////////////
// MT3Q3.cpp - ReTry for callable object                             //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2018 //
///////////////////////////////////////////////////////////////////////

#include "MT3Q3b.h"
#include <iostream>
#include "../StringUtilities/StringUtilities.h"

int main()
{
  Utilities::Title("MT3Q3b - simple reTry");
  auto throws = []() { throw("always throws"); };
  auto noThrow = []() { "never throws"; };

  if (reTry(noThrow, std::chrono::milliseconds(100), 5))
    std::cout << "\n  operation succeeded";
  else
    std::cout << "\n  operation failed";
  if (reTry(throws, std::chrono::milliseconds(100), 5))
    std::cout << "\n  operation succeeded";
  else
    std::cout << "\n  operation failed";
  std::cout << "\n\n";
  return 0;
}

