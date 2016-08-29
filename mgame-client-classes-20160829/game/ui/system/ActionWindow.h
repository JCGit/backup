#pragma once

#include  "game/ui/system/Window.h"
#include "core/luaHelp/CppCallLuaHelper.h"

enum ActionWindowPopType
{
	E_POP_TYPE_SMALL,
	E_POP_TYPE_BIG,
	E_POP_TYPE_NONE,
};


class ActionWindow :public Window
{
public:
	ActionWindow() :mArmatureBg(0), mCenter(0), mSender(0), m_eType(E_POP_TYPE_SMALL), mIsPlayEndOnClose(true), mIsPlayBorn(true){};
	~ActionWindow(){};

	CREATE_FUNC(ActionWindow);

	virtual void setShow(bool b) override;

	void setPopType(ActionWindowPopType type);
	bool setAnimation(std::string name);
	void changeBone(cocostudio::Armature* pAr,std::string stBone, Node* display,int index = 0); 
	void changeBoneByDefaultArmature(std::string stBone, Node* display, int index = 0); 

	void playAnimationByName(std::string name);
	void setStandName(std::string name);

	cocostudio::Armature* getArmatureBg();
	Point getBonePosition(std::string boneName, Point offPos = ccp(0, 0), cocostudio::Armature* ani = NULL); //��ȡ�����ڵ�ǰ֡��λ��

	virtual bool _initWindow(ePriority priority, Widget* showFrom);
	
	void setIsPlayEndOnClose(bool isPlay){ mIsPlayEndOnClose = isPlay; };

	void setIsPlayBorn(bool isPlay){ mIsPlayBorn = isPlay; };

	
protected:
	

	virtual void onMenuItemAction(std::string, cocos2d::Ref*, Widget::TouchEventType);
	virtual void onMenuItemAction(std::string, cocos2d::Ref*){};

	cocostudio::Armature* mArmatureBg;
	Node* mCenter;

	//�������ص�
	virtual void windowAniCallback(cocostudio::Armature * pAr, cocostudio::MovementEventType type, const std::string& aniName);
	//�������¼�
	void onWindowFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//�������ص� ���� �̳�
	virtual void aniCallback(cocostudio::Armature * pAr, cocostudio::MovementEventType type, const std::string& aniName);
	//�������¼��ص� ���� �̳�
	virtual void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	ui::Widget* mSender;
	ActionWindowPopType m_eType;

	virtual void onActionClose(){};
	virtual void onActionShow(){};

	bool mAnimationFinish;

	std::string mStandName;

	bool mIsPlayEndOnClose;

	bool mIsPlayBorn;
};