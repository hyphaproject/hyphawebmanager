#pragma once

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

class HandlersHandler : public Poco::Net::HTTPRequestHandler
{
public:
    HandlersHandler();

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

protected:
    void handleGETRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void handlePUTRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void handleDELETERequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void handlePOSTRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    std::string getHandlers();
    std::string getHandlerInstances();
};
