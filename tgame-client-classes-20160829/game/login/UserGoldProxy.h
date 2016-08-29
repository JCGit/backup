#ifndef __UserGoldProxy_H_
#define __UserGoldProxy_H_

#include "QBaseProxy.h"
#include "basic/BasicTypes.h"

class UserGoldProxy : public QBaseProxy {
public:
    UserGoldProxy();

    virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);
    virtual void buyGold();

    //RMB store
    void sendBuyRMBStoreRqst(const std::string& strJson);
    void sendQueryRMBStoreRqst();
    
    //app store
    void sendAppAuthedBuyRqst(const std::string& strProductID, const int iNum);
    void sendAppAuthedBuySucceedRqst(const std::string& strProductID, const int iTotalMoney,
                              const std::string& strOrderID, const int iNum);

	void sendQueryRMBStoreItemRqst(const int productID);
private:
    void sendBuyMessage(const char * msg);
    void onGoldBuyRsp(vmsg::VLogicSvr2CliMsg &stMsg);
};

#endif //__UserGoldProxy_H_

