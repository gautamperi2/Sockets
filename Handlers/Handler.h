#ifndef HANDLER123_H
#define HANDLER123_H
///////////////////////////////////////////////////////////////////
// Handler.h - Handler to receive and process requests            //
// Ver 1.3                                                       //
// Application: Pr#3, Spring 2015					             //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013		  //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package has two major classes CommMessage to deframe and save message
and Client Handler to receive the socket stream and process them.
*
*
* public interface:
*----------------------
	CommMessage
	------------
	std::string cmd = obj.GetCommand();
	std::string sport = obj.GetSrcPort();
	std::string dport = obj.GetDestPort();
	std::string dip = obj.GetSrcIP();
	std::string sip = obj.GetDestIP();
	std::string name = obj.GetFileName()
	std::string body= obj.GetMsgBody();


	obj.SetCommand(command);
	obj.SetDestPort(destPort);
	obj.SetDestIP(destIP);
	obj.SetSrcPort(srcPort);
	obj.SetSrcIP(srcIP);
	obj.SetFileName(fileName);
	obj.SetMsgBody(obj.GetMsgBody() + MessageBody);

	ClientHandler
	-------------
	operator();
	DequeueAndProcess();
	EnqueueReceivedData();
	DeFrameMessage();
	FileTransferHandler();
	SendAck();
	FinishProcessing();

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


#include <thread>
#include <mutex>
#include <stdlib.h>
#include "../../Sockets/Sockets/Sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "Cpp11-BlockingQueue.h"
using namespace ApplicationHelpers;


///////////////////////////////////////////////////////////////////////////////
//// Message class to store the Deframed recived message

class CommMessage
{

public:
	std::string GetCommand(){ return Command_; }
	std::string GetSrcPort(){ return SrcPort_; }
	std::string GetDestPort(){ return DestPort_; }
	std::string GetSrcIP(){ return SrcIP_; }
	std::string GetDestIP(){ return DestIP_; }
	std::string GetFileName(){ return FileName_; }
	std::string GetMsgBody(){ return MsgBody_; }


	void SetCommand(std::string Command_){ this->Command_ = Command_; }
	void SetSrcPort(std::string SrcPort_){ this->SrcPort_ = SrcPort_; }
	void SetDestPort(std::string DestPort_){ this->DestPort_ = DestPort_; }
	void SetSrcIP(std::string SrcIP_){ this->SrcIP_ = SrcIP_; }
	void SetDestIP(std::string DestIP_){ this->DestIP_ = DestIP_; }
	void SetFileName(std::string FileName_){ this->FileName_ = FileName_; }
	void SetMsgBody(std::string MsgBody_){ this->MsgBody_ = MsgBody_; }

	
private:
	std::string Command_;
	std::string SrcPort_;
	std::string DestPort_;
	std::string SrcIP_;
	std::string DestIP_;
	std::string FileName_;
	std::string MsgBody_;
};

//
///////////////////////////////////////////////////////////////////////////////
//// Server's client handler class
//// - must be callable object so we've built as a functor
//// - passed to SocketListener::start(CallObject& co)
//// - Client handling thread starts by calling operator()
class ClientHandler
{
public:
	void operator()(Socket& socket_);	
	void DequeueAndProcess(Socket& socket_);
	void EnqueueReceivedData();
	void DeFrameMessage(std::string msg, CommMessage& obj);
	void FileTransferHandler(CommMessage& obj);
	void SendAck(CommMessage& obj, std::string str);
	void FinishProcessing(CommMessage& obj, Socket& socket_);

private:
	BlockingQueue<std::string> bQ;
};


#endif