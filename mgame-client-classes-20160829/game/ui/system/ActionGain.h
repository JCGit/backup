/***************************************************
File		: ActionGain.h
Description	: ��ȡ�����ĵĶ�������
Author		: Edward Chan
Date		: 2015-09-25

Copyright (C) 2015 - All Rights Reserved.
****************************************************/
#ifndef __MGAME_ACTIONGAIN_H__
#define __MGAME_ACTIONGAIN_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;
using namespace cocos2d::ui;

enum eReceiverType
{
	RECEIVER_ALONE,		//������������
	RECEIVER_ACCOUNT,	//�����ֵ�
};

/*
*	eg.1
*	node = sgPageFunction()->getActionPowerLayer() or ->getActionDiamondLayer() or ->getActionPackageLayer()
*
*	Node* pOriginReceiver = sgPageFunction()->getDiamondPanel()->getChildren().at(0);
*	Node* pReceiver = CSLoader::getInstance()->createNode("MoneyBuyNode.csb");
*	node->addReceiver(RECEIVER_ACCOUNT, pReceiver, pOriginReceiver->getPosition(), false, pOriginReceiver);
*
*	then
*
*	node->addItem("ui/general/general_diamonds.png", btn->getPosition(), 100, 0);
*
*	ĳЩ����ķ��ش�����Ҫ����stopActionGain�ӿ�,����������ڷ��еĶ���
*/
class ActionGain : public Node
{
public:
	ActionGain();
	~ActionGain();

	CREATE_FUNC(ActionGain);

	virtual bool init();

	/*
	*	@function		����ռ���
	*	@parm
	*	type			�ռ�������
	*	receiver		�ռ���
	*	position		�ռ���λ��
	*	isVisible		ԭ�ڵ��Ƿ�����
	*/
	void addReceiver(eReceiverType type, Node* receiver, Vec2 position, bool isVisible, Node* originReceiver = NULL);

	/*
	*	@function		����ռ���Ŀ(�Ƚ��ȳ�ԭ��)
	*	@parm
	*	item			��Ŀ�ڵ�
	*	position		�ռ���λ��
	*	itemCount		��Ŀ����
	*	originCount		ԭʼ����
	*/
	void addItem(Node* item, Vec2 position, int itemCount, int originCount, bool isRandomRotation = true, bool isRandomPos = true);

	/*
	*	@function		����ռ���Ŀ(�Ƚ��ȳ�ԭ��)
	*	@parm
	*	itemName		��ĿͼƬ����
	*	position		�ռ���λ��
	*	itemCount		��Ŀ����
	*	originCount		ԭʼ����
	*/
	void addItem(std::string itemName, Vec2 position, int itemCount, int originCount);

	//ɾ���ռ���
	//ɾ��������������ⲿ����
	void removeReceiver();

	//ֹͣ����
	void stopActionGain();

	//�ƶ������ӿ�,�ݲ�����
	void setMoveAction();

protected:
	void actionCallback();
	
	void clearItemList();

	void particleCallback(Node* node);

	Node*				mItemsNode;
	eReceiverType		mType;
	Node*				mOriginReceiver;
	Node*				mReceiver;
	Vec2				mReceiverPosition;
	Text*				mAccountText;
	int					mAccount;
	bool				mVisible;
	std::queue<int>		mItemCountVec;
};

#endif //__MGAME_ACTIONGAIN_H__