

#include <iostream>
#include <string>
#include "Parser.h"
#include "config.h"
#include "sender.h"
#include "receiver.h"
#include "core.h"

using namespace std;
AppConfig appConfig;


int main(int argc, char** argv) {
  StringOption configOpt('c' ,"config" ,true, "Specify Config File");
  StringOption serverOpt('s' ,"server" ,true, "Specify Server Type (receiver|core|sender)");

  Parser parser;
  parser.addOption(configOpt);
  parser.addOption(serverOpt);
  vector<string> otherArguments = parser.parse(argc, argv);

  appConfig.loadJsonConfig(configOpt.getValue());
  if(appConfig.configIsInvalid())
    return EXIT_FAILURE;

  if(serverOpt.getValue() == "receiver"){
    RunReceiverServer(appConfig);

  } else if(serverOpt.getValue() == "core"){
    RunCoreServer(appConfig);
  
  } else if(serverOpt.getValue() == "sender"){
    RunSenderServer(appConfig);
  
  } else {
    cout << "Specify Server Type (receiver|core|sender)" << endl;
  }

  return 0;
}



