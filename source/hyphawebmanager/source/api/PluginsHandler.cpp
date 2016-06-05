#include "api/PluginsHandler.h"

#include <strstream>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>

#include <hypha/plugin/pluginloader.h>
#include <hypha/core/settings/pluginsettings.h>

using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"

PluginsHandler::PluginsHandler()

{}

void PluginsHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
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

void PluginsHandler::handleGETRequest(Poco::Net::HTTPServerRequest &request,
                                      Poco::Net::HTTPServerResponse &response) {
  string send;
  if (request.getURI() == "/api/plugins/instances/") {
    send = getPluginInstances();
  } else if (request.getURI() == "/api/plugins/") {
    send = getPlugins();
  }

  response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  response.setContentType("application/json");

  std::ostream &ostr = response.send();

  ostr << send;
  response.setContentLength(send.size());
}

void PluginsHandler::handlePUTRequest(Poco::Net::HTTPServerRequest &request,
                                      Poco::Net::HTTPServerResponse &response) {

}

void PluginsHandler::handleDELETERequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void PluginsHandler::handlePOSTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

std::string PluginsHandler::getPlugins() {
  Object::Ptr pPlugins = new Object;
  Array::Ptr pArr = new Array;
  for (hypha::plugin::HyphaPlugin *plugin :
       hypha::plugin::PluginLoader::instance()->getPlugins()) {
    pArr->add(plugin->name());
  }

  pPlugins->set("plugins", pArr);
  std::stringstream sstream;
  pPlugins->stringify(sstream, 2);
  return sstream.str();
}

string PluginsHandler::getPluginInstances() {
  Object::Ptr pPlugins = new Object;
  Array::Ptr pArr = new Array;
  for (std::string plugin :
       hypha::settings::PluginSettings::instance()->getAllPluginIds()) {
    pArr->add(plugin);
  }

  pPlugins->set("plugins", pArr);
  std::stringstream sstream;
  pPlugins->stringify(sstream, 2);
  return sstream.str();
}
