#pragma once

#include <set>
#include <unordered_map>
#include <boost/filesystem/path.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

using Paths = std::vector<bfs::path>; 
using ScanPath = std::pair<bfs::path, std::size_t>; 

} // Otus::