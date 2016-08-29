#pragma once
#include "cocos2d.h"
#include "Constant.h"
USING_NS_CC;


/*
�����ࣺ��Formation)
     ��map�ṹ���������еĶ��У�FormationRaw����
     ��list�ṹ������·����Ϣ��MoveInfo����
ʹ�ã�
     create����������start2end()��ȡ����Ѱ·�ĵ㡣
	 ��Ѱ·�ĵ�����MoveInfo���б�ÿ�θ����������ӵ���nextPosition��
*/

class Solider;
class FormationRow;
class BaseBuild;

typedef std::vector<Solider*> Vect_Soldier;

class Formation : public cocos2d::Ref
{

public:

	struct MoveInof
	{
		MoveInof() : maxDis(0), isEnd(false), isBend(false){};
		float maxDis;
		float cos;
		float sin;
		cocos2d::Vec2 currentPos;
		cocos2d::Vec2 aimPos;
		cocos2d::Vec2 midPos;
		bool isBend;
		float rotation;
		bool isEnd;
	};
	
	Formation() : mSrcBuild(0), mParent(0), mTarBuild(0), mFlage(0), m_iRowSpace(0), mMaxRow(0), mPause(false), m_RowIndex(0), m_time(0){};
	~Formation();
	
	//����������Ӫ
	static Formation* create(eCampType campType, unsigned int num, unsigned id, BaseBuild* srcBuild,float time = 0);
	bool init(eCampType campType, unsigned int num, unsigned id, BaseBuild* srcBuild,float time);
	//����Ψһ��Id
	static unsigned int createId();

	void enter(cocos2d::Node*);
	void nextPostion();
	void star2end(const cocos2d::Vec2&, const cocos2d::Vec2&); 

	bool	checkCanCreateRow();

	Vec2 getCenterPos();

	//��ǰ����
	int getUnit();
	//��������
	int getExpectUint(){return mExpectNum; }; 

	//����Ƿ񱻷���������
	bool checkBeAttack(BaseBuild* pAttack);
	void removeSoliderWithRect(const Rect& rect);

	int		getBeAttackKey();
	eCampType getTargetCamp();

	void changeTarget(BaseBuild* pOld, BaseBuild* pNew);

	typedef std::map<int, FormationRow*> Map_FormationRow;
	typedef std::list<MoveInof> Vect_MoveInof;

	//�����о��������ı�  ʿ����ʾ
	void  changeSoliderIns();

	void setSrcBuild(BaseBuild* srcBuild);
	void setTarBuild(BaseBuild* tarBuild);

	BaseBuild* getSrcBuild(){return mSrcBuild;};
	BaseBuild* getTarBuild(){return mTarBuild;};

	//��ͣʱ������ͣʱ��
	void	addPauseTime(float time);
protected:

	void pushMove(const cocos2d::Vec2 pos);
	void createRow();
	//cocos2d::Vec2 pointOnCubicBezier(cocos2d::Vec2* cp, float t);
private:

	Map_FormationRow _FormationRow;
	Vect_MoveInof _MoveList;
	BaseBuild* mSrcBuild;
	BaseBuild* mTarBuild; 
	eCampType mCamp;
	Node* mParent;
	int mContent;
	int mExpectNum;
	bool mFlage;
	
	std::vector<MoveInof> m_sTempMoveInfo;
	int m_iRowSpace;

	//TODO:set����FormationRow���ٶ�
	CC_SYNTHESIZE(float, _speed, Speed);
	//����Id
	CC_SYNTHESIZE(unsigned int, mId, Id);
	// ��ǰ�����ж�����ʿ��
	CC_SYNTHESIZE(float, mMaxRow, MaxRow);

	CC_SYNTHESIZE(float, mPreFrame, PreFrame);//��ǰ֡��

	CC_SYNTHESIZE(bool, mPause, Pause);
	

	double m_time;

	int m_RowIndex;

};


/*
�����ࣺ��FormationRow��
�����˵�ǰÿһ����MoveInfo��

��һ��Row��ɵ�ǰMoveInfo���䴫�������Row������ǰ���Row��ȡ��һ��MoveInfo
*/
class FormationRow
{
public:

	
	FormationRow() :mCurrentDis(0), next(0), spaceCloum(20), mTarBuild(0), readyEnd(0), mSrcBuild(0), m_IsLastRow(0), m_RowID(0), m_FormationID(0)
	{};
	~FormationRow();;

	void NextPostion();
	void addSoldier(Solider*);
	void enter(cocos2d::Node*);
	bool isNext(){ return next; };
	void setNext(bool n){ next = n; };
	void addMoveInfo(Formation::MoveInof); 
	void setDefault();
	int	 getUnit();

	//����Ƿ񱻷���������
	bool checkBeAttack(BaseBuild* pAttack);
	Formation::MoveInof getMoveInfo();
	void removeSoliderWithRect(const Rect& rect);

	void changeTarget(BaseBuild* pOld, BaseBuild* pNew);


	const Vect_Soldier&  getSoldierList(){ return mSoldierList; };

	
private:


	friend Formation;
	Vect_Soldier mSoldierList;
	float mCurrentDis;
	bool next;
	bool readyEnd;

	Formation::MoveInof mMoveInfo;

	Formation::Vect_MoveInof _MoveInfoList; 
	eCampType mCampType;
	CC_SYNTHESIZE(float, _speed, Speed);
	CC_SYNTHESIZE(float, spaceCloum, SpaceCloum);
	CC_SYNTHESIZE(int, m_IsLastRow, IsLastRow);
	CC_SYNTHESIZE(int, m_RowID, RowID);
	CC_SYNTHESIZE(int, m_FormationID, FormationID);
	CC_SYNTHESIZE(BaseBuild*, mTarBuild, TarBuild);
	CC_SYNTHESIZE(BaseBuild*, mSrcBuild, SrcBuild);
};