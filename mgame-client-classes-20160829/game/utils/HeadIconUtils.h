#pragma once
/*
ͷ�񹤾���
create by sovio
*/
#include "cocos2d.h"
using namespace cocos2d;


class HeadIconUtils :public Node
{
public:
	enum eIconShapeType
	{
		E_SHAPE_CIRCLE = 1,
		E_SHAPE_SQUARE,
	};

	HeadIconUtils();
	~HeadIconUtils();
	//�滻ͷ��
	void changeIcon(Node* iconNode, const std::string iconId, int type = 1);	//type 1ΪԲ�Σ� 2Ϊ����

private:
	
	void loadImgCallback(cocos2d::EventCustom* customData);
	void updateIcon(Node* iconNode, const std::string iconId, bool isUrlIcon = true);

	std::map<std::string, Node*> mChangeIconMap;
};

#define sgHeadIconUtils() Singleton<HeadIconUtils>::Get()