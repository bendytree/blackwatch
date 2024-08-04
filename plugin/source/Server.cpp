#include "Server.h"
#include <iostream>
#include <thread>

Server::Server(const std::string& proxy_host, int proxy_port)
    : cli("https://www.joshwright.com"), port(proxy_port), host(proxy_host) {
  cli.set_follow_location(true);
}

Server& Server::getInstance(const std::string& proxy_host, int proxy_port) {
  static Server instance(proxy_host, proxy_port);
  return instance;
}

void Server::start() {
  svr.Get("/(.*)", [this](const httplib::Request& req, httplib::Response& res) {
    auto result = this->cli.Get(req.path);
    if (result) {
      std::cout << "Status Code: " << result->status << std::endl;
      std::cout << "Content-Type: " << result->get_header_value("Content-Type") << std::endl;

      res.set_content(result->body, result->get_header_value("Content-Type"));
      res.status = result->status;
//      for (const auto& header : result->headers) {
//        res.set_header(header.first.c_str(), header.second);
//      }
    } else {
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
    }
  });

  std::cout << "Server starting on http://" << host << ":" << port << std::endl;
  std::thread([this]() { svr.listen(host.c_str(), port); }).detach();
}
