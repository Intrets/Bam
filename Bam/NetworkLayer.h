#pragma once

#include <sstream>
#include <mutex>
#include <vector>
#include <queue>
#include <memory>

namespace NETWORK
{
	namespace MESSAGE
	{
		enum class TYPE
		{

		};
	}

	struct Message
	{
		MESSAGE::TYPE type;
		std::stringstream buffer;
	};

	class ClientHidden;
	class Client
	{
	public:
		std::unique_ptr<ClientHidden> hidden;

		std::mutex mutex;

		bool closed = false;

		std::vector<Message> receivedMessages;

		std::stringstream receivedBuffer;

		std::queue<Message> sendMessages;

		bool isClosed();
		void close();
		void receive(const char* bytes, int32_t count);
		void receiveNewMessage();

		void send(Message&& message);

		Client();
		~Client();
	};

	namespace RAWBYTES
	{
		constexpr char START = '\0';
		constexpr char CONTINUE = '\1';
	}

	class NetworkHidden;
	class Network
	{
	public:
		std::unique_ptr<NetworkHidden> hidden;

		std::string port;
		std::string address;

		std::mutex mutex;
		std::vector<std::unique_ptr<Client>> clients;

		bool initializeServer(int32_t portNumber);
		bool initializeClient(std::string address, int32_t portNumber);

		bool run();

		Network();
		~Network();
	};
}
