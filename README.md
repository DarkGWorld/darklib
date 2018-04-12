# darklib
Dark World Library for C++ Sockets with some additional functions (e.g *real* random number generator)

# Pre-Beta
this API is created by me for learning purposes, no specfic time for "releasing" a version ***still 15th.***<br>
I don't know either if it's good to use it as your essential Socket API, but I'll try to make all things possible. ***You gotta help me too!***
Feel free to help developing it ***Coding for fun, never worked in a team.***
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
Feel free to submit a bug, improve a function, look at the source code<br>
and if you have a better solutions or ideas, I'd like to know.
