# darklib
Dark World Library for C++ Sockets with some additional functions (e.g *real* random number generator)

Supported
-
Socket: **TCP**, **UDP**, (**SSL** *client-only*)<br>
Web: **HTTP**, **HTTPS**, **Headers**, **GET**, **POST**<br>
Math: **random**, **abs**, **mean**, **reverse**, **first/last-digit**, **digits**, **type**

Pre-Beta
-
this API is created by me for just learning, no specfic time for "releasing" a version.<br>
I don't know either if it's good to use it as your essential Socket API, but<br>
I'll try to make all things possible. Feel free to help developing it.

Header-Only Libraries
-
No need for compiling/linking, just add `dark` directory into your include path and let's get coding!


# Examples

TCP Server
-
```cpp
#include <iostream>
#include <dark/socket.hpp>
dark::Socket client; // creates a socket for some lucky client.
int main()
{
	// NULL for any address, port 8076
	dark::Socket s(NULL, 8076);
	// dark::TCPSocket == SOCK_STREAM in C
	s.setProtocol(dark::TCPSocket);
	// bind the socket, bc we're the server!
	s.bind();
	// start listening
	s.listen(); 
	if (s.accept(client)) // accept the upcoming client to this socket `client` (like an room number or ID)
	{
		// client connection request is accepted
		dark::Packet packet("Hello client, You're connected."); // Get your packet ready
		// note that the size of this packet is automatically assigned
		client.send(packet); // send your packet, note that we used the `client` socket to send him
		while (true) // infinite data exchange between you and the client
		{
			// we want to receive a packet with maximum size of 314 bytes
			client.recv(packet, 314); // *assigning* it to `packet` which will overwrite the prevous one
			// now print what we recieved with some details
			std::cout << client.getIP() << ":" << client.getPort() << " | " << packet.getData() << "\n";
			packet.setData("Received your message!"); // prepare your reply on what he sent
			client.send(packet); // send back a packet of size 22 and it is *Received your message!*
		}
	}
}
```
