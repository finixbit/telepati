/* North-Pole
* Copyright (C) 2012-2016 Samuel Asirifi <samuelasirifi1@gmail.com>
* All rights reserved.
*
* Use and distribution licensed under the North-Pole license.  
*/

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>

using namespace std;

class AppConfig
{

private:
	bool 
	configIsValid;

	bool 
	parseJsonConfig(const string &buffer);

public:
	bool 		 debugMode;
	string   interface;
	string   receiverFilter;
	string 	 coreGrpcServer;
	string 	 senderGrpcServer;
	string 	 redisHost;
	uint32_t redisPort;
	
	const string readFileContent(const string &filepath);

	void loadJsonConfig(const string &filepath);

	bool configIsInvalid();

	static string getDateTime();

	static string generateRandomString(size_t length);
};


#endif  // APP_CONFIG_H