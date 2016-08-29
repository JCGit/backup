
#ifndef __MGAME_PVPROOMPROXY_H__
#define __MGAME_PVPROOMPROXY_H__

#include "BaseProxy.h"




class PvpRoomProxy : public BaseProxy
{
public:
	PvpRoomProxy();
	~PvpRoomProxy();

	virtual void DoRsp(NetPack& stRsp);

	virtual bool fault(const fgame::CSErrorMsg &err);

	//����
	void sendCreateRoomRequest();

	//���뷿��
	void sendEnterRoomRequest(int roomID,std::string serverNode);

	//���������ģ
	void sendChangeRoomModeRequest(int mode);

	//�뿪����
	void sendLeaveRequest(int ID);

	//�߳����
	void sendKickOffRequest(int uin);

	//�������
	void sendInviteFriendRequest(int uin);

	//��սȫ��
	void sendChallengeRequest();
	
	//����
	void sendChatRequest(int id );

	//��ʼս��
	void sendBeginFightRequest();

	//��������
	void sendAddFriendRequest(int ID);


};




#endif //__MGAME_PVPROOMPROXY_H__