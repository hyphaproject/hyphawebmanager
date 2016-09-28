// Copyright (c) 2015-2016 Hypha
#pragma once

#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

class HandlersHandler : public Poco::Net::HTTPRequestHandler {
 public:
  HandlersHandler();

  void handleRequest(Poco::Net::HTTPServerRequest& request,
                     Poco::Net::HTTPServerResponse& response);

 protected:
  void handleGETRequest(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response);
  void handlePUTRequest(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response);
  void handleDELETERequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response);
  void handlePOSTRequest(Poco::Net::HTTPServerRequest& request,
                         Poco::Net::HTTPServerResponse& response);

  Poco::JSON::Object::Ptr getHandlers();
  Poco::JSON::Object::Ptr getHandlerInstances();
  Poco::JSON::Object::Ptr getHandler(std::string id);
  Poco::JSON::Object::Ptr getHandlerInstance(std::string id);
};
