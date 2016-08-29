#ifndef SetSysControl_h__
#define SetSysControl_h__

#include "vlib/base/CVSingleton.h"
#include <string>
#include "SetPanel.h"

using namespace std;

class SetSysControl
{
public:
	static const string INIT_SET;						//��ʼ����ֵ

	enum SET_SYS_TYPE{
		ARENA_FIRST_TIME_TYPE = 0,						//�Ƿ��һ�ν��뾺����
		BACKGROUND_SOUND_TYPE = 1,						//�������ֿ���
		SOUND_EFFECT_TYPE = 2,							//��Ч����
		HUD_ROCKER_STILL_TYPE = 3,						//ҡ����������
	};
public:
	SetSysControl();
	virtual ~SetSysControl();

	void initSet();

	//�������ֵ
	bool isSet(unsigned int uType);						

	//����op��״̬�Ƿ�ѡ��,�Ƿ����̨ͨѶ
	void setOpStat(unsigned int uType, bool bSel, bool isSendUpdate = true);		

	//�ı�str�ַ���charIdx�����ַ�Ϊch
	string setCharByIdx(string str, const char* ch, unsigned int charIdx);		

	//�ַ���oriStr�ĳ�������ΪnewLen�����Ӳ������ַ�0
	string add0str(string str, unsigned int newLen);

	//���ûر�
	void doSetRsp();	

	void updateLocalData();

	CSetPanel* getSetPanel();

private:
	string m_strSys;

	bool m_bIsInited;

	CSetPanel m_stSetPanel;
};

#define P_SET_CTRL CVLib::CVSingleton<SetSysControl>::Instance()

#endif // SetSysControl_h__
