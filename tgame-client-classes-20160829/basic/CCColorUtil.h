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
	static void addGray(CCNode* sp);		//是图片变灰
	static void removeGray(CCNode* sp);  

	//阴影
	static void addShadow(UILabel *pLabel);
	static void addShadow(CCLabelTTF *pLabel);

	//描边
	static void addStroke(UILabel *pLabel, float strokeSize = 1.0f, const ccColor3B &strokeColor = ccBLACK);
	static void addStroke(CCLabelTTF *pLabel, float strokeSize = 1.0f, const ccColor3B &strokeColor = ccBLACK);

    //设置文本颜色
    static void setLabelColor(UILabel *pLabel, const ccColor3B &strokeColor = ccBLACK);
};

#endif // CCColorUtil_h__
