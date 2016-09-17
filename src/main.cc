/*
*
* MIT License
*
* Copyright (c) 2016 Intelburg
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
*/

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
  StringOption configOpt('c' , "config" , true, "Specify Config File");
  StringOption serverOpt('s' , "server" , true, "Specify Server Type (receiver|core|sender)");

  Parser parser;
  parser.addOption(configOpt);
  parser.addOption(serverOpt);
  vector<string> otherArguments = parser.parse(argc, argv);

  appConfig.loadJsonConfig(configOpt.getValue());
  if (appConfig.configIsInvalid())
    return EXIT_FAILURE;

  if (serverOpt.getValue() == "receiver") {
    RunReceiverServer(appConfig);

  } else if (serverOpt.getValue() == "core") {
    RunCoreServer(appConfig);

  } else if (serverOpt.getValue() == "sender") {
    RunSenderServer(appConfig);

  } else {
    cout << "Specify Server Type (receiver|core|sender)" << endl;
  }

  return 0;
}



