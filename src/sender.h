

#ifndef SENDER_H
#define SENDER_H

#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using transmitter::TransmitterRequest;
using transmitter::TransmitterReply;
using transmitter::TransmitterManager;

void RunSenderServer(AppConfig &appConfig);

void SendNullMessage(const string &msgId, const string &dst, const string &msg);

class TransmitterManagerServiceImpl final : public TransmitterManager::Service {
  Status SendMessage(ServerContext* context, const TransmitterRequest* request,
                  TransmitterReply* reply) override;
};

#endif  // SENDER_H