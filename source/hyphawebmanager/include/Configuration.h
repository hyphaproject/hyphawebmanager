#pragma once

constexpr const char*           DocumentRoot  = "/var/www/localhost";
#ifndef RESOURCES_ROOT
constexpr const char*           ResourceRoot  = "/var/lib/filebrowser";
#else
constexpr const char*           ResourceRoot  = RESOURCES_ROOT;
#endif
constexpr const unsigned int    DefaultPort   = 9780;
constexpr const char*           ServerName    = "File Browser";
constexpr const char*           ServerVersion = "0.1";
