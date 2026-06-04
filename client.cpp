#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main() {

	// creating socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// specifying address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// sending connection request to connect to server 
	if ( connect(
		clientSocket, 
		reinterpret_cast<sockaddr*> (&serverAddress), 
		sizeof(serverAddress)
	) == -1 ) { return -1; }; // ERROR
								
	// sending data
	std::string message;	
	std::cout << "Enter your message: ";
	std::getline(std::cin, message); 

	send(clientSocket, message.c_str(), message.length(), 0);
	
	// close socket
	close(clientSocket);

	return 0;
}
