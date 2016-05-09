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

#include "../GameMessage.h"

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

	void send(Message* message) {
		send_buf.resize(sizeof(Message) + message->size);
		memcpy(send_buf.data(), message, send_buf.size());
		// cout << "Sending data of total size: " << send_buf.size() << endl;
		socket.async_send_to(boost::asio::buffer(send_buf), endpoint, [this](auto error, auto size) {
			// cout << "Send success\n";
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
			auto message = reinterpret_cast<Message*>(recv_buffer.data());
			// cout << "INCOMING" << endl;
			switch (message->message_type) {
				case MessageType::Connect: {
					ID id = id_counter++;
					message->origin = id;
					std::cout << "Connecting. Client id: " << id << std::endl;
					auto result = clients.emplace(id, Client{ id, source, socket,Clock::now() });
					result.first->second.send(message);
					break;
				}
				case MessageType::Disconnect: {
					if (message->size == sizeof(ID)) {
						auto id = *reinterpret_cast<const ID*>(message->data);
						auto it = clients.find(id);
						clients.erase(it);
					}
					break;
				}
				case MessageType::Broadcast: {
					// cout << "Recevied Status Message from: " << pm->id << endl;
					for (auto &client : clients) {
						if (client.first != message->origin) {
							client.second.send(message);
						} else {
							client.second.last_packet = Clock::now();
							client.second.endpoint = source;
						}
					}
					break;
				}
			}
			socket.async_receive_from(boost::asio::buffer(recv_buffer.data(), recv_buffer.size()), source, handler);
		};

		socket.async_receive_from(boost::asio::buffer(recv_buffer.data(), recv_buffer.size()), source, handler);
		service.run();
	} catch (exception e) {
		cerr << e.what() << endl;
	}

	return 0;
}
