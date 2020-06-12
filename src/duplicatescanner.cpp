#include "duplicatescanner.h"

namespace Otus 
{

Duplicatescanner::Duplicatescanner(boost::optional<size_t>& a_szBlockSize, boost::optional<std::string> a_strHashAlg)
{
  m_szBlockSize = 1024;
  if (a_szBlockSize) {
    m_szBlockSize = a_szBlockSize.get();
  }

  if (a_strHashAlg) {

  }
  
}

std::vector<Paths> Duplicatescanner::Scan(PathGroupedBySize /*a_groupPath*/)
{
  return{};
}

} // Otus::