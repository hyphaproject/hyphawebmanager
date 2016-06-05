// Copyright (c) 2015-2016 Hypha
#pragma once

#include <Poco/Util/OptionSet.h>
#include <Poco/Util/ServerApplication.h>

class HyphaWebManager : public Poco::Util::ServerApplication {
 public:
  HyphaWebManager() {}

  ~HyphaWebManager() {}

 protected:
  void initialize(Application &self);
  void uninitialize();
  int main(const std::vector<std::string> &args);
  void defineOptions(Poco::Util::OptionSet &options);
  void handleHelp(const std::string &name, const std::string &value);
  void handleExampleFile(const std::string &name, const std::string &value);
  void handleConfig(const std::string &name, const std::string &value);
  void displayHelp();

 private:
  bool _helpRequested = false;
  bool _exampleRequested = false;
  std::string _docroot;
  std::string _resroot;
};
