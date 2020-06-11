#pragma once

#include <vector>
#include <string>

namespace Otus {

using paths = std::vector<std::string>;

class FileScanner
{
public:
  FileScanner() = default;
  ~FileScanner() = default;

  void Exec(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask, int a_nMinSize);

private:
  static paths GetListOfFiles(const paths& a_Includes, const paths& a_Excludes, int a_nLevel, std::string a_strMask, int a_nMinSize);
};

} // Otus::