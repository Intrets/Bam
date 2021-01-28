#pragma once

#include <sstream>
#include <mutex>
#include <vector>
#include <queue>
#include <memory>
#include <list>

namespace NETWORK
{
	class Message
	{
	public:
		std::stringstream buffer;

		void restore();

		Message() = default;
		~Message() = default;

		Message(Message& other);
		Message& operator=(Message& other);

		Message(Message&& other);
		Message& operator=(Message&& other);
	};

	constexpr auto BUFFER_SIZE = 1024;

	class ClientHidden;
	class Client
	{
	public:
		std::unique_ptr<ClientHidden> hidden;

		bool closed = false;

		std::vector<Message> receivedMessages;

		std::stringstream receivedBuffer;

		std::stringstream receivedPartialSize;
		int32_t receivedPartialSizeCount = 0;

		int32_t messageRemaining = 0;

		std::queue<Message> sendMessages;

		bool isClosed();
		void close();
		void receive(const char* bytes, int32_t count);
		void receiveNewMessage();
		void cycleMessage();

		void ingestBuffer(std::array<char, BUFFER_SIZE>& buffer, int32_t size);

		void send(Message message);

		Client();
		~Client();
	};

	class NetworkHidden;
	class Network
	{
	public:
		std::unique_ptr<NetworkHidden> hidden;

		std::string port;
		std::string address;

		std::mutex mutex;
		std::list<std::unique_ptr<Client>> clients;

		bool initializeServer(int32_t portNumber);
		bool initializeClient(std::string address, int32_t portNumber);

		bool run();

		Network();
		~Network();
	};
}
