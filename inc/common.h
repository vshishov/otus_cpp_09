#pragma once

#include <set>
#include <unordered_map>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

using Path = bfs::path; 
using Paths = std::vector<bfs::path>; 
using ScanPath = std::pair<bfs::path, std::size_t>; 

using UniquePath = std::set<Path>;
using PathGroupedBySize = std::unordered_map< std::size_t, UniquePath >;

} // Otus::