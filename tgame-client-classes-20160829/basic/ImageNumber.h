#ifndef ImageNumber_h__
#define ImageNumber_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define MAX_ROLL_NUM 30			//������������Ĵ���

typedef void (CCObject::*ON_ROLL_DONE_FUN)();
#define rolldone_selector(_SELECTOR) (ON_ROLL_DONE_FUN)(&_SELECTOR)

class ImageNumber : public CCNode
{
public:
	ImageNumber();
	~ImageNumber();

	/*
	* ��������ͼƬ
	*/
	static ImageNumber* create(unsigned int numIdx);

	bool initWithNum(unsigned int numIdx);

	/*
	* number����ʾ������
	* interX�����ּ��
	* operatorStr: ��ʾ������ǰ������ķ����ļ������Ӻš����ŵ�
	* operatorIdx�����ŵ�λ�ã�0��ʾ��ǰ
	*/
	void setNum(const string number, float interX = 0, const string operatorStr = "", unsigned int operatorIdx = 0);

	string getNumStr(){
		return m_iNum;
	}

	/*
	* number����ʾ������
	* interX�����ּ��
	* reapeatNum��������������Ĵ���
	*/
	void showNumWithAction(unsigned int number, float interX = 0, int reapeatNum = MAX_ROLL_NUM);

	/*
	* fromNum����ʼ������
	* toNum�������������
	* interX�����ּ��
	* reapeatNum��������������Ĵ���
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
	int m_iStartNum;			//��ʼֵ��999
	int m_iEndNum;				//����ֵ
	int m_iRandNum;				//�����
	int m_iRollNum;				//�����������
	int m_iBitNum;				//�ж���λ
	int m_iReapeatNum;			//�ظ�����
    int m_iRemainRepeatNum;     //��Ҫ�ظ�����


    ON_ROLL_DONE_FUN m_pOnRollDoneCallFunc;
    CCObject *m_pOnRollDoneTarget;
};

#endif // ImageNumber_h__
