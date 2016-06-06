#pragma once

#include <string>

#include <Poco/URI.h>

class UriParser
{
public:
    UriParser(std::string uri);

    bool isPlugins();
    bool isPluginInstances();
    bool isHandlers();
    bool isHandlerInstances();
    bool isConnections();

    std::string getParameter(std::string key, std::string defaultValue);
protected:
    Poco::URI uri;
};
