///////////////////////////////////////////////////////////////////////
// MT3Q3.cpp - ReTry for callable object                             //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2018 //
///////////////////////////////////////////////////////////////////////

#include "MT3Q3.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <future>

const std::string file = "../MT3Q3/TestFiles/Test.txt";

//----< test state of stream instance >--------------------------------
/*
* - works for instances of any class that derives from std::ios
*   through the magic of Liskov Substitution.
*/
bool testStream(std::ios& strm, const std::string& name)
{
  if (strm.good())
  {
    std::cout << "\n  fstream " << name.c_str() << " opened successfully";
    return true;
  }
  else
  {
    std::cout << "\n  fstream " << name.c_str() << " failed to open";
    return false;
  }
}
//----< specialization of ErrorPolicy >--------------------------------

class StreamErrorPolicy
{
public:
  void setErrorState(std::ios& strm)
  {
    std::ios* pStream = dynamic_cast<std::ios*>(&strm);
    if (pStream != nullptr)
      pStream->setstate(std::ios::failbit);
  }
};
//----< demonstration >------------------------------------------------

int main()
{
  std::cout << "\n  MT3Q3 - ReTry operation";
  std::cout << "\n =========================";

  auto openFileForReading = [=](const std::string& fileSpec)->std::ifstream
  {
    std::ifstream in;
    in.open(fileSpec, std::ios::in, _SH_DENYWR);  // Windows specific shared reading
    if (!in.good())
      throw("io exception");
    return std::move(in);
  };

  auto openFileForWriting = [=](const std::string& fileSpec)->std::ofstream
  {
    std::ofstream out;
    out.open(fileSpec, std::ios::out | std::ios::ate, _SH_DENYRW);  // Windows specific
    if (!out.good())
      throw("io exception");
    return std::move(out);
  };

  std::cout << "\n -------------------------------------------------------------------";
  std::cout << "\n  attempting to open \"" << file.c_str() << "\" with ofstream out";
  std::cout << "\n -------------------------------------------------------------------";

  // should succeed
  std::ofstream out = openFileForWriting(file);
  if (testStream(out, "out"))
  {
    std::cout << "\n  writing to file:";
    std::string str = "\n  first line";
    out << str;
    std::cout << str;
    str = "\n  second line";
    out << str;
    std::cout << str;
  }
  bool debug = out.good();

  std::cout << "\n -------------------------------------------------------------------";
  std::cout << "\n  attempting to open \"" << file.c_str() << "\" with ofstream out2";
  std::cout << "\n -------------------------------------------------------------------";

  // should fail - out still open
  StreamErrorPolicy sep;
  using RO = std::ofstream;
  using A = std::string;
  using CW = decltype(openFileForWriting);

  std::ofstream out2 = reTry<RO,CW,A,StreamErrorPolicy>(openFileForWriting, file, sep);
  if (testStream(out2, "out2"))
  {
    std::cout << "\n  writing to file:";
    std::string str = "\n  first line";
    out2 << str;
    std::cout << str;
    str = "\n  second line";
    out2 << str;
    std::cout << str;
  }
  debug = out2.good();
  out2.close();

  std::cout << "\n -------------------------------------------------------------------";
  std::cout << "\n  attempting to open \"" << file.c_str() << "\" with ifstream in";
  std::cout << "\n -------------------------------------------------------------------";
  
  // should fail - out still open
  using RI = std::ifstream;
  using CR = decltype(openFileForReading);
  std::ifstream in = reTry<RI,CR,A,StreamErrorPolicy>(openFileForReading, file, sep);
  if (testStream(in, "in"))
  {
    in.seekg(0, std::ios::end);
    size_t bytes = static_cast<size_t>(in.tellg());
    std::cout << "\n  file size = " << bytes << " bytes";
    in.seekg(std::ios::beg);
    std::cout << "\n  reading from file:";
    std::cout << in.rdbuf();
  }
  in.close();

  std::cout << "\n ----------------------";
  std::cout << "\n  closing ofstream out";
  std::cout << "\n ----------------------";
  out.close();

  std::cout << "\n -------------------------------------------------------------------";
  std::cout << "\n  attempting to open \"" << file.c_str() << "\" with ifstream in";
  std::cout << "\n -------------------------------------------------------------------";

  // should succeed - out closed
  std::ifstream in2 = reTry<RI, CR, A, StreamErrorPolicy>(openFileForReading, file, sep);
  if (testStream(in2, "in"))
  {
    in2.seekg(0, std::ios::end);
    size_t bytes = static_cast<size_t>(in2.tellg());
    std::cout << "\n  file size = " << bytes << " bytes";
    in2.seekg(std::ios::beg);
    std::cout << "\n  reading from file:";
    std::cout << in2.rdbuf();
  }
  std::cout << "\n";

  std::cout << "\n -------------------------------------------------------------------";
  std::cout << "\n  attempting to open \"" << file.c_str() << "\" with ifstream in";
  std::cout << "\n -------------------------------------------------------------------";

  // should succeed - in open for sharing
  std::ifstream in3 = reTry<RI, CR, A, StreamErrorPolicy>(openFileForReading, file, sep);
  if (testStream(in3, "in"))
  {
    in3.seekg(0, std::ios::end);
    size_t bytes = static_cast<size_t>(in3.tellg());
    std::cout << "\n  file size = " << bytes << " bytes";
    in3.seekg(std::ios::beg);
    std::cout << "\n  reading from file:";
    std::cout << in3.rdbuf();
  }
  std::cout << "\n";
  in.close();

  std::cout << "\n  raw contents of file:";
  in2.seekg(std::ios::beg);
  int count = 0;
  while (true)
  {
    int i = in2.get();
    if (in2.eof())
    {
      std::cout << i << std::endl;
      break;
    }
    if(i == 10)  // newline
    {
      std::cout << "\n";
    }
    std::cout << std::setw(3) << i << ":";
  }
  in2.close();
  in3.close();

  std::cout << "\n\n";
  return 0;
}


