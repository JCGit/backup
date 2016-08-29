#include "EquipmentCfgMaster.h"

#include <set>
//#include "SubHeroCfgMaster.h"

using std::set;

int CEquipmentCfgMaster::ValidateCfg()const
{
	int iRet = 0;


	set<unsigned int> stEquipIDSet;
	
	for(int iIdx = 0; iIdx < m_stCfg.equipcfg1_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equipcfg1(iIdx).equipcfgid();
		if(stEquipIDSet.insert(uiCfgID).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__FUNCTION__, uiCfgID));
			return -1;//id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CEquipmentCfgMaster::SetCfg()
{
	int iRet = 0;
	m_stID2IDX.clear();
    m_stStarStepID2Idx.clear();

	for(int iIdx = 0; iIdx < m_stCfg.equipcfg1_size(); iIdx++)
	{
		unsigned int uiCfgID = m_stCfg.equipcfg1(iIdx).equipcfgid();
		if(m_stID2IDX.insert(make_pair(uiCfgID, iIdx)).second == false)
		{
			CV_ERROR(("%s, cfgid = %u is dup cfg id !",
				__FUNCTION__, uiCfgID));
			return -1;//id重复
		}

        unsigned int uiKey = MakeKey(m_stCfg.equipcfg1(iIdx).equipid(), 
            m_stCfg.equipcfg1(iIdx).initstar(),
            m_stCfg.equipcfg1(iIdx).initstep());
        if(m_stStarStepID2Idx.insert(make_pair(uiKey, iIdx)).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return ERET_SYS_ERR;
        }
	}
	
	return iRet;
}

const EquipmentCfg* CEquipmentCfgMaster::GetEquipCfgByID(const unsigned int uiCfgID)
{
	map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

	if(it == m_stID2IDX.end())
	{
		CV_ERROR(("%s, can not find cfgid = %u equipment", __PRETTY_FUNCTION__, uiCfgID));
		return NULL; //找不到该武将配置
	}
	return &(m_stCfg.equipcfg1(it->second));
}

unsigned int CEquipmentCfgMaster::GetHoleCnt(const unsigned int uiCfgID)const
{
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u equipment", __PRETTY_FUNCTION__, uiCfgID));
        return 0; //找不到该武将配置
    }
    /*//TODO:EQUIP
    switch (m_stCfg.equipcfg1(it->second).color())
    {
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
        return 4;
    case 4:
        return 5;
    default:
        return 0;
    }
    */
    return 0;
}


bool CEquipmentCfgMaster::CheckJob( unsigned int uiEquipCfgID, unsigned int uiJobID )
{
    const EquipmentCfg* pstEquipCfg = GetEquipCfgByID(uiEquipCfgID);
    if (pstEquipCfg == NULL)
    {
        CV_WARNING(("%s, get equipment cfg %u fail", __FUNCTION__, uiEquipCfgID));
        return false;
    }
    /*
    for (int iIdx = 0; iIdx < pstEquipCfg->jobidlst_size(); iIdx++)
    {
        if (pstEquipCfg->jobidlst(iIdx) == uiJobID)
        {
            return true;
        }
    }
    */
    return false;
}


const std::string  CEquipmentCfgMaster::GetEquipName(const unsigned int uiCfgID)
{

    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u equipment", __PRETTY_FUNCTION__, uiCfgID));
        return ""; //找不到该武将配置
    }

    return m_stCfg.mutable_equipcfg1(it->second)->equipname();
}


const std::string  CEquipmentCfgMaster::GetIconPath(const unsigned int uiCfgID) {

    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u equipment", __PRETTY_FUNCTION__, uiCfgID));
        return ""; //找不到该武将配置
    }


    std::string strIconURL = "item/";
    strIconURL.append( m_stCfg.mutable_equipcfg1(it->second)->icon() );
    strIconURL.append(".png");

    return strIconURL;
}

const int CEquipmentCfgMaster::GetEquipColor(const unsigned int uiCfgID) {
    map<unsigned int, int>::const_iterator it = m_stID2IDX.find(uiCfgID);

    if(it == m_stID2IDX.end())
    {
        CV_ERROR(("%s, can not find cfgid = %u equipment", __PRETTY_FUNCTION__, uiCfgID));
        return 1; //找不到该武将配置
    }
    return m_stCfg.mutable_equipcfg1(it->second)->color();
}


//使用的是uiEquipID，不是CfgID！！！
const EquipmentCfg* CEquipmentCfgMaster::GetEquipCfgByStarStep(unsigned int uiEquipID, unsigned int uiStar, unsigned int uiStep)const
{
    unsigned int uiKey = MakeKey(uiEquipID, uiStar, uiStep);
    map<unsigned int, int>::const_iterator it = m_stStarStepID2Idx.find(uiKey);

    if(it == m_stStarStepID2Idx.end())
    {
        CV_ERROR(("%s, can not find equipid %u star %u step %u item", __PRETTY_FUNCTION__, uiEquipID, uiStar, uiStep));
        return NULL; 
    }

    return &(m_stCfg.equipcfg1(it->second));
}

unsigned int CEquipmentCfgMaster::MakeKey(unsigned int uiEquipID, unsigned int uiStar, unsigned int uiStep)
{
    return ((uiEquipID & 0xFFFFFFFF ) << 8)
        | ((uiStar & 0xFFFFFFFF ) << 4)
        | (uiStep & 0xFFFFFFFF );
}


