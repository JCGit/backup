#include "DailyInstanceCfgMaster.h"

int DailyInstanceCfgMaster::ValidateCfg() const {
    int iRet = 0;

    set<unsigned int> stCfgIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.instancecfg_size(); iIdx++)
    {
        unsigned int uCfgID = m_stCfg.instancecfg(iIdx).key();

        if(stCfgIDSet.insert(uCfgID).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

            return -1; // id重复
        }

        //to add other validate check, such as skill etc.
    }
    return iRet;

}

int DailyInstanceCfgMaster::SetCfg() {
    int iRet = 0;
    m_stDayMap.clear();

    int iDay = 0;
    int iKey = 0;
    for(int iIdx = 0; iIdx < m_stCfg.instancecfg_size(); iIdx++)
    {
        const DailyInstanceCfg &stOneCfg = m_stCfg.instancecfg(iIdx);
        iKey = stOneCfg.key();
        for (int i = 0; i < stOneCfg.openday_size(); ++i)
        {
            iDay = stOneCfg.openday(i);
            m_stDayMap[iDay].insert(iKey);
        }
    }

    return iRet;
}

int DailyInstanceCfgMaster::getInstanceSize() {
    return m_stCfg.instancecfg_size();
}

const DailyInstanceCfg* DailyInstanceCfgMaster::getOneCfg(const int iKey) {
    int idx = iKey - 1;
    if(idx >= 0 && idx < m_stCfg.instancecfg_size()) {
        return &(m_stCfg.instancecfg(idx));
    } else {
        return NULL;
    }
}

void DailyInstanceCfgMaster::sortTodayInstance(const int iToday, vector<int>& stResult) {
    int iKey = 0;
    stResult.clear();
    if(m_stDayMap.count(iToday) <= 0) {
        
        return;
    }
    const set<int>& stTodayKeySet = m_stDayMap[iToday];
    for(auto iter = stTodayKeySet.begin(); iter != stTodayKeySet.end(); ++iter) {
        stResult.push_back(*iter);
    }
    
    for(int iIdx = 0; iIdx < m_stCfg.instancecfg_size(); iIdx++)
    {
        iKey = m_stCfg.instancecfg(iIdx).key();
        if(stTodayKeySet.count(iKey)) {
            continue;
        }        
        //把没有的加进去
        stResult.push_back(iKey);
    }
}

bool DailyInstanceCfgMaster::isInstanceOpen(const int iToday, const int iCfgKey) {
    if(m_stDayMap.count(iToday) <= 0) {
        return false;
    }
    return m_stDayMap[iToday].count(iCfgKey) > 0;
}
