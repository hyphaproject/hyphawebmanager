#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class ResourceHandler : public Poco::Net::HTTPRequestHandler
{
public:
 	ResourceHandler(const std::string& docroot, const std::string& resroot);

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
	void handleResource(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	void handleInternalResource(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

	const std::string& _docroot;
	const std::string& _resroot;
};
