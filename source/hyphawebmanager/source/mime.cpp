#include "mime.h"

#include <vector>
#include <Poco/Path.h>

struct FileMime
{
	const char* extension;
	const char* mime;
};

static const FileMime FileMimes[] {
	{ "html", "text/html"  },
	{ "js",   "application/javascript" },
	{ "txt",  "text/plain" },
	{ "css",  "text/css"   },
	{ "gif",  "image/gif"  },
	{ "jpeg", "image/jpeg" },
	{ "jpg",  "image/jpeg" },
	{ "png",  "image/png"  }
};

static const FileMime UnknownMime { "", "application/octet-stream"} ;

const char* mime(const std::string& file_path)
{
	Poco::Path path(file_path);

	const FileMime* begin = &FileMimes[0];
	const FileMime* end = &FileMimes[0] + sizeof(FileMimes) / sizeof(FileMimes[0]);

	for (const FileMime* i = begin; i != end; ++i) {
		if (path.getExtension().compare(i->extension) == 0)
			return i->mime;
	}

	return UnknownMime.mime;
}
