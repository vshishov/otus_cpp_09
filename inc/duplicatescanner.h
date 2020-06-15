#pragma once

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <string>
#include <memory>

#include "common.h"

namespace Otus 
{

using HashFunc_t = boost::function<std::size_t (char*, std::size_t)>;

class Duplicatescanner
{
public:
  Duplicatescanner(boost::optional<std::size_t>& a_szBlockSize, boost::optional<std::string>& a_strHashAlg);

  std::vector<Paths> Scan(PathGroupedBySize a_groupPath);

private:
  Paths CheckPaths(const UniquePaths& a_paths);
  Paths CheckPaths2(const UniquePaths& a_paths);
  std::vector<Paths>  CheckPaths3(const UniquePaths& a_paths);

  static HashFunc_t CreateHashFunc(boost::optional<std::string>& a_strHashAlg);

  template<typename T>
  static HashFunc_t HashFunc();

private:
  std::size_t m_szBlockSize;
  HashFunc_t m_hashFunc;
};

} // Otus::