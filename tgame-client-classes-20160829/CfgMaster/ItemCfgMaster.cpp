#include "ItemCfgMaster.h"

#include <set>

using std::set;

int CItemCfgMaster::ValidateCfg()const
{
    int iRet = 0;


    set<unsigned int> stItemIDSet;

    for(int iIdx = 0; iIdx < m_stCfg.itemcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.itemcfg(iIdx).itemid();
        if(stItemIDSet.insert(uiCfgID).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }

        //to add other validate check, such as skill etc.
    }
    return iRet;
}

int CItemCfgMaster::SetCfg()
{
    int iRet = 0;

    for(int iIdx = 0; iIdx < m_stCfg.itemcfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.itemcfg(iIdx).itemid();
        if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//id重复
        }

		unsigned int uiType = m_stCfg.itemcfg(iIdx).itemtype();
		map<int, vector<unsigned int> >::iterator iter = m_stType2Items.find(uiType);

		if(iter == m_stType2Items.end()){
			vector<unsigned int> stItems;
			stItems.clear();
			stItems.push_back(uiCfgID);
			m_stType2Items[uiType] = stItems;
		}else{
			vector<unsigned int> stItems = iter->second;
			stItems.push_back(uiCfgID);
			m_stType2Items[uiType] = stItems;
		}
    }

    return iRet;
}

const ItemCfg* CItemCfgMaster::GetItemCfgByID(const unsigned int uiCfgID)
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_WARNING(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return NULL; //找不到该物品配置
    }

    return &(m_stCfg.itemcfg(it->second));
}

const vector<unsigned int>* CItemCfgMaster::GetItemsByType(unsigned int uiType){
	map<int, vector<unsigned int> >::iterator iter = m_stType2Items.find(uiType);

	if(iter == m_stType2Items.end())
	{
		CV_WARNING(("%s, can not find items by type = %u", __PRETTY_FUNCTION__, uiType));
		return NULL; //找不到该类型
	}

	return &(iter->second);
}

const char* CItemCfgMaster::GetItemIcon(const unsigned int uiCfgID) {
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_WARNING(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return "default"; //找不到该物品配置
    }
    return (m_stCfg.itemcfg(it->second)).itemicon().c_str();
}

const char* CItemCfgMaster::GetItemName(const unsigned int uiCfgID) {
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_WARNING(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return ""; //找不到该物品配置
    }
    return (m_stCfg.itemcfg(it->second)).itemname().c_str();
}


int CItemCfgMaster::GetItemColor(const unsigned int uiCfgID) {
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_WARNING(("%s, can not find cfgid = %u item", __PRETTY_FUNCTION__, uiCfgID));
        return 0; //找不到该物品配置
    }
    return (m_stCfg.itemcfg(it->second)).itemcolor();
}
