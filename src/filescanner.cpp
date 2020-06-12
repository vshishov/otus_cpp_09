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

void FileScanner::Scan(const Paths& a_Includes)
{
  Paths listOfFiles;
  Mask mask("*.*");
  long a_nMinSize = 0;
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
            if (bfs::is_regular_file(iter->path()) && 
                bfs::file_size(iter->path()) >= a_nMinSize &&
                mask.Valid(iter->path().filename().string())
              ) 
            {
              listOfFiles.push_back(iter->path().string());
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
  
  for (auto path : listOfFiles) {
    std::cout << path << std::endl;
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