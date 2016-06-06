#include "api/UriParser.h"

UriParser::UriParser(std::string uri) : uri(uri) {}

bool UriParser::isPlugins() { return uri.getPath() == "/api/plugins"; }

bool UriParser::isPluginInstances() {
  return uri.getPath() == "/api/plugins/instances";
}

bool UriParser::isHandlers() { return uri.getPath() == "/api/handlers"; }

bool UriParser::isHandlerInstances() {
    return uri.getPath() == "/api/handlers/instances";
}

bool UriParser::isConnections()
{
    return uri.getPath() == "/api/connections";
}

std::string UriParser::getParameter(std::string key, std::string defaultValue) {
  for (auto parameter : uri.getQueryParameters()) {
    if (parameter.first == key) return parameter.second;
  }
  return defaultValue;
}
