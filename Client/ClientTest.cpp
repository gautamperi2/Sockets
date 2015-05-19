/////////////////////////////////////////////////////////////////////////////
// ClientTest.cpp - Executive to Demosntrate the Peer to Peer Communication//
// Ver 1.1                                                                 //
// Jim Fawcett, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com     //
//-------------------------------------------------------------------------//
// Gautam peri (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Demo for Project #3, Communication Channel                 //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013					//
//Author:      Gautam Nagesh Peri, CST 4 - 484, 3154846562				  //
//              nperi@syr.edu											  ////
/////////////////////////////////////////////////////////////////////////////

/*
*  Package Operations:
*  -------------------
*  This package provides tests for Sockets-4.5:
*  - passing byte buffers
*  - passing strings
*
*  Required Files:
*  ---------------
*  ClientTest.cpp, ServerTest.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 30 Mar 15
*  - minor modification to comments, above
*  ver 1.0 : 30 Mar 15
*  - first release
*/

#include "../ApplicationHelpers/AppHelpers.h"
#include "../Handlers/Communication.h"
#include "../Handlers/Handler.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace ApplicationHelpers;

//----< test stub starts here >----------------------------------------------

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		//std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//Starts reciever to recive from sockets from other peers
void StartReceiver(Receiver& objRec, std::string SrcPort)
{
	std::thread receiverThread(&Receiver::Connect, std::ref(objRec), std::stoi(SrcPort));
	receiverThread.detach();
}


#ifdef TEST_CLIENT
int main(int argc, char*argv[])
{
	try
	{
		// Commmand Line Arguments
		std::string SrcPort = argv[1];
		std::string DestPort = argv[2];
		std::string FilePath = argv[3];

		std::string SrcIP = "localhost";
		std::string DestIP = "localhost";

		title("Testing Socket PEER1: " + SrcPort, '=');
				
		Receiver objRec;
		Sender objSend;
		StartReceiver(objRec, SrcPort);

		// Performs Demos
		DemoHandler objDemo;
		objDemo.DemoStringSend(objSend, std::stoi(DestPort), DestIP, std::stoi(SrcPort), SrcIP);
		objDemo.DemoFileTransfer(objSend, std::stoi(DestPort), DestIP, std::stoi(SrcPort), SrcIP, FilePath);
		getchar();

	}
	catch (std::exception& ex)
	{
		Verbose v(true);
		Verbose::show("  Exception caught:", true);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif
