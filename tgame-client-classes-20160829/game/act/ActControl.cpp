#include "ActControl.h"
#include "bag/BagControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "basic/PopTipLayer.h"
#include "Global.h"
#include "ActCumulativeRechargeLayout.h"
#include "NounHelper.h"
#include "game/notification/NotificationControl.h"
#include "CfgMaster/SumChargeActCfgMaster.h"
#include "CfgMaster/DailyLoginActCfgMaster.h"
#include "CfgMaster/DailyChargeActCfgMaster.h"
#include "CfgMaster/GradeGeekAwardCfgMaster.h"
#include "CfgMaster/FightGeekAwardCfgMaster.h"
#include "CfgMaster/DailyConsumeAwardCfgMaster.h"
#include "CfgMaster/SumConsumeActCfgMaster.h"
#include "CfgMaster/OnlineAwardCfgMaster.h"
#include "UserObj.h"
#include "city/CityControl.h"
using namespace vmsg;

CActControl::CActControl()
:m_pHappyNewYearPanel(NULL),
m_pSumRechargePaenl(NULL)
{

}

CActControl::~CActControl(){
	CC_SAFE_RELEASE_NULL(m_pHappyNewYearPanel);
}

void CActControl::sendClaimCouponRqst(const string couponID){
	m_stProxy.sendClaimCouponRqst(couponID);
}

void CActControl::sendClaimFirstChargePkgRqst(){
	m_stProxy.sendClaimFirstChargePkgRqst();
}

void CActControl::sendQueryAllPkgStatusRqst(){
	m_stProxy.sendQueryAllPkgStatusRqst();
}

void CActControl::sendClaimStoredManualRqst() {
	m_stProxy.sendClaimStoredManualRqst();
}

void CActControl::sendClaimSpringPkgRqst(unsigned int uDay) {
	m_stProxy.sendClaimSpringPkgRqst(uDay);
}

void CActControl::sendSumChargeRqst(int _lvl)
{
	m_stProxy.sendSumChargeRqst(_lvl);
}

void CActControl::sendDailyChargeRqst(int _lvl)
{
	m_stProxy.sendDailyChargeRqst(_lvl);
}

void CActControl::sendDailyLoginRqst(int _lvl)
{
	m_stProxy.sendDailyLoginRqst(_lvl);
}

void CActControl::sendGradeGeekRqst(int _lvl)
{
	m_stProxy.sendGradeGeekRqst(_lvl);
}

void CActControl::sendFightGeekRqst(int _lvl)
{
	m_stProxy.sendFightGeekRqst(_lvl);
}

void CActControl::sendVipGiftRqst(int _lvl)
{
	m_stProxy.sendVipGiftRqst(_lvl);
}

void CActControl::sendDailyConsumeRqst(int _lvl)
{
	m_stProxy.sendDailyConsumeRqst(_lvl);
}

void CActControl::sendSumConsumeRqst(int _lvl)
{
	m_stProxy.sendSumConsumeRqst(_lvl);
}

void CActControl::sendOnlinePkgRqst(int _lvl)
{
	m_stProxy.sendOnlinePkgRqst(_lvl);
}

void CActControl::sendNextDayPkgRqst(int _lvl)
{
	m_stProxy.sendNextDayPkgRqst(_lvl);
}

CActPanel* CActControl::getActPanel(){
	if(!m_stActPanel.isInited()){
		m_stActPanel.init();
	}

	return &m_stActPanel;
}

CDKPanel* CActControl::getCDKPanel(){
	if(!m_stCDKPanel.isInited()){
		m_stCDKPanel.init();
	}

	return &m_stCDKPanel;
}

OnlineAwardPanel* CActControl::getOnlinePanel()
{
	if (!m_sOnlinePanel.isInited())
	{
		m_sOnlinePanel.init();
	}
	return &m_sOnlinePanel;
}

NextDasyAwardPanel* CActControl::getNextDayPanel()
{
	if (!m_sNextDayPanel.isInited())
	{
		m_sNextDayPanel.init();
	}
	return &m_sNextDayPanel;
}

HappyNewYearPanel* CActControl::getHappyNewYearPanel() {
	if(m_pHappyNewYearPanel == NULL) {
		m_pHappyNewYearPanel = HappyNewYearPanel::create();
		CC_SAFE_RETAIN(m_pHappyNewYearPanel);
	}

	return m_pHappyNewYearPanel;
}

ActCumulativeRechargeLayout* CActControl::getSumRechargePanel()
{
	if (m_pSumRechargePaenl == NULL) {
		m_pSumRechargePaenl = ActCumulativeRechargeLayout::create();
		CC_SAFE_RETAIN(m_pSumRechargePaenl);
	}

	return m_pSumRechargePaenl;
}

void CActControl::doClaimCouponRsp(const CSClaimCouponRsp& stMsg){
	P_BAG_CTRL->setBagInfo(stMsg.baginfo());

	COMMON_DIALOG->show(TextStr(TEXT_ACT_CDK_CLAIM_TIP));

	for(int i = 0; i < stMsg.allacts_size(); i++){
		unsigned int uState = stMsg.allacts(i).status();
		if(i < m_stStatusInfo.allacts_size()){
			m_stStatusInfo.mutable_allacts(i)->set_status(uState);
		}
	}

	if(m_stActPanel.isInited() && m_stActPanel.isOpen()){
		m_stActPanel.updateCurType();
	}
}

void CActControl::doClaimFirstChargePkgRsp(const CSClaimFirstChargePkgRsp& stMsg){
	P_BAG_CTRL->setBagInfo(stMsg.baginfo());

	COMMON_DIALOG->show(TextStr(TEXT_ACT_FIRST_CHARGE_TIP));

	for(int i = 0; i < stMsg.allacts_size(); i++){
		unsigned int uState = stMsg.allacts(i).status();
		if(i < m_stStatusInfo.allacts_size()){
			m_stStatusInfo.mutable_allacts(i)->set_status(uState);
		}
	}

	if(m_stActPanel.isInited() && m_stActPanel.isOpen()){
		m_stActPanel.updateCurType();
	}
}

void CActControl::doQueryAllPkgStatusRsp(const CSQueryAllPkgStatusRsp& stMsg){
	m_stStatusInfo  = stMsg;

	notifyAct();

	if(m_stActPanel.isInited() && m_stActPanel.isOpen()){
		m_stActPanel.updateUI();
	}
	//test
	/*
	vmsg::CSCouponActStatusItem* pStatusItemOnline = m_stStatusInfo.add_allacts();
	pStatusItemOnline->set_type(ONLINE_PKG_AWARD);
	
	vmsg::OnlinePkgStatus* pOnlineStatus = pStatusItemOnline->mutable_onlinepkgstatus();
	pOnlineStatus->set_status(0);
	pOnlineStatus->set_onlinetime(0);

	vmsg::CSCouponActStatusItem* pStatusItemNextDasy = m_stStatusInfo.add_allacts();
	pStatusItemNextDasy->set_type(NEXTDAY_PKG_AWARD);
	vmsg::NextDayPkgStatus* pNextDayStaus = pStatusItemNextDasy->mutable_nextdaypkgstatus();
	pNextDayStaus->set_status(1);
	*/
	//在线礼包、次日礼包刷新
	P_CITY_CTRL->updateOnlineAndMorrow();
}

void CActControl::doClaimSpringPkgRsp(const vmsg::CSClaimSpringPkgRsp& stRsp) {
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	//COMMON_DIALOG->show(TextStr(TEXT_ACT_CDK_CLAIM_TIP));

	unsigned int uPkgID = stRsp.pkgid();

	for(int i = 0; i < stRsp.awardlst_size(); i++) {
		const CSAwardElement &stElmt = stRsp.awardlst(i);

		if(vmsg::THING_TYPE_COIN != stElmt.elementtype()) {
			if(vmsg::THING_TYPE_WEAPON == stElmt.elementtype()) {
				P_GLOBAL->showAdvItemEff(stElmt.elementtype(), stElmt.elementid() ); //显示高级物品获得特效
			} else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcnt());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

const CSCouponActStatusItem& CActControl::getItemInfoByType(unsigned int uTyp){
	for(int i = 0; i < m_stStatusInfo.allacts_size(); i++){
		if(m_stStatusInfo.allacts(i).type() == uTyp){
			return m_stStatusInfo.allacts(i);
		}
	}

	return m_stStatusInfo.allacts(0);
}

void CActControl::doSumChargePkgRsp(const vmsg::SumChargePkgPkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doDailyChargePkgRsp(const vmsg::DailyChargePkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doSumLoginPkgRsp(const vmsg::DailyLoginPkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doGradeGeekPkgRsp(const vmsg::GradeGeekPkgRsp& stRsp)
{
	if (stRsp.success())
	{
		P_BAG_CTRL->setBagInfo(stRsp.baginfo());

		for (int i = 0; i < stRsp.awardinfo_size(); i++) {
			const AwardInfo &stElmt = stRsp.awardinfo(i);

			if (vmsg::THING_TYPE_COIN != stElmt.type()) {
				if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
					P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
				}
				else {
					POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
				}
			}
		}

		sendQueryAllPkgStatusRqst();
	}
	else{

		COMMON_DIALOG->show(TextStr(TEXT_ACT_GET_FAILED), NULL, NounStr(OK));
	}
}

void CActControl::doFightGeekPkgRsp(const vmsg::FightGeekPkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doVipGiftPkgRsp(const vmsg::VipGiftPkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doDailyConsumePkgRsp(const vmsg::DailyConsumePkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doSumConsumePkgRsp(const vmsg::SumConsumePkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}

	sendQueryAllPkgStatusRqst();
}

void CActControl::doOnlinePkgRsp(const vmsg::OnlinePkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}
	sendQueryAllPkgStatusRqst();
}

void CActControl::doNextDayPkgRsp(const vmsg::NextDayPkgRsp& stRsp)
{
	P_BAG_CTRL->setBagInfo(stRsp.baginfo());

	for (int i = 0; i < stRsp.awardinfo_size(); i++) {
		const AwardInfo &stElmt = stRsp.awardinfo(i);

		if (vmsg::THING_TYPE_COIN != stElmt.type()) {
			if (vmsg::THING_TYPE_WEAPON == stElmt.type()) {
				P_GLOBAL->showAdvItemEff(stElmt.type(), stElmt.id()); //显示高级物品获得特效
			}
			else {
				POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.type(), stElmt.id(), stElmt.num());
			}
		}
	}
	sendQueryAllPkgStatusRqst();
}

void CActControl::notifyAct()
{
	if (hasActGet())
	{
		PUSH_NOTIFICATION(ENTRY_ACTIVITIES, NOTIFY_TAG_ACT, 1);
	}else
	{
		PUSH_NOTIFICATION(ENTRY_ACTIVITIES,NOTIFY_TAG_ACT,0);
	}

	if (hasOnlineActGet())
	{
		PUSH_NOTIFICATION(ENTRY_ONLINE_AWARD,NOTIFY_TAG_ONLINE_AWARD,1);
	}else
	{
		PUSH_NOTIFICATION(ENTRY_ONLINE_AWARD,NOTIFY_TAG_ONLINE_AWARD,0);
	}

	if (hasNextDayActGet())
	{
		PUSH_NOTIFICATION(ENTRY_MORROW_AWARD,NOTIFY_TAG_NEXTDAY_AWARD,1);
	}else
	{
		PUSH_NOTIFICATION(ENTRY_MORROW_AWARD,NOTIFY_TAG_NEXTDAY_AWARD,0);
	}
}
bool CActControl::hasActGet()
{
	for(int i = 0; i < m_stStatusInfo.allacts_size(); i++) {
		const vmsg::CSCouponActStatusItem &stItemInfo = m_stStatusInfo.allacts(i);
		if (stItemInfo.type() == ONLINE_PKG_AWARD || stItemInfo.type() == NEXTDAY_PKG_AWARD)
		{
			continue;
		}
		if (IsActCanGet(stItemInfo))
		{
			return true;
		}
	}

	return false;
}

bool CActControl::hasOnlineActGet()
{
	for(int i = 0; i < m_stStatusInfo.allacts_size(); i++) {
		const vmsg::CSCouponActStatusItem &stItemInfo = m_stStatusInfo.allacts(i);
		if (stItemInfo.type() == ONLINE_PKG_AWARD )
		{
			if (IsActCanGet(stItemInfo))
			{
				return true;
			}
			break;
		}
	}
	return false;
}

bool CActControl::hasNextDayActGet()
{
	for(int i = 0; i < m_stStatusInfo.allacts_size(); i++) {
		const vmsg::CSCouponActStatusItem &stItemInfo = m_stStatusInfo.allacts(i);
		if (stItemInfo.type() == NEXTDAY_PKG_AWARD)
		{
			if (IsActCanGet(stItemInfo))
			{
				return true;
			}
			break;
		}
	}
	return false;
}

bool CActControl::IsActCanGet(const vmsg::CSCouponActStatusItem &stItemInfo)
{
	switch (stItemInfo.type())
	{
	case FIRST_BUY_PKG_ACT:
		break;
	case NEW_YEAR_ONESHOT_ACT:
		break;
	case NEW_YEAR_7DAY_ACT:
		break;
	case STORE_MANUAL_ACT:
		break;


	case SUM_CHARGE_ACT:
		{
			for (int idx = 0; idx <SumChargeAct_CFG_MASTER->getSumChargeTypeNum(); idx++)
			{
				const SumChargeActCfg* pChargeLevel = SumChargeAct_CFG_MASTER->getSumChargeActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.sumchargeaward().status();
				bool _b_state = stItemInfo.sumchargeaward().totalcharge() >= pChargeLevel->totalcharge() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&&!_i_state)
				{
					return true;
				}
			}
		}
		break;
	case DAILY_LOGIN_ACT:
		{
			const DailyLoginAwardCfg* pBestCharge = DailyLoginAct_CFG_MASTER->getBestDailyLoginActCfg();
			unsigned int _state = stItemInfo.dailyloginstatus().status();
			bool _b_state = stItemInfo.dailyloginstatus().logincnt() >= (pBestCharge->day() -1)? true : false;
			int _i_state = CV_BIT_TEST(_state, (DailyLoginAct_CFG_MASTER->getDailyLoginTypeNum() - 1));// 0 未领取
			if (_b_state&&!_i_state)
			{
				return true;
			}

			for (int idx = 0; idx<DailyLoginAct_CFG_MASTER->getDailyLoginTypeNum(); idx++)
			{
				const DailyLoginAwardCfg* pChargeLevel = DailyLoginAct_CFG_MASTER->getDailyLoginActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.dailyloginstatus().status();
				bool _b_state = stItemInfo.dailyloginstatus().logincnt() >= pChargeLevel->day() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if(_b_state&&!_i_state)
				{
					return true;
				}
			}
		}
		break;
	case DAILY_CHARGE_ACT:
		{
			for (int idx = 0; idx < DailyChargeAct_CFG_MASTER->getDailyChargeTypeNum(); idx++)
			{
				const DailyChargeAwardCfg* pChargeLevel = DailyChargeAct_CFG_MASTER->getDailyChargeActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.dailychargestatus().status();
				bool _b_state = stItemInfo.dailychargestatus().charge() >= pChargeLevel->totalcharge() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&& !_i_state)
				{
					return true;
				}
			}
		}
		break;
	case GRADE_GEEK_ACT:
		{
			for (int idx = 0; idx < GradeGeekAward_CFG_MASTER->getGradeGeekAwardTypeNum(); idx++)
			{
				const GradeGeekAwardCfg* pChargeLevel = GradeGeekAward_CFG_MASTER->getGradeGeekAwardCfg(idx + 1);
				if (!pChargeLevel)
					break;;

				int curLast = 0;
				if (stItemInfo.gradegeekstatus().getnum_size() != 0 && idx <= stItemInfo.gradegeekstatus().getnum_size() - 1)
				{
					curLast = pChargeLevel->limitnum() - stItemInfo.gradegeekstatus().getnum(idx);
				}

				unsigned int _state = stItemInfo.gradegeekstatus().status();
				bool _b_state = P_USER_OBJ->getUserInfo()->level >= pChargeLevel->grade() ? true : false;
				_b_state = curLast > 0 ? _b_state : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&& !_i_state)
				{
					return true;
				}
			}
		}
		break;
	case FIGHT_GEEK_ACT:
		{
			for (int idx = 0 ; idx < FightGeekAward_CFG_MASTER->getFightGeekAwardTypeNum(); idx++)
			{
				const FightGeekAwardCfg* pChargeLevel = FightGeekAward_CFG_MASTER->getFightGeekAwardActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.fightgeekstatus().status();
				bool _b_state = P_USER_OBJ->getUserInfo()->fightPower >= pChargeLevel->fight() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&& !_i_state)
				{
					return true;
				}
			}
		}
		break;
	case VIP_GIFT_ACT:
		{
			if (P_USER_OBJ->getUserInfo()->vipLv>0 && stItemInfo.has_vipgiftpkgstatus())
			{
				unsigned int _state = stItemInfo.vipgiftpkgstatus().status();
				if (_state == 0)
				{
					return true;
				}
			}
		}
		break;
	case DAILY_CONSUME_ACT:
		{
			for (int idx = 0 ; idx < DAILYCONSUME_CFG_MASTER->getDailyConsumeTypeNum(); idx++)
			{
				const DailyConsumeAwardCfg* pChargeLevel = DAILYCONSUME_CFG_MASTER->getDailyConsumeActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.dailyconsumepkgstatus().status();
				bool _b_state = stItemInfo.dailyconsumepkgstatus().consume() >= pChargeLevel->totalconsume() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&& !_i_state)
				{
					return true;
				}
			}
		}
		break;
	case SUM_CONSUME_ACT:
		{
			for (int idx = 0 ; idx < SumConsumeAct_CFG_MASTER->getSumConsumeAwardTypeNum(); idx++)
			{
				const SumConsumeActCfg* pChargeLevel = SumConsumeAct_CFG_MASTER->getSumConsumeActCfg(idx + 1);
				if (!pChargeLevel)
					break;

				unsigned int _state = stItemInfo.sumconsumepkgstatus().status();
				bool _b_state = stItemInfo.sumconsumepkgstatus().consume() >= pChargeLevel->totalconsume() ? true : false;
				int _i_state = CV_BIT_TEST(_state, idx);// 0 未领取
				if (_b_state&& !_i_state)
				{
					return true;
				}
			}
		}
		break;
	case ONLINE_PKG_AWARD:
		{
			
			
			if (stItemInfo.onlinepkgstatus().status() != OnlineAward_CFG_MASTER->getOnlinePKGCfg(OnlineAward_CFG_MASTER->getOnlineAwardTypeNum())->level())
			{
				const OnlinePKGCfg* pOnlineCfg = OnlineAward_CFG_MASTER->getOnlinePKGCfg(stItemInfo.onlinepkgstatus().status()+1);
				int pLeftTime =   pOnlineCfg->onlinetime() - stItemInfo.onlinepkgstatus().onlinetime();
				if (pLeftTime<=0)
				{
					return true;
				}
				
			}
		}
		break;
	case NEXTDAY_PKG_AWARD:
		{
			if(stItemInfo.nextdaypkgstatus().status() == 0)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
}