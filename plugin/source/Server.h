#pragma once

#ifndef SERVER_H
#define SERVER_H

#include <httplib.h>
#include <string>

class Server {
public:
  static Server& getInstance(const std::string& proxy_host, int proxy_port);
  void start();

private:
  httplib::Client cli;
  httplib::Server svr;
  int port;
  std::string host;

  // Singleton pattern
  Server(const std::string& proxy_host, int proxy_port);
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;
};

#endif // SERVER_H
