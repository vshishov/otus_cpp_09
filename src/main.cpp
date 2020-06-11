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
  fileScanner.Scan(options.get().includePaths);
  // Otus::CBayan bayan;
  // bayan.Exec(vInputPaths, vExcludePaths, nLevel, strMask, nMinFileSize);
  return 0;
}