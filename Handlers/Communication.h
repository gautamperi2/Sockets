#ifndef COMM_H
#define COMM_H
///////////////////////////////////////////////////////////////////
// Handler.h - Communication package to handle Send and recive	//
//				 communications
// Ver 1.0										                  //
// Application: Pr#3, Spring 2015					             //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013		  //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package has  major classes Sender to send messages to sockets
Receiver to create connection to recive and DemoHandler to display demos
*
*
* public interface:
*----------------------
Sender objSend;
------------
objSend.Connect("localhost",9080);
objSend.sendString("quit");
objSend.SendAck("TEST_FILE_RECEIVED")
objSend.EnQString("quit");
objSend.GetAndSendData("localhost",9080);
std::string header = objSend.FrameMessage("TEST_STRING_HANDLING", DestiPort, DestiIP, SrcPort, SrcIP, "");
objSend.SendFile("client.txt",header);

obj.SetCommand(command);
obj.SetDestPort(destPort);
obj.SetDestIP(destIP);
obj.SetSrcPort(srcPort);
obj.SetSrcIP(srcIP);
obj.SetFileName(fileName);
obj.SetMsgBody(obj.GetMsgBody() + MessageBody);

DemoHandler objDemo;
----------------------
objDemo.DemoStringSend(objSend, std::stoi(DestPort), DestIP, std::stoi(SrcPort), SrcIP);
objDemo.DemoFileTransfer(objSend, std::stoi(DestPort), DestIP, std::stoi(SrcPort), SrcIP, FilePath);

Receiver
-------------
obj.Connect(9080);

* Required Files:
* ---------------
*   - Cpp11-BlockingQueue.h, AppHelpers.h,
*     Sockets.h
*
* Build Process:
* --------------
*   devenv Sockets.sln /debug rebuild
*
* Maintenance History:
* --------------------
*
* Ver 1.0 : 21 Apr 15
* - first release
*
*/



#include <stdio.h>
#include <stdlib.h>
#include "../Sockets/Sockets.h"
#include "Cpp11-BlockingQueue.h"

class Sender
{

public:
	void Connect(const std::string& ip, size_t port);
	void sendString(const std::string& str, Socket::byte terminator = '\0')
	{
		si.sendString(str);
		//si.shutDownSend(); 
	}
	void SendAck(std::string str){ si.sendString(str); }
	void EnQString(std::string strMessage);
	void GetDataAndSend(std::string DestIP, size_t DestPort);
	std::string Sender::FrameMessage(std::string Command, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP, std::string FileName);
	void SendFile(std::string FileName, std::string header);


private:
	SocketSystem ss;
	SocketConnecter si;
	BlockingQueue<std::string> bQ;
};

class Receiver
{
public:
	void Connect(size_t port);

};


class DemoHandler
{

public:
	void DemoStringSend(Sender& objSend, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP);
	void DemoFileTransfer(Sender& objSend, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP, std::string FileName);

private:
};

#endif