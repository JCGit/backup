#include "ErrorCodeCfgMaster.h"

#include <set>

using std::set;


const char* ErrorCodeCfgMaster::getErrDesc(const int iError) {
    map<unsigned int, int>::const_iterator iter = m_stID2Idx.find(iError);

    if(iter == m_stID2Idx.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u !", __PRETTY_FUNCTION__, iError));

        return NULL; // 找不到相应配置
    }

    return m_stCfg.mutable_errorcode(iter->second)->desc().c_str();
}

int ErrorCodeCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.errorcode_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.errorcode(iIdx).id();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int ErrorCodeCfgMaster::SetCfg() {
	int iRet = 0;

	for(int iIdx = 0; iIdx < m_stCfg.errorcode_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.errorcode(iIdx).id();

		if(m_stID2Idx.insert(make_pair(uCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1;//id重复
		}
	}

	return iRet;
}
