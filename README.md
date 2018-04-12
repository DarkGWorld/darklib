# darklib
Dark World Library for C++ Sockets with some additional functions (e.g *real* random number generator)

Header-Only Libraries
-
No need for compiling/linking, just add `dark` directory into your include path and let's get coding!
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
Feel free to submit a bug, improve a function, look at the source code, and
if you have a better solutions or ideas, I'd like to know.
