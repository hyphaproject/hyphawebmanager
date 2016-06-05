#include "RequestHandlerFactory.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <Poco/Util/Application.h>

RequestHandlerFactory::RequestHandlerFactory(const std::string &docroot, const std::string& resroot)
: _docroot(docroot),
  _resroot(resroot)
{
}

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
	Poco::Util::Application &app = Poco::Util::Application::instance();
	app.logger().information("Request URI " + request.getURI());

    //return new ResourceHandler(_docroot, _resroot);

	/*const std::string& uri = request.getURI();
	Poco::Path uriPath(uri, _docroot);*/

	// request.clientAddress().toString()

	// routing

	/*if (uri == "/") {
		return new FileRequestBrowser(_docroot + "/index.html");
	} else if (urlPath.isDirectory()) {
		return new DirectoryRequestHandler(_docroot);
    } else {
		return new FileRequestBrowser(urlPath.path());
    }*/

    return nullptr;
}
