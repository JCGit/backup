/***************************************************
	File		: FriendControl.h
	Description	: ����ϵͳ�����൥��
	Author		: Edward Chan
	Date		: 2015-10-01
	
	Copyright (C) 2015 - All Rights Reserved.
****************************************************/
#ifndef __MGAME_FRIENDCONTROL_H__
#define __MGAME_FRIENDCONTROL_H__

#include "Constant.h"
#include "Singleton.h"
#include "net/NetPack.h"
#include "game/Message.h"
#include "FriendProxy.h"
#include <vector>

using namespace std;

class FriendControl
{
public:
	FriendControl();
	~FriendControl();

	FriendProxy* getProxy() { return m_pProxy; };

	vector<FriendSystem::PlayerData*> getFriendList() { return m_vFriendList; };
	vector<FriendSystem::PlayerData*> getSearchList() { return m_vSearchList; };
	vector<FriendSystem::PlayerData*> getApplicationList() { return m_vApplicationList; };
	vector<FriendSystem::PlayerData*> getPkList() { return m_vPkList; };
	vector<FriendSystem::PlayerData*> getBlackList() { return m_vBlackList; };

	//���º����б�
	void updateFriendList(const fgame::CSFriendGetInfoResp& stMsg);
	//�Ӻ���
	void updateFriendList(const fgame::CSFriendConfirmAddResp& stMsg);
	//��ɾ��
	void updateFriendList(const fgame::CSFriendRequestRemoveResp& stMsg);
	//ɾ������
	void deleteFriendItem(int uin);

	//���������б�
	void updateSearchList(const fgame::CSFriendQueryResp& stMsg);
	void clearSearchList();
	
	void updateFriendList(const fgame::CSFriendQueryResp& stMsg);

	//���������б�
	void updateApplicationList(const fgame::CSFriendGetInfoResp& stMsg);
	//������
	void updateApplicationList(const fgame::CSFriendRequestAddResp& stMsg);
	//ɾ��������
	void deleteApplicationItem(int index);

	void clearApplicationList();
	
	//����Լս�б�
	void updatePkList(const fgame::CSFriendGetInfoResp& stMsg);
	//��Լս
	void updatePkList(const fgame::CSPersonalRoomInviteResp& stMsg);
	//�յ�Լս��Ϣ
	void updatePkList(const fgame::CSPersonalRoomApplyOrRefuseInviteResp& stMsg);
	//ɾ��Լս��
	void deletePkItem(int uin, int roomId);
	void deletePkItem(int roomId);

	//���η�װ�ӿ�
	void sendAddFriendRequest(int index);
	void sendAddConfirmRequest(bool isAgree, int index);
	void sendPkConfirmRequest(bool isAgree, int index);

	//�ж��Ƿ����Ǻ���
	bool isFriend(int uin);
	//�ж��Ƿ��к���
	bool hasFriend();

	//��ʾԼս��ʾ
	void showPvpInviteTip(const ::fgame::FriendPkRequestInfo& stMsg);
	void reomovePvpInviteTip();

	//��ȡ�������Ϣ
	FriendSystem::PlayerData* getInviterData() { return m_pInviterData; };
	//�洢Ӧս����Ϣ
	FriendSystem::PlayerData* getPkPlayerData() { return m_pPkPlayerData; };
	void setPkPlayerData(FriendSystem::PlayerData*);

#ifdef MGAME_PLATFORM_MPLUS
	//ͬ��ƽ̨�����б�
	void updateFriendList(const fgame::CSSycFriendListResp& stMsg);
	void addFriendInfo(const std::string extStr);
#endif
	
private:
	void onPvpInvite(cocos2d::Ref*);
	void acceptPvpInviteCallback(cocos2d::Ref*);

	FriendProxy*				m_pProxy;				//�������
	Node*						m_pPvpInviteTip;		//Լս��ʾ��ť
	FriendSystem::PlayerData*	m_pInviterData;			//��ս����Ϣ
	FriendSystem::PlayerData*	m_pPkPlayerData;		//Ӧս����Ϣ

	vector<FriendSystem::PlayerData*>		m_vFriendList;			//�����б�
	vector<FriendSystem::PlayerData*>		m_vSearchList;			//��������б�
	vector<FriendSystem::PlayerData*>		m_vApplicationList;		//���������б�
	vector<FriendSystem::PlayerData*>		m_vPkList;				//Լս�б�
	vector<FriendSystem::PlayerData*>		m_vBlackList;			//�������б�
	CC_SYNTHESIZE(int, mSeachType,SeachType);
};

#define sgFriendControl() Singleton<FriendControl>::Get()

#endif //__MGAME_FRIENDCONTROL_H__