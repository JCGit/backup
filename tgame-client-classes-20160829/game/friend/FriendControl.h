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
    void doSendManualRsp(const CSSendManualRsp& stRsp); //���������ذ�
    void doAdminRefuseUin(const UIN_t uUin);

    void doSendAddReqRsp(const UIN_t uUin); //�����ͺ�������Ļذ�,���Ƽ��б���ɾ��

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
    void doRecvManualRsp(const CSRecvManualRsp& stRsp); //��ȡ�����ذ�
    bool isCanRecvManul() {
        return (m_stRsp.recvmanualcnt() < 10); //ÿ��Զ���10��
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

    vmsg::CSFriendQueryRsp m_stRsp; //��������
    vmsg::CSProposeFriendRsp m_stProposeRsp; //�����Ƽ�����

    std::set<UIN_t> m_stAddReqSet; //�Ѿ����ͼӺ��������uin����
    std::set<UIN_t> m_stSendManualUinSet; //�Ѿ�����������uin����
    std::set<UIN_t> m_stRecvManualUinSet; //������ȡ������uin����

    int m_iFriendReqNum; //��ǰ������Һ��ѵ�����

    FriendPanelUI m_stFriendPanelUI; //�µĺ������
    FriendAddPanelUI m_stFriendAddPanelUI; //�����������
    FriendProposePanel m_stFriendProposePanel; //�Ƽ����

	bool m_bPresentedManual; // ����������

};

#define P_FRIEND_CTRL CVLib::CVSingleton<FriendControl>::Instance()

#endif // __FRIEND_CONTROL_h__
