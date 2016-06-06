// Copyright (c) 2015-2016 HyphaRunner

#include "hyphawebmanager.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "RequestHandlerFactory.h"
#include "hyphawebmanager/hyphawebmanager-version.h"

#ifdef __linux__
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#endif

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>

#include <hypha/core/database/database.h>
#include <hypha/core/database/databasegenerator.h>
#include <hypha/core/database/userdatabase.h>
#include <hypha/core/settings/configgenerator.h>
#include <hypha/core/settings/databasesettings.h>
#include <hypha/core/settings/hyphasettings.h>
#include <hypha/handler/handlerloader.h>
#include <hypha/plugin/pluginloader.h>
#include <hypha/utils/logger.h>

using namespace Poco::Util;
using namespace hypha::plugin;
using namespace hypha::handler;
using namespace hypha::utils;
using namespace hypha::settings;
using namespace hypha::database;

using Poco::Util::Option;
using Poco::Util::HelpFormatter;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;

#ifndef RESOURCES_ROOT
constexpr const char *ResourceRoot =
    "/usr/local/share/hyphawebmanager/resources";
#else
constexpr const char *ResourceRoot = RESOURCES_ROOT;
#endif
constexpr const unsigned int DefaultPort = 9780;

void HyphaWebManager::initialize(Application &self) {
  loadConfiguration();  // load default configuration files, if present
  ServerApplication::initialize(self);
  Logger::info("Starting Hypha Web Manager!");
}

void HyphaWebManager::uninitialize() {
  Logger::info("Shutting Down Hypha Web Manager");
  ServerApplication::uninitialize();
}

int HyphaWebManager::main(const std::vector<std::string> &args) {
  if (_helpRequested) return Application::EXIT_USAGE;

  try {
    HyphaSettings::loadInstance(
        config().getString("config-file", "/etc/hypha/hypha.conf"));
    Logger::info("\nLoading Database Settings ...\n");
    DatabaseSettings::instance();
    UserDatabaseSettings::instance();
    Logger::info(UserDatabaseSettings::instance()->getDatabase());
    Logger::info("Loading Database ...");
    UserDatabase::instance();

    std::string stdHandlersDir;
    std::string stdPluginsDir;
#ifdef __linux__
    stdHandlersDir = "/usr/local/lib/hyphahandlers";
    stdPluginsDir = "/usr/local/lib/hyphaplugins";
#else
    stdHandlersDir = "../hyphahandlers";
    stdPluginsDir = "../hyphaplugins";
#endif

    Logger::info("Loading Handler ...");
    HandlerLoader::instance()->loadHandlers(
        config().getString("handlersdir", stdHandlersDir));
    Logger::info("Loading Plugins ...");
    PluginLoader::instance()->loadPlugins(
        config().getString("pluginsdir", stdPluginsDir));

    unsigned short port = (unsigned short)config().getInt("port", DefaultPort);

    HTTPServerParams *params = new HTTPServerParams;
    params->setServerName(HYPHAWEBMANAGER_PROJECT_NAME);
    params->setSoftwareVersion(HYPHAWEBMANAGER_VERSION);

    // set-up a server socket
    ServerSocket svs(port);

    // set-up a HTTPServer instance
    HTTPServer srv(new RequestHandlerFactory(_docroot, _resroot), svs, params);
    // start the HTTPServer
    srv.start();
    // wait for CTRL-C or kill
    waitForTerminationRequest();
    // Stop the HTTPServer
    srv.stop();
  } catch (Poco::Exception &e) {
    Logger::fatal(e.what());
  } catch (std::exception &e) {
    Logger::fatal(e.what());
  } catch (...) {
    Logger::fatal("unknown Exception");
  }

  return Application::EXIT_OK;
}

void HyphaWebManager::defineOptions(OptionSet &options) {
  Application::defineOptions(options);
  options.addOption(
      Option("help", "h", "Display help information on command line arguments")
          .required(false)
          .repeatable(false)
          .callback(OptionCallback<HyphaWebManager>(
              this, &HyphaWebManager::handleHelp)));
  options.addOption(Option("port", "p", "Port to serve")
                        .required(true)
                        .repeatable(false)
                        .argument("number")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("user", "u", "Name to login.")
                        .required(true)
                        .repeatable(false)
                        .argument("name")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("password", "pw", "Password to login.")
                        .required(true)
                        .repeatable(false)
                        .argument("pass")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("docroot", "d", "Document Root directory")
                        .required(false)
                        .repeatable(false)
                        .argument("path")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("resroot", "r", "Resources directory")
                        .required(false)
                        .repeatable(false)
                        .argument("path")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(
      Option("config-file", "f", "load configuration data from a file")
          .required(false)
          .repeatable(false)
          .argument("file")
          .callback(OptionCallback<HyphaWebManager>(
              this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("handlersdir", "hd", "location for handlers")
                        .required(false)
                        .repeatable(false)
                        .argument("dir")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
  options.addOption(Option("pluginsdir", "pd", "location for plugins")
                        .required(false)
                        .repeatable(false)
                        .argument("dir")
                        .callback(OptionCallback<HyphaWebManager>(
                            this, &HyphaWebManager::handleConfig)));
}

void HyphaWebManager::handleHelp(const std::string &name,
                                 const std::string &value) {
  _helpRequested = true;
  displayHelp();
  stopOptionsProcessing();
}

void HyphaWebManager::handleConfig(const std::string &name,
                                   const std::string &value) {
  if (name == "port" || name == "p") {
    Logger::info("Port given: " + value);
    config().setInt("port", stoi(value));
  } else if (name == "user" || name == "u") {
    Logger::info("User given: " + value);
    config().setString("user", value);
  } else if (name == "password" || name == "pw") {
    Logger::info("Password given: " + value);
    config().setString("password", value);
  } else if (name == "docroot" || name == "d") {
    Logger::info("Docroot given: " + value);
    _docroot = value;
  } else if (name == "resroot" || name == "r") {
    Logger::info("Resroot given: " + value);
    _resroot = value;
  } else if (name == "config-file" || name == "f") {
    Logger::info("Loading Config from File: " + value);
    config().setString("config-file", value);
  } else if (name == "handlersdir" || name == "hd") {
    Logger::info("Loading Handlers from dir: " + value);
    config().setString("handlersdir", value);
  } else if (name == "pluginsdir" || name == "pd") {
    Logger::info("Loading Plugins from dir: " + value);
    config().setString("pluginsdir", value);
  }
}

void HyphaWebManager::displayHelp() {
  HelpFormatter helpFormatter(options());
  helpFormatter.setCommand(commandName());
  helpFormatter.setUsage("OPTIONS");
  helpFormatter.setHeader("Hypha Web Manager ...");
  helpFormatter.format(std::cout);
}

POCO_SERVER_MAIN(HyphaWebManager)
