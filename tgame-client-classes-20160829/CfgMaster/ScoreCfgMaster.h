#ifndef ScoreCfgMaster_h__
#define ScoreCfgMaster_h__

#include "CfgMasterBase.h"
#include "ScoreCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CScoreCfgMaster : public CCfgMasterBase<ScoreCfgSet>
{
public:
	CScoreCfgMaster(){}
	~CScoreCfgMaster(){}

	ScoreCfg* GetScoreCfgByID(const unsigned int uiCfgID);
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

#define SCORE_CFG_MASTER (CVSingleton<CScoreCfgMaster>::Instance())

#endif // ScoreCfgMaster_h__
