#pragma once

#include "BaseProxy.h"

class DayTaskProxy : public BaseProxy
{
public:
	DayTaskProxy();
	~DayTaskProxy();

	virtual void	DoRsp(NetPack& stRsp);
	void			sendMissionListReq();                   // ����ÿ�������б�
	void			sendMissionComplete(int missionId);  // �����������
};

