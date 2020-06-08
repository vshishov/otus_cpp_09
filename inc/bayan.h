#pragma once

#include <vector>
#include <string>

namespace Otus {

class CBayan
{
public:
  CBayan() = default;
  ~CBayan() = default;

  void Exec(const std::vector<std::string>& a_strIncludePaths, const std::vector<std::string>& a_strExcludePaths);
  static std::vector<std::string> GetFileList(const std::vector<std::string>& a_strIncludePaths, const std::vector<std::string>& a_strExcludePaths, int a_nLevel);

private:
  std::vector<std::string> GetAllFilesInDir(const std::vector<std::string> &dirPaths, const std::vector<std::string> dirSkipList, int a_nLevel);
};

} // Otus::