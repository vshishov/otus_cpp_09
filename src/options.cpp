#include "options.h"

#include <iostream>
#include <exception>

namespace Otus {

class InvalidOption: public std::logic_error 
{
public:
  explicit InvalidOption(const std::string &input)
    : std::logic_error(input) 
  { }
};

OptionsParser::OptionsParser()
  : m_Description("Options")
{
  m_Description.add_options()
    ("include,i",     bpo::value<std::vector<bfs::path>>(),   "Include paths for scaning")
    ("exclude,e",     bpo::value<std::vector<bfs::path>>(),   "Exclude paths from scaning")
    ("level,l",       bpo::value<long>(),                     "Scan nesting level, 0 - without nested")
    ("min-size,s",    bpo::value<long>(),                     "Minimum file size in bytes for scaning")
    ("masks,m",       bpo::value<std::vector<std::string>>(), "Masks for file names allowed for comparison")
    ("block-size,b",  bpo::value<long>(),                     "Block size in bytes for reading file")
    ("algorithm,a",   bpo::value<std::string>(),              "Hash algorithm: crc32 or md5")
    ("help,h",        "Show help")  
  ;
}

OptionsParser::OptionsResult OptionsParser::GetOptions(int argc, char** argv)
{
  try {
    bpo::store(bpo::parse_command_line(argc, argv, m_Description), m_Variables);
    bpo::notify(m_Variables);    

    if (m_Variables.count("help")) {
      std::cout << m_Description << "\n";
      return OptionsResult();
    }

    Options options;

    if (m_Variables.count("include")) {
      options.includePaths = m_Variables["include"].as<std::vector<bfs::path>>();
      for (auto& path : options.includePaths) {
        if (path.is_relative()) {
          path = bfs::canonical(path);
        }
      }
    }
    else {
      throw InvalidOption("Include scan paths not defined");
    }

    if (m_Variables.count("exclude")) {
      options.excludePaths = m_Variables["exclude"].as<std::vector<bfs::path>>();
      for (auto& path : options.excludePaths) {
        if (path.is_relative()) {
          path = bfs::canonical(path);
        }
      }
    }

    if (m_Variables.count("level")) {
      long nLevel = m_Variables["level"].as<long>();
      if (nLevel < 0) {
        throw InvalidOption("Level scaning must be integer");
      }
      options.levelScannig = static_cast<std::size_t>(nLevel);
    }

    if (m_Variables.count("min-size")) {
      long nMinSize = m_Variables["min-size"].as<long>();
      if (nMinSize < 1) {
        throw InvalidOption("Minimum file size must be greater then 1");
      }
      options.minFileSize = static_cast<std::size_t>(nMinSize);
    }

    if (m_Variables.count("masks")) {
      options.masks = m_Variables["masks"].as<std::vector<std::string>>();
    }

    if (m_Variables.count("block-size")) {
      long nBlockSize = m_Variables["block-size"].as<long>();
      if (nBlockSize < 1) {
        throw InvalidOption("Minimum file size must be greater then 1");
      }
      options.blockSize = static_cast<std::size_t>(nBlockSize);
    }

    if (m_Variables.count("algorithm")) {
      std::string strHash = m_Variables["algorithm"].as<std::string>();
      if (strHash != "crc32" && strHash != "crc16") {
        throw InvalidOption("The hash algorithm '" + strHash + "' not supported");
      }
      options.hash = strHash;
    }

    return options;
  }
  catch(std::exception& ex) {
    std::cout << "Error: " << ex.what() << std::endl << std::endl;
    std::cout << m_Description << std::endl;
    return OptionsResult();
  }  
}
} // Otus::