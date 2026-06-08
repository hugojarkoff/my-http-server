#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unistd.h>

#define PORT 8080  // port users connect to
#define BACKLOG 10 // how many pending connections queue will hold before accepting

int main() {

	// open listening server socket
	// basically get access to the file (descriptor)
	int serverSocket = socket(
		AF_INET,  // IPv4 
		SOCK_STREAM,  // TCP stream sockets
		0  // protocol, autoset as per type above
	);

	// specifying local server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;  // Address family, AF_INET = IPv4
	serverAddress.sin_port = htons(PORT);  // Port, with endian conversion -> Host TO Network Small
	serverAddress.sin_addr.s_addr = INADDR_ANY;  // Internet address. 32 bit unsigned integer, as IPv4 is 4 bytes e.g 32 bits. Ex 192.168.1.10 is 11000000 10101000 00000001 00001010 = 3232235786 as a single integer. 

	// binding the socket to a port on host. Only necessary for the server
	if ( bind(
		serverSocket, 
		reinterpret_cast<sockaddr*> (&serverAddress),  // connect wants a sockaddr, but we use IPv4 internet -> sockaddr_in
		sizeof(serverAddress)
	) == -1 ) { return -1; }; // ERROR

	// listening to the assigned socket
	listen(serverSocket, BACKLOG);

	std::cout << "server: waiting for connections...\n";

	while (true) {

		// accepting connection request
		// we get a new socket (e.g file)
		int clientSocket = accept(serverSocket, nullptr, nullptr);

		// fork yourself after accepting a connection
		pid_t c_pid = fork();

		if (c_pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		};

		if (c_pid == 0) {
			// child process

			// child process doesn't need listening socket. Close it
			// Note that it still exists in the parent process since 
			// it's just a reference to the fd (reference count by the kernel)
			close(serverSocket);
			
			// receiving data from user, if ever
			char buffer[1024] = {0};
			int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytes > 0) {
				std::cout << "Message from client: " << buffer << '\n';
			};

			// send stuff back to client
			std::string body = "hello from hugo!";
			std::string response =
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: " + std::to_string(body.size()) + "\r\n"
					"\r\n" + body;

			send(clientSocket, response.c_str(), response.size(), 0);
			close(clientSocket);

			exit(0);
		};

		if (c_pid > 0) {
			// parent: keep listening
			// Reference count
			close(clientSocket);
		};
	};

	return 0;
};
