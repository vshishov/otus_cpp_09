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

  void Scan(const Paths& a_Includes);

private:
  // static paths GetListOfFiles(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask, int a_nMinSize);
  DirFilter* m_DirFilter;
};

} // Otus::