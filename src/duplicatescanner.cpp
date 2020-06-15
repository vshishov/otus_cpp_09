#include "duplicatescanner.h"

#include <boost/crc.hpp>

#include <fstream>
#include <memory>
#include <iostream>


namespace Otus 
{

  struct Rec {
    std::string strPath;
    std::fstream stBuffer;
    std::size_t curHash;
    std::size_t curLevel;
  };


Duplicatescanner::Duplicatescanner(boost::optional<size_t>& a_szBlockSize, boost::optional<std::string>& a_strHashAlg)
{
  m_szBlockSize = 1024;
  if (a_szBlockSize) {
    m_szBlockSize = a_szBlockSize.get();
  }

  m_hashFunc = CreateHashFunc(a_strHashAlg);  
}

std::vector<Paths> Duplicatescanner::Scan(PathGroupedBySize a_groupPath)
{
  std::vector<Paths> resultPaths;

  for (const auto& group : a_groupPath) {
    // auto findDuplicates = CheckPaths(group.second);
    auto findDuplicates = CheckPaths3(group.second);
    if (!findDuplicates.empty()) {
      resultPaths.emplace_back(findDuplicates);
    }
  }

  return resultPaths;
}

Paths Duplicatescanner::CheckPaths(const UniquePaths& a_paths)
{
  Paths result;
  std::unordered_map<std::string, std::pair<std::fstream, std::size_t>> hashes;

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
      value.second = m_hashFunc(buffer.data(), buffer.size());
    }

    auto it = hashes.begin();
    while (it != hashes.end()) {
      size_t szCount = 0;
      size_t curHash = it->second.second;

      std::for_each(hashes.begin(), hashes.end(), 
        [curHash, &szCount](const auto& value)
        {
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

std::vector<Paths> Duplicatescanner::CheckPaths3(const UniquePaths& a_paths)
{
  
  struct Rec
  {
    std::fstream stBuffer;
    std::size_t curHash;
    std::size_t curLevel;
  };
  
  std::unordered_map<std::string, Rec> hashes;

  for (const Path& path : a_paths) {
    std::string strPath = path.string();
    std::fstream read_stream(strPath, std::fstream::in);
    hashes[strPath] = {std::move(read_stream), 0, 0};
  }

  std::vector<char> buffer(m_szBlockSize);

  bool endFiles = false;
  while (!hashes.empty() && !endFiles) {
    for (auto& elem : hashes) {
      memset(buffer.data(), 0, m_szBlockSize);
      auto& value = elem.second;

      auto readBytes = value.stBuffer.readsome(buffer.data(), m_szBlockSize);
      endFiles = static_cast<size_t>(readBytes) < m_szBlockSize;
      value.curHash = m_hashFunc(buffer.data(), buffer.size());
    }

    auto it = hashes.begin();
    while (it != hashes.end()) {
      size_t szCount = 0;
      size_t curHash = it->second.curHash;

      std::for_each(hashes.begin(), hashes.end(), 
        [curHash, &szCount](const auto& value)
        {
          if (value.second.curHash == curHash) {
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


  std::vector<Paths>  result;

  auto it = hashes.begin();
  while (hashes.empty()) {
    Paths paths;
    
    std::size_t nLevel = it->second.curLevel;
    paths.emplace_back();
    it = hashes.erase(it);
    
    
    auto it = hashes.begin();
    while (it != hashes.end()) {
      size_t szCount = 0;
      auto curLevel = it->second.curLevel;

      auto sub_it = hashes.begin();
      while (it != hashes.end()) {
        if (it->second.curLevel == curLevel) {
          paths.emplace_back(it->first);
          it = hashes.erase(it);
        }
        else {
          ++it;
        }
      }
    }
  }

  return result;
}

HashFunc_t Duplicatescanner::CreateHashFunc(boost::optional<std::string>& a_strHashAlg)
{
  if (a_strHashAlg) {
    if (a_strHashAlg.get() == "crc16") {
      return HashFunc<boost::crc_16_type>();
    }
    else if (a_strHashAlg.get() == "crc32") {
      return HashFunc<boost::crc_32_type>();
    }
  }
  
  return HashFunc<boost::crc_32_type>();
}

template<typename T>
HashFunc_t Duplicatescanner::HashFunc()
{
  return [](char* a_buffer, std::size_t a_szCountBytes)
  {
    T algorithm;
    algorithm.process_bytes(a_buffer, a_szCountBytes);
    return algorithm.checksum();
  };
}

} // Otus::