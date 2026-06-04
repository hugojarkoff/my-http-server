#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {

	// creating socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// specifying the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// binding socket
	if ( bind(
		serverSocket, 
		reinterpret_cast<sockaddr*> (&serverAddress), 
		sizeof(serverAddress)
	) == -1 ) { return -1; }; // ERROR

	// listening to the assigned socket
	listen(serverSocket, 5);

	// accepting connection request
	int clientSocket = accept(serverSocket, nullptr, nullptr);

	// receiving data
	char buffer[1024] = {0};  // 1024 0s Buffer
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << '\n';


	// closing the sockets
	close(clientSocket);
	close(serverSocket);

	return 0;
};
