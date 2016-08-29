/***************************************************
	File		: FriendProxy.h
	Description	: ������������
	Author		: Edward Chan
	Date		: 2015-10-01
	
	Copyright (C) 2015 - All Rights Reserved.
****************************************************/
#ifndef __MGAME_FRIENDPROXY_H__
#define __MGAME_FRIENDPROXY_H__

#include "BaseProxy.h"
#include "game/Constant.h"

class FriendProxy : public BaseProxy
{
public:
	FriendProxy();
	~FriendProxy();

	virtual void DoRsp(NetPack& stRsp);

	//��ȡȫ���б�
	void sendAllListRequest();
	//�����б�
	void sendFriendListRequest();
	//��Ӻ���
	void sendAddFriendRequest(int uin);
	//ɾ������
	void sendSplitUpRequest(int uin);
	//�Ӻ���Ӧ��
	void sendAddConfirmRequest(bool isAgree, int uin);
	void sendAddConfirmRequest(bool isAgree, std::vector<int> uins);
	//�����û�
	void sendSearchRequest(int uin, std::string name,int type =0);
	//���������б�
	void sendApplicationListRequest();
	//Լս�����б�
	void sendPKListRequest();
	//ԼսӦ��
	void sendPkConfirmRequest(bool isAgree, int roomId, int uin);

#ifdef MGAME_PLATFORM_MPLUS
	//ͬ�������б�
	void sendSycFriendListRequest(std::vector<std::string>);
#endif
	
};

#endif //__MGAME_FRIENDPROXY_H__