#include "bayan.h"

#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  int nLevel, nSize, nBlock;
  po::options_description desc("General options");
  desc.add_options()
    ("include,i", po::value< std::vector<std::string> >(),   "include path for scaning")
    ("exclude,e", po::value< std::vector<std::string> >(),   "exclude path from scaning")
    ("mask,m",    po::value<std::string>(),                  "masks for file names allowed for comparison")
    ("level,l",   po::value<int>(&nLevel)->default_value(0), "level scaning")
    ("size,s",    po::value<int>(&nSize)->default_value(1),  "minimum file size in bytes for scaning")
    ("block,b",   po::value<int>(&nBlock)->default_value(1), "block size for reading file")
    ("alg,a",     po::value<std::string>(),                  "hash algorithm: crc32, md5")
    ("help,h",    "produce help message")  
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
      std::cout << "Include path was set to " << vm["include-path"].as<int>() << ".\n";
    } else {
      std::cout << "Include path was not set.\n";
    }
  }
  catch(std::exception& ex) {
    std::cout << desc << std::endl;
  }
  //Otus::run(argc, argv);
  return 0;
}