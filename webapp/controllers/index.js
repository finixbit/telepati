var express = require('express')
	  , responder = require('../helpers/responder')
  	, router = express.Router();


/**
 * Home page
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var homePage = function(request, response) {
	response.render('index');
};

/**
 * Setup page
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var setupPage = function(request, response) {
  response.render('setup');
};


router.get('/', homePage);
router.get('/setup', setupPage);
router.use('/api', require("./api"));
module.exports = router