#include "RequestHandlerFactory.h"
#include "Index.h"
#include "ResourceHandler.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <Poco/Util/Application.h>

RequestHandlerFactory::RequestHandlerFactory(const std::string &docroot,
                                             const std::string &resroot)
    : _docroot(docroot), _resroot(resroot) {}

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest &request) {
  Poco::Util::Application &app = Poco::Util::Application::instance();
  app.logger().information("Request URI " + request.getURI());

  //

  const std::string &uri = request.getURI();
  Poco::Path uriPath(uri, _docroot);

  // request.clientAddress().toString()

  // routing

  if (uri == "/") {
    return new Index;
  } else if (uri.find("/css/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  } else if (uri.find("/js/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  } else if (uri.find("/fonts/") == 0) {
    return new ResourceHandler(_docroot, _resroot);
  }

  return nullptr;
}
