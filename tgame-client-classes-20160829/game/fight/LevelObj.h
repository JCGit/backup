#ifndef LevelObj_h__
#define LevelObj_h__

#include "cocos2d.h"
#include "vlib/base/CVSingleton.h"
#include "CSWarMapMsg.pb.h"
#include "CSPlayerMsg.pb.h"

USING_NS_CC;

class CLevelObj : public CCObject
{
public:
	CLevelObj();
	virtual ~CLevelObj();

	void setMapInfo(const vmsg::CSWarMapQueryRsp& info);
	vmsg::CSWarMapQueryRsp& getMapInfo();

	void updateNewWarMapId();

	void setCardInfo(const vmsg::CSWarMapCardRsp& info);
	vmsg::CSWarMapCardRsp& getCardInfo();

	void setEnterLevelID(unsigned int var);
	unsigned int getEnterLevelID();

	void setFriendDetailInfo(const vmsg::CSPlayerDetailInfo& stMsg);
	vmsg::CSPlayerDetailInfo& getFriendDetailInfo();
	void resetFriendDetailInfo();
	bool getIsHasFriend();
	void setIsHasFriend(bool var);

	void setSweepInfo(const vmsg::CSSweepWarRsp& stMsg);
	const vmsg::CSSweepWarRsp& getSweepInfo();

	void updateMapInfoByLvID(unsigned int levelID, unsigned int sweepNum);	//���ݹؿ�ID��ɨ���������¹ؿ���Ϣ
	void doBuyFightCntRsp(const vmsg::CSBuyInstanceRsp& stRsp);

	unsigned int getMaxEliteWarMapID();
	void setMaxEliteWarMapID(unsigned int uMaxEliteWarMapID);

	const vmsg::CSEliteWarInfo& getEliteWarInfo();
	void setEliteWarInfo(const vmsg::CSEliteWarInfo& stEliteWarInfo);

	const vmsg::CSSweepEliteWarRsp& getEliteSweepInfo();
	void setEliteSweepInfo(const vmsg::CSSweepEliteWarRsp& stEliteSweepInfo);

protected:
private:
	vmsg::CSWarMapQueryRsp mapInfo;
	vmsg::CSWarMapCardRsp cardInfo;	

	unsigned int _enterLevelID;	//����ؿ�ID

	bool m_IsHasFriend;								//�Ƿ��к�����ս
	vmsg::CSPlayerDetailInfo friendDetailInfo;		//������ϸ��Ϣ

	vmsg::CSSweepWarRsp m_stSweepRsp;

	CC_SYNTHESIZE(unsigned int, _sweepLevelID, SweepLevelID);		//ɨ���ؿ�ID
	CC_SYNTHESIZE(unsigned int, _sweepNum, SweepNum);				//ɨ������

	unsigned int m_uMaxEliteWarMapID; // �����ͨ�ؾ�Ӣ����ID

	vmsg::CSEliteWarInfo m_stEliteWarInfo; // ��ǰ��Ӣ������Ϣ

	vmsg::CSSweepEliteWarRsp m_stEliteSweepInfo;

};

#define P_LEVEL_OBJ CVLib::CVSingleton<CLevelObj>::Instance()

#endif // LevelObj_h__
