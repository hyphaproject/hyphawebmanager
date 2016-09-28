// Copyright (c) 2015-2016 Hypha
#include "api/PluginsHandler.h"
#include <sstream>
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

#include <hypha/controller/plugin.h>
#include <hypha/core/settings/pluginsettings.h>
#include <hypha/plugin/hyphaplugin.h>
#include <hypha/plugin/pluginloader.h>
#include <hypha/utils/logger.h>

using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"

PluginsHandler::PluginsHandler() {}

void PluginsHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
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

void PluginsHandler::handleGETRequest(Poco::Net::HTTPServerRequest &request,
                                      Poco::Net::HTTPServerResponse &response) {
  UriParser uri(request.getURI());
  Poco::JSON::Object::Ptr send;
  if (uri.isPluginInstances()) {
    std::string id = uri.getParameter("id", "");
    if (id.empty())
      send = getPluginInstances();
    else
      send = getPluginInstance(id);
  } else if (uri.isPlugins()) {
    std::string id = uri.getParameter("id", "");
    if (id.empty())
      send = getPlugins();
    else
      send = getPlugin(id);
  }

  if (send->size() == 0) {
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND,
                                "No Plugin with this ID!");
    send->set("error", "No Plugin with this ID!");
  } else {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  }
  response.setContentType("application/json");
  send->stringify(response.send(), 2);
}

void PluginsHandler::handlePUTRequest(Poco::Net::HTTPServerRequest &request,
                                      Poco::Net::HTTPServerResponse &response) {
  try {
    Poco::Net::HTMLForm form(request, request.stream());

    std::string id = form["id"];
    std::string config = form["config"];
    if (id.empty()) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    hypha::controller::Plugin con(hypha::database::Database::instance());
    con.updateConfig(id, config);
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

void PluginsHandler::handleDELETERequest(
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
    hypha::controller::Plugin con(hypha::database::Database::instance());
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

void PluginsHandler::handlePOSTRequest(
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

    if (hypha::plugin::PluginLoader::instance()->getPlugin(type) == nullptr) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
      response.send();
      return;
    }

    hypha::controller::Plugin con(hypha::database::Database::instance());
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

Poco::JSON::Object::Ptr PluginsHandler::getPlugins() {
  Object::Ptr pPlugins = new Object;
  Array::Ptr pArr = new Array;
  for (hypha::plugin::HyphaPlugin *plugin :
       hypha::plugin::PluginLoader::instance()->getPlugins()) {
    pArr->add(plugin->name());
  }

  pPlugins->set("plugins", pArr);
  return pPlugins;
}

Poco::JSON::Object::Ptr PluginsHandler::getPluginInstances() {
  Object::Ptr pPlugins = new Object;
  Array::Ptr pArr = new Array;
  for (std::string plugin :
       hypha::settings::PluginSettings::instance()->getAllPluginIds()) {
    pArr->add(plugin);
  }

  pPlugins->set("plugins", pArr);
  return pPlugins;
}

Poco::JSON::Object::Ptr PluginsHandler::getPlugin(string id) {
  Object::Ptr pPlugin = new Object;

  hypha::plugin::HyphaPlugin *plugin =
      hypha::plugin::PluginLoader::instance()->getPlugin(id);
  if (plugin) {
    pPlugin->set("name", plugin->name());
    pPlugin->set("description", plugin->getDescription());
    pPlugin->set("title", plugin->getTitle());
    pPlugin->set("configdescription", plugin->getConfigDescription());
  }
  return pPlugin;
}

Poco::JSON::Object::Ptr PluginsHandler::getPluginInstance(string id) {
  Object::Ptr pPlugin = new Object;
  std::string type = hypha::settings::PluginSettings::instance()->getName(id);
  if (!type.empty()) {
    pPlugin->set("id", id);
    pPlugin->set("type", type);
    pPlugin->set("host",
                 hypha::settings::PluginSettings::instance()->getHost(id));
    pPlugin->set("config",
                 hypha::settings::PluginSettings::instance()->getConfig(id));
  }
  return pPlugin;
}
