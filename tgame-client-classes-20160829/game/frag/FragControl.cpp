#include "FragControl.h"
#include "DebrisCfgMaster.h"
#include "Global.h"
#include "basic/PopTipLayer.h"

FragControl::FragControl()
:m_pFragPanel(NULL)
{
}

FragControl::~FragControl()
{
	finalize();
}


void FragControl::finalize() {
    CC_SAFE_RELEASE_NULL(m_pFragPanel); 

}

FragPanel* FragControl::getFragPanel() {
    if(m_pFragPanel == NULL) {
        m_pFragPanel = FragPanel::create();
        m_pFragPanel->retain(); // release at FragControl::finalize()
    }

	return m_pFragPanel;
}


const CSDebrisInfo* FragControl::getOneFragByIdx(const int iBagIdx) {
    if(iBagIdx >= 0 && iBagIdx < m_stBagInfo.debrisinfos_size()) {
        return &(m_stBagInfo.debrisinfos(iBagIdx));
    }
    return NULL;
}

void FragControl::setDebrisBagInfo(const CSDebrisBagInfo& stInfo, const bool bIsComposeRsp) {
    m_stBagInfo = stInfo;
    m_stTypeMap.clear();
    m_stNumMap.clear();
    m_stCfgTypeMap.clear();

    for(int i = 0; i < m_stBagInfo.debrisinfos_size(); ++i) {
        const CSDebrisInfo& oneInfo = m_stBagInfo.debrisinfos(i);
        const DebrisCfg* pstCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(oneInfo.debriscfgid());
        if(NULL == pstCfg) {
            continue;
        }
        m_stTypeMap[pstCfg->composetype()].push_back(i); //index
        m_stNumMap[oneInfo.debriscfgid()] += oneInfo.debriscnt(); //num map

		//if (bIsComposeRsp) {

		//	if (vmsg::THING_TYPE_COIN != pstCfg->composetype()) {
		//		if (vmsg::THING_TYPE_WEAPON == pstCfg->composetype()) {
		//			P_GLOBAL->showAdvItemEff(pstCfg->composetype(), pstCfg->composecfgid()); //显示高级物品获得特效
		//		}
		//		else {
		//			POP_TIP_LAYER_MASTER->ShowAwardTip(pstCfg->composetype(), pstCfg->composecfgid(), 1);
		//		}
		//	}
		//}
    }

	if(bIsComposeRsp)
	{
		 const DebrisCfg* pstCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(m_composeDebrisID);
		 if(vmsg::THING_TYPE_WEAPON == pstCfg->composetype())
		 {
			 P_GLOBAL->showAdvItemEff(pstCfg->composetype(), pstCfg->composecfgid()); //显示高级物品获得特效
		 }else
		 {
			 POP_TIP_LAYER_MASTER->ShowAwardTip(pstCfg->composetype(), pstCfg->composecfgid(), 1);
		 }
	}

    for(auto iter = m_stTypeMap.begin(); iter != m_stTypeMap.end(); ++iter) {
        vector<int>& vec = iter->second;
        sort(vec.begin(), vec.end(), compareDebris);
    }

    //过滤出我没有的碎片CfgID list
    int iThingType = 0;
    int iCfgID = 0;
    const map<int, vector<int> >& stCfgTypeMap = DEBRIS_CFG_MASTER->getTypeMap();
    for(auto iter = stCfgTypeMap.begin(); iter != stCfgTypeMap.end(); ++iter ) {
        iThingType = iter->first;
        const vector<int>& stCfgTypeList = iter->second;

        for(unsigned int k = 0; k < stCfgTypeList.size(); ++k ) {
            iCfgID = stCfgTypeList[k];

            if(m_stNumMap.count(iCfgID) <= 0) {
                //我没有的
                m_stCfgTypeMap[iThingType].push_back(iCfgID);
            }
        }
    }

    if(m_pFragPanel && m_pFragPanel->isOpen() ) {
        m_pFragPanel->updateUI();
	}
}
bool FragControl::compareDebris(const int iAIndex, const int iBIndex) {
    const CSDebrisInfo* aInfo = P_FRAG_CTRL->getOneFragByIdx(iAIndex);
    const CSDebrisInfo* bInfo = P_FRAG_CTRL->getOneFragByIdx(iBIndex);
    /*
    当碎片都不可合成并碎片数量不同时，碎片数量越多，则碎片排序越靠前；
    当碎片都不可合成并碎片数量相同时，碎片对应枪械或装备星级越高，则碎片排序越靠前；
    当有几种碎片可合成时，碎片对应枪械或装备星级越高，则碎片排序越靠前；
    */
    const DebrisCfg* aCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(aInfo->debriscfgid()); 
    const DebrisCfg* bCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(bInfo->debriscfgid());
    
    const bool aIsEnough = (aInfo->debriscnt() >= aCfg->composeneedcnt());
    const bool bIsEnough = (bInfo->debriscnt() >= bCfg->composeneedcnt());

    const int aStar = DEBRIS_CFG_MASTER->getComposeThingStar(aInfo->debriscfgid());
    const int bStar = DEBRIS_CFG_MASTER->getComposeThingStar(bInfo->debriscfgid());

    if( aIsEnough ) {
        if(bIsEnough) {
            //星级高的
            return (aStar > bStar);
        } else {
            return true;
        }
    } else if (bIsEnough) {
        return false;
    } else {
        //both not enough
        if(aInfo->debriscnt() == bInfo->debriscnt()) {
            return (aStar > bStar);
        } else {
            return (aInfo->debriscnt() > bInfo->debriscnt() );
        }
    }
    return false;
}

const vector<int>* FragControl::getTypeIndexList(const int iThingType) {
    if(m_stTypeMap.count(iThingType) ) {
        return &(m_stTypeMap[iThingType]);
    }
    return NULL;

}


const int FragControl::getBagIdxByCfgID(const int iCfgID) {
    for(int i = 0; i < m_stBagInfo.debrisinfos_size(); ++i) {
        const CSDebrisInfo& oneInfo = m_stBagInfo.debrisinfos(i);
        if(iCfgID == oneInfo.debriscfgid()) {
            return i;
        }
    }
    return -1;
}

int FragControl::getNumByCfgID(const int iCfgID) {
    if(m_stNumMap.count(iCfgID)) {
        return m_stNumMap[iCfgID];
    }
    return 0;
}

const vector<int>* FragControl::getTypeCfgList(const int iThingType) {
    if(m_stCfgTypeMap.count(iThingType) ) {
        return &(m_stCfgTypeMap[iThingType]);
    }
    return NULL;
}

void FragControl::sendDebrisCompose(const int iCfgID)
{
	m_stFragProxy.sendDebrisComposeRqst(iCfgID);
	m_composeDebrisID = iCfgID;
}


