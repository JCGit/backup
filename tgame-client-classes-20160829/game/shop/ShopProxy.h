#ifndef ShopProxy_h__
#define ShopProxy_h__

#include "basic/QBaseProxy.h"
#include "CSStoreMsg.pb.h"

class ShopProxy: public QBaseProxy
{
public:
	ShopProxy();
	virtual ~ShopProxy();

	// override start

    virtual void DoRsp(vmsg::VLogicSvr2CliMsg& stRsp);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

	// override end

	void sendStoreBuyRqst(unsigned int uType, unsigned int uCfgID, unsigned int uCnt); // 购买道具

    void sendBlackShopQueryRqst();
    void sendBlackShopResetRqst();
    void sendBlackShopBuyRqst(const int iItemIndex);

	//限购商城
	void sendRestrictShopQueryRqst();
	void sendRestrictShopBuyRqst(const int iItemIndex);
private:
	
};

#endif // ShopProxy_h__