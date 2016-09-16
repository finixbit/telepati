
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"
#include "config.h"
#include "redis.h"
#include "core.h"

using namespace std;
using namespace Tins;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using transmitter::TransmitterRequest;
using transmitter::TransmitterReply;
using transmitter::TransmitterManager;
using transmitter::ProfileUsersReply;
using transmitter::UserDataMessagesReply;

LightRedis redisCli;
CoreTransmitterManagerClient coreTransmitterManager;

Status CoreTransmitterManagerServiceImpl::SaveMessage(ServerContext* context, 
	const TransmitterRequest* request, TransmitterReply* reply) {

  if(request->messagetype() == "ping"){
  	SavePingMessage(request);
  }

  if(request->messagetype() == "data"){
    SaveDataMessage(request);
  }
  reply->set_data("OK");
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::SetUserProfile(ServerContext* context, 
 	const ProfileRequest* request, TransmitterReply* reply) {

  if(request->name().empty()){
  	reply->set_data("Ops");
  	return Status::OK;
  }

  string hashKey("profile");
  string fieldKey("name");
  bool res = redisCli.redisHashSet(hashKey, fieldKey, request->name());
  if(!res){
  	cout << "redis err" << endl;
  	reply->set_data("Ops");
  	return Status::OK;
  }

  reply->set_response(true);
  reply->set_data("OK");
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::GetUserProfile(ServerContext* context, 
	const ProfileRequest* request, TransmitterReply* reply) {
    
 	string hashKey("profile");
  string fieldKey("name");
  string res = redisCli.redisHashGet(hashKey, fieldKey);
  if(res == string(CACHE_INVALID_PARAM)){
  	reply->set_data("Ops");
  	return Status::OK;
  }

	reply->set_response(true);
  reply->set_data(res);
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::UserProfilePing(ServerContext* context, 
	const ProfileRequest* request, TransmitterReply* reply) {
  
  if((request->name().empty()) || (request->publickey().empty())){
  	reply->set_data("Ops");
  	return Status::OK;
  }

  TransmitterRequest msgRequest;
  msgRequest.set_magicnumber("telepati");
  msgRequest.set_messageid(AppConfig::generateRandomString(10));
  msgRequest.set_messagetype("ping");
  msgRequest.set_messagedatatype("ping");
  msgRequest.set_messagetimestamp(AppConfig::getDateTime());
  msgRequest.set_sendername(request->name());
  msgRequest.set_senderkey(request->publickey());
  msgRequest.set_recipientname("any");
  msgRequest.set_messagedata("helloworld");
  coreTransmitterManager.SendMessage(msgRequest);

  cout << "Sented Ping Message to Sender" << endl;
  reply->set_response(true);
  reply->set_data("OK");
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::GetPingMessages(ServerContext* context, 
  const ProfileRequest* request, ProfileUsersReply* reply) {
  
  string hashKey("users");
  map<string, string> data = redisCli.redisHashGetAll(hashKey);

  for(map<string, string>::iterator iter(data.begin()); iter != data.end(); ++iter){
    char sep = ',';
    vector<string> info(split(iter->second, sep));

    ProfileRequest* userProfile = reply->add_users();
    userProfile->set_name(iter->first);
    userProfile->set_publickey(info.at(0));
    userProfile->set_lasttimestamp(info.at(1));
  }
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::SendDataMessage(ServerContext* context, 
  const TransmitterRequest* request, TransmitterReply* reply) {

  TransmitterRequest msgRequest;
  msgRequest.set_magicnumber("telepati");
  msgRequest.set_messageid(AppConfig::generateRandomString(10));
  msgRequest.set_messagetype(request->messagetype());
  msgRequest.set_messagedatatype(request->messagedatatype());
  msgRequest.set_messagetimestamp(AppConfig::getDateTime());
  msgRequest.set_sendername(request->sendername());
  msgRequest.set_senderkey(request->senderkey());
  msgRequest.set_recipientname(request->recipientname());
  msgRequest.set_messagedata(request->messagedata());
  coreTransmitterManager.SendMessage(msgRequest);

  cout << "Sented Data Message to Sender" << endl;
  reply->set_response(true);
  reply->set_data("OK");
  return Status::OK;
}

Status CoreTransmitterManagerServiceImpl::GetDataMessages(ServerContext* context, 
  const ProfileRequest* request, UserDataMessagesReply* reply) {
  
  string hashKey("username:"+request->name());
  map<string, string> data = redisCli.redisHashGetAll(hashKey);

  for(map<string, string>::iterator iter(data.begin()); iter != data.end(); ++iter){
    TransmitterRequest dummpDataMessage;

    if(dummpDataMessage.ParseFromString(iter->second)){
      TransmitterRequest* dataMessage = reply->add_messages();
      dataMessage->ParseFromString(iter->second);
    }
  }
  return Status::OK;
}


CoreTransmitterManagerClient::CoreTransmitterManagerClient(){
}

CoreTransmitterManagerClient::CoreTransmitterManagerClient(std::shared_ptr<Channel> channel)
  : stub_(TransmitterManager::NewStub(channel)){
};

void CoreTransmitterManagerClient::SetChannel(std::shared_ptr<Channel> channel) {
  stub_ = TransmitterManager::NewStub(channel);
}

std::string CoreTransmitterManagerClient::SendMessage(const TransmitterRequest request) {
  TransmitterReply reply;
  ClientContext context;
  Status status = stub_->SendMessage(&context, request, &reply);

  if (status.ok()) {
    return reply.data();
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }
}

void RunCoreServer(AppConfig &appConfig) {
  std::string server_address(appConfig.coreGrpcServer);
  CoreTransmitterManagerServiceImpl service;

  redisCli.init(appConfig.redisHost, appConfig.redisPort);

  coreTransmitterManager.SetChannel(grpc::CreateChannel(
    appConfig.senderGrpcServer, grpc::InsecureChannelCredentials()));

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Core.Server: " << server_address << std::endl;
  server->Wait();
}

vector<string> split(string &text, char &sep) {
  vector<string> tokens;
  size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

void SavePingMessage(const TransmitterRequest *request){
  string hashKey("users");
  string fieldKey(request->sendername());

  std::ostringstream oss;
  oss << request->senderkey() << "," << request->messagetimestamp(); 

  std::string fieldValue = oss.str();
  bool res = redisCli.redisHashSet(hashKey, fieldKey, fieldValue);
  if(!res){
    cout << "Error @SavePingMessage: redis" << endl;
  } else{
    cout << "Saved UserProfile: " << request->sendername() << endl;
  }
}

void SaveDataMessage(const TransmitterRequest *request){
  std::ostringstream stream;
  request->SerializeToOstream(&stream);
  const string data = stream.str();

  string hashKey("profile");
  string fieldKey("name");
  string res = redisCli.redisHashGet(hashKey, fieldKey);
  if(res == string(CACHE_INVALID_PARAM))
    return;
  
  if(res != request->recipientname()){
    return;
  }
  
  hashKey = "username:"+request->sendername();
  fieldKey = request->messageid();
  bool response = redisCli.redisHashSet(hashKey, fieldKey, data);
  if(!response){
    cout << "Error @SaveDataMessage: redis" << endl;
  } else{
    cout << "Saved DataMessage From: " << request->sendername() << endl;
  }
}

