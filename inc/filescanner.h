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
  static std::unique_ptr<DirFilter> CreateDirFilter(boost::optional<std::size_t>& a_szLevel, const Paths& a_Excludes);
  static std::unique_ptr<FileFilter> CreateFileFilter(boost::optional<std::size_t>& a_szMinSize, const std::vector<std::string>& a_strMasks);

  PathGroupedBySize FindPath(const Paths& a_Includes);
  void DeleteUniqPath(PathGroupedBySize& a_groupPath);

private:  
  std::shared_ptr<DirFilter> m_DirFilter;
  std::shared_ptr<FileFilter> m_FileFilter;
};

} // Otus::
