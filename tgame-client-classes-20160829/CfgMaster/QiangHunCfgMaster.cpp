#include "QiangHunCfgMaster.h"

#include <set>

using std::set;


int CQiangHunCfgMaster::SetCfg()
{
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.qianghuncfg_size(); iIdx++)
	{
		unsigned int layer = m_stCfg.qianghuncfg(iIdx).layer();
		if(m_stID2IDX.find(layer) == m_stID2IDX.end()){
			set<int> stTmp;
			stTmp.insert(iIdx);
			m_stID2IDX[layer] = stTmp;
		}else{
			m_stID2IDX[layer].insert(iIdx);
		}
	}

	return iRet;
}

const QiangHunCfg* CQiangHunCfgMaster::GetCfgByLayerAndLev(const int layer, const unsigned int lev){
	set<int> stVec = m_stID2IDX[layer];
	set<int>::iterator stIter;
	for(stIter = stVec.begin(); stIter != stVec.end(); stIter++){
		const QiangHunCfg& stCfg = m_stCfg.qianghuncfg(*stIter);
		if(lev >= stCfg.minlevel() && lev <= stCfg.maxlevel()){
			return &stCfg;
		}
	}

	return NULL;
}

