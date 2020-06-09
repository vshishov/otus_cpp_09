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
  int nLevel, nSize, nBlock;
  po::options_description desc("Options");
  desc.add_options()
    ("include-path,i", po::value< std::vector<std::string> >(),   "include path for scaning")
    ("exclude-path,e", po::value< std::vector<std::string> >(),   "exclude path from scaning")
    ("mask,m",         po::value<std::string>(),                  "masks for file names allowed for comparison")
    ("level,l",        po::value<int>(&nLevel)->default_value(0), "level scaning")
    ("min-size,s",     po::value<int>(&nSize)->default_value(1),  "minimum file size in bytes for scaning, bytes")
    ("block-size,b",   po::value<int>(&nBlock)->default_value(1), "block size for reading file, bytes")
    ("hash",           po::value<std::string>(),                  "hash algorithm: crc32, md5")
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
  Otus::CBayan bayan;
  bayan.Exec(vInputPaths, vExcludePaths, nLevel);
  return 0;
}