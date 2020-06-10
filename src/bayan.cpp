#include "bayan.h"
#include "mask.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace Otus {

namespace bfs = boost::filesystem;

void CBayan::Exec(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask, int a_nMinSize)
{
  paths filenames = GetListOfFiles(a_Includes, a_Excludes, a_nLevel, a_strMask, a_nMinSize);

  for (auto path : filenames) {
    std::cout << path << std::endl;
  }


}

paths CBayan::GetListOfFiles(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask, int a_nMinSize)
{ 
	paths listOfFiles;
  Mask mask(a_strMask);
	try {
    for (auto& incPath : a_Includes) {
      if (bfs::exists(incPath) && bfs::is_directory(incPath)) {
        bfs::recursive_directory_iterator iter(incPath), end;
        while (iter != end) {
          if (iter.level() > a_nLevel || 
              ( bfs::is_directory(iter->path()) &&
                (std::find(a_Excludes.begin(), a_Excludes.end(), iter->path().string()) != a_Excludes.end())
              ) 
            )
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
	return listOfFiles;
}

} // Otus::