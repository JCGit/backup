/***************************************************
	File		: ActionTip.h
	Description	: ��̬��ʾ
	Author		: Edward Chan
	Date		: 2015-09-23
	
	Copyright (C) 2015 - All Rights Reserved.
****************************************************/
#ifndef __MGAME_ACTIONTIP_H__
#define __MGAME_ACTIONTIP_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;

/*
*	Ŀǰֻ��ʵ�����ı�+ͼƬ����ʽ(ս����ʾ��Ϣ),��������������չ
*
*eg.1:(ս����ʾ��Ϣʾ��)
*	Action* pAction = Sequence::create(
*		MoveBy::create(.3f, Vec2(0, 50)),
*		DelayTime::create(1.5f),
*		Spawn::create(MoveBy::create(.2f, Vec2(0, 30)), FadeOut::create(.2f), NULL),
*		NULL);
*	ActionTip* tip = ActionTip::create(string, pAction);
*	tip->addText(string-1);
*	...
*	tip->addText(string-n);
*	if want to hide pic, you can add "tip->setIsShowPic(false);"
*	tip->run();
*	xxx->addChild(tip);
*
*eg.2:
*	ActionTip* tip = ActionTip::create(vector<string>, action);
*	tip->run();
*	xxx->addChild(tip);
*
*eg.3:
*	ActionTip* tip = ActionTip::create(vector<string>, distance, duration);
*	tip->run();
*	xxx->addChild(tip);
*
*eg.4:
*	ActionTip* tip = ActionTip::create(string, distance, duration);
*	tip->addText(string-1);
*	...
*	tip->addText(string-n);
*	tip->run();
*	xxx->addChild(tip);
*
*/
class ActionTip : public Node
{
public:
	ActionTip();
	~ActionTip();

	/*
	*	@parm
	*	text		�ı�
	*	action		����
	*	callBack	�ص�����
	*/
	static ActionTip* create(string text, Action* action);

	/*
	*	@parm
	*	textList	�ı��б�
	*	action		����
	*/
	static ActionTip* create(vector<string> textList, Action* action);

	/*
	*	@parm
	*	text		�ı�
	*	distance	����
	*	duration	����ʱ��
	*/
	static ActionTip* create(string text, Vec2 distance, float duration);

	/*
	*	@parm
	*	textList	�ı��б�
	*	distance	����
	*	duration	����ʱ��
	*/
	static ActionTip* create(vector<string> textList, Vec2 distance, float duration);

	virtual bool init();
	virtual bool init(Vec2 distance, float duration);

	//����
	virtual void run();

	//��ʱֻ��������ı��Ľӿ�
	virtual void addText(string text);

	//����ʾͼƬ
	virtual void setIsShowPic(bool visible) { m_bIsShowPic = visible; }

	//���ûص�
	virtual void setCallBackFunc(const std::function<void()>& callBack);

	CC_SYNTHESIZE(Action*, mAction, Action);

protected:
	virtual void runChildrenActions(Node*);

	virtual void runActions(Node* target, bool removeParent);

	void addTextWidget(string text);

	std::function<void()> mCallBack;

	Color4B mColor;
	CC_SYNTHESIZE(int, mFontSize, FontSize);
	string mFontName;
	vector<string> mTextList;
	int mMargin;
	float mContentHeight;

	bool mIsRun;

	bool m_bIsShowPic;

private:
	Vec2 mDistance;
	float mDuration;

};

#endif //__MGAME_ACTIONTIPTEXT_H__