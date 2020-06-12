#pragma once

#include "common.h"
#include "filter.h"

#include <vector>
#include <string>
#include <memory>
#include <boost/optional.hpp>

namespace Otus {

class FileScanner
{
public:
  FileScanner(const Paths& a_Excludes, boost::optional<std::size_t>& a_szLevel, std::vector<std::string> a_Masks, boost::optional<std::size_t>& a_szMinSize);
  ~FileScanner() = default;

  PathGroupedBySize Scan(const Paths& a_Includes);

private:
  static DirFilter* CreateDirFilter(boost::optional<std::size_t>& a_szLevel, const Paths& a_Excludes);
  static FileFilter* CreateFileFilter(boost::optional<std::size_t>& a_szMinSize, const std::vector<std::string>& a_strMasks);

  PathGroupedBySize FindPath(const Paths& a_Includes);
  void DeleteUniqPath(PathGroupedBySize& a_groupPath);

private:  
  DirFilter* m_DirFilter;
  FileFilter* m_FileFilter;
};

} // Otus::