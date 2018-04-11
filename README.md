# darklib
Dark World Library for C++ Sockets with some additional functions (e.g *real* random number generator)

Header Only Libraries
-
No need for compiling/linking, just add `dark` directory into your include path and let's get coding!
```cpp
#include <iostream>
#include <dark/socket.hpp>
int main()
{
	dark::Socket s(NULL, 8076);
	s.bind();
	s.listen();
	if(s.accept(client))
	{
		dark::Socket client;
		Packet packet("Hello client, You're connected.");
		s.send(packet);
		while(true)
		{
			s.recv(packet, 314);
			std::cout << client.getIP() << ":" << client.getPort() << " | " << packet.getData() << "\n";
			packet.setData("Received your message!");
			s.send(packet, 314);
		}
	}
}
```
