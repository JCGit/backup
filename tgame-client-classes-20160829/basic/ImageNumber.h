#ifndef ImageNumber_h__
#define ImageNumber_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define MAX_ROLL_NUM 30			//数字随机滚动的次数

typedef void (CCObject::*ON_ROLL_DONE_FUN)();
#define rolldone_selector(_SELECTOR) (ON_ROLL_DONE_FUN)(&_SELECTOR)

class ImageNumber : public CCNode
{
public:
	ImageNumber();
	~ImageNumber();

	/*
	* 创建数字图片
	*/
	static ImageNumber* create(unsigned int numIdx);

	bool initWithNum(unsigned int numIdx);

	/*
	* number：显示的数字
	* interX：数字间隔
	* operatorStr: 显示在数字前面或后面的符号文件名，加号、减号等
	* operatorIdx：符号的位置，0表示最前
	*/
	void setNum(const string number, float interX = 0, const string operatorStr = "", unsigned int operatorIdx = 0);

	string getNumStr(){
		return m_iNum;
	}

	/*
	* number：显示的数字
	* interX：数字间隔
	* reapeatNum：数字随机滚动的次数
	*/
	void showNumWithAction(unsigned int number, float interX = 0, int reapeatNum = MAX_ROLL_NUM);

	/*
	* fromNum：开始的数字
	* toNum：滚动后的数字
	* interX：数字间隔
	* reapeatNum：数字随机滚动的次数
	*/
	void rollNumFromTo(int fromNum, int toNum, float interX = 0, int reapeatNum = MAX_ROLL_NUM);

    void setRollDoneCallback(CCObject* pTarget, ON_ROLL_DONE_FUN fun) {
        m_pOnRollDoneTarget = pTarget;
        m_pOnRollDoneCallFunc = fun;
    }
protected:
	void rollNum(float dt);
	void reapeatRollNum();
private:
    CCSpriteBatchNode* m_pNode;
    void onRollDoneCallBack();

	string m_stFileStr;

	string m_iNum;
private:
	float m_fInterX;
	int m_iStartNum;			//起始值，999
	int m_iEndNum;				//最终值
	int m_iRandNum;				//随机数
	int m_iRollNum;				//随机滚动次数
	int m_iBitNum;				//有多少位
	int m_iReapeatNum;			//重复次数
    int m_iRemainRepeatNum;     //还要重复几次


    ON_ROLL_DONE_FUN m_pOnRollDoneCallFunc;
    CCObject *m_pOnRollDoneTarget;
};

#endif // ImageNumber_h__
