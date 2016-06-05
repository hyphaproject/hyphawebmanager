#include "api/PluginsHandler.h"
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
#include <Poco/URI.h>

#include <hypha/core/settings/pluginsettings.h>
#include <hypha/plugin/pluginloader.h>
#include <hypha/plugin/hyphaplugin.h>

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
    handleGETRequest(request, response);
  else if (request.getMethod() == "DELETE")
    handleGETRequest(request, response);
  else if (request.getMethod() == "POST")
    handleGETRequest(request, response);
}

void PluginsHandler::handleGETRequest(Poco::Net::HTTPServerRequest &request,
                                      Poco::Net::HTTPServerResponse &response) {
  UriParser uri(request.getURI());
  string send;
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

string PluginsHandler::getPlugin(string id) {
    Object::Ptr pPlugin = new Object;

    hypha::plugin::HyphaPlugin * plugin = hypha::plugin::PluginLoader::instance()->getPlugin(id);
    if(plugin)
    {
        pPlugin->set("name", plugin->name());
        pPlugin->set("description", plugin->getDescription());
        pPlugin->set("title", plugin->getTitle());
        pPlugin->set("configdescription", plugin->getConfigDescription());
    }
    std::stringstream sstream;
    pPlugin->stringify(sstream, 2);
    return sstream.str();
}

string PluginsHandler::getPluginInstance(string id) {
  Object::Ptr pPlugin = new Object;
  pPlugin->set("id", id);
  pPlugin->set("type",
               hypha::settings::PluginSettings::instance()->getName(id));
  pPlugin->set("host",
               hypha::settings::PluginSettings::instance()->getHost(id));
  pPlugin->set("config",
               hypha::settings::PluginSettings::instance()->getConfig(id));
  std::stringstream sstream;
  pPlugin->stringify(sstream, 2);
  return sstream.str();
}
