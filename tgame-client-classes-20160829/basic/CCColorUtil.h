#ifndef CCColorUtil_h__
#define CCColorUtil_h__

#include "cocos2d.h" 
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;

class CCColorUtil  
{  
public:  
	static void addGray(CCNode* sp);		//��ͼƬ���
	static void removeGray(CCNode* sp);  

	//��Ӱ
	static void addShadow(UILabel *pLabel);
	static void addShadow(CCLabelTTF *pLabel);

	//���
	static void addStroke(UILabel *pLabel, float strokeSize = 1.0f, const ccColor3B &strokeColor = ccBLACK);
	static void addStroke(CCLabelTTF *pLabel, float strokeSize = 1.0f, const ccColor3B &strokeColor = ccBLACK);

    //�����ı���ɫ
    static void setLabelColor(UILabel *pLabel, const ccColor3B &strokeColor = ccBLACK);
};

#endif // CCColorUtil_h__
