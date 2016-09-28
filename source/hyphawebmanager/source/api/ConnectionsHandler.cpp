// Copyright (c) 2015-2016 Hypha
#include "api/ConnectionsHandler.h"
#include <sstream>
#include "api/UriParser.h"

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>
#include <Poco/URI.h>

#include <hypha/controller/connection.h>
#include <hypha/core/settings/pluginsettings.h>
#include <hypha/handler/handlerloader.h>
#include <hypha/plugin/hyphaplugin.h>
#include <hypha/plugin/pluginloader.h>
#include <hypha/utils/logger.h>

using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"

ConnectionsHandler::ConnectionsHandler() {}

void ConnectionsHandler::handleRequest(
    Poco::Net::HTTPServerRequest &request,
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

void ConnectionsHandler::handleGETRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  UriParser uri(request.getURI());
  Poco::JSON::Object::Ptr send;
  std::string id = uri.getParameter("id", "");
  if (id.empty())
    send = getConnections();
  else
    send = getConnection(id);

  if (send->size() == 0) {
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND,
                                "No Connection with this ID!");
    send->set("error", "No Connection with this ID!");
  } else {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  }
  response.setContentType("application/json");
  send->stringify(response.send(), 2);
}

void ConnectionsHandler::handlePUTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void ConnectionsHandler::handleDELETERequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void ConnectionsHandler::handlePOSTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  try {
    UriParser uri(request.getURI());
    std::string handler = uri.getParameter("handler", "");
    std::string plugin = uri.getParameter("plugin", "");
    if (handler.empty() || plugin.empty()) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    if (hypha::handler::HandlerLoader::instance()->getHandlerInstance(
            handler) == nullptr ||
        hypha::plugin::PluginLoader::instance()->getPluginInstance(plugin) ==
            nullptr) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    hypha::controller::Connection con(hypha::database::Database::instance());
    con.create(handler, plugin);
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

Poco::JSON::Object::Ptr ConnectionsHandler::getConnections() {
  Object::Ptr pConnection = new Object;
  Array::Ptr pArr = new Array;
  hypha::controller::Connection con(hypha::database::Database::instance());
  for (std::tuple<std::string, std::string, std::string> connection :
       con.getConnections()) {
    Object::Ptr pObj = new Object;
    pObj->set("id", std::get<0>(connection));
    pObj->set("handler", std::get<1>(connection));
    pObj->set("plugin", std::get<2>(connection));
    pArr->add(pObj);
  }

  pConnection->set("connections", pArr);
  return pConnection;
}

Poco::JSON::Object::Ptr ConnectionsHandler::getConnection(string id) {
  Object::Ptr pConnection = new Object;
  hypha::controller::Connection con(hypha::database::Database::instance());
  for (std::tuple<std::string, std::string, std::string> connection :
       con.getConnections()) {
    if (std::get<0>(connection) == id) {
      pConnection->set("id", std::get<0>(connection));
      pConnection->set("handler", std::get<1>(connection));
      pConnection->set("plugin", std::get<2>(connection));
    }
  }
  return pConnection;
}
