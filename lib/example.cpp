// client.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "winsock2.h"
#include "conio.h"

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	xml_document doc;
	if (!doc.load_file("sample.xml")) return -1;
	xml_node tools = doc.child("EmployeesData").child("Employees");

	for (xml_node_iterator it = tools.begin();  
		it!=tools.end(); 
		++it)
	{
		for (xml_attribute_iterator ait = it->attributes_begin(); 
			ait!=it->attributes_end(); 
			++ait)
		{

		}
	}

	//std::string xmltext = ;

	//const int bufflen = 1024;
	//WORD		wVersionRequested;
	//WSADATA		wsaData;
	//SOCKADDR_IN target; //Socket address information
	//SOCKET		s;
	//int			err;
	//int			bytesSent;
	//char		buf[bufflen] = "hi hello";
	//
	//
	//while(1) {
	////--- INITIALIZATION -----------------------------------
	//wVersionRequested = MAKEWORD( 1, 1 );
	//err = WSAStartup( wVersionRequested, &wsaData );

	//if ( err != 0 ) {
	//	printf("WSAStartup error %ld", WSAGetLastError() );
	//	WSACleanup();
	//	return false;
	//}
	////------------------------------------------------------
	//
	////---- Build address structure to bind to socket.--------  
	//target.sin_family = AF_INET; // address family Internet
	//target.sin_port = htons (SERVER_PORT); //Port to connect on
	//target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP
	////--------------------------------------------------------

	//
	//// ---- create SOCKET--------------------------------------
	//s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	//if (s == INVALID_SOCKET)
	//{
	//	printf("socket error %ld" , WSAGetLastError() );
	//	WSACleanup();
	//	return false; //Couldn't create the socket
	//}  
	////---------------------------------------------------------

	//
	////---- try CONNECT -----------------------------------------
	//if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	//{
	//	printf("connect error %ld", WSAGetLastError() );
	//	WSACleanup();
	//	return false; //Couldn't connect
	//}
	////-----------------------------------------------------------
	//
	////---- SEND bytes -------------------------------------------
	//
	////gets(buf);
	//bytesSent = send( s, buf, 10, 0 ); 
	//printf( "Bytes Sent: %ld \n", bytesSent );

	////------------------------------------------------------------
	//closesocket( s );
	//WSACleanup();
	//}

	getche();
	return 0;
}

