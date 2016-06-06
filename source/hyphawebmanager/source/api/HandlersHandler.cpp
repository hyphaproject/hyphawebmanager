#include "api/HandlersHandler.h"
#include <strstream>
#include "api/UriParser.h"

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>

#include <hypha/core/settings/handlersettings.h>
#include <hypha/handler/handlerloader.h>

using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"

HandlersHandler::HandlersHandler() {}

void HandlersHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                    Poco::Net::HTTPServerResponse &response) {
  if (request.getMethod() == "GET")
    handleGETRequest(request, response);
  else if (request.getMethod() == "PUT")
    handlePUTRequest(request, response);
  else if (request.getMethod() == "DELETE")
    handleDELETERequest(request, response);
  else if (request.getMethod() == "POST")
    handlePOSTRequest(request, response);
}

void HandlersHandler::handleGETRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  UriParser uri(request.getURI());
  Object::Ptr send;
  if (uri.isHandlerInstances()) {
    std::string id = uri.getParameter("id", "");
    if (id.empty())
      send = getHandlerInstances();
    else
      send = getHandlerInstance(id);
  } else if (uri.isHandlers()) {
    std::string id = uri.getParameter("id", "");
    if (id.empty())
      send = getHandlers();
    else
      send = getHandler(id);
  }

  if (send->size() == 0) {
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND,
                                "No Handler with this ID!");
    send->set("error", "No Handler with this ID!");
  } else {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  }
  response.setContentType("application/json");
  send->stringify(response.send(), 2);
}

void HandlersHandler::handlePUTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void HandlersHandler::handleDELETERequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void HandlersHandler::handlePOSTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

Object::Ptr HandlersHandler::getHandlers() {
  Object::Ptr pHandler = new Object;
  Array::Ptr pArr = new Array;
  for (hypha::handler::HyphaHandler *handler :
       hypha::handler::HandlerLoader::instance()->getHandlers()) {
    pArr->add(handler->name());
  }

  pHandler->set("handlers", pArr);
  return pHandler;
}

Object::Ptr HandlersHandler::getHandlerInstances() {
  Object::Ptr pHandler = new Object;
  Array::Ptr pArr = new Array;
  for (std::string handler :
       hypha::settings::HandlerSettings::instance()->getAllHandlerIds()) {
    pArr->add(handler);
  }

  pHandler->set("handlers", pArr);
  return pHandler;
}

Object::Ptr HandlersHandler::getHandler(string id) {
  Object::Ptr pHandler = new Object;

  hypha::handler::HyphaHandler *handler =
      hypha::handler::HandlerLoader::instance()->getHandler(id);
  if (handler) {
    pHandler->set("name", handler->name());
    pHandler->set("description", handler->getDescription());
    pHandler->set("title", handler->getTitle());
    pHandler->set("configdescription", handler->getConfigDescription());
  }
  return pHandler;
}

Object::Ptr HandlersHandler::getHandlerInstance(string id) {
  Object::Ptr pHandler = new Object;
  std::string type = hypha::settings::HandlerSettings::instance()->getName(id);
  if (!type.empty()) {
    pHandler->set("id", id);
    pHandler->set("type", type);
    pHandler->set("host",
                  hypha::settings::HandlerSettings::instance()->getHost(id));
    pHandler->set("config",
                  hypha::settings::HandlerSettings::instance()->getConfig(id));
  }
  return pHandler;
}
