#include "common.h"

#include "NetworkLayer.h"

#include <stdio.h>
#include <cassert>

constexpr auto BUFFER_SIZE = 1024;

#include <string>
#include <iostream>

namespace NETWORK
{
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

		assert(receivedBufferSize > 4);

		if (receivedBufferSize > 4) {
			int32_t type;
			this->receivedBuffer >> type;

			this->receivedMessages.emplace_back();
			this->receivedMessages.back().type = static_cast<MESSAGE::TYPE>(type);
			this->receivedMessages.back().buffer = std::move(this->receivedBuffer);
		}
		else {
			this->receivedBuffer.clear();
		}
	}

	void Client::send(Message&& message) {
		std::lock_guard<std::mutex> lock(this->mutex);
		this->sendMessages.push(std::move(message));
	}

	Client::Client() {
		this->sendMessages.emplace();
		this->sendMessages.back().buffer << "1";
		this->sendMessages.emplace();
		this->sendMessages.back().buffer << "2";
		this->sendMessages.emplace();
		this->sendMessages.back().buffer << "3";
		this->sendMessages.emplace();
		this->sendMessages.back().buffer << "4";
		this->sendMessages.emplace();
		this->sendMessages.back().buffer << "5";
	}

	Network::Network() {
	}

	bool Network::initialize(int32_t portNumber) {
		std::lock_guard<std::mutex> guard(this->mutex);
		portNumber = portNumber > 0 ? portNumber : 27015;
		portNumber = portNumber < 65536 ? portNumber : 27015;

		this->PORT = std::to_string(portNumber);

		std::cout << "opening server on port: " << this->PORT << '\n';

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
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
		iResult = getaddrinfo(NULL, PORT.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return false;
		}

		// Create a SOCKET for connecting to server
		this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (this->listenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Setup the TCP listening socket
		iResult = bind(this->listenSocket, result->ai_addr, (int) result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(this->listenSocket);
			WSACleanup();
			return false;
		}

		freeaddrinfo(result);

		iResult = listen(this->listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(this->listenSocket);
			WSACleanup();
			return false;
		}

		return true;
	}

	bool Network::run() {
		while (true) {
			fd_set readFDs;
			fd_set writeFDs;
			fd_set exceptFDs;

			FD_ZERO(&readFDs);
			FD_ZERO(&writeFDs);
			FD_ZERO(&exceptFDs);

			if (this->listenSocket != INVALID_SOCKET) {
				FD_SET(this->listenSocket, &readFDs);
				FD_SET(this->listenSocket, &exceptFDs);
			}

			{
				std::lock_guard<std::mutex> guard(this->mutex);
				for (auto& client : clients) {
					FD_SET(client->socket, &readFDs);
					if (!client->sendMessages.empty()) {
						FD_SET(client->socket, &writeFDs);
					}
				}
			}

			// first parameter compatibility with Berkeley sockets
			// last parameter: timeout duration, 0 for blocking
			if (select(0, &readFDs, &writeFDs, &exceptFDs, 0) > 0) {
				std::lock_guard<std::mutex> guard(this->mutex);
				printf("something happen\n");

				if (FD_ISSET(this->listenSocket, &readFDs)) {
					printf("listen socket read\n");
					sockaddr newClientAdress;
					int newClientAddressLength = sizeof(sockaddr);
					auto newClientSocket = accept(this->listenSocket, &newClientAdress, &newClientAddressLength);

					if (newClientSocket != INVALID_SOCKET) {
						clients.emplace_back(std::make_unique<Client>());
						clients.back()->socket = newClientSocket;
						clients.back()->address = newClientAdress;
						clients.back()->addressLength = newClientAddressLength;

						printf("new client\n");
					}
				}

				for (auto& client : clients) {
					if (FD_ISSET(client->socket, &exceptFDs)) {
						client->close();
					}
					else {
						if (FD_ISSET(client->socket, &readFDs)) {
							printf("client socket read\n");
							// TODO: handle closing of connection (0 bytes received) or an error (bytes == SOCKET_ERROR)
							char buffer[BUFFER_SIZE];
							int bytesReceived = recv(client->socket, buffer, BUFFER_SIZE, 0);
							// connection closed
							if (bytesReceived == 0) {
								client->close();
							}
							else if (bytesReceived == SOCKET_ERROR) {
								client->close();
							}
							else if (bytesReceived > 0) {
								switch (buffer[0]) {
									case RAWBYTES::START:
										client->receiveNewMessage();
										client->receive(buffer + 1, bytesReceived - 1);
										break;
									case RAWBYTES::CONTINUE:
										client->receive(buffer + 1, bytesReceived - 1);
										break;
									default:
										std::string m(buffer + 1, bytesReceived - 1);
										printf("some message: %s\n", m.c_str());
										//client->close();
										//assert(0);
										break;
								}
							}
							else {
								client->close();
								assert(0);
							}
						}

						if (FD_ISSET(client->socket, &writeFDs)) {
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

									int32_t bytesSend = send(client->socket, sendBuffer, sendLength, 0);

									if (bytesSend == SOCKET_ERROR) {
										int32_t error;
										int32_t errorLength = sizeof(error);
										getsockopt(client->socket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &errorLength);

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
				printf("select() failed\n");
				return false;
			}
		}
	}
}
