#include "options.h"

#include <iostream>

namespace Otus {

OptionsParser::OptionsParser()
  : m_Description("Options")
{
  m_Description.add_options()
    ("include,i",     bpo::value<std::vector<bfs::path>>(), "Include paths for scaning")
    ("exclude,e",     bpo::value<std::vector<bfs::path>>(), "Exclude paths from scaning")
    ("level,l",       bpo::value<size_t>(),                 "Scan nesting level")
    ("min-size,ms",   bpo::value<size_t>(),                 "Minimum file size in bytes for scaning, bytes")
    ("masks,m",       bpo::value<std::string>(),            "Masks for file names allowed for comparison")
    ("block-size,bs", bpo::value<size_t>(),                 "Block size for reading file, bytes")
    ("algorithm,a",   bpo::value<std::string>(),            "Hash algorithm: crc32 or md5")
    ("help,h",        "show help")  
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
    }

    if (m_Variables.count("exclude")) {
      options.excludePaths = m_Variables["exclude"].as<std::vector<bfs::path>>();
    }

    // if (nMinFileSize < 1) {
    // std::cerr << "Error: Minimum file size must be greater than 1!" << std::endl;
    // }
    return options;
  }
  catch(std::exception& ex) {
    std::cout << m_Description << std::endl;
    return OptionsResult();
  }  
}
} // Otus::