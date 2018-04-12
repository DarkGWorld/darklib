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
dark::Socket client;
int main()
{
	dark::Socket s(NULL, 8076);
	s.setProtocol(dark::TCPSocket);
	s.bind();
	s.listen();
	if (s.accept(client))
	{
		dark::Packet packet("Hello client, You're connected.");
		client.send(packet);
		while (true)
		{
			client.recv(packet, 314);
			std::cout << client.getIP() << ":" << client.getPort() << " | " << packet.getData() << "\n";
			packet.setData("Received your message!");
			client.send(packet);
		}
	}
}
```
