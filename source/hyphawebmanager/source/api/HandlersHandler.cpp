#include "api/HandlersHandler.h"

#include <strstream>

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
    handleGETRequest(request, response);
  else if (request.getMethod() == "DELETE")
    handleGETRequest(request, response);
  else if (request.getMethod() == "POST")
    handleGETRequest(request, response);
}

void HandlersHandler::handleGETRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  string send;
  if (request.getURI() == "/api/handlers/instances/") {
    send = getHandlerInstances();
  } else if (request.getURI() == "/api/handlers/") {
    send = getHandlers();
  }

  response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  response.setContentType("application/json");

  std::ostream &ostr = response.send();

  ostr << send;
  response.setContentLength(send.size());
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

std::string HandlersHandler::getHandlers() {
  Object::Ptr pHandler = new Object;
  Array::Ptr pArr = new Array;
  for (hypha::handler::HyphaHandler *handler :
       hypha::handler::HandlerLoader::instance()->getHandlers()) {
    pArr->add(handler->name());
  }

  pHandler->set("handlers", pArr);
  std::stringstream sstream;
  pHandler->stringify(sstream, 2);
  return sstream.str();
}

string HandlersHandler::getHandlerInstances() {
  Object::Ptr pHandler = new Object;
  Array::Ptr pArr = new Array;
  for (std::string handler :
       hypha::settings::HandlerSettings::instance()->getAllHandlerIds()) {
    pArr->add(handler);
  }

  pHandler->set("handlers", pArr);
  std::stringstream sstream;
  pHandler->stringify(sstream, 2);
  return sstream.str();
}
