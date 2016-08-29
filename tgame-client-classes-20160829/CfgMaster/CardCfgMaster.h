#ifndef CardCfgMaster_h__
#define CardCfgMaster_h__

#include "CfgMasterBase.h"
#include "CardCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CCardCfgMaster : public CCfgMasterBase<CardCfgSet>
{
public:
	CCardCfgMaster(){}
	~CCardCfgMaster(){}

	CardCfg* GetCardCfgByID(const unsigned int uiCfgID);
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

#define CARD_CFG_MASTER (CVSingleton<CCardCfgMaster>::Instance())

#endif // CardCfgMaster_h__
