#pragma once

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>

// hints to link libs
#pragma comment(lib, "WS2_32.Lib")
#pragma comment(lib, "MsWSock.lib")
#pragma comment(lib, "AdvApi32.lib")

#include <sstream>
#include <mutex>
#include <vector>
#include <queue>

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

	class Client
	{
	public:
		std::mutex mutex;

		bool closed = false;

		SOCKET socket;
		sockaddr address;
		int addressLength = sizeof(sockaddr);

		std::vector<Message> receivedMessages;

		std::stringstream receivedBuffer;

		std::queue<Message> sendMessages;

		bool isClosed();
		void close();
		void receive(const char* bytes, int32_t count);
		void receiveNewMessage();

		void send(Message&& message);

		Client();
	};

	namespace RAWBYTES
	{
		constexpr char START = '\0';
		constexpr char CONTINUE = '\1';
	}


	class Network
	{
	public:
		std::string PORT;
		WSADATA wsaData;

		std::mutex mutex;
		std::vector<std::unique_ptr<Client>> clients;

		SOCKET listenSocket = INVALID_SOCKET;

		Network();

		bool initialize(int32_t portNumber);

		bool run();
	};
}
