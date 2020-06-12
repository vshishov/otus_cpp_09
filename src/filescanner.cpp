#include "filescanner.h"
#include "mask.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

FileScanner::FileScanner(const Paths& a_Excludes, boost::optional<std::size_t>& a_szLevel, std::vector<std::string> a_Masks, boost::optional<std::size_t>& a_szMinSize)
{
  m_DirFilter = CreateDirFilter(a_szLevel, a_Excludes);
  m_FileFilter = CreateFileFilter(a_szMinSize, a_Masks);
}

PathGroupedBySize FileScanner::Scan(const Paths& a_Includes)
{
  PathGroupedBySize groupPath = FindPath(a_Includes);
  DeleteUniqPath(groupPath);

  for (auto& group : groupPath) {
    std::cout << group.first << ' ' ;
    for (auto& path : group.second) {
      std::cout << path << ' ';
    }
    std::cout << std::endl;
  }

  return groupPath;
}

PathGroupedBySize FileScanner::FindPath(const Paths& a_Includes)
{
  PathGroupedBySize resultPaths;

	try {
    for (auto& incPath : a_Includes) {
      if (bfs::exists(incPath) && bfs::is_directory(incPath)) {
        bfs::recursive_directory_iterator iter(incPath), end;
        while (iter != end) {
          ScanPath scanPath = std::make_pair(iter->path(), iter.level() );
          if (bfs::is_directory(iter->path()) && !m_DirFilter->IsValid(scanPath))
          {
            iter.no_push();            
          }
          else {
            if ( bfs::is_regular_file(iter->path()) && m_FileFilter->IsValid(iter->path()) ) {
              auto path = iter->path();
              std::size_t szSize = bfs::file_size(path);

              auto& uniquepaths = resultPaths[szSize];
              uniquepaths.insert(path);
              // auto find_it = resultPaths.find(szSize);
              // if (find_it == resultPaths.end() ) {
              //   resultPaths.
              // }
            }
          }
  
          boost::system::error_code ec;
          iter.increment(ec);
          if (ec) {
            std::cerr << "Error while accessing: " << iter->path().string() << " :: " << ec.message() << '\n';
          }
        }
      }
		}
	}
	catch (const std::system_error& ex) {
		std::cerr << "Error: " << ex.what();
	}
  
  return resultPaths;
}

void FileScanner::DeleteUniqPath(PathGroupedBySize& a_groupPath)
{
  auto it = a_groupPath.begin();

  while(it != a_groupPath.end()) {
    if (it->second.size() < 2) {
      it = a_groupPath.erase(it);
    }
    else {
      ++it;
    }
  }
}

DirFilter* FileScanner::CreateDirFilter(boost::optional<std::size_t>& a_szLevel, const Paths& a_Excludes)
{
  std::size_t szLevel = 0;
  if (a_szLevel) {
    szLevel = a_szLevel.get();
  }
  auto levelFilter = new LevelDirFilter(szLevel);
  auto excludeFilter = new ExcludeDirFilter(a_Excludes);
  levelFilter->SetNext(excludeFilter);
  return levelFilter;
}

FileFilter* FileScanner::CreateFileFilter(boost::optional<std::size_t>& a_szMinSize, const std::vector<std::string>& a_strMasks)
{
  std::size_t szMinSize = 0;
  if (a_szMinSize) {
    szMinSize = a_szMinSize.get();
  }
  auto sizeFilter = new SizeFileFilter(szMinSize);
  auto masksFilter = new MasksFileFilter(a_strMasks);
  sizeFilter->SetNext(masksFilter);
  return sizeFilter;
}

} // Otus::