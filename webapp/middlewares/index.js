var express = require('express');
var responder = require('../helpers/responder')
var app = express();
var router = express.Router();


/**
 * handle lost connections to Redis 
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var redisConnectionCheck = function(request, response, next){
  if (!request.session) {
    response
  	  .json(responder.errorResponse("Ops"));
  	return;
  }
  next() // otherwise continue 
}

/**
 * Home 
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var limitRateCheck = function(request, response, next){
  // console.log('__limitRateCheck__');
  next();
}

/**
 * Home 
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var queryApiAuthenticated = function(request, response, next){
  // console.log('__apiAuthenticated__');
  next();
}


var auth = {
	redisConnectionCheck: redisConnectionCheck,
	limitRateCheck: limitRateCheck,
  queryApiAuthenticated: queryApiAuthenticated
}

module.exports = auth;