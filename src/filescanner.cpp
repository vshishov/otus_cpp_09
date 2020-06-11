#include "filescanner.h"
#include "mask.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

FileScanner::FileScanner(const Paths& a_Excludes, boost::optional<std::size_t>& a_szLevel, std::vector<std::string> a_Masks, boost::optional<std::size_t>& a_szMinSize)
{
  m_DirFilter = new LevelDirFilter(a_szLevel.get());
  m_DirFilter->SetNext(new ExcludeDirFilter(a_Excludes));
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

} // Otus::