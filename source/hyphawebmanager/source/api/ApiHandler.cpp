#include "api/ApiHandler.h"
#include "api/HandlersHandler.h"
#include "api/PluginsHandler.h"

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
  const std::string& uri = request.getURI();

  if (uri.find("/api/plugins") == 0) {
    PluginsHandler pluginsHandler;
    pluginsHandler.handleRequest(request, response);
  } else if (uri.find("/api/handlers") == 0) {
    HandlersHandler handlersHandler;
    handlersHandler.handleRequest(request, response);
  }
}
