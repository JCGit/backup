#ifndef LoginObj_h__
#define LoginObj_h__

#include "vlib/base/CVSingleton.h"
#include "CSPlayerMsg.pb.h"

using namespace std;

enum eSvrStatus
{
	ESS_NOT_SERVICES = 0,	//未开服
	ESS_IN_SERVICES,		//已开服
	ESS_STOP_SERVICES,		//停机维护
	ESS_RECOMMEND,			//新服
	ESS_CROWED,				//拥挤
	ESS_FULL,				//爆满
};

typedef struct svr_info {
	unsigned int zoneid;
	string name;
	string ip;
	string status;
	unsigned int iFlag;
	string szTime;
} SvrInfo;

typedef struct {
	string cdn;
	string notification;
	bool isstop;
	string whitelist;
} DirSvrInfo;

class LoginObj
{
public:
    
	LoginObj();
	~LoginObj();

	std::vector<vmsg::CSPlayerRoleInfo>& getRoleInfoList();

	std::vector<SvrInfo>& getSvrInfoList();

	const SvrInfo& getCurSvrInfo();

	void setCurSvrInfo(const SvrInfo &stSvrInfo);

	const DirSvrInfo& getDirSvrInfo();

	void setDirSvrInfo(const DirSvrInfo &stDirSvrInfo);

	std::vector<SvrInfo>& getRecentLoginSvrInfos();

	void updateRecentLoginSvrInfo();

	void setRecentLoginRoleName(std::string nameStr);

	std::vector<vmsg::CSPlayerRoleInfo> getRecentRoleInfoList();

	bool checkInRoleList(std::string nameStr);

	const vmsg::CSPlayerInfo& getOtherPlayerInfo();

	void setOtherPlayerInfo(const vmsg::CSPlayerInfo &stPlayerInfo);

	const vmsg::CSPlayerDetailInfo& getOtherPlayerDetailInfo();

	void setOtherPlayerDetailInfo(const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo);

    void setStartMovieDone(); //设置 已经播放过漫画
    bool getIsStartMovieDone(); //是否已经播放过漫画了，(输入帐号进入游戏出现)。

    void resetOnConnClose();
private:
	std::vector<vmsg::CSPlayerRoleInfo> m_stRoleInfoList;

	std::vector<SvrInfo> m_stSvrInfoList;

	SvrInfo m_stCurSvrInfo;

	DirSvrInfo m_stDirSvrInfo;

	std::vector<SvrInfo> m_stRecentLoginSvrInfos;

	vmsg::CSPlayerInfo m_stOtherPlayerInfo;

	vmsg::CSPlayerDetailInfo m_stOtherPlayerDetailInfo;
};

#define P_LOGIN_OBJ CVLib::CVSingleton<LoginObj>::Instance()

#define MAX_RECENT_LOGIN_SVR_NUM 3

#endif // LoginObj_h__
