#pragma once

#include "mask.h"
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



class FileFilter
{
public:
  virtual FileFilter* SetNext(FileFilter* a_dirFilter) = 0;
  virtual bool IsValid(const Path& a_path) = 0;
};

class BaseFileFilter : public FileFilter
{
public:
  BaseFileFilter()
    : nextFileFilter(nullptr)
  { }

  FileFilter* SetNext(FileFilter* a_dirFilter) override
  {
    this->nextFileFilter = a_dirFilter;
    return a_dirFilter;
  }

  bool IsValid(const Path& a_path) override
  {
    if (this->nextFileFilter) {
      return this->nextFileFilter->IsValid(a_path);
    }

    return true;
  }

private:
  FileFilter* nextFileFilter;
};

class SizeFileFilter : public BaseFileFilter
{
public:
  SizeFileFilter(std::size_t a_szSize)
    : BaseFileFilter()
    , m_szSize(a_szSize)
  { }

  bool IsValid(const Path& a_path) override
  {
    if (bfs::file_size(a_path) < m_szSize) {;
      return false;    
    }
    return BaseFileFilter::IsValid(a_path);
  }

private:
  std::size_t m_szSize;
};

class MasksFileFilter : public BaseFileFilter
{
public:
  MasksFileFilter(const std::vector<std::string>& a_vstrMasks)
    : BaseFileFilter()
  { 
    for (auto& mask : a_vstrMasks) {
      m_masks.emplace_back(Mask(mask));
    }
  }

  bool IsValid(const Path& a_path) override
  {
    if (!m_masks.empty()) {
      auto filename = a_path.filename().string();
      return std::any_of(m_masks.begin(), m_masks.end(), 
        [filename](Mask& mask) {
          return mask.Valid(filename);
        } 
      );
    }
    
    return BaseFileFilter::IsValid(a_path);
  }

private:
  std::vector<Mask> m_masks;
};

} // Otus::