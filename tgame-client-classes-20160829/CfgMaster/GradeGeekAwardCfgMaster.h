#ifndef GradeGeekAwardCfgMaster_h__
#define GradeGeekAwardCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "GradeGeekAwardCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class GradeGeekAwardCfgMaster : public CCfgMasterBase<GradeGeekAwardCfgSet>
{
public:
	GradeGeekAwardCfgMaster(){}
	~GradeGeekAwardCfgMaster(){}

	const GradeGeekAwardCfg* getGradeGeekAwardCfg(unsigned int uLevel);
	unsigned int	getGradeGeekAwardTypeNum();


protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2IDX.clear(); 
	}

private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

};

#define GradeGeekAward_CFG_MASTER (CVSingleton<GradeGeekAwardCfgMaster>::Instance())

#endif // GradeGeekAwardCfgMaster_h__