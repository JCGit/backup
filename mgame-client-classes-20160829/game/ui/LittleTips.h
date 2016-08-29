#pragma once

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "cocostudio/CCArmature.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define TIPS_UP_CONTROL_ACOUT 21 //С��ʾ�Ĺؿ�����

class LittleTips : public Node
{
public:

	LittleTips();
	~LittleTips();

	enum EDT{//���ݷ���
		EDT_INVALID,
		EDT_LEFT,
		EDT_RIGHT
	};

	static LittleTips* create(EDT texType, bool isChange = true);//�̶��ط��л���ʾ
	static LittleTips* create(EDT texType, std::string textStr, bool isPerson, Point panelPos, bool isChange = false);//��ʾ�̶�

	bool init();
	bool init(EDT dType, bool isChange);
	bool init(EDT texType, std::string textStr, bool isPerson, Point panelPos, bool isChange);

	void change(float t);
	void showTips();

private:
	Armature* mTips;
	ImageView* mTipsPanel;
	Text* mTipsText;
	int mTipsDirection;
	int ms;
	int preIndex;
};