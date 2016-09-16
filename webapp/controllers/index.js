var express = require('express')
		, grpc = require('grpc')
	  , responder = require('../helpers/responder')
    , messages = require('../services/transmitter_pb')
    , services = require('../services/transmitter_grpc_pb')
  	, router = express.Router()
 		, client = new services.TransmitterManagerClient(
                  'localhost:50052', grpc.credentials.createInsecure());

/**
 * Set Profile Info
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var setUserProfile = function(request, response) {
	request
    .checkBody('name', 'Invalid Name')
    .notEmpty();

  var errors = request.validationErrors();
  if (errors) {
    response
      .status(200)
      .json(responder.errorResponse("Invalid Param", errors))
      .end();
    return;
  }

  var profileRequest = new messages.ProfileRequest();
  profileRequest.setName(request.body.name);

  client
    .setUserProfile(profileRequest , function(err, profileResponse) {
      if((!err) && (profileResponse.getResponse())){
        response
          .json(responder.successResponse("ok"))
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Get Profile Info
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var getUserProfile = function(request, response) {
	var profileRequest = new messages.ProfileRequest();

  client
    .getUserProfile(profileRequest , function(err, profileResponse) {
      if((!err) && (profileResponse.getResponse())){
        response
          .json(responder.successResponse(profileResponse.getData()))
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Send Profile Info and keys to Other Users
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var userProfilePing = function(request, response) {
  request
    .checkBody('name', 'Invalid Name')
    .notEmpty();
  request
    .checkBody('public_key', 'Invalid Key')
    .notEmpty();

  var errors = request.validationErrors();
  if (errors) {
    response
      .status(200)
      .json(responder.errorResponse("Invalid Param", errors))
      .end();
    return;
  }

	var profileRequest = new messages.ProfileRequest();
  profileRequest.setName(request.body.name);
  profileRequest.setPublickey(request.body.public_key);

  client
    .userProfilePing(profileRequest , function(err, profileResponse) {
      if((!err) && (profileResponse.getResponse())){
        response
          .json(responder.successResponse("ok"))
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Get Encryption Keys
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var getProfileKeys = function(request, response) {
	response
    .json(responder.successResponse("ok"))
    .end();
}

/**
 * Create Encryption Keys
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var setProfileKeys = function(request, response) {
	response
    .json(responder.successResponse("ok"))
    .end();
}

/**
 * Get User Profiles
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var getUsers = function(request, response) {
	var profileRequest = new messages.ProfileRequest();

  client
    .getPingMessages(profileRequest , function(err, profileUsers) {
      if(!err){
        response
          .json(profileUsers.toObject())
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Get User Messages
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var getUserMessages = function(request, response) {
  var profileRequest = new messages.ProfileRequest();
  profileRequest.setName(request.body.name);

  client
    .getDataMessages(profileRequest , function(err, userMessages) {
      if(!err){
        response
          .json(userMessages.toObject())
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Send User Message
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var sendUserMessage = function(request, response) {
  request
    .checkBody('name', 'Invalid Name')
    .notEmpty();
  request
    .checkBody('public_key', 'Invalid Key')
    .notEmpty();
  request
    .checkBody('message', 'Invalid Message')
    .notEmpty();
  request
    .checkBody('recipient', 'Invalid Recipient')
    .notEmpty();

  var errors = request.validationErrors();
  if (errors) {
    response
      .status(200)
      .json(responder.errorResponse("Invalid Param", errors))
      .end();
    return;
  }

  var dataRequest = new messages.TransmitterRequest();
  dataRequest.setSendername(request.body.name);
  dataRequest.setSenderkey(request.body.public_key);
  dataRequest.setRecipientname(request.body.recipient);
  dataRequest.setMessagedata(request.body.message);
  dataRequest.setMessagetype("data");
  dataRequest.setMessagedatatype("text");

  client
    .sendDataMessage(dataRequest , function(err, dataResponse) {
      if((!err) && (dataResponse.getResponse())){
        response
          .json(responder.successResponse("ok"))
          .end();
        return;
      } else {
        response
          .json(responder.errorResponse("Ops"))
          .end();
        return;
      }
    });
}

/**
 * Home page
 * @param  {Object} request  HTTP request data
 * @param  {Object} response HTTP response
 * @return {Object}          Response
 */
var homePage = function(request, response) {
	response.render('index');
};


router.post('/api/profile', setUserProfile);
router.get('/api/profile', getUserProfile);
router.post('/api/ping', userProfilePing);
router.get('/api/users', getUsers);
router.get('/api/keys', getProfileKeys);
router.post('/api/keys', setProfileKeys);
router.post('/api/messages', getUserMessages);
router.post('/api/messages/send', sendUserMessage);
router.get('/', homePage);
module.exports = router