#include "common.h"

#include "NetworkLayer.h"

#include <stdio.h>
#include <cassert>

#undef APIENTRY
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

// hints to link libs
#pragma comment(lib, "WS2_32.Lib")
#pragma comment(lib, "MsWSock.lib")
#pragma comment(lib, "AdvApi32.lib")

constexpr auto BUFFER_SIZE = 1024;

#include <string>
#include <iostream>

namespace NETWORK
{
	class ClientHidden
	{
	public:
		SOCKET socket;
	};

	class NetworkHidden
	{
	public:
		WSADATA wsaData;
		SOCKET listenSocket = INVALID_SOCKET;
	};

	bool Client::isClosed() {
		std::lock_guard<std::mutex> lock(this->mutex);
		return this->closed;
	}

	void Client::close() {
		std::lock_guard<std::mutex> lock(this->mutex);
		this->closed = true;
	}

	void Client::receive(const char* bytes, int32_t count) {
		std::lock_guard<std::mutex> lock(this->mutex);

		this->receivedBuffer.write(bytes, count);
	}

	void Client::receiveNewMessage() {
		std::lock_guard<std::mutex> lock(this->mutex);

		int32_t receivedBufferSize = static_cast<int32_t>(this->receivedBuffer.tellp()) - static_cast<int32_t>(this->receivedBuffer.tellg());

		int32_t startPos1 = static_cast<int32_t>(this->receivedBuffer.tellg());
		int32_t endPos1 = static_cast<int32_t>(this->receivedBuffer.tellp());

		if (receivedBufferSize >= 4) {
			int32_t startPos = static_cast<int32_t>(this->receivedBuffer.tellg());
			int32_t endPos = static_cast<int32_t>(this->receivedBuffer.tellp());

			this->receivedMessages.emplace_back();
			//this->receivedMessages.back().type = ;
			this->receivedMessages.back().buffer = std::move(this->receivedBuffer);
		}
		else {
			this->receivedBuffer.clear();
		}
	}

	void Client::send(Message&& message) {
		std::lock_guard<std::mutex> lock(this->mutex);
		this->sendUnlocked(std::move(message));
	}

	void Client::sendUnlocked(Message&& message) {
		this->sendMessages.push(std::move(message));
	}

	Client::Client() {
		this->hidden = std::make_unique<ClientHidden>();
	}

	Client::~Client() {
	}

	Network::Network() {
		this->hidden = std::make_unique<NetworkHidden>();
	}

	Network::~Network() {
	}

	bool Network::initializeServer(int32_t portNumber) {
		std::lock_guard<std::mutex> guard(this->mutex);
		portNumber = portNumber > 0 ? portNumber : 27015;
		portNumber = portNumber < 65536 ? portNumber : 27015;

		this->port = std::to_string(portNumber);

		std::cout << "opening server on port: " << this->port << '\n';

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &this->hidden->wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return false;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return false;
		}

		// Create a SOCKET for connecting to server
		this->hidden->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (this->hidden->listenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Setup the TCP listening socket
		iResult = bind(this->hidden->listenSocket, result->ai_addr, (int) result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(this->hidden->listenSocket);
			WSACleanup();
			return false;
		}

		freeaddrinfo(result);

		iResult = listen(this->hidden->listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(this->hidden->listenSocket);
			WSACleanup();
			return false;
		}

		printf("Successfully opened server!\n\n\n");

		return true;
	}

	bool Network::initializeClient(std::string address_, int32_t portNumber) {
		std::lock_guard<std::mutex> guard(this->mutex);

		portNumber = portNumber > 0 ? portNumber : 27015;
		portNumber = portNumber < 65536 ? portNumber : 27015;

		this->port = std::to_string(portNumber);
		this->address = address_;

		std::cout << "connecting to server at:" << address_ << ":" << port << '\n';

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &this->hidden->wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return false;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(this->address.c_str(), port.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return false;
		}

		auto serverClient = std::make_unique<Client>();
		serverClient->hidden->socket = INVALID_SOCKET;

		// Attempt to connect to an address until one succeeds
		for (auto ptr = result; ptr != NULL; ptr = ptr->ai_next) {
			// Create a SOCKET for connecting to server
			serverClient->hidden->socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (serverClient->hidden->socket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return false;
			}

			// Connect to server.
			iResult = connect(serverClient->hidden->socket, ptr->ai_addr, (int) ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(serverClient->hidden->socket);
				serverClient->hidden->socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (serverClient->hidden->socket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return false;
		}

		this->clients.push_back(std::move(serverClient));

		printf("Successfully connected to server\n\n\n");

		return true;
	}

	bool Network::run() {
		timeval interval;
		interval.tv_sec = 1;
		interval.tv_usec = 1;

		while (true) {
			fd_set readFDs;
			fd_set writeFDs;
			fd_set exceptFDs;

			FD_ZERO(&readFDs);
			FD_ZERO(&writeFDs);
			FD_ZERO(&exceptFDs);

			if (this->hidden->listenSocket != INVALID_SOCKET) {
				FD_SET(this->hidden->listenSocket, &readFDs);
				FD_SET(this->hidden->listenSocket, &exceptFDs);
			}

			{
				std::lock_guard<std::mutex> guard(this->mutex);
				for (auto& client : this->clients) {
					if (!client->closed) {
						FD_SET(client->hidden->socket, &readFDs);
						if (!client->sendMessages.empty()) {
							FD_SET(client->hidden->socket, &writeFDs);
						}
					}
				}
			}

			// first parameter compatibility with Berkeley sockets
			// last parameter: timeout duration, 0 for blocking
			if (select(0, &readFDs, &writeFDs, &exceptFDs, &interval) >= 0) {
				std::lock_guard<std::mutex> guard(this->mutex);
				printf("\nsomething happen\n");

				if (FD_ISSET(this->hidden->listenSocket, &readFDs)) {
					printf("listen socket read\n");
					sockaddr newClientAdress;
					int newClientAddressLength = sizeof(sockaddr);
					auto newClientSocket = accept(this->hidden->listenSocket, &newClientAdress, &newClientAddressLength);

					if (newClientSocket != INVALID_SOCKET) {
						this->clients.emplace_back(std::make_unique<Client>());
						this->clients.back()->hidden->socket = newClientSocket;
						printf("new client\n");
					}
				}

				for (auto& client : this->clients) {
					if (FD_ISSET(client->hidden->socket, &exceptFDs)) {
						client->close();
					}
					else {
						if (FD_ISSET(client->hidden->socket, &readFDs)) {
							printf("client socket read\n");
							char buffer[BUFFER_SIZE];
							int bytesReceived = recv(client->hidden->socket, buffer, BUFFER_SIZE, 0);
							// connection closed
							if (bytesReceived == 0) {
								client->close();
							}
							else if (bytesReceived == SOCKET_ERROR) {
								client->close();
							}
							else if (bytesReceived > 0) {
								std::string m(buffer + 1, bytesReceived - 1);
								printf("some message: %s\n", m.c_str());

								switch (buffer[0]) {
									case RAWBYTES::START:
										client->receiveNewMessage();
										client->receive(buffer + 1, bytesReceived - 1);
										break;
									case RAWBYTES::CONTINUE:
										client->receive(buffer + 1, bytesReceived - 1);
										break;
									default:
										client->close();
										assert(0);
										break;
								}
							}
							else {
								client->close();
								assert(0);
							}
						}

						if (FD_ISSET(client->hidden->socket, &writeFDs)) {
							printf("client socket write\n");
							std::lock_guard<std::mutex> lock(client->mutex);
							if (!client->sendMessages.empty()) {
								auto& message = client->sendMessages.front();

								int32_t startPos = static_cast<int32_t>(message.buffer.tellg());
								int32_t endPos = static_cast<int32_t>(message.buffer.tellp());

								int32_t size = endPos - startPos;

								if (size < 1) {
									client->sendMessages.pop();
								}
								else {
									char sendBuffer[BUFFER_SIZE];
									if (startPos == 0) {
										sendBuffer[0] = RAWBYTES::START;
									}
									else {
										sendBuffer[1] = RAWBYTES::CONTINUE;
									}

									int32_t sendLength;
									int32_t headerLength = sizeof(RAWBYTES::CONTINUE);

									if (size + headerLength <= BUFFER_SIZE) {
										sendLength = size + headerLength;
									}
									else {
										sendLength = BUFFER_SIZE;
									}

									message.buffer.read(sendBuffer + headerLength, sendLength - headerLength);

									int32_t bytesSend = send(client->hidden->socket, sendBuffer, sendLength, 0);

									std::string m(sendBuffer + 1, bytesSend - 1);
									printf("some message: %s\n", m.c_str());

									if (bytesSend == SOCKET_ERROR) {
										int32_t error;
										int32_t errorLength = sizeof(error);
										getsockopt(client->hidden->socket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &errorLength);

										if (error != WSAEWOULDBLOCK) {
											client->closed = true;
										}
									}
									else {
										if (bytesSend != sendLength) {
											int32_t bufferBytesSend = bytesSend - headerLength;
											if (bufferBytesSend < 0) {
												client->closed = true;
											}
											else {
												message.buffer.seekg(startPos + bufferBytesSend);
											}
										}
									}

									if (message.buffer.tellg() == message.buffer.tellp()) {
										client->sendMessages.pop();
									}
								}
							}
						}
					}
				}
			}
			else {
				printf("select failed with error: %ld\n", WSAGetLastError());
				return false;
			}
		}
	}
}
