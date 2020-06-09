#pragma once

#include <vector>
#include <string>

namespace Otus {

using paths = std::vector<std::string>;

class CBayan
{
public:
  CBayan() = default;
  ~CBayan() = default;

  void Exec(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask);

private:
  static paths GetListOfFiles(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask);
};

} // Otus::