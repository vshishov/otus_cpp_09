#pragma once

#include <boost/optional.hpp>
#include <string>

#include "common.h"

namespace Otus 
{

class Duplicatescanner
{
public:
  Duplicatescanner(boost::optional<std::size_t>& a_szBlockSize, boost::optional<std::string> a_strHashAlg);

  std::vector<Paths> Scan(PathGroupedBySize a_groupPath);
  

private:
  std::size_t m_szBlockSize;
};

} // Otus::