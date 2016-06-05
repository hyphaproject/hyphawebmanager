#include "ResourceHandler.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/Application.h>
#include <Poco/File.h>
#include <Poco/URI.h>
#include <Poco/RegularExpression.h>

#include "FileHandler.h"
#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"

ResourceHandler::ResourceHandler(const std::string& docroot, const std::string& resroot)
: _docroot(docroot),
  _resroot(resroot)
{
}

void ResourceHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::URI uri(request.getURI());

	Poco::Path path(_docroot, uri.getPath());
	Poco::File file(path);

	// is existing file in docroot?
	if (file.exists()) {
		handleResource(request, response);
		return;
	}

	// is resource file?
	path = Poco::Path (_resroot, request.getURI());
	file = Poco::File(path);

	if (file.exists()) {
		handleInternalResource(request, response);
		return;
    }

	NotFoundHandler handler;
	handler.handleRequest(request, response);
	return;
}

void ResourceHandler::handleResource(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::URI uri(request.getURI());
	Poco::Path path(_docroot, uri.getPath());
	Poco::File file(path);

	if (!file.exists()) {
		response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_NOT_FOUND, "Not found");

		NotFoundHandler handler;
		handler.handleRequest(request, response);
		return;
	}

	if (file.isDirectory()) {
		FileHandler handler(Poco::Path(_resroot, "/index.html"));
		handler.handleRequest(request, response);
		return;
	} else if (file.isFile()) {
		FileHandler handler(path);
		handler.handleRequest(request, response);
		return;
	}
}

void ResourceHandler::handleInternalResource(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	Poco::Path path(_resroot, request.getURI());
	Poco::File file(path);

	if (!file.exists()) {
		response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_NOT_FOUND, "Not found");

		NotFoundHandler handler;
		handler.handleRequest(request, response);
		return;
	}

	if (file.isFile()) {
		FileHandler handler(path);
		handler.handleRequest(request, response);
		return;
	} else {
		NotFoundHandler handler;
		handler.handleRequest(request, response);
		return;
	}
}
