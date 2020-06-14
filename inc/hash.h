#pragma once

#include <memory>
#include <boost/crc.hpp> 

namespace Otus
{

class Hash
{
public:
  virtual std::size_t Process(const char* data, std::size_t size) = 0;
  virtual ~Hash() = default;
};

class CRC32 : public Hash
{
public:
  CRC32() = default;
  ~CRC32() = default;

  std::size_t Process(const char* data, std::size_t size) override
  {
    m_algorithm.process_bytes(data, size);
    std::size_t szResult =  m_algorithm.checksum();
    m_algorithm.reset();
    return szResult;
  }

private:
  boost::crc_16_type m_algorithm;
};

class MD5 : public Hash
{
public:
  MD5() = default;
  ~MD5() = default;

  std::size_t Process(const char* data, std::size_t size) override
  {
    m_algorithm.process_bytes(data, size);
    std::size_t szResult =  m_algorithm.checksum();
    m_algorithm.reset();
    return szResult;
  }

private:
  boost::crc_16_type m_algorithm;
};

} // Otus::
