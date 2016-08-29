#include "FriendControl.h"
#include "cocos2d.h"
#include "city/CityControl.h"
#include "city/EntryID.h"
USING_NS_CC;

#include "game/notification/NotificationControl.h"

FriendControl::FriendControl()
    :m_pFriendProxy(NULL)
{
    m_stRsp.clear_adminreqlst();
    m_stRsp.clear_blacklst();
    m_stRsp.clear_friendlst();
    m_stRsp.clear_recvmanualuinlst();
    m_stRsp.clear_sendmanualuinlst();

    m_stProposeRsp.clear_friendlst();
    m_iFriendReqNum = 0;
	m_bPresentedManual = false;
}

FriendControl::~FriendControl()
{
    CC_SAFE_DELETE(m_pFriendProxy);
}

FriendProxy *FriendControl::getProxy() {
    if(m_pFriendProxy == NULL) {
        m_pFriendProxy = new FriendProxy();
    }

    return m_pFriendProxy;
}


FriendProposePanel* FriendControl::getProposePanel() {
    if(!m_stFriendProposePanel.isInited()) {
        m_stFriendProposePanel.init();
    }
    return &m_stFriendProposePanel;
}

FriendPanelUI* FriendControl::getFriendPanelUI() {
    if(!m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.init();
    }
    return &m_stFriendPanelUI;
}

FriendAddPanelUI* FriendControl::getFriendAddPanelUI() {
    if(!m_stFriendAddPanelUI.isInited()) {
        m_stFriendAddPanelUI.init();
    }
    return &m_stFriendAddPanelUI;
}

void FriendControl::doQueryRsp(vmsg::CSFriendQueryRsp *pstRsp) {
    if (NULL != pstRsp) {
        m_stRsp = *pstRsp;
        m_iFriendReqNum = m_stRsp.adminreqlst_size(); //update req num
        updateSendManualUinSet();
        updateRecvManualUinSet();
        
        if(m_stFriendPanelUI.isInited()) {
            m_stFriendPanelUI.updateUI();
        }

        doFriendReqNumNotify(m_iFriendReqNum);
    }
}

void FriendControl::doAdminAddRsp(const CSOneFriend *pAdminAddRsp) {
    if(NULL == pAdminAddRsp) {
        return;
    }

    //del from admin req list
    int iIdx = -1;
    for(int i = 0; i < m_stRsp.adminreqlst_size(); ++i) {
        if (pAdminAddRsp->uin() == m_stRsp.adminreqlst(i).uin()) {
            iIdx = i;
            m_stRsp.mutable_adminreqlst()->DeleteSubrange(iIdx, 1);
            m_iFriendReqNum = m_stRsp.adminreqlst_size(); //update req num
            break;
        }
    }

    //del from blacklist
    if(iIdx < 0) {
        for(int i = 0; i < m_stRsp.blacklst_size(); ++i) {
            if (pAdminAddRsp->uin() == m_stRsp.blacklst(i).uin()) {
                iIdx = i;
                m_stRsp.mutable_blacklst()->DeleteSubrange(iIdx, 1);
                break;
            }
        }
    }
    if(iIdx < 0) {
        return;
    }
    doFriendReqNumNotify(m_iFriendReqNum);
    CSOneFriend *pstOne = m_stRsp.add_friendlst();
    *pstOne = *pAdminAddRsp;
    
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateUI();
    }
}
void FriendControl::doAddBlackRsp(const CSOneFriend& stAddRsp)
{
    //del from friend list
    int iIdx = -1;
    for(int i = 0; i < m_stRsp.friendlst_size(); ++i) {
        if (stAddRsp.uin() == m_stRsp.friendlst(i).uin()) {
            iIdx = i;
            break;
        }
    }
    if(iIdx >= 0) {
        m_stRsp.mutable_friendlst()->DeleteSubrange(iIdx, 1);
        CSOneFriend *pstOne = m_stRsp.add_blacklst();
        *pstOne = stAddRsp;

        if(m_stFriendPanelUI.isInited()) {
            m_stFriendPanelUI.updateUI();
        }
    }
}

void FriendControl::doSendManualRsp(const CSSendManualRsp& stRsp)
{
    *(m_stRsp.mutable_sendmanualuinlst()) = stRsp.sendmanualuinlst();
    updateSendManualUinSet();

    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateUI();
    }
}

void FriendControl::doAdminRefuseUin(const UIN_t uUin) {
    int iIdx = -1;
    for(int i = 0; i < m_stRsp.adminreqlst_size(); ++i) {
        if (uUin == m_stRsp.adminreqlst(i).uin()) {
            iIdx = i;
            break;
        }
    }
    if(iIdx >= 0) {
        m_stRsp.mutable_adminreqlst()->DeleteSubrange(iIdx, 1);
        m_iFriendReqNum = m_stRsp.adminreqlst_size(); //update req num
        if(m_stFriendPanelUI.isInited()) {
            m_stFriendPanelUI.updateUI();
        }
    }
}


void FriendControl::doSendAddReqRsp(const UIN_t uUin) {
    m_stAddReqSet.insert(uUin);
    vmsg::CSProposeFriendRsp stNewData;
    stNewData.Clear();
    for (int i = 0; i < m_stProposeRsp.friendlst_size(); ++i) {
        if(m_stAddReqSet.count(m_stProposeRsp.friendlst(i).uin() ) <= 0) {
            *(stNewData.add_friendlst()) = m_stProposeRsp.friendlst(i);
        }
    }
    m_stProposeRsp = stNewData; //赋值

    CCLOG("[trace]FriendControl::doSendAddReqRsp, rsp %lu", uUin);
    if(m_stFriendProposePanel.isInited()) {
        m_stFriendProposePanel.updateUI();
    }
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateProposeRsp(); //更新推荐列表
    }
}


void FriendControl::doBatchAddReqRsp(const CSBatchAddUinLst& stBatchUinLst) {
    for(int i = 0; i < stBatchUinLst.uinlst_size(); ++i) {
        m_stAddReqSet.insert(stBatchUinLst.uinlst(i));
    }
    if(m_stFriendProposePanel.isInited()) {
        m_stFriendProposePanel.updateUI();
    }
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateUI();
    }
}

void FriendControl::doFriendReqNumNotify(const int iReqNum) {
    m_iFriendReqNum = iReqNum;
    CCLOG("[trace]FriendControl::doFriendReqNumNotify,req num %d", iReqNum);
    //P_CITY_CTRL->getModuleEntryLayout()->setEntryTip(ENTRY_FRIEND, iReqNum);
	notifyFriendAddOrRecvManual();
}

void FriendControl::doClearBlacklistRsp() {
    CCLOG("[trace]doClearBlacklistRsp");
    m_stRsp.clear_blacklst();
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateUI();
    }

}


void FriendControl::updateSendManualUinSet() {
    m_stSendManualUinSet.clear();
    for(int i = 0; i < m_stRsp.sendmanualuinlst_size(); ++i) {
        m_stSendManualUinSet.insert(m_stRsp.sendmanualuinlst(i));
    }
}

void FriendControl::updateRecvManualUinSet() {
    m_stRecvManualUinSet.clear();
    for(int i = 0; i < m_stRsp.recvmanualuinlst_size(); ++i) {
        m_stRecvManualUinSet.insert(m_stRsp.recvmanualuinlst(i));
    }

	notifyFriendAddOrRecvManual();
}

void FriendControl::doRecvManualRsp(const CSRecvManualRsp& stRsp) {
    m_stRsp.set_recvmanualcnt(stRsp.recvmanualcnt());
    *(m_stRsp.mutable_recvmanualuinlst()) = stRsp.recvmanualuinlst();
    updateRecvManualUinSet();
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateUI();
    }
}

void FriendControl::doProposeRsp(vmsg::CSProposeFriendRsp *pstRsp) {
    if (NULL == pstRsp) {
        return;
    }
    //处理一下，已经添加过的就不显示了
    vmsg::CSProposeFriendRsp stNewData;
    stNewData.Clear();
    for (int i = 0; i < pstRsp->friendlst_size(); ++i) {
        if(m_stAddReqSet.count(pstRsp->friendlst(i).uin() ) <= 0) {
            *(stNewData.add_friendlst()) = pstRsp->friendlst(i);
        }
    }
    m_stProposeRsp = stNewData; //赋值

    if(m_stFriendProposePanel.isInited()) {
        m_stFriendProposePanel.updateUI();
    }
    if(m_stFriendPanelUI.isInited()) {
        m_stFriendPanelUI.updateProposeRsp();
    }
}

void FriendControl::doDelFriendRsp(const UIN_t uUin) {
    int iIdx = -1;
    for(int i = 0; i < m_stRsp.friendlst_size(); ++i) {
        if (uUin == m_stRsp.friendlst(i).uin() ) {
            iIdx = i;
            break;
        }
    }
    if(iIdx >= 0) {
        m_stRsp.mutable_friendlst()->DeleteSubrange(iIdx, 1);
        if(m_stFriendPanelUI.isInited()) {
            m_stFriendPanelUI.updateUI();
        }
    }
}


bool FriendControl::isMyFriend(const UIN_t uUin) {
    for(int i = 0; i < m_stRsp.friendlst_size(); ++i) {
        if(uUin == m_stRsp.friendlst(i).uin() ) {
            return true;
        }
    }
    return false;
}

void FriendControl::notifyFriendAddOrRecvManual() {
	if(m_stRsp.adminreqlst_size() > 0
	|| (isCanRecvManul() && m_stRsp.recvmanualuinlst_size() > 0)) {
		PUSH_NOTIFICATION(ENTRY_FRIEND, NOTIFY_TAG_FRIEND_ADD_OR_RECV_MANUAL, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_FRIEND, NOTIFY_TAG_FRIEND_ADD_OR_RECV_MANUAL, 0);
	}

}

bool FriendControl::isPresentedManual() {
	return m_bPresentedManual;
}

void FriendControl::setPresentedManual(bool bPresentedManual) {
	m_bPresentedManual = bPresentedManual;
}
