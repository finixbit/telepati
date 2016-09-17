/*
*
* MIT License
*
* Copyright (c) 2016 Intelburg
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
*/

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