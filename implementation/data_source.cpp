/**
    asio_http: http client library for boost asio
    Copyright (c) 2017 Julio Becerra Gomez
    See COPYING for license information.
*/

#include "asio_http/internal/data_source.h"

#include "asio_http/internal/compression.h"

namespace asio_http
{
namespace internal
{
namespace
{
auto compress_data_source(std::vector<uint8_t> data, http_request_interface::compression_policy policy)
{
  if (policy != http_request_interface::compression_policy::never && !data.empty())
  {
    auto compressed_data = compress(data);
    if (!compressed_data.empty() &&
        (policy == http_request_interface::compression_policy::always || compressed_data.size() < data.size()))
    {
      data.swap(compressed_data);
      return std::make_pair(data, std::vector<std::string>{ "Content-Encoding: gzip" });
    }
  }
  return std::make_pair(data, std::vector<std::string>{});
}
}

data_source::data_source(std::vector<uint8_t> data, http_request_interface::compression_policy policy)
    : data_source(compress_data_source(std::move(data), policy))
{
}

size_t data_source::read_callback(char* data, size_t size)
{
  m_data.read(data, size);

  return static_cast<size_t>(m_data.gcount());
}

bool data_source::seek_callback(int32_t offset, std::ios_base::seekdir origin)
{
  m_data.clear();
  m_data.seekg(static_cast<std::streamoff>(offset), origin);
  return m_data.good();
}
}
}
