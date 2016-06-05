#pragma once

#include <string>

#include <Poco/URI.h>

class UriParser
{
public:
    UriParser(std::string uri);

    bool isPlugins();
    bool isPluginInstances();

    std::string getParameter(std::string key, std::string defaultValue);
protected:
    Poco::URI uri;
};
