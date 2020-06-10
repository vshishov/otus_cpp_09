#include "bayan.h"

#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  std::vector<std::string> vInputPaths, vExcludePaths;
  std::string strMask;
  int nLevel, nBlockSize;
  long nMinFileSize;
  po::options_description desc("Options");
  desc.add_options()
    ("include-path,i", po::value< std::vector<std::string> >(),              "include path for scaning")
    ("exclude-path,e", po::value< std::vector<std::string> >(),              "exclude path from scaning")
    ("mask,m",         po::value<std::string>(&strMask)->default_value("*"), "mask for file names allowed for comparison")
    ("level,l",        po::value<int>(&nLevel)->default_value(0),            "scan nesting level")
    ("min-size,s",     po::value<long>(&nMinFileSize)->default_value(1),      "minimum file size in bytes for scaning, bytes")
    ("block-size,b",   po::value<int>(&nBlockSize)->default_value(1),            "block size for reading file, bytes")
    ("hash",           po::value<std::string>(),                             "hash algorithm: crc32, md5")
    ("help,h",         "produce help message")  
  ;

  po::variables_map vm;
  
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }

    if (vm.count("include-path")) {
      vInputPaths = vm["include-path"].as< std::vector<std::string> >();
    }

    if (vm.count("exclude-path")) {
      vExcludePaths = vm["exclude-path"].as< std::vector<std::string> >();
    }
  }
  catch(std::exception& ex) {
    std::cout << desc << std::endl;
  }

  if (nMinFileSize < 1) {
    std::cerr << "Error: Minimum file size must be greater than 1!" << std::endl;
  }

  Otus::CBayan bayan;
  bayan.Exec(vInputPaths, vExcludePaths, nLevel, strMask, nMinFileSize);
  return 0;
}