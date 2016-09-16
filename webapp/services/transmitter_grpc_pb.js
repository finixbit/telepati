// GENERATED CODE -- DO NOT EDIT!

'use strict';
var grpc = require('grpc');
var transmitter_pb = require('./transmitter_pb.js');

function serialize_transmitter_ProfileRequest(arg) {
  if (!(arg instanceof transmitter_pb.ProfileRequest)) {
    throw new Error('Expected argument of type transmitter.ProfileRequest');
  }
  return new Buffer(arg.serializeBinary());
}

function deserialize_transmitter_ProfileRequest(buffer_arg) {
  return transmitter_pb.ProfileRequest.deserializeBinary(new Uint8Array(buffer_arg));
}

function serialize_transmitter_ProfileUsersReply(arg) {
  if (!(arg instanceof transmitter_pb.ProfileUsersReply)) {
    throw new Error('Expected argument of type transmitter.ProfileUsersReply');
  }
  return new Buffer(arg.serializeBinary());
}

function deserialize_transmitter_ProfileUsersReply(buffer_arg) {
  return transmitter_pb.ProfileUsersReply.deserializeBinary(new Uint8Array(buffer_arg));
}

function serialize_transmitter_TransmitterReply(arg) {
  if (!(arg instanceof transmitter_pb.TransmitterReply)) {
    throw new Error('Expected argument of type transmitter.TransmitterReply');
  }
  return new Buffer(arg.serializeBinary());
}

function deserialize_transmitter_TransmitterReply(buffer_arg) {
  return transmitter_pb.TransmitterReply.deserializeBinary(new Uint8Array(buffer_arg));
}

function serialize_transmitter_TransmitterRequest(arg) {
  if (!(arg instanceof transmitter_pb.TransmitterRequest)) {
    throw new Error('Expected argument of type transmitter.TransmitterRequest');
  }
  return new Buffer(arg.serializeBinary());
}

function deserialize_transmitter_TransmitterRequest(buffer_arg) {
  return transmitter_pb.TransmitterRequest.deserializeBinary(new Uint8Array(buffer_arg));
}

function serialize_transmitter_UserDataMessagesReply(arg) {
  if (!(arg instanceof transmitter_pb.UserDataMessagesReply)) {
    throw new Error('Expected argument of type transmitter.UserDataMessagesReply');
  }
  return new Buffer(arg.serializeBinary());
}

function deserialize_transmitter_UserDataMessagesReply(buffer_arg) {
  return transmitter_pb.UserDataMessagesReply.deserializeBinary(new Uint8Array(buffer_arg));
}


var TransmitterManagerService = exports.TransmitterManagerService = {
  sendMessage: {
    path: '/transmitter.TransmitterManager/SendMessage',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.TransmitterRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_TransmitterRequest,
    requestDeserialize: deserialize_transmitter_TransmitterRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  saveMessage: {
    path: '/transmitter.TransmitterManager/SaveMessage',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.TransmitterRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_TransmitterRequest,
    requestDeserialize: deserialize_transmitter_TransmitterRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  getUserProfile: {
    path: '/transmitter.TransmitterManager/GetUserProfile',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.ProfileRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_ProfileRequest,
    requestDeserialize: deserialize_transmitter_ProfileRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  setUserProfile: {
    path: '/transmitter.TransmitterManager/SetUserProfile',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.ProfileRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_ProfileRequest,
    requestDeserialize: deserialize_transmitter_ProfileRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  userProfilePing: {
    path: '/transmitter.TransmitterManager/UserProfilePing',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.ProfileRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_ProfileRequest,
    requestDeserialize: deserialize_transmitter_ProfileRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  getPingMessages: {
    path: '/transmitter.TransmitterManager/GetPingMessages',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.ProfileRequest,
    responseType: transmitter_pb.ProfileUsersReply,
    requestSerialize: serialize_transmitter_ProfileRequest,
    requestDeserialize: deserialize_transmitter_ProfileRequest,
    responseSerialize: serialize_transmitter_ProfileUsersReply,
    responseDeserialize: deserialize_transmitter_ProfileUsersReply,
  },
  sendDataMessage: {
    path: '/transmitter.TransmitterManager/SendDataMessage',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.TransmitterRequest,
    responseType: transmitter_pb.TransmitterReply,
    requestSerialize: serialize_transmitter_TransmitterRequest,
    requestDeserialize: deserialize_transmitter_TransmitterRequest,
    responseSerialize: serialize_transmitter_TransmitterReply,
    responseDeserialize: deserialize_transmitter_TransmitterReply,
  },
  getDataMessages: {
    path: '/transmitter.TransmitterManager/GetDataMessages',
    requestStream: false,
    responseStream: false,
    requestType: transmitter_pb.ProfileRequest,
    responseType: transmitter_pb.UserDataMessagesReply,
    requestSerialize: serialize_transmitter_ProfileRequest,
    requestDeserialize: deserialize_transmitter_ProfileRequest,
    responseSerialize: serialize_transmitter_UserDataMessagesReply,
    responseDeserialize: deserialize_transmitter_UserDataMessagesReply,
  },
};

exports.TransmitterManagerClient = grpc.makeGenericClientConstructor(TransmitterManagerService);
