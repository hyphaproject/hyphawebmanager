#include "api/HandlersHandler.h"
#include <strstream>
#include "api/UriParser.h"

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>
#include <Poco/URI.h>

#include <hypha/controller/handler.h>
#include <hypha/core/settings/handlersettings.h>
#include <hypha/handler/handlerloader.h>
#include <hypha/utils/logger.h>

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
    Poco::Net::HTTPServerResponse &response) {
  try {
    Poco::Net::HTMLForm form(request, request.stream());

    std::string id = form["id"];
    if (id.empty()) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }
    hypha::controller::Handler con(hypha::database::Database::instance());
    con.remove(id);
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.send();
    return;
  } catch (Poco::Exception &e) {
    hypha::utils::Logger::error(e.what());
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
                                e.what());
    response.send();
    return;
  }
}

void HandlersHandler::handlePOSTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  try {
    Poco::Net::HTMLForm form(request, request.stream());

    std::string id = form["id"];
    std::string type = form["type"];
    std::string host = form["host"];
    if (id.empty() || type.empty()) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    if (hypha::handler::HandlerLoader::instance()->getHandler(type) ==
        nullptr) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    hypha::controller::Handler con(hypha::database::Database::instance());
    con.add(id, host, type, "{}");
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.send();
    return;
  } catch (Poco::Exception &e) {
    hypha::utils::Logger::error(e.what());
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
                                e.what());
    response.send();
    return;
  }
}

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
