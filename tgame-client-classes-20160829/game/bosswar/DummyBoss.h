#ifndef DummyBoss_h__
#define DummyBoss_h__

#include "role/Enemy.h"
#include "BasicTypes.h"

class CDummyBoss : public CEnemy
{
public:
	static const string BOSS_DIE_UP_DATA;		//����ǿ��һ��Ѫʱ�ϱ�����

	CDummyBoss();
	virtual ~CDummyBoss();

	CREATE_FUNC(CDummyBoss);
	bool init(){
		CEnemy::init();
		return true;
	}

	virtual void HurtWithDamage(float fDamage); //��дQStageEntity�ķ���

	//ͬ��������
	void SyncHP(UINT64_t iHP); //ͬ������ͬ��Ѫ��

protected:
private:
};

#endif // DummyBoss_h__
