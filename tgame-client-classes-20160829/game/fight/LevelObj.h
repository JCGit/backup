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

	void updateMapInfoByLvID(unsigned int levelID, unsigned int sweepNum);	//根据关卡ID，扫荡次数更新关卡信息
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

	unsigned int _enterLevelID;	//进入关卡ID

	bool m_IsHasFriend;								//是否有好友助战
	vmsg::CSPlayerDetailInfo friendDetailInfo;		//好友详细信息

	vmsg::CSSweepWarRsp m_stSweepRsp;

	CC_SYNTHESIZE(unsigned int, _sweepLevelID, SweepLevelID);		//扫荡关卡ID
	CC_SYNTHESIZE(unsigned int, _sweepNum, SweepNum);				//扫荡数量

	unsigned int m_uMaxEliteWarMapID; // 最大已通关精英副本ID

	vmsg::CSEliteWarInfo m_stEliteWarInfo; // 当前精英副本信息

	vmsg::CSSweepEliteWarRsp m_stEliteSweepInfo;

};

#define P_LEVEL_OBJ CVLib::CVSingleton<CLevelObj>::Instance()

#endif // LevelObj_h__
