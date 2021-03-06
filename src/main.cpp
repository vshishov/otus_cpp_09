#include "options.h"
#include "filescanner.h"
#include "duplicatescanner.h"

#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, char** argv)
{
  Otus::OptionsParser optionsParser;
  auto options = optionsParser.GetOptions(argc, argv);
  if (!options) {
    return 0;
  }

  Otus::FileScanner fileScanner(options.get().excludePaths, options.get().levelScannig, options.get().masks, options.get().minFileSize);
  auto groupPath = fileScanner.Scan(options.get().includePaths);

  Otus::Duplicatescanner dupScanner(options.get().blockSize, options.get().hash);
  auto duplicates = dupScanner.Scan(groupPath);

  for (auto& dup : duplicates) {
    for (auto& path : dup) {
      std::cout << path << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}