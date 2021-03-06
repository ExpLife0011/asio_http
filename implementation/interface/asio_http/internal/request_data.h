/**
    asio_http: http client library for boost asio
    Copyright (c) 2017 Julio Becerra Gomez
    See COPYING for license information.
*/

#ifndef ASIO_HTTP_REQUEST_DATA_H
#define ASIO_HTTP_REQUEST_DATA_H

#include "asio_http/http_request_result.h"

#include <boost/asio.hpp>
#include <chrono>
#include <memory>

namespace asio_http
{
namespace internal
{
class http_client_connection;

// It must be that Waiting < InProgress, so pending requests keep at the beginning
// of the IndexState index
enum class request_state
{
  waiting     = 0,  // Waiting in the requests queue
  in_progress = 1   // Request being executed
};

using completion_handler = std::function<void(http_request_result)>;

struct request_data
{
  request_data(std::shared_ptr<const http_request_interface> web_request,
               completion_handler                            completion_handler,
               boost::asio::executor                         executor,
               std::string                                   cancellation_token)
      : m_request_state(request_state::waiting)
      , m_http_request(std::move(web_request))
      , m_completion_handler(std::move(completion_handler))
      , m_completion_executor(std::move(executor))
      , m_cancellation_token(std::move(cancellation_token))
      , m_creation_time(std::chrono::steady_clock::now())
      , m_retries(0)
  {
  }
  request_state                                 m_request_state;
  std::shared_ptr<http_client_connection>       m_connection;
  std::shared_ptr<const http_request_interface> m_http_request;
  completion_handler                            m_completion_handler;
  boost::asio::executor                         m_completion_executor;
  std::string                                   m_cancellation_token;
  std::chrono::steady_clock::time_point         m_creation_time;
  uint32_t                                      m_retries;
};
}
}
#endif
