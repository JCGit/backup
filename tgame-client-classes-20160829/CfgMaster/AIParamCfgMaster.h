#ifndef AIParamCfg_H__
#define AIParamCfg_H__

#include "AIParamCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CAIParamCfgMaster : public CCfgMasterBase<AIParamCfgSet>
{
public:

	CAIParamCfgMaster(){}
	~CAIParamCfgMaster(){}

	const AIParamCfg* GetAICfgByID(const unsigned int uCfgID);

protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;
};

#define AI_PARAM_CFG_MASTER (CVSingleton<CAIParamCfgMaster>::Instance())

#endif

