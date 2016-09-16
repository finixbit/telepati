

#ifndef RECEIVER_H
#define RECEIVER_H

#include <tins/tins.h>
#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"
#include "config.h"

using namespace std;
using namespace Tins;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using transmitter::TransmitterRequest;
using transmitter::TransmitterReply;
using transmitter::TransmitterManager;

class TransmitterManagerClient {
 public:
  TransmitterManagerClient();
  TransmitterManagerClient(std::shared_ptr<Channel> channel);

  void SetChannel(std::shared_ptr<Channel> channel);
  std::string SaveMessage(const TransmitterRequest request);

 private:
  std::unique_ptr<TransmitterManager::Stub> stub_;
};

bool invalidWlanPacket(const PDU& pdu);

bool processPackets(const PDU& pdu);

bool RunReceiverServer(AppConfig &appConfig);

#endif  // RECEIVER_H