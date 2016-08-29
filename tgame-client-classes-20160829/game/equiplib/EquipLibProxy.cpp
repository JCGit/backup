#include "EquipLibProxy.h"

#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "EquipLibControl.h"
#include "bag/BagControl.h"
#include "UserObj.h"


using namespace vmsg;

EquipLibProxy::EquipLibProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_EQUIPMENT_LVUP, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_EQUIPMENT_STARSTEP, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_EQUIPMENT_REFIT, this);	
}

EquipLibProxy::~EquipLibProxy()
{
}


void EquipLibProxy::sendEquipLvUpRqst(unsigned int bagIdx, UINT64_t equipGUID, const vector<vmsg::CSEquipLvUpConsume>& stConsume){
    const unsigned int uCmd = vmsg::CS_CMD_EQUIPMENT_LVUP;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    
    vmsg::CSEquipLvUpRqst* stLvUpRqst = stMsg.mutable_equiprequest()->mutable_equiplvup_rqst();
    stLvUpRqst->set_bagidx(bagIdx);
    stLvUpRqst->set_equipguid(equipGUID);

    for(unsigned int i = 0; i < stConsume.size(); i++){
        vmsg::CSEquipLvUpConsume* pstConsume = stLvUpRqst->add_consumes();
        pstConsume->set_id(stConsume.at(i).id());
        pstConsume->set_type(stConsume.at(i).type());
        pstConsume->set_cnt(stConsume.at(i).cnt());
    }

    P_NET_MGR->SendPkg(stMsg);
}

//½ø½×
void EquipLibProxy::sendEquipAdvRqst(unsigned int bagIdx, UINT64_t equipGUID, const vector<UINT64_t>& stNeedEquipGUIDs){
    const unsigned int uCmd = vmsg::CS_CMD_EQUIPMENT_STARSTEP;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSEquipStarStepRqst* stRqst = stMsg.mutable_equiprequest()->mutable_equipstarstep_rqst();
    stRqst->set_bagidx(bagIdx);
    stRqst->set_equipguid(equipGUID);

    for(unsigned int i = 0; i < stNeedEquipGUIDs.size(); i++){
        stRqst->add_needequipguids(stNeedEquipGUIDs[i]);
    }

    P_NET_MGR->SendPkg(stMsg);
}

//ÈÚºÏ
void EquipLibProxy::sendEquipMixRqst(unsigned int bagIdx, UINT64_t equipGUID){
    const unsigned int uCmd = vmsg::CS_CMD_EQUIPMENT_REFIT;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSEquipRefitRqst* stRqst = stMsg.mutable_equiprequest()->mutable_equiprefit_rqst();
    stRqst->set_bagidx(bagIdx);
    stRqst->set_equipguid(equipGUID);

    P_NET_MGR->SendPkg(stMsg);
}


void EquipLibProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) 
{
    const int iCmd = stMsg.msg_head().msg_id();
    switch (iCmd)
    {
    case vmsg::CS_CMD_EQUIPMENT_LVUP:
        doUpdateRsp(stMsg.equiprespond().equiplvup_rsp().baginfo(), stMsg.equiprespond().equiplvup_rsp().playerequipinfo() );
        break;
    case vmsg::CS_CMD_EQUIPMENT_STARSTEP:
        doUpdateRsp(stMsg.equiprespond().equipstarstep_rsp().baginfo(), stMsg.equiprespond().equipstarstep_rsp().playerequipinfo() );
        P_EQUIP_LIB_CTRL->getEquipLibPanel()->playAdvEffect();
        break;
    case vmsg::CS_CMD_EQUIPMENT_REFIT:
        doUpdateRsp(stMsg.equiprespond().equiprefit_rsp().baginfo(), stMsg.equiprespond().equiprefit_rsp().playerequipinfo() );
        P_EQUIP_LIB_CTRL->getEquipLibPanel()->playMixEffect();
        break;
    default:
        break;
    }
}
void EquipLibProxy::fault(const vmsg::CSCommonErrMsg &err) {
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->closeLoading();
}

void EquipLibProxy::doUpdateRsp(const CSBagInfo& stBagInfo, const CSPlayerEquipInfo& stEquipInfo) {
    P_BAG_CTRL->setBagInfo(stBagInfo);
    UserInfo* pUserInfo = P_USER_OBJ->getMutableUserInfo();
    pUserInfo->stEquipLst.clear(); //reset
    for (int i = 0; i < stEquipInfo.equiplst_size(); ++i){
        pUserInfo->stEquipLst.push_back(stEquipInfo.equiplst(i));
    }
    pUserInfo->spareEquip = stEquipInfo.spareequip();
    CCLOG("[trace]EquipLibProxy,update bag & equip info done.");

	P_EQUIP_LIB_CTRL->notifyEquipAdvance();
    //do update UI
    P_EQUIP_LIB_CTRL->getEquipLibPanel()->doUpdateUI(true);
}


