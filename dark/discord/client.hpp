#ifndef DARK_DISCORD
#define DARK_DISCORD
#ifndef DARK_DISCORD_CLIENT
#define DARK_DISCORD_CLIENT
#include "../socket.hpp"
namespace dark
{
	namespace discord
	{
		class Client
		{
		public:
			const char * id;
			const char * secret;
			const char * token;
			Client() : token("")
			{
				m_s = Session("discordapp.com/api/v6/gateway/bot", HTTPS);
			}
			Client(const char * client_id, const char * client_secret) :
				id(client_id),
				secret(client_secret),
				token("")
			{
				m_s = Session("discordapp.com/api/v6", HTTPS);
			}

			int connect()
			{
				if (m_s.run())
				{
					std::cout << "Connected\n";
					Header header = {
						"Host","discordapp.com",
						"Authorization","Bot " + std::string(token),
						"User-Agent","DiscordBot (Dark World AI)",
						"grant_type","authorization_code",
						"Content-Type","application/x-www-form-urlencoded",
						"Content-Length", "0",
						"Connection","close"
					};
					m_s.setHeader(header);
					m_s.post();
					std::cout << "{Sent}\n" << m_s.getLastRequest();
					Packet packet;
					int i = m_s.getResponse(packet, 1024);
					std::cout << "{Received}\n" << packet.getData();

					return 1;
				}
				else
				{
					std::cout << "Failed to connect\n";
				}
				return 0;
			}
			int wakeup()
			{
				return 1;
			}

		private:
			Session m_s;
		};
	}
}
#endif
#endif
