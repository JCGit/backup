#ifndef RollNumber_h__
#define RollNumber_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;
using namespace std;

#define MAX_ROLL_NUM 30			//������������Ĵ���

class CRollNumber : public Widget
{
public:
	CRollNumber();
	~CRollNumber();

	static CRollNumber* createWithProperty(const string& stringValue, const string& charMapFile, 
		int itemWidth, int itemHeight, const string& startCharMap);

	bool initWithProperty(const string& stringValue, const string& charMapFile, 
		int itemWidth, int itemHeight, const string& startCharMap);

	void setLabelAnchorPoint(const CCPoint &pt);

	void showNumWithAction(unsigned int number, int reapeatNum = MAX_ROLL_NUM);

	void showNumDirect(unsigned int number);

	void rollNum(float dt);

	void rollNumFromTo(int fromNum, int toNum, int reapeatNum = MAX_ROLL_NUM);

	void reapeatRollNum();
protected:
private:
	LabelAtlas* m_pNumLabel;

	int m_iStartNum;			//��ʼֵ��999
	int m_iEndNum;				//����ֵ
	int m_iRandNum;				//�����
	int m_iRollNum;				//�����������
	int m_iBitNum;				//�ж���λ
	int m_iReapeatNum;			//�ظ�����
};


#endif // RollNumber_h__
