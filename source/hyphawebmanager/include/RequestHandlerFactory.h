// Copyright (c) 2015-2016 Hypha
#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  RequestHandlerFactory(const std::string& docroot, const std::string& resroot);

  Poco::Net::HTTPRequestHandler* createRequestHandler(
      const Poco::Net::HTTPServerRequest& request);

 private:
  const std::string& _docroot;
  const std::string& _resroot;
};
