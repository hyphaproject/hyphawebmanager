#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Path.h>

class FileHandler : public Poco::Net::HTTPRequestHandler
{
public:
	FileHandler(const Poco::Path& path);

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
	const Poco::Path _path;
};
