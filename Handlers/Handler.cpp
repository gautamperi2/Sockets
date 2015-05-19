/////////////////////////////////////////////////////////////////////////////
// Handler.cpp - Handler file to receive and process the socket stream		//
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
#include "Handler.h"
#include "Communication.h"
#include <iostream>
#include <fstream>
#include "FileSystem.h"

using namespace FileSystem;
//----< Client Handler thread starts running this function >-----------------

void ClientHandler::operator()(Socket& socket_)
{
	std::thread dequeueThread(&ClientHandler::DequeueAndProcess, this, std::ref(socket_));
	dequeueThread.detach();
	while (true)
	{
		// interpret test command		
		std::string command = socket_.recvString('>');
		if (command.size() == 0)
			break;
		this->bQ.enQ(command);

	}
	this->bQ.enQ("quit");
	::Sleep(1000);
}

// Dequeues from the Blocking queue and process received message
void ClientHandler::DequeueAndProcess(Socket& socket_)
{
	try
	{
		std::string msg;
		while (bQ.size() == 0)
		{
			::Sleep(100);
		}
		CommMessage obj;
		while (true)
		{
			msg = bQ.deQ();

			if (msg != "quit")
			{
				DeFrameMessage(msg, obj);
			}
			else
				break;
		}

		FinishProcessing(obj, socket_);
	}
	catch (std::exception& ex)
	{
		Verbose v(true);
		Verbose::show("  Exception caught:", true);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

// processes completely received messages to perform actions
void ClientHandler::FinishProcessing(CommMessage& obj, Socket& socket_)
{
	if (obj.GetCommand() == "TEST_FILE_HANDLING")
	{
		this->FileTransferHandler(obj);
		std::cout << "FILE_RECEIVED: " << obj.GetFileName() << " FROM " << obj.GetSrcPort() << " \n\n";
		//Send Acknowledement
		this->SendAck(obj, "TEST_FILE_RECEIVED");
		std::cout << "\n\nAcknowlegement Sent For:" << obj.GetFileName() << "to" << obj.GetSrcPort() << "\n";
	}
	else if (obj.GetCommand() == "TEST_STRING_HANDLING")
	{
		std::cout << "\nRECEIVED STRING FROM " << obj.GetSrcPort() << "\n" << obj.GetMsgBody() << "\n\n";
		//Send Acknowledement
		this->SendAck(obj, "TEST_STRING_RECEIVED");
	}
	else if (obj.GetCommand() == "TEST_FILE_RECEIVED")
	{
		std::cout << "\n\nAcknowlegement:" << "\n";
		std::cout << "FILE_RECEIVED: " << obj.GetFileName() << " FROM " << obj.GetSrcPort() << " \n\n";
	}
	else if (obj.GetCommand() == "TEST_STRING_RECEIVED")
	{
		std::cout << "\n\nAcknowlegement:" << "\n";
		std::cout << "STRING_RECEIVED FROM " << obj.GetSrcPort() << " \n\n";
	}

	Verbose::show("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	Verbose::show("ClientHandler thread terminating");
}

// Send Acknowlegement after task completion
void ClientHandler::SendAck(CommMessage& obj, std::string str)
{
	Sender objSend;
	SocketSystem ss;
	objSend.Connect(obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	std::string header = objSend.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	objSend.SendAck(header);
}

// Deframe the recived message and store it in an object
void ClientHandler::DeFrameMessage(std::string msg, CommMessage& obj)
{
	std::string command = msg.substr(msg.find("COMMAND:") + 8, msg.find("DEST_PORT:") - (msg.find("COMMAND:") + 8) - 1);
	std::string destPort = msg.substr(msg.find("DEST_PORT:") + 10, msg.find("DEST_IP:") - (msg.find("DEST_PORT:") + 10) - 1);
	std::string destIP = msg.substr(msg.find("DEST_IP:") + 8, msg.find("SRC_PORT:") - (msg.find("DEST_IP:") + 8) - 1);
	std::string srcPort = msg.substr(msg.find("SRC_PORT:") + 9, msg.find("SRC_IP:") - (msg.find("SRC_PORT:") + 9) - 1);
	std::string srcIP = msg.substr(msg.find("SRC_IP:") + 7, msg.find("FILENAME:") - (msg.find("SRC_IP:") + 7) - 1);
	std::string fileName = msg.substr(msg.find("FILENAME:") + 9, msg.find("MSG_BODY:") - (msg.find("FILENAME:") + 9) - 2);
	std::string MessageBody = msg.substr(msg.find("MSG_BODY:") + 9, msg.length() - (msg.find("MSG_BODY:") + 9));


	obj.SetCommand(command);
	obj.SetDestPort(destPort);
	obj.SetDestIP(destIP);
	obj.SetSrcPort(srcPort);
	obj.SetSrcIP(srcIP);
	obj.SetFileName(fileName);
	obj.SetMsgBody(obj.GetMsgBody() + MessageBody);
}

//creates directory on Source port and saves the recived file to that path
void ClientHandler::FileTransferHandler(CommMessage& obj)
{
	if (!Directory::exists(obj.GetSrcPort()))
	{
		Directory::create(obj.GetSrcPort());
	}

	std::string path = ".\\" + obj.GetSrcPort() + "\\" + obj.GetFileName();
	std::ofstream myfile;
	myfile.open(path, std::ios::out | std::ios::binary);
	myfile << obj.GetMsgBody();
	myfile.close();
}


#ifdef TEST_HANDLER
int main()
{

}
#endif
