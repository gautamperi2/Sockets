
#include <stdio.h>
#include "Communication.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "Handler.h"
#include "FileSystem.h"
#include <iostream>
#include <fstream>

using namespace FileSystem;

void Sender::Connect(const std::string& ip, size_t port)
{
	ApplicationHelpers::Verbose v(false);

	while (!si.connect(ip, port))
	{
		ApplicationHelpers::Verbose::show("client waiting to connect");
		::Sleep(100);
	}
}

void Sender::EnQString(std::string strMessage)
{
	bQ.enQ(strMessage);
}

void Sender::GetDataAndSend(std::string DestIP, size_t DestPort)
{
	try
	{

		SocketSystem ss;
		this->Connect(DestIP, DestPort);

		std::string msg;
		while (bQ.size() == 0)
		{
			::Sleep(100);
		}

		while (true)
		{
			msg = bQ.deQ();
			if (msg != "quit")
			{
				si.sendString(msg);
			}
			else
				break;
		}
		si.sendString(">>");
		//si.shutDownSend();
	}
	catch (std::exception& ex)
	{
		Verbose v(true);
		Verbose::show("  Exception caught:", true);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

std::string Sender::FrameMessage(std::string Command, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP, std::string FileName)
{

	std::stringstream DestPort;
	DestPort << DestiPort;

	std::stringstream srcPort;
	srcPort << SrcPort;

	std::string strFile = Path::getName(FileName, true);

	std::string Message;
	Message = "COMMAND:" + Command + ",";
	Message += "DEST_PORT:" + DestPort.str() + ",";
	Message += "DEST_IP:" + DestiIP + ",";
	Message += "SRC_PORT:" + srcPort.str() + ",";
	Message += "SRC_IP:" + SrcIP + ",";
	Message += "FILENAME:" + strFile;
	Message += "\n\n";
	Message += "MSG_BODY:";

	return Message;
}

void Sender::SendFile(std::string FileName, std::string header)
{
	int i = 0;
	std::string msg;
	size_t buffer_size = 512;	  //number of blocks -->set int = 512//---reading blocks
	std::ifstream inputFile;
	inputFile.open(FileName, std::ios::binary);  //open file to upload
	if (!inputFile){
		std::cout << "Error: File not found\n";
		exit(1);
	}

	std::cout << "\n Sharing File :" << FileName << " to the other Peer\n";

	//std::vector<char> myBuffer(512);     //buffer length
	char myBuffer[512];
	std::ostream_iterator<char> osIt(std::cout);
	while (!inputFile.eof())
	{
		i++;
		//std::cout << "READING FILE: " << FileName << ":" << i << "\n";
		inputFile.read(myBuffer, buffer_size);
		/*if (!inputFile)
		myBuffer.resize(static_cast<int>(inputFile.gcount()));*/    //
		for (size_t k = 0; k < sizeof(myBuffer); k++)
		{
			msg = msg + myBuffer[k];
		}
		msg = header + msg;                               //attaching header to message 
		this->EnQString(msg);
		msg = "";
		//::Sleep(20); // sleep time increases with each addition Sender
	}
	//std::cout << "I am Here1" << "\n";
	/*msg = header + ">";
	this->EnQString(msg);*/
	//this->EnQString("quit");
	//this->EnQString("END_TEST");
	msg = "";
}

void Receiver::Connect(size_t port)
{
	SocketSystem ss;
	SocketListener sl(port, Socket::IP6);
	ClientHandler cp;
	sl.start(cp);

	ApplicationHelpers::Verbose::show("press key to exit: ", ApplicationHelpers::always);
	std::cout.flush();
	std::cin.get();
}

void DemoHandler::DemoStringSend(Sender& objSend, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP)
{
	std::cout << "\n########################################\n";
	std::cout << "Sending sample string from Peer: " << SrcPort << " to " << " Peer: " << DestiPort;
	std::cout << "\n########################################\n";
	std::string text;
	std::string header = objSend.FrameMessage("TEST_STRING_HANDLING", DestiPort, DestiIP, SrcPort, SrcIP, "");
	for (size_t i = 0; i < 2; ++i)
	{
		text += "Hello World " + std::string("#") + ApplicationHelpers::toString(i + 1);

	}
	std::cout << "\n String Sent: \n" << text << "\n";
	text = header + text;
	objSend.EnQString(text);
	objSend.EnQString("quit");

	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), DestiIP, DestiPort);
	deqThread.join();
}

void DemoHandler::DemoFileTransfer(Sender& objSend, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP, std::string FileName)
{
	std::cout << "\n########################################\n";
	std::cout << "Sending file from Peer: " << SrcPort << " to " << " Peer: " << DestiPort;
	std::cout << "\n########################################\n";
	std::string header = objSend.FrameMessage("TEST_FILE_HANDLING", DestiPort, DestiIP, SrcPort, SrcIP, FileName);
	objSend.SendFile(FileName, header);
	objSend.EnQString("quit");

	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), DestiIP, DestiPort);
	deqThread.join();
}



#ifdef TEST_COMM
int main()
{

}
#endif
