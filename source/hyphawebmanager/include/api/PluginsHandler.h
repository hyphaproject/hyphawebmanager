// Copyright (c) 2015-2016 Hypha
#pragma once

#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

class PluginsHandler : public Poco::Net::HTTPRequestHandler {
 public:
  PluginsHandler();

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

  Poco::JSON::Object::Ptr getPlugins();
  Poco::JSON::Object::Ptr getPluginInstances();
  Poco::JSON::Object::Ptr getPlugin(std::string id);
  Poco::JSON::Object::Ptr getPluginInstance(std::string id);
};
