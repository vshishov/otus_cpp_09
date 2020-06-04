#include "bayan.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

namespace po = boost::program_options;


int run(int argc, char** argv)
{
  int opt;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")    
    ("include-path,I", po::value< std::vector<std::string> >(), "include path")
    ("exclude-path,E", po::value< std::vector<std::string> >(), "exclude path")
    ("min-size,m",     po::value<int>(&opt)->default_value(1),  "minimum file size in bytes")
  ;

  po::variables_map vm;
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