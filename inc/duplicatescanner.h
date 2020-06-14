#pragma once

#include <boost/optional.hpp>
#include <string>
#include <memory>

#include "common.h"
#include "hash.h"

namespace Otus 
{

class Duplicatescanner
{
public:
  Duplicatescanner(boost::optional<std::size_t>& a_szBlockSize, boost::optional<std::string> a_strHashAlg);

  std::vector<Paths> Scan(PathGroupedBySize a_groupPath);

private:
  Paths CheckPaths(const UniquePaths& a_paths);

private:
  std::size_t m_szBlockSize;
  std::unique_ptr<Hash> m_hash;
};

} // Otus::