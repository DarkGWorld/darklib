#pragma once
#ifndef  DARK_SOCKET_H
#define  DARK_SOCKET_H
#include "string.hpp"
#include <string>
#include <vector>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>

#pragma warning(disable:4996)
#pragma warning(disable:4018)
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket>
#include "socket.h"
#endif
//
// IPv4 Internet address
// This is an 'on-wire' format structure.
//

namespace dark
{
	class UDSocket;
	class UDPacket;
	class Http;
	typedef const char byte;
	typedef const void * data;
	typedef std::vector<std::string> Header;
	typedef struct
	{
		byte * IP;
		int	PORT;
	}Address;
	enum {
		AUTO,
	};
	enum Protocol {
		TCPSocket,
		UDPSocket,
		HTTP,
		HTTPS
	};


	

	class Http
	{
	public:
		static std::string GetRoot(const char * URL)
		{
			std::string url(URL ? URL : "");
			size_t pattern;

			if ((pattern = url.find("http://")) != std::string::npos)
				url.replace(pattern, 7, "");

			else if ((pattern = url.find("https://")) != std::string::npos)
				url.replace(pattern, 8, "");

			if ((pattern = url.find("/")) != std::string::npos)
				url.replace(pattern, url.length(), "");
			return url;
		}
		static std::string ClearURL(const char * URL)
		{
			std::string url(URL ? URL : "");
			size_t pattern;

			if ((pattern = url.find("http://")) != std::string::npos)
				url.replace(pattern, 7, "");

			else if ((pattern = url.find("https://")) != std::string::npos)
				url.replace(pattern, 8, "");

			return url;
		}
	};
	/*- -----------------*/
	/*- TCP	Packet Class-*/
	/*- -----------------*/
	class Packet
	{
		friend class Socket;
		friend class Session;
	public:
		Packet() : _Data(""), _Length(0)
		{

		}
		Packet(int size) : _Length(size)
		{

		}
		Packet(const char * bytes) : _Data(bytes ? bytes : ""), _Length(strlen((dark::byte*)bytes)) /* Since NULL = (void*)0 == false, we replace it with an empty string */
		{

		}
		// Use when trying to send a specified length of an buffer/object.
		Packet(const char * bytes, int length) : _Data(bytes ? bytes : ""), _Length(length)
		{

		} 
		/* Set the data buffer for this packet. */
		void setData(dark::byte * bytes)
		{
			if (bytes)
			{
				this->_Data = bytes;
				this->_Length = strlen((dark::byte*)bytes);
			}
			else
			{
				bytes = "";
				this->_Length = 0;
			}
		}
		/* Returns the data buffer. */
		const char * getData()
		{
			return this->_Data;
		}
		std::string getString()
		{
			return std::string(this->_Data);
		}
		void setSize(int size)
		{
			this->_Length = size;
		}
		int size()
		{
			return this->_Length;
		}

	private:
		const char * _Data;
		int _Length;
	};
	/*- -----------------*/
	/*- TCP	Socket Class-*/
	/*- -----------------*/
	class Socket
	{
	public:
		/* Empty Socket. */
		Socket() : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if (WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
		}
		Socket(int port) : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if (WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
			this->_Addr.sin_port = htons(port);
			if ((this->_Master = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		/* Initializes Socket. */
		Socket(dark::byte * address, int port) : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if (WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			_Addr.sin_addr.s_addr = address ? inet_addr(address) : htonl(INADDR_ANY);
			_Addr.sin_family = AF_INET;
			_Addr.sin_port = htons(port);

			if ((_Master = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		void setAddress(dark::byte * address)
		{
			_Addr.sin_family = AF_INET;
			_Addr.sin_addr.s_addr = address ? inet_addr(address) : INADDR_ANY;
		}
		void setPort(int port)
		{
			_Addr.sin_port = htons(port);
		}
		void setProtocol(Protocol protocol)
		{
			if ((_Master = socket(AF_INET, protocol == TCPSocket ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		/* Bind the Socket with the specified address.*/
		int bind()
		{
			if (::bind(this->_Master, (struct sockaddr*)&this->_Addr, sizeof(this->_Addr)) != 0)
			{
				lastError = "failed to bind the socket with the specified address";
				return 0;
			}
			return 1;
		}
		/* Accepts the upcoming connection in a TCP socket, returns 1 on success, -1 on fail.*/
		int accept(Socket & socket)
		{
			sockaddr_in sockaddr;
			int len = sizeof(sockaddr);
			socket._Master = ::accept(_Master, (struct sockaddr*)&sockaddr, &len);
			socket._Addr = sockaddr;
			return socket._Master != NULL;
		}
		/* Connects to the specefied socket*/
		int connect()
		{
			if (::connect(this->_Master, (const struct sockaddr*)&this->_Addr, sizeof(this->_Addr)) != 0)
			{
				switch (WSAGetLastError())
				{
				case 10056:
					lastError = "You're already connected.";
					break;
				default:
					lastError = "failed to connect.";
					break;
				}

				return 0;
			}
			return 1;
		}
		/* try to connect */
		int trytoconnect(int timeInMs, int times, dark::byte* print, int startFrom)
		{
			int i;
			times == dark::AUTO ? i = -1 : i = startFrom;
			while (i <= times)
			{
				if (this->connect())
					return 1;
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(timeInMs));
				printf(print ? print : "", i);
				times == dark::AUTO ? i = -1 : ++i;
			}
			return 0;
		}
		/* Starts listening on a socket */
		void listen()
		{
			if (::listen(this->_Master, SOMAXCONN) != 0)
			{
				int err;
#ifdef _WIN32
				err = WSAGetLastError();
#endif
				switch (err)
				{
				case 10022:
					lastError = "Unable to listen on the supplied address."; /* An Invalid Arguemnt was supplied */
					break;
				}
			}
		}
		/* Send a packet over a socket */
		int send(Packet packet)
		{
			int res = this->_ssl ? SSL_write(_ssl, packet._Data, packet._Length) : ::send(this->_Master, (dark::byte*)packet._Data, packet._Length, 0);
			if (res < 0)
			{
#ifdef _WIN32
				switch (WSAGetLastError())
				{
				case 10057:
					lastError = "failed to send a packet on this disconnected socket.";
					break;
				}
#endif
			}
			return res;
		}
		/* Receive a packet from a socket */
		int recv(Packet & packet, int maxByte)
		{
			char* data = (char*)calloc(1, maxByte);
			memset(data, 0, maxByte);
			int len = this->_ssl ? SSL_read(_ssl, data, maxByte) : ::recv(this->_Master, data, maxByte, 0);
			data[maxByte] = '\0';
			packet.setData(data);
			return len;
		}
		/* Returns the Network IP address in American Standard Code */
		const char* getIP()
		{
			return inet_ntoa(this->_Addr.sin_addr);
		}
		/* Returns the Network Host Port */
		int getPort()
		{
			return ntohs(this->_Addr.sin_port);
		}
		/* Returns the protocol type of the socket*/
		Protocol getType()
		{
			int type;
			int length = sizeof(int);
			getsockopt(this->_Master, SOL_SOCKET, SO_TYPE, (char*)&type, &length);
			return type == SOCK_STREAM ? TCPSocket : UDPSocket;
		}
		/* Returns the last error in a string literal. */
		const char * getError()
		{
			return lastError;
		}
		int getCore()
		{
			return this->_Master;
		}
		static cstr GetIP(const char * host)
		{
#ifdef _WIN32
			WSAData wsaData;
			WSAStartup(0x101, &wsaData);
#endif
			hostent * h = gethostbyname(host);
			return (host && h) ? inet_ntoa(*(in_addr *)h->h_addr) : (char*)"";
		}
		int ssl(int go)
		{
			if (go)
			{
				OpenSSL_add_all_algorithms();
				ERR_load_BIO_strings();
				ERR_load_crypto_strings();
				SSL_load_error_strings();
				if (SSL_library_init() < 0)
					puts("Could not initialize the OpenSSL library !");

				_method = SSLv23_client_method();


				if ((_ctx = SSL_CTX_new(_method)) == NULL)
					puts("Unable to create a new SSL context structure.");

				_ssl = SSL_new(_ctx);

				SSL_set_fd(_ssl, _Master);


				if (!SSL_connect(_ssl))
				{
					lastError = std::string(std::string("Failed to open SSL Connection with ") + std::string(getIP())).c_str();
					return 0;
				}
				else
					return 1;
			}
			else if (go == 0)
			{
				_ssl = NULL;
				return 0;
			}
			else
			{
				lastError = "Undefined boolean value with the function Socket::ssl(bool)";
				return 0;
			}
			return 0;
		}

	private:
#ifdef _WIN32
		WSAData wsaData;
#endif
		dark::byte * lastError = "";
		int _Master;
		struct sockaddr_in _Addr;
		int _AddrLen;
		SSL* _ssl; // SSL Encryption for HTTPS Sockets / OpenSSL library is required.
		const SSL_METHOD * _method;
		SSL_CTX * _ctx;
	};
	/*- -----------------*/
	/*- UDP	Packet Class-*/
	/*- -----------------*/
	class UDPacket
	{
		friend class Socket;
		friend class UDSocket;
	public:
		/*--- Constructors */
		/*##################*/
		UDPacket() : _Length(0), _AddrLen(sizeof(this->_Addr))
		{

		}
		UDPacket(int size) : _Length(size), _AddrLen(sizeof(this->_Addr))
		{
			this->_Data = (char*)calloc(1, size);

		}
		UDPacket(byte * address, int port) : _AddrLen(sizeof(this->_Addr))
		{
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = address ? inet_addr(address) : INADDR_ANY;
			this->_Addr.sin_port = htons(port);
		}
		UDPacket(byte * bytes, dark::byte * address, int port) : _Length(strlen(bytes)), _AddrLen(sizeof(this->_Addr))
		{
			this->_Data = (char*)bytes;
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = address ? inet_addr(address) : INADDR_ANY;
			this->_Addr.sin_port = htons(port);
		}
		UDPacket(data data, int length, dark::byte * address, int port) : _Length(length), _AddrLen(sizeof(this->_Addr))
		{
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = address ? inet_addr(address) : INADDR_ANY;
			this->_Addr.sin_port = htons(port);
		}
		/*--- Get Functions */
		/*##################*/
		/* Return the Network IP address in American Standard Code */
		byte * ip()
		{
			return inet_ntoa(this->_Addr.sin_addr);
		}
		/* Returns the Network Host Port */
		int port()
		{
			return ntohs(this->_Addr.sin_port);
		}
		/* Return the data buffer. */
		byte * bytes()
		{
			return (dark::byte*)this->_Data;
		}

		/* Return a boolean whether the packet buffer equals the parameter. */
		int eq(byte * bytes)
		{
			return strcmp(this->_Data, bytes) == 0;
		}
		/* Return the length of the packet buffer */
		int len()
		{
			return sizeof(this->_Data) == sizeof(int) ? strlen(this->_Data) : sizeof(this->_Data);
		}
		/* Return the packet maximum buffer size */
		int size()
		{
			return this->_Length;
		}
		/* Return the data in a string. */
		std::string getString()
		{
			return std::string(this->_Data);
		}
		Address getAddress()
		{
			Address addr;
			addr.IP = inet_ntoa(_Addr.sin_addr);
			addr.PORT = ntohs(_Addr.sin_port);
			return addr;
		}
		/*--- Set Functions */
		/*##################*/
		/* Set the IP Address for this packet. */
		void address(dark::byte * address)
		{
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = address ? inet_addr(address) : INADDR_ANY;
		}
		/* Set the port for this packet. */
		void port(int port)
		{
			this->_Addr.sin_port = htons(port);
		}
		/* Set the data buffer for this packet. */
		void bytes(byte * bytes)
		{
			if (bytes)
			{
				this->_Data = bytes;
				this->_Length = strlen(bytes);
			}
			else
			{
				bytes = "";
				this->_Length = 0;
			}
		}
		/* Set the size of the packet  */
		void size(int size)
		{
			this->_Length = size;
		}


	protected:
		
	private:
		byte * _Data;
		int _Length;
		struct sockaddr_in _Addr;
		int _AddrLen;
	};
	/*- -----------------*/
	/*- UDP	Socket Class-*/
	/*- -----------------*/
	class UDSocket
	{
	public:
		UDSocket() : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if ((&wsaData == NULL) && WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			if ((this->_Master = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		UDSocket(int port) : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if (WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
			this->_Addr.sin_port = htons(port);
			if ((this->_Master = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		UDSocket(byte * address, int port) : _AddrLen(sizeof(this->_Addr))
		{
#ifdef _WIN32
			if (WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			this->_Addr.sin_family = AF_INET;
			this->_Addr.sin_addr.s_addr = address ? inet_addr(address) : htonl(INADDR_ANY);
			this->_Addr.sin_port = htons(port);
			if ((this->_Master = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			{
				lastError = "failed to create socket.";
			}
		}
		void bind()
		{
			if (::bind(this->_Master, (struct sockaddr*)&this->_Addr, sizeof(this->_Addr)) != 0)
			{
				lastError = "failed to bind the socket with the specified address";
			}
		}
		int sendto(UDPacket packet)
		{
			return ::sendto(this->_Master, packet._Data, packet._Length, 0, (struct sockaddr*)&packet._Addr, packet._AddrLen);
		}
		int recvfrom(UDPacket & packet)
		{
			char* PACKET = (char*)calloc(1, packet._Length);
			int len = ::recvfrom(this->_Master, PACKET, packet._Length, 0, (struct sockaddr*)&packet._Addr, &packet._AddrLen);
			PACKET[packet._Length] = '\0';
			packet._Data = PACKET;
			return len;
		}
		int mode(long cmd, u_long argp)
		{
			return ioctlsocket(this->_Master, cmd, &argp);
		}
		/* Returns the Network IP address in American Standard Code */
		const char* getIP()
		{
			return inet_ntoa(this->_Addr.sin_addr);
		}
		/* Returns the Network Host Port */
		int getPort()
		{
			return ntohs(this->_Addr.sin_port);
		}
	protected:
		
	private:
#ifdef _WIN32
		WSAData wsaData;
#endif
		dark::byte * lastError = "";
		int _Ping;
		int _Master;
		fd_set _MasterSet;
		struct sockaddr_in _Addr;
		int _AddrLen;
	};

	class Session
	{
	public:
		Session()
		{
#ifdef _WIN32
			if ((&wsaData == NULL) && WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			m_so.setPort(80);
			m_so.setProtocol(TCPSocket);
		}
		Session(const char * URL, Protocol connection)
		{
#ifdef _WIN32
			if ((&wsaData == NULL) && WSAStartup(0x101, &wsaData) != 0)
			{
				lastError = "Couldn't start WSA";
			}
#endif
			m_so.setAddress(Socket::GetIP(Http::GetRoot(URL).c_str()));
			m_so.setPort(connection == HTTPS ? 443 : 80);
			m_so.setProtocol(TCPSocket);
			m_url = Http::ClearURL(URL);
			m_pro = connection;
		}
		~Session()
		{
			if (m_pro == HTTPS)
			{

			}
		}
		int run()
		{
			if (m_so.connect())
			{
				m_pro == HTTPS ? m_so.ssl(1) : 0;
				return 1;
			}
			return 0;
		}
		void setHeader(Header header)
		{
			for (int i = 0; i < header.size(); ++i)
			{
				// i % 2 != 0 = true, cause we start from 0 then 1 woud be the value and 0 is the key
				if (i > 0 && (i % 2))
				{
					m_header.append(header.at(i));
					m_header.append("\r\n");
				}
				else
				{
					m_header.append(header.at(i));
					m_header.append(": ");
				}
			}
			m_header.append("\r\n");
		}
		int get()
		{
			m_data = "";
			m_data.append("GET ");
			m_data.append(m_pro == HTTP ? "http://" : "https://");
			m_data.append(m_url);
			m_data.append(" HTTP/1.1\r\n");
			m_data.append(m_header);
			Packet packet(m_data.c_str());
			return m_so.send(packet);
		}

		int post()
		{
			m_data = "";
			m_data.append("POST ");
			m_data.append(m_pro == HTTP ? "http://" : "https://");
			m_data.append(m_url);
			m_data.append(" HTTP/1.1\r\n");
			m_data.append(m_header);
			Packet packet(m_data.c_str());
			return m_so.send(packet);
		}
		int getResponse(Packet & packet, int size)
		{
			return m_so.recv(packet, size);
		}

		const char * getLastRequest()
		{
			return m_data.c_str();
		}
	private:
#ifdef _WIN32
		WSAData wsaData;
#endif
		dark::byte * lastError = "";
		std::string m_data;
		std::string m_header;
		Socket m_so;
		std::string m_url;
		Protocol m_pro;
	};
}
/*- -----------------*/
/*- END -------------*/
/*- -----------------*/
#endif