// client.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "winsock2.h"
#include "conio.h"

#include <chrono>
#include <thread>

#include "./XmlParser/Functions.h"


//#define SERVER_PORT 12345
#define SERVER_PORT 2000
#define BUF_SIZE 4096 // block transfer size  
#define QUEUE_SIZE 10
//#define IPAddress "127.0.0.1" // Local to the system - Loop back address
#define IPAddress "192.168.56.1"

int _tmain(int argc, _TCHAR* argv[])
{
	const int bufflen = 1024;
	WORD		wVersionRequested;
	WSADATA		wsaData;
	SOCKADDR_IN target; //Socket address information
	SOCKET		s;
	int			err;
	int			bytesSent;
	char		buf[bufflen] = "hi hello";
	
	
	while(1) {
	//--- INITIALIZATION -----------------------------------
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 ) {
		printf("WSAStartup error %ld", WSAGetLastError() );
		WSACleanup();
		return false;
	}
	//------------------------------------------------------
	
	//---- Build address structure to bind to socket.--------  
	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons (SERVER_PORT); //Port to connect on
	target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP
	//--------------------------------------------------------

	
	// ---- create SOCKET--------------------------------------
	s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		printf("socket error %ld" , WSAGetLastError() );
		WSACleanup();
		return false; //Couldn't create the socket
	}  
	//---------------------------------------------------------

	
	//---- try CONNECT -----------------------------------------
	if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		printf("connect error %ld", WSAGetLastError() );
		WSACleanup();
		return false; //Couldn't connect
	}
	//-----------------------------------------------------------
	
	//---- SEND bytes -------------------------------------------
	
	//gets(buf);

	//std::string xml = LibParser::;
	
	std::string xml = LibParser::Functions::Valstr();

	//bytesSent = send( s, buf, 10, 0 ); 
	bytesSent = send(s, xml.c_str(), 1024, 0);
	printf( "Bytes Sent: %ld; XML: %s \n", bytesSent, xml.c_str() );

	//------------------------------------------------------------
	closesocket( s );
	WSACleanup();
	std::this_thread::sleep_for(1500ms);
	}

	getche();
	return 0;
}

