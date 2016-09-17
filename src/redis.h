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


#ifndef API_REDIS_H
#define API_REDIS_H

#include <cstdint> /* uint32_t */
#include <string>
#include <map>
#include <hiredis/hiredis.h> /* redisContext */

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