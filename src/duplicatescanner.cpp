#include "duplicatescanner.h"

#include <fstream>
#include <memory>
#include <iostream>

namespace Otus 
{

Duplicatescanner::Duplicatescanner(boost::optional<size_t>& a_szBlockSize, boost::optional<std::string> a_strHashAlg)
{
  m_szBlockSize = 1024;
  if (a_szBlockSize) {
    m_szBlockSize = a_szBlockSize.get();
  }

  if (a_strHashAlg) {
    if (a_strHashAlg.get() == "crc32") {
      m_hash = std::make_unique<CRC32>();
    }
    else if (a_strHashAlg.get() == "md5") {
      m_hash = std::make_unique<MD5>();
    }
  }
  else {
    m_hash = std::make_unique<CRC32>();
  }
  
}

std::vector<Paths> Duplicatescanner::Scan(PathGroupedBySize a_groupPath)
{
  std::vector<Paths> resultPaths;

  for (const auto& group : a_groupPath) {
    auto findDuplicates = CheckPaths(group.second);
    if (!findDuplicates.empty()) {
      resultPaths.emplace_back(findDuplicates);
    }
  }

  return resultPaths;
}

Paths Duplicatescanner::CheckPaths(const UniquePaths& a_paths)
{
  Paths result;
  std::unordered_map<std::string, std::pair<std::fstream, size_t>> hashes;

  for (const Path& path : a_paths) {
    std::string strPath = path.string();
    std::fstream read_stream(strPath, std::fstream::in);
    hashes[strPath] = std::make_pair(std::move(read_stream), 0);
  }

  std::vector<char> buffer(m_szBlockSize);

  bool endFiles = false;
  while (!hashes.empty() && !endFiles) {
    for (auto& elem : hashes) {
      memset(buffer.data(), 0, m_szBlockSize);
      auto& value = elem.second;

      auto readBytes = value.first.readsome(buffer.data(), m_szBlockSize);
      endFiles = static_cast<size_t>(readBytes) < m_szBlockSize;
      std::cout << m_szBlockSize << " end " << endFiles << std::endl;
      std::cout << buffer.data() << std::endl;
      char b[] = "12345";
      std::cout << m_hash->Process(b, 6) << std::endl;
      std::cout << m_hash->Process(b, 6) << std::endl;
      std::cout << m_hash->Process(b, 6) << std::endl;
      value.second = m_hash->Process(buffer.data(), buffer.size());
    }

    auto it = hashes.begin();
    while (it != hashes.end()) {
      size_t szCount = 0;
      size_t curHash = it->second.second;

      std::for_each(hashes.begin(), hashes.end(), 
        [curHash, &szCount](const auto& value)
        {
          std::cout << value.second.second << ' ' << curHash << std::endl;
          if (value.second.second == curHash) {
            ++szCount;
          }
        }
      );

      if (szCount == 1) {
        it = hashes.erase(it);
      }
      else {
        ++it;
      }
    }
  }

  for (auto& elem : hashes) {
    result.emplace_back(elem.first);
  }

  return result;
}

} // Otus::