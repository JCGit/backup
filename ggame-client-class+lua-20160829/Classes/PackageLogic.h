#pragma once

#include "GamePackets.h"

class PacketHandlerGeneral : public PacketHandler
{
public:
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet){};
	virtual void onSendPacketFailed(const int opcode);
	virtual void onReceivePacketFailed();
	virtual void onConnectFailed(std::string ip, int port){};
	virtual void onTimeout(const int opcode);
	virtual void onPacketError(const int opcode);
private:
	virtual int _getOpcode() = 0;
};



#ifndef DEFINE_CREATE_REGISTER_HANDLER

#define DEFINE_CREATE_REGISTER_HANDLER(class_name,message_opcode) \
class class_name : public PacketHandlerGeneral{ \
public: \
	static const int mOpcode = message_opcode; \
	class_name() \
	{ \
		PacketManager::Get()->registerPacketHandler(mOpcode,this); \
	} \
	virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet); \
	virtual void onConnectFailed(std::string ip, int port){} \
private: \
	virtual int _getOpcode() { return mOpcode;}; \
}; \
class_name * _handler_##class_name = new class_name; \
void class_name::onReceivePacket( const int opcode, const ::google::protobuf::Message* packet ){ \
	if(opcode == message_opcode){ \
	MsgSeverInfoUpdate msg;msg.opcode = opcode; \
	MessageManager::Get()->sendMessage(&msg);

#define DEFINE_CREATE_REGISTER_HANDLER_END }}

#endif

#ifndef NEW_DEFINE_CREATE_REGISTER_HANDLER

#define NEW_DEFINE_CREATE_REGISTER_HANDLER(class_name,message_opcode) \
class class_name : public PacketHandlerGeneral \
	{ \
public: \
	static const int mOpcode = message_opcode; \
private: \
	virtual int _getOpcode() { return mOpcode;}; \
public: \
	class_name() \
		{ \
		PacketManager::Get()->registerPacketHandler(mOpcode,this); \
		} \
		virtual void onReceivePacket(const int opcode, const ::google::protobuf::Message* packet) \
		{ \
		if(opcode == mOpcode) \
			{ \
			MsgSeverInfoUpdate msg;msg.opcode = opcode; \
			MessageManager::Get()->sendMessage(&msg);


#define NEW_DEFINE_CREATE_REGISTER_HANDLER_END(class_name) \
			} \
		}; \
	}; \
	class_name * _handler_##class_name = new class_name; \

#define SEND_FAILED_HNDLER \
			} \
		}; \
		virtual void onSendPacketFailed(const int opcode) \
		{ \
		if(opcode == mOpcode) \
			{ 

#define CONNECT_FAILED_HNDLER \
			} \
		}; \
		virtual void onConnectFailed(const int opcode) \
		{ \
		if(opcode == mOpcode) \
			{ 

#define TIMEOUT_HNDLER \
			} \
		}; \
		virtual void onTimeout(const int opcode) \
		{ \
		if(opcode == mOpcode) \
			{ 

#endif
