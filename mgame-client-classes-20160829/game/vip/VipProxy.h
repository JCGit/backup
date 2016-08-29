
#ifndef __MGAME_VIPPROXY_H__
#define __MGAME_VIPPROXY_H__

#include "BaseProxy.h"




class VipProxy : public BaseProxy
{
public:
	VipProxy();
	~VipProxy();

	virtual void DoRsp(NetPack& stRsp);

	virtual bool fault(const fgame::CSErrorMsg &err);

	
	// vipID=1,2,3		������,�ƽ�,��ʯ��
	// price
	//  vipBuyID =1,2,3	������,��ʯ,�ֽ�
	// vipSelectID =1,2 ��7��,30�졿
	void sendBuyVipRequest(int vipID, int price, int vipBuyId, int vipSelectID);//�����Ա

	void sendVipTimeRequest();
};




#endif //__MGAME_VIPPROXY_H__