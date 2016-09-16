
#ifndef CORE_H
#define CORE_H

#include <tins/tins.h>
#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"
#include "config.h"
#include "redis.h"

using namespace std;
using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;
using transmitter::TransmitterRequest;
using transmitter::TransmitterReply;
using transmitter::TransmitterManager;
using transmitter::ProfileRequest;
using transmitter::ProfileUsersReply;
using transmitter::UserDataMessagesReply;


void RunCoreServer(AppConfig &appConfig);
vector<string> split(string &text, char &sep);
void SavePingMessage(const TransmitterRequest *request);
void SaveDataMessage(const TransmitterRequest *request);


class CoreTransmitterManagerServiceImpl final : public TransmitterManager::Service {
  Status SaveMessage(ServerContext* context, const TransmitterRequest* request,
                  TransmitterReply* reply) override;

  Status SetUserProfile(ServerContext* context, const ProfileRequest* request,
                  TransmitterReply* reply) override;

  Status GetUserProfile(ServerContext* context, const ProfileRequest* request,
                  TransmitterReply* reply) override;

  Status UserProfilePing(ServerContext* context, const ProfileRequest* request,
                  TransmitterReply* reply) override;

  Status GetPingMessages(ServerContext* context, const ProfileRequest* request,
                  ProfileUsersReply* reply) override;

  Status SendDataMessage(ServerContext* context, const TransmitterRequest* request,
                  TransmitterReply* reply) override;

  Status GetDataMessages(ServerContext* context, const ProfileRequest* request,
                  UserDataMessagesReply* reply) override;
};


class CoreTransmitterManagerClient {
 public:
  CoreTransmitterManagerClient();
  CoreTransmitterManagerClient(std::shared_ptr<Channel> channel);

  void SetChannel(std::shared_ptr<Channel> channel);
  std::string SendMessage(const TransmitterRequest request);

 private:
  std::unique_ptr<TransmitterManager::Stub> stub_;
};

#endif