#ifndef __EQUIP_LIB_PROXY__
#define __EQUIP_LIB_PROXY__

#include "QBaseProxy.h"
#include "BasicTypes.h"
#include <vector>
#include "CSEquipOpMsg.pb.h"

using namespace vmsg;
using namespace std;

class EquipLibProxy: public QBaseProxy
{ 
public:
	EquipLibProxy();
	virtual ~EquipLibProxy();

	// override rsp handler
    virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);
    virtual void fault(const vmsg::CSCommonErrMsg &err);

    //Éý¼¶
    void sendEquipLvUpRqst(unsigned int bagIdx, UINT64_t equipGUID, const vector<vmsg::CSEquipLvUpConsume>& stConsume);

    //½ø½×
    void sendEquipAdvRqst(unsigned int bagIdx, UINT64_t equipGUID, const vector<UINT64_t>& stNeedEquipGUIDs);

    //ÈÚºÏ
    void sendEquipMixRqst(unsigned int bagIdx, UINT64_t equipGUID);
private:
    void doUpdateRsp(const CSBagInfo& stBagInfo, const CSPlayerEquipInfo& stEquipInfo);
};

#endif // __EQUIP_LIB_PROXY__
