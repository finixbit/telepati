/*
*
*	MIT License
*
*	Copyright (c) 2016 Intelburg
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*
*
*/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/time.h>
#include <time.h>
#include <algorithm> //generate_n
#include <jansson.h>
#include "config.h"

#define CACHE_INVALID_PARAM	"null"
using namespace std;


bool AppConfig::parseJsonConfig(const string &buffer)
{
  json_t *root;
  json_t *data;
  json_error_t error;
  root = json_loads(buffer.c_str(), 0, &error);

  if(!root)
	{
	  cout << error.text << buffer.c_str() << endl;
    return false;
	}

	try
	{
		data = json_object_get(root, "debug");
		istringstream(json_string_value(data)) >> boolalpha >> debugMode;

		data = json_object_get(root, "interface");
		interface = json_string_value(data);

		data = json_object_get(root, "receiver_filter");
		receiverFilter = json_string_value(data);

		data = json_object_get(root, "core_grpc_server");
		coreGrpcServer= json_string_value(data);

		data = json_object_get(root, "sender_grpc_server");
		senderGrpcServer= json_string_value(data);

		data = json_object_get(root, "redis_host");
		redisHost = json_string_value(data);

		data = json_object_get(root, "redis_port");
		redisPort = json_integer_value(data);
	}
	catch(...)
	{
		cout << error.text << endl;
		json_decref(data);
		json_decref(root);
		return false;
	}

	json_decref(data);
	json_decref(root);
	return true;
}

const string AppConfig::readFileContent(const string &filepath)
{
	string rawContent(string(CACHE_INVALID_PARAM));
	stringstream buffer;
	try
	{
		ifstream in(filepath);
		buffer 	<< in.rdbuf();

		rawContent = buffer.str();
	}
	catch(...){
		cout << "Error Reading File Content ..." << endl;
	}
	return rawContent;
}

void AppConfig::loadJsonConfig(const string &filepath)
{
	const string fileContent = AppConfig::readFileContent(filepath);
	
	if(fileContent==string(CACHE_INVALID_PARAM))
		configIsValid=false;
	else
		configIsValid=true;

	if(configIsValid)
	{
		if(parseJsonConfig(fileContent))
			configIsValid=true;

		else
			configIsValid=false;
	}
	else
		cout << "Invalid Configuration File" << endl;
}

bool AppConfig::configIsInvalid()
{
	if(configIsValid)
		return false;

	return true;
}

string AppConfig::getDateTime(){
  time_t ltime; /* calendar time */
  ltime=time(NULL); /* get current cal time */

  string asctimeStr;

	asctimeStr.append(asctime(localtime(&ltime)), 
		strlen(asctime(localtime(&ltime)))-1 
	);	
  return asctimeStr;
}

string AppConfig::generateRandomString(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}