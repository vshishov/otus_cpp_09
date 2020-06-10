#pragma once

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>

namespace Otus {

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

struct Options
{
  std::vector<bfs::path>       includePaths;
  std::vector<bfs::path>       excludePaths;  
  boost::optional<std::size_t> levelScannig;
  boost::optional<std::size_t> minFileSize;
  std::vector<std::string>     masks;
  boost::optional<std::size_t> blockSize;
  boost::optional<std::string> hash;
};

class OptionsParser
{
public:
  OptionsParser();

  using OptionsResult = boost::optional<Options>;
  
  OptionsResult GetOptions(int argc, char** argv);

private:
  bpo::options_description m_Description;
  bpo::variables_map m_Variables;
};

} // Otus::