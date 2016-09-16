#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <tins/tins.h>
#include <grpc++/grpc++.h>
#include "transmitter.grpc.pb.h"
#include "config.h"
#include "receiver.h"

/*  
wlan frame types 
*/
#define MGMT_FRAME "management"
#define CTRL_FRAME "control"
#define DATA_FRAME "data"

using namespace std;
using namespace Tins;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using transmitter::TransmitterRequest;
using transmitter::TransmitterReply;
using transmitter::TransmitterManager;

TransmitterRequest request;
TransmitterManagerClient transmittermanager;

TransmitterManagerClient::TransmitterManagerClient(){
}

TransmitterManagerClient::TransmitterManagerClient(std::shared_ptr<Channel> channel)
  : stub_(TransmitterManager::NewStub(channel)){
};

void TransmitterManagerClient::SetChannel(std::shared_ptr<Channel> channel) {
  stub_ = TransmitterManager::NewStub(channel);
}

std::string TransmitterManagerClient::SaveMessage(const TransmitterRequest request) {
    TransmitterReply reply;
    ClientContext context;
    Status status = stub_->SaveMessage(&context, request, &reply);

    if (status.ok()) {
      return reply.data();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
}

bool invalidWlanPacket(const PDU& pdu){
	if(pdu.pdu_type() != PDU::PDUType::RADIOTAP)
		return true;
	return false;
}

bool processPackets(const PDU& pdu){	
	if(invalidWlanPacket(pdu))  
		return true;

	try{
		const RadioTap &frame = pdu.rfind_pdu<RadioTap>();
		const Dot11Data &data_pdu = frame.rfind_pdu<Dot11Data>();
		const SNAP &snap_pdu = data_pdu.rfind_pdu<SNAP>();
		const IP &ip_pdu = snap_pdu.rfind_pdu<IP>();
		const TCP &tcp_pdu = ip_pdu.rfind_pdu<TCP>();
		const RawPDU &raw_pdu = tcp_pdu.rfind_pdu<RawPDU>();

		const RawPDU::payload_type& payload = raw_pdu.payload();
		string msg(payload.begin(), payload.end());

		if(request.ParseFromString(msg)){
			cout << endl << "Receiver Message: " << msg << " size: " << msg.size() << endl;
			transmittermanager.SaveMessage(request);
		}
	}catch(...){
		cout << "."; //Error Decoding Data/Invalid Data 
		return true;
	}
	
	return true;
}

bool RunReceiverServer(AppConfig &appConfig){
	transmittermanager.SetChannel(grpc::CreateChannel(
    appConfig.coreGrpcServer, grpc::InsecureChannelCredentials()));

	Sniffer sniffer(appConfig.interface, Sniffer::PROMISC);
	sniffer.set_filter(appConfig.receiverFilter);
	sniffer.sniff_loop(processPackets);
	return true;
}

