#pragma once

#include "net/NetPack.h"
#include "VipProxy.h"
#include "game/data/TableManager.h"
#include "game/ui/dialogs/VipDialog.h"
class VipControl
{
public:
	VipControl();
	~VipControl();

	
	void sendBuyVipRequest(int vipID ,int buyID);

	void sendVipTimeRequest();

	void doBuyVipResp(const fgame::CSBuyVipResp& resp);

	//void setView(VipDialog * view){ mVipDialog = view; };
private:
	void buyVip(Ref* sendr, sVipType vipType, int buyID, int selectID);

	VipProxy mVipProxy;
//	VipDialog * mVipDialog;
	CC_SYNTHESIZE(int, mSelectVip, SelectVip);			//��ǰѡ���Vip 1��2��3
	CC_SYNTHESIZE(int, mSelectBuyID, SelectBuyID);		//ѡ��7�� 30�칺��  7����1��30����2

};

#define  sgVipControl()  Singleton<VipControl>::Get()

