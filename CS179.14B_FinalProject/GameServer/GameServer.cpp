#include <string>
#include <system_error>
#include <iostream>
#include <vector>
#include <chrono>
#include <array>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <atomic>
#include <boost/asio.hpp>
#include "GameMessage.h"

using namespace std;

typedef chrono::steady_clock Clock;
const auto time_beforeDC = 6.0f;

atomic<ID> id_counter(0);



struct Client{
	ID client_id;
	boost::asio::ip::udp::endpoint endpoint;
	boost::asio::ip::udp::socket &socket;
	chrono::time_point<Clock> last_packet;
	std::vector<uint8_t> send_buf;

	void send(MessageType type, const void *data, size_t length) {
		send_buf.resize(sizeof(Message) + length);
		auto p = reinterpret_cast<Message*>(send_buf.data());
		p->type = type;
		p->size = length;
		memcpy(p->data, data, length);
		cout << "Sending data of total size: " << send_buf.size() << endl;
		socket.async_send_to(boost::asio::buffer(send_buf), endpoint, [this](auto error, auto size) {
			cout << "Send success\n";
		});
	}
};

unordered_map<ID, Client> clients;

int main(int argc, char **argv) {
	int port = 8080;
	
	if (argc > 1) {
		port = strtoul(argv[1], 0, 10);
	}
	std::array<uint8_t, 1024> recv_buffer;
	try {
		boost::asio::io_service service;
		boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v6(),port);
		boost::asio::ip::udp::socket socket(service, endpoint);/*server stuff*/
		boost::asio::ip::udp::endpoint source; /*server's endpoint*/

		std::function<void(boost::system::error_code, size_t)> handler = [&](boost::system::error_code err, size_t length) {
			auto msg = reinterpret_cast<const Message*>(recv_buffer.data());
			cout << "INCOMMING" << endl;
			switch (msg->type) {
			case MessageType::Connect:{
				ID id = id_counter++;
				auto result = clients.emplace(id, Client{ id,source,socket,Clock::now()});
				result.first->second.send(MessageType::Connect, &id, sizeof(id));
				break;
			}
			case MessageType::Disconnect:
				if (msg->size == sizeof(ID)) {
					auto id = *reinterpret_cast<const ID*>(msg->data);
					auto it = clients.find(id);
					clients.erase(it);
				}
				break;
			case MessageType::Status: {
				if (msg->size < sizeof(StatusMessage)) {
					cerr << "error" << endl;
				}
				else {

					auto pm = reinterpret_cast<const StatusMessage*> (msg->data);
					cout << "Recevied Status Message from: " << pm->id << endl;
					for (auto &client : clients) {
						if (client.first != pm->id) {
							client.second.send(MessageType::Status, msg->data, msg->size);
						}
						else {
							client.second.last_packet = Clock::now();
							client.second.endpoint = source;
						}
					}
				}
				}
				break;
			case MessageType::Attack: {
				if (msg->size < sizeof(AttackMessage)) {
					cerr << "error" << endl;
				}
				else {

					auto pm = reinterpret_cast<const AttackMessage*> (msg->data);
					cout << "Recevied Attack Message from: " << pm->id << endl;
					for (auto &client : clients) {
						if (client.first != pm->id) {
							client.second.send(MessageType::Attack, msg->data, msg->size);
						}
						else {
							client.second.last_packet = Clock::now();
							client.second.endpoint = source;
						}
					}
				}
			}
			}
			socket.async_receive_from(boost::asio::buffer(recv_buffer.data(), recv_buffer.size()), source, handler);
		};

		socket.async_receive_from(boost::asio::buffer(recv_buffer.data(), recv_buffer.size()), source, handler);
		service.run();
	}
	catch (exception e) {
		cerr << e.what() << endl;
	}

	return 0;
}