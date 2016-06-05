#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class ApiHandler : public Poco::Net::HTTPRequestHandler
{
public:
    ApiHandler();

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};
