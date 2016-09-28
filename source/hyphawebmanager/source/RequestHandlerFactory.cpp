// Copyright (c) 2015-2016 Hypha
#include "RequestHandlerFactory.h"
#include "ConnectionsPage.h"
#include "HandlerInstancePage.h"
#include "HandlersPage.h"
#include "Index.h"
#include "PluginInstancePage.h"
#include "PluginsPage.h"
#include "ResourceHandler.h"
#include "api/ApiHandler.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <Poco/Util/Application.h>

RequestHandlerFactory::RequestHandlerFactory(const std::string &docroot,
                                             const std::string &resroot)
    : _docroot(docroot), _resroot(resroot) {}

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest &request) {
  Poco::Util::Application &app = Poco::Util::Application::instance();
  app.logger().information("Request URI " + request.getURI());

  //

  const std::string &uri = request.getURI();
  Poco::Path uriPath(uri, _docroot);

  // request.clientAddress().toString()

  // routing

  if (uri == "/") {
    return new Index;
  } else if (uri.find("/plugins/instance") == 0) {
    return new PluginInstancePage;
  } else if (uri == "/plugins") {
    return new PluginsPage;
  } else if (uri.find("/handlers/instance") == 0) {
    return new HandlerInstancePage;
  } else if (uri == "/handlers") {
    return new HandlersPage;
  } else if (uri == "/connections") {
    return new ConnectionsPage;
  } else if (uri.find("/css/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  } else if (uri.find("/js/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  } else if (uri.find("/fonts/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  } else if (uri.find("/api/") == 0) {
    return new ApiHandler();
  }

  return nullptr;
}
