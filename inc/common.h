#pragma once

#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

using Path = bfs::path; 
using Paths = std::vector<bfs::path>; 
using ScanPath = std::pair<bfs::path, std::size_t>; 

using UniquePaths = std::set<Path>;
using PathGroupedBySize = std::unordered_map< std::size_t, UniquePaths >;

using PathDataRead = std::pair<std::fstream, std::size_t>;
using PathGroupedForRead = std::unordered_map<std::string, PathDataRead> ;
using PathGroupedByDup = std::list<Paths>;

} // Otus::
