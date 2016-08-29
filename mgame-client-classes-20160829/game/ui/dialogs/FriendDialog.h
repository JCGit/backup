/***************************************************
	File		: FriendDialog.h
	Description	: �������
	Author		: Edward Chan
	Date		: 2015-10-01
	
	Copyright (C) 2015 - All Rights Reserved.
****************************************************/
#ifndef __MGAME_FRIENDDIALOG_H__
#define __MGAME_FRIENDDIALOG_H__

#include "ui/system/ActionWindow.h"
#include "Message.h"
#include "ui/CocosGUI.h"
#include "friend/FriendControl.h"

using namespace ui;

class FriendDialog : public ActionWindow, public MessageHandler, public cocos2d::ui::EditBoxDelegate
{
public:
	FriendDialog();
	~FriendDialog();

	CREATE_FUNC(FriendDialog);

	virtual bool init();

	virtual void onShow();
	virtual void onClose();

	virtual void update(float dt);

	//��������ص�
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

private:
	virtual void onMenuItemAction(std::string, cocos2d::Ref*);

	virtual void onReceiveMassage(GameMessage * message);

	//�л�ѡ�
	virtual void changeTab(int index);
	//�ı������
	virtual void changeFriendCount(int count);
	//������������ť
	virtual void enableBatchButton(bool visible);

	//���Ӻ�����
	virtual void addFriendItem(int index);
	//���º����б�
	virtual void updateFriendList();

	//���������б�
	virtual void updateSearchList();

	//���������б�
	virtual void updateApplicationList();
	//ɾ��������
	virtual void deleteApplicationItem(int index);

	//���������б�
	virtual void updatePkList();
	//ɾ��Լս��
	virtual void deletePkItem(int uin, int roomId);

	//��ʾ����
	virtual void showDetail(FriendSystem::PlayerData*, int type = 0);
	//����������
	virtual void onBatchEvent(bool isAgree);

	//У�����볤��
	virtual bool checkCharacter(const std::string str);
	//ʱ��ת�������������
	std::string time2Chinese(int time);
	//��������ʱ��ת��
	std::string LogintTime2Chinese(int loginTime, int logoutTime);

	//����Լս�ص�
	virtual void acceptPvpInviteCallback(cocos2d::Ref*);
	//�ܾ���ս�ص�
	virtual void rejcectPvpInviteCallback(cocos2d::Ref*);

	//����
	virtual void onSearch();

	void onClickBuyEngerCallBack(cocos2d::Ref*);
private:
	ui::EditBox*				m_pSearchBox;
	Button*						m_pTabButton[4];
	Layout*						m_pPanel[4];
	ListView*					m_pListView[4];
	Sprite*						m_pSelectButtonBg;
	Button*						m_pBatchAcceptButton;
	Button*						m_pBatchRejectButton;
	TextField*					m_pSearchTextTield;
	std::string					m_sRealSearchText;
	bool						m_bNewApply;
	bool						m_bNewPk;
	cocostudio::Armature* 		m_pNewApplyTipNode;
	cocostudio::Armature* 		m_pNewPkTipNode;
	int							m_iSelectTabIndex;
	int							m_iSelectItemIndex;

	int							m_iClickFriendTabCount;
	int							m_iClickApplyTabCount;
	int							m_iClickPkTabCount;

	int							m_iTag;

};

#endif //__MGAME_FRIENDDIALOG_H__