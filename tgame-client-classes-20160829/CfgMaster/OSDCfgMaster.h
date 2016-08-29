#ifndef OSDCfgMaster_h__
#define OSDCfgMaster_h__

#include "CfgMasterBase.h"
#include "OSDCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class OSDCfgMaster: public CCfgMasterBase<OSDCfgSet>
{
public:
	OSDCfgMaster() {};
	~OSDCfgMaster() {};

	OSDCfg* getOSDCfgByID(const unsigned int uSceneID);

	OSDCfg* getPrevOSDCfg(unsigned int uCurSceneID); // ���ݵ�ǰ����ID��ȡ��һ�����������ã���Ϊ��һ�������򷵻ص�ǰ��������

	OSDCfg* getNextOSDCfg(unsigned int uCurSceneID); // ���ݵ�ǰ����ID��ȡ��һ�����������ã���Ϊ���һ�������򷵻ص�ǰ��������
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id���±��ӳ��
	map<unsigned int, int> m_stID2Idx;
};

#define OSD_CFG_MASTER CVLib::CVSingleton<OSDCfgMaster>::Instance()

#endif // OSDCfgMaster_h__
