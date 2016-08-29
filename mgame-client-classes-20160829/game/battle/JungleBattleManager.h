/**
* @file     JungleBattleManager.h
* @brief    ��Ұϵͳ
* @details  
* @author   Alvin
* @date     2015-8-17
*/
#ifndef __JUNGLEBATTLEMANAGER_H__
#define __JUNGLEBATTLEMANAGER_H__
#include "cocos2d.h"
#include "Message.h"
#include "battle/Build.h"
#include "data/Tables.h"
USING_NS_CC;
class JungleBattleInfo
{
public:
	JungleBattleInfo();
	~JungleBattleInfo();

private:
	CC_SYNTHESIZE(int, m_SrcID, SrcID);
	CC_SYNTHESIZE(int, m_DestID, DestID); //���洢��Ұ����ID
	CC_SYNTHESIZE(int, m_FormationID, FormationID);
};

typedef std::pair<int, int> MyKey;

class MyPredicator
{
public:
	MyPredicator(){};
	~MyPredicator(){};

	bool operator()(const MyKey& v1, const MyKey& v2)
	{
		if( v1.first * 1e8 + v1.second  < v2.first * 1e8 + v2.second)
		{
			return false;
		}
		return true;
	}
};


class JungleBattleManager: public MessageHandler
{
public:
	static JungleBattleManager* getInstance();
	JungleBattleManager();
	~JungleBattleManager();

	//�ͷŵ���
	static void cleanJungleBattleManager(); 

	bool init();
	void update(float delta);

	//������Ұ����
	void createJungleBuild(int buildID, int locationID, int buffID);  

	//��Ӫ�Ӵ�Ұ��������
	void returnFromJungleBuild(int srcID, int destID, int srcCamp, int destCamp);

	//������ӪBuff
	void addJungleBuildBuff(int camp, int jungleBuildID);

	void onReceiveMassage(GameMessage * message);

	//��������
	void clearJungleBattleData(int gateID);

	
private:
	static JungleBattleManager* m_Instance; 
	
	typedef std::map<int, JungleData*>  JungleBuildMap;
	JungleBuildMap m_JungleBuildMap;
	JungleBuildMap m_JungleBuildMap2;

	std::queue<int> m_ClearJungleQuence;

	//�����ڴ�Ұ��������
	int m_MaxBuildCount;
	//ÿ��ˢ�´�Ұ��������
	int m_RefreshCount;
	//��ǰ��Ұ��������
	int m_JungleBuildCount;
	

	//��ǰ�ȴ�ˢ��ʱ��
	float m_WaitRefreshTime;
	//ˢ��ʱ��
	float m_RefreshTime;

	
	
	int m_State;

	//�洢���Ұ���������ľ�����Ϣ
	typedef std::vector<JungleBattleInfo* > JungleBattleInfoVec;
	JungleBattleInfoVec m_JungleBattleInfoVec;

	//�洢�ܵ����������Ĵ�Ұ������
	typedef std::map<int, int> JungleBuildRefMap;
	JungleBuildRefMap m_JungleBuildRefMap;

	//��Ұ����ˢ�±�
	typedef std::vector<JungleData* > JungleDataVec;
	JungleDataVec m_JungleDataVec; 
private:
	void addJungleBattleInfo(int fromationID, int srcID, int destID);
	void removeJungleBattleInfo(int fromationID,int srcID, int destID, int srcCamp, int destCamp, int buildID, int locationID);
	void closeJungleBattleInfo(int srcID, int srcCamp, int destCamp, int buildID, int locationID);
	void clearJungleBuildRefresh(int buildID, int locationID);
}; 
 
#endif