#pragma once

#include "common.h"

namespace Otus
{

class DirFilter
{
public:
  virtual DirFilter* SetNext(DirFilter* a_dirFilter) = 0;
  virtual bool IsValid(const ScanPath& a_path) = 0;
};

class BaseDirFilter : public DirFilter
{
public:
  BaseDirFilter()
    : nextDirFilter(nullptr)
  { }

  DirFilter* SetNext(DirFilter* a_dirFilter) override
  {
    this->nextDirFilter = a_dirFilter;
    return a_dirFilter;
  }

  bool IsValid(const ScanPath& a_path) override
  {
    if (this->nextDirFilter) {
      return this->nextDirFilter->IsValid(a_path);
    }

    return true;
  }

private:
  DirFilter* nextDirFilter;
};

class LevelDirFilter : public BaseDirFilter
{
public:
  LevelDirFilter(std::size_t a_szLevel)
    : BaseDirFilter()
    , m_szLevel(a_szLevel)
  { }

  bool IsValid(const ScanPath& a_path) override
  {
    if (a_path.second >= m_szLevel) {;
      return false;    
    }
    return BaseDirFilter::IsValid(a_path);
  }

private:
  std::size_t m_szLevel;
};

class ExcludeDirFilter : public BaseDirFilter
{
public:
  ExcludeDirFilter(const Paths& a_excludePaths)
    : BaseDirFilter()
    , m_excludePaths(a_excludePaths)
  { }

  bool IsValid(const ScanPath& a_path) override
  {
    if (!m_excludePaths.empty()) {
      auto path = a_path.first;
      auto it = std::find_if(m_excludePaths.begin(), m_excludePaths.end(), 
        [path](const bfs::path& exclude_path) {
          return path == exclude_path;
        } 
      );
    
      if (it != m_excludePaths.end()) {
        return false;    
      }
    }
    
    return BaseDirFilter::IsValid(a_path);
  }

private:
  Paths m_excludePaths;
};

} // Otus::