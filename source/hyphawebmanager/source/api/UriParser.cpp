#include "api/UriParser.h"

UriParser::UriParser(std::string uri) : uri(uri) {}

bool UriParser::isPlugins() { return uri.getPath() == "/api/plugins"; }

bool UriParser::isPluginInstances() {
  return uri.getPath() == "/api/plugins/instances";
}

std::string UriParser::getParameter(std::string key, std::string defaultValue) {

  for (auto parameter : uri.getQueryParameters()) {
    if (parameter.first == key) return parameter.second;
  }
  return defaultValue;
}
