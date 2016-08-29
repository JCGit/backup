#ifndef WarMapCfgMaster_h__
#define WarMapCfgMaster_h__

#include "CfgMasterBase.h"
#include "WarMapCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWarMapCfgMaster : public CCfgMasterBase<WarMapCfgSet>
{
public:
	CWarMapCfgMaster(){}
	~CWarMapCfgMaster(){}

	const vector<WarMapCfg*>* getWarChapter(unsigned int uChapterID);

	WarMapCfg* GetWarMapCfgByID(const unsigned int uiCfgID);
	pair<unsigned int, unsigned int> GetIDXByWarID(const unsigned int uiWarID);

	unsigned int GetTotalMonsterNum();
	unsigned int GetTotalItemNum();
	unsigned int GetNormalSize();//副本数量
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stWarChapters.clear();
        m_stWarID2IDx.clear();
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

	map<unsigned int, vector<WarMapCfg*> > m_stWarChapters; // <ChapterID, Chapter>

	map<unsigned int, pair<unsigned int, unsigned int> > m_stWarID2IDx;		//战役ID对应起始关卡和最后关卡

	unsigned int m_stTotalMonsterNum;			//怪物总数
	unsigned int m_stTotalItemNum;				//item总数
};

#define  WAR_MAP_CFG (CVSingleton<CWarMapCfgMaster>::Instance())

#endif // WarMapCfgMaster_h__
