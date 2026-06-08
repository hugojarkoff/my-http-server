#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {

	while (true) {
		// Each loop get its own connection

		// creating socket
		int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		
		// specifying local server address
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;	
		serverAddress.sin_port = htons(8080);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		// sending connection request to connect to server 
		if ( connect(
			clientSocket, 
			reinterpret_cast<sockaddr*> (&serverAddress),
			sizeof(serverAddress)
		) == -1 ) { return -1; }; 
	
		std::string message;	
		std::cout << "Enter your message: ";
		std::getline(std::cin, message); 

		// check send result
		int bytes = send(clientSocket, message.c_str(), message.length(), 0);
		if (bytes == -1) {
			std::cerr << "Connection closed\n";
			break;
		};

		// read answer from server
		char buffer[4096] {0} ;
		int received = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (received <= 0) {
			std::cerr << "Error receiving response from server\n";
			break;
		};

		std::cout << "Server says: " << buffer << '\n';

		close(clientSocket);
	}
	
	return 0;
}
