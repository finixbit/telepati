/* North-Pole
* Copyright (C) 2012-2016 Samuel Asirifi <samuelasirifi1@gmail.com>
* All rights reserved.
*
* Use and distribution licensed under the North-Pole license.  
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