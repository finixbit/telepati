/* Intelfort
* Copyright (C) 2015 Samuel Asirifi <samuelasirifi1@gmail.com>
* All rights reserved.
*
* Use and distribution licensed under the Intelfort license.  
*/


#ifndef API_REDIS_H
#define API_REDIS_H

#include <cstdint> /* uint32_t */
#include <string>
#include <map>
#include <hiredis.h> /* redisContext */

#define CACHE_INVALID_PARAM	"null"

using namespace std;
//using string = basic_string<char32_t>;

class LightRedis
{
	private:
		redisContext*	m_redis_conn;
		string 				m_redis_host;
		uint32_t 			m_redis_port;
		bool 					m_redis_conn_is_valid;

	public:
		LightRedis();
		
		LightRedis(string &redisHost, uint32_t &redisPort);
		
		~LightRedis(void);

		void init(string &redisHost, uint32_t &redisPort);

		bool redisConnIsInvalid();

		bool connectToRedis();
		
		bool redisSet(string &redisKey, string &redisValue);
		
		string redisGet(string &redisKey);											
		
		bool redisHashSet(string &hashKey, string fieldKey, string fieldValue);
		
		string redisHashGet(string &hashKey, string fieldKey);  

		map<string, string> redisHashGetAll(string &hashKey);  			
};

#endif // API_REDIS_H