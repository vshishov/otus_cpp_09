#include "bayan.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

std::vector<std::string> CBayan::GetFileList(const std::vector<std::string>& a_strIncludePaths, const std::vector<std::string>& a_strExcludePaths, int a_nLevel)
{

}

void CBayan::Exec(const std::vector<std::string>& a_vIncludePaths, const std::vector<std::string>& a_vExcludePaths)
{
  // for(auto& path : a_strIncludePaths) {
  //   for (directory_entry& x : directory_iterator(path))
  //     std::cout << "    " << x.path() << '\n';
  //   std::cout << path << std::endl;
  // }

  auto filenames = GetAllFilesInDir(a_vIncludePaths, a_vExcludePaths, 2);

  for (auto path : filenames) {
    std::cout << path << std::endl;
  }
}

std::vector<std::string> CBayan::GetAllFilesInDir(const std::vector<std::string> &dirPaths, const std::vector<std::string> dirSkipList, int a_nLevel)
{ 
	std::vector<std::string> listOfFiles;
	try {
    for (auto& strPath : dirPaths) {
      if (bfs::exists(strPath) && bfs::is_directory(strPath)) {
        bfs::recursive_directory_iterator iter(strPath), end;
        while (iter != end) {
          if (iter.level() > a_nLevel || 
              ( bfs::is_directory(iter->path()) &&
              (std::find(dirSkipList.begin(), dirSkipList.end(), iter->path().string()) != dirSkipList.end())) )
          {
            iter.no_push();
          }
          else {
            if (bfs::is_regular_file(iter->path())) {
              listOfFiles.push_back(iter->path().string());
            }
          }
  
          boost::system::error_code ec;
          iter.increment(ec);
          if (ec) {
            std::cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << '\n';
          }
        }
      }
		}
	}
	catch (const std::system_error& ex) {
		std::cerr << "Error: " << ex.what();
	}
	return listOfFiles;
}

} // Otus::