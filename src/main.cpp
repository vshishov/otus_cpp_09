#include "filescanner.h"
#include "options.h"

#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  Otus::OptionsParser optionsParser;
  auto options = optionsParser.GetOptions(argc, argv);
  if (!options) {
    return 0;
  }

  Otus::FileScanner fileScanner(options.get().excludePaths, options.get().levelScannig, options.get().masks, options.get().minFileSize);
  auto groupPath = fileScanner.Scan(options.get().includePaths);

  for (auto& group : groupPath) {
    std::cout << group.first << ' ' ;
    for (auto& path : group.second) {
      std::cout << path << ' ';
    }
    std::cout << std::endl;
  }

  return 0;
}