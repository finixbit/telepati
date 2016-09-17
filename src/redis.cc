/*
*
*  MIT License
*
*  Copyright (c) 2016 Intelburg
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*
*
*/

#include <iostream>
#include <map>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
#include <sstream>
#include <hiredis/hiredis.h>
#include "redis.h"

using namespace std;

LightRedis::LightRedis() {}

LightRedis::LightRedis(string &redisHost, uint32_t &redisPort):
	m_redis_host(redisHost),
	m_redis_port(redisPort) {

	if (connectToRedis())
		m_redis_conn_is_valid = true;
	else
		m_redis_conn_is_valid = false;
}

LightRedis::~LightRedis(void)
{
}

void LightRedis::init(string &redisHost, uint32_t &redisPort) {
	m_redis_host = redisHost;
	m_redis_port = redisPort;

	if (connectToRedis())
		m_redis_conn_is_valid = true;
	else
		m_redis_conn_is_valid = false;
}

bool LightRedis::redisConnIsInvalid() {
	if (m_redis_conn_is_valid)
		return false;

	return true;
}

bool LightRedis::connectToRedis() {
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds

	m_redis_conn = redisConnectWithTimeout(
	                 m_redis_host.c_str(),
	                 m_redis_port,
	                 timeout
	               );

	if (m_redis_conn == NULL || m_redis_conn->err) {

		if (m_redis_conn) {
			cout << "REDIS ERROR: Connection " << endl;
			redisFree(m_redis_conn);
		} else {
			cout << "REDIS ERROR: can't allocate redis context" << endl;
		}
		return false;
	}
	cout << "connected to redis ... " << m_redis_host << ":" << m_redis_port << endl;
	return true;
}

bool LightRedis::redisSet(string &redisKey, string &redisValue) {
	try
	{
		redisReply *redisReply_t;

		redisReply_t = (redisReply*)redisCommand(m_redis_conn,
		               "SET %s %s",
		               redisKey.c_str(),
		               redisValue.c_str()
		                                        );

		freeReplyObject(redisReply_t);
	}
	catch (...)
	{
		cout << "Redis, SET Error" << endl;
		return false;
	}
	return true;
}

string LightRedis::redisGet(string &redisKey) {
	string redisValue = string(CACHE_INVALID_PARAM);
	try
	{
		redisReply    *redisReply_t;

		redisReply_t = (redisReply*)redisCommand(m_redis_conn,
		               "GET %s",
		               redisKey.c_str()
		                                        );

		redisValue = string(redisReply_t->str);

		freeReplyObject(redisReply_t);
	}
	catch (...)
	{
		cout << "Redis, GET Error" << endl;
		return redisValue;
	}
	return redisValue;
}

bool LightRedis::redisHashSet(string &hashKey, string fieldKey, string fieldValue) {
	try {
		redisReply    *redisReply_t;
		redisReply_t = (redisReply*)redisCommand(m_redis_conn,
		               "HSET %s %s %s",
		               hashKey.c_str(),
		               fieldKey.c_str(),
		               fieldValue.c_str()
		                                        );

		freeReplyObject(redisReply_t);
	}
	catch (...) {
		cout << "Redis, HashSET Error" << endl;
		return false;
	}
	return true;
}

string LightRedis::redisHashGet(string &hashKey, string fieldKey)
{
	string redisValue = string(CACHE_INVALID_PARAM);
	try
	{
		redisReply *redisReply_t;

		redisReply_t = (redisReply*)redisCommand(m_redis_conn,
		               "HGET %s %s",
		               hashKey.c_str(),
		               fieldKey.c_str()
		                                        );

		redisValue = string(redisReply_t->str);

		freeReplyObject(redisReply_t);
	}
	catch (...)
	{
		cout << "Redis, HashGET Error" << endl;
		return redisValue;
	}
	return redisValue;
}

map<string, string> LightRedis::redisHashGetAll(string &hashKey)
{
	std::map<string, string> data;
	try
	{
		redisReply *reply = (redisReply*)redisCommand(m_redis_conn, "HGETALL %s", hashKey.c_str());

		if ( reply->type == REDIS_REPLY_ERROR ) {
			printf( "Error: %s\n", reply->str );

		} else if ( reply->type != REDIS_REPLY_ARRAY ) {
			printf( "Unexpected type: %d\n", reply->type );

		} else {
			int i;
			for (i = 0; i < reply->elements; i = i + 2 ) {
				//printf( "Result: %s = %s \n", reply->element[i]->str, reply->element[i + 1]->str );
				data[string(reply->element[i]->str)] = string(reply->element[i + 1]->str);
			}
		}

		freeReplyObject(reply);
		return data;
	}
	catch (...)
	{
		cout << "Redis, HashGETAll Error" << endl;
		return data;
	}
}


