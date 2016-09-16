
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <tins/tins.h>
#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"
#include "config.h"
#include "sender.h"

using namespace std;
using namespace Tins;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

string interfaceName;

Status TransmitterManagerServiceImpl::SendMessage(ServerContext* context, 
  const TransmitterRequest* request, TransmitterReply* reply) {
    
  std::ostringstream stream;
  request->SerializeToOstream(&stream);
  const string data = stream.str();

  SendNullMessage(request->messageid(), request->recipientname(), data);
  reply->set_response(true);
  reply->set_data("OK");
  return Status::OK;
}

void RunSenderServer(AppConfig &appConfig){
  std::string server_address(appConfig.senderGrpcServer);
  TransmitterManagerServiceImpl service;

  interfaceName = appConfig.interface;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Sender.Server: " << server_address << std::endl;
  server->Wait();
}

void SendNullMessage(const string &msgId, const string &dst, const string &msg){
  
  PacketSender sender;
  NetworkInterface interface(interfaceName);
  const string rawMessage(msg);

  try{ 

    RadioTap pkt =  RadioTap() / 
                    Dot11Data("00:00:00:00:00:00", "00:00:00:00:00:00") /
                    SNAP() /
                    IP() /
                    TCP() /
                    RawPDU(rawMessage);

    sender.send(pkt, interface);
    cout << "Sender Message: " << msg << " size: " << msg.size() << endl;
  }
  catch(...){
    cout << "Error Sender Packet ..." << endl;
  }
}
