// Copyright (c) 2015-2016 Hypha
#include "api/ApiHandler.h"
#include "api/ConnectionsHandler.h"
#include "api/HandlersHandler.h"
#include "api/PluginsHandler.h"
#include "api/UriParser.h"

#include <Poco/File.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>
#include <Poco/URI.h>
#include <Poco/Util/Application.h>

#include "AccessDeniedHandler.h"
#include "FileHandler.h"
#include "NotFoundHandler.h"

ApiHandler::ApiHandler() {}

void ApiHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response) {
  UriParser uri(request.getURI());

  if (uri.isPlugins() || uri.isPluginInstances()) {
    PluginsHandler pluginsHandler;
    pluginsHandler.handleRequest(request, response);
  } else if (uri.isHandlers() || uri.isHandlerInstances()) {
    HandlersHandler handlersHandler;
    handlersHandler.handleRequest(request, response);
  } else if (uri.isConnections()) {
    ConnectionsHandler connectionsHandler;
    connectionsHandler.handleRequest(request, response);
  } else {
    NotFoundHandler notFoundHandler;
    notFoundHandler.handleRequest(request, response);
  }
}
