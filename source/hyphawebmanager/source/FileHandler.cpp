#include "FileHandler.h"

#include "mime.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/Application.h>
#include <Poco/File.h>

FileHandler::FileHandler(const Poco::Path& path)
: _path(path)
{
}

void FileHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
	Poco::File file(_path);

	if (!file.exists()) {
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "File not found");
		response.send();
		return;
	}

	response.sendFile(_path.toString(), mime(_path.toString()));
}
