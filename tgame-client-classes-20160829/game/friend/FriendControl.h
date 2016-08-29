#ifndef __FRIEND_CONTROL_h__
#define __FRIEND_CONTROL_h__

#include "FriendProxy.h"
#include "FriendProposePanel.h"
#include "FriendPanelUI.h"
#include "FriendAddPanelUI.h"

using namespace vmsg;


class FriendControl
{
public:
    FriendControl();
    ~FriendControl();

    FriendProxy* getProxy();

    FriendProposePanel* getProposePanel();

    FriendPanelUI* getFriendPanelUI();

    FriendAddPanelUI* getFriendAddPanelUI();

    void doQueryRsp(vmsg::CSFriendQueryRsp *pstRsp);
    vmsg::CSFriendQueryRsp& getFriendRsp() {
        return m_stRsp;
    }

    void doProposeRsp(vmsg::CSProposeFriendRsp *pstRsp);
    vmsg::CSProposeFriendRsp& getProposeRsp() {
        return m_stProposeRsp;
    }

    void doAdminAddRsp(const CSOneFriend *pAdminAddRsp);

    void doDelFriendRsp(const UIN_t uUin);

    void doAddBlackRsp(const CSOneFriend& stAddRsp);
    void doSendManualRsp(const CSSendManualRsp& stRsp); //赠送体力回包
    void doAdminRefuseUin(const UIN_t uUin);

    void doSendAddReqRsp(const UIN_t uUin); //处理发送好友申请的回包,从推荐列表里删除

    bool isInReq(const UIN_t uUin) {
        return m_stAddReqSet.count(uUin) > 0;
    }
    void doBatchAddReqRsp(const CSBatchAddUinLst& stBatchUinLst); 

    int getFriendReqNum() {
        return m_iFriendReqNum;
    }
    void doFriendReqNumNotify(const int iReqNum);
    void doClearBlacklistRsp();
  
    const std::set<UIN_t>& getSendManualUinSet() {
        return m_stSendManualUinSet;
    }
    const std::set<UIN_t>& getRecvManualUinSet() {
        return m_stRecvManualUinSet;
    }
    void doRecvManualRsp(const CSRecvManualRsp& stRsp); //收取体力回包
    bool isCanRecvManul() {
        return (m_stRsp.recvmanualcnt() < 10); //每天对多收10次
    }

    bool isMyFriend(const UIN_t uUin);

	void notifyFriendAddOrRecvManual();

	bool isPresentedManual();

	void setPresentedManual(bool bPresentedManual);

private:
    void updateSendManualUinSet();
    void updateRecvManualUinSet();
private:
    FriendProxy *m_pFriendProxy;

    vmsg::CSFriendQueryRsp m_stRsp; //好友数据
    vmsg::CSProposeFriendRsp m_stProposeRsp; //好友推荐数据

    std::set<UIN_t> m_stAddReqSet; //已经发送加好友申请的uin集合
    std::set<UIN_t> m_stSendManualUinSet; //已经赠送体力的uin集合
    std::set<UIN_t> m_stRecvManualUinSet; //可以收取体力的uin集合

    int m_iFriendReqNum; //当前请求加我好友的人数

    FriendPanelUI m_stFriendPanelUI; //新的好友面板
    FriendAddPanelUI m_stFriendAddPanelUI; //输入名字面板
    FriendProposePanel m_stFriendProposePanel; //推荐面板

	bool m_bPresentedManual; // 赠送了体力

};

#define P_FRIEND_CTRL CVLib::CVSingleton<FriendControl>::Instance()

#endif // __FRIEND_CONTROL_h__
