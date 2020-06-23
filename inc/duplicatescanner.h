#pragma once

#include "common.h"

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <string>
#include <memory>


namespace Otus 
{

using HashFunc_t = boost::function<std::size_t (char*, std::size_t)>;

class Duplicatescanner
{
public:
  Duplicatescanner(boost::optional<std::size_t>& a_szBlockSize, boost::optional<std::string>& a_strHashAlg);

  PathGroupedByDup Scan(PathGroupedBySize a_groupPath);

private:
  PathGroupedForRead CheckPaths(const UniquePaths& a_paths);
  PathGroupedByDup FormGropByDuplicates(PathGroupedForRead& a_paths);

  static HashFunc_t CreateHashFunc(boost::optional<std::string>& a_strHashAlg);

  template<typename T>
  static HashFunc_t HashFunc();

private:
  std::size_t m_szBlockSize;
  HashFunc_t m_hashFunc;
};

} // Otus::
