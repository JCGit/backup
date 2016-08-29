#ifndef OSDSceneInf_h__
#define OSDSceneInf_h__

#include "BasicTypes.h"
#include "CSSenceOSD.pb.h"

/*
ͬ����Ĵ���ӿڣ���CityProxy��ʹ��
*/
class OSDSceneInf {
public:
    OSDSceneInf(){};
    virtual ~OSDSceneInf(){};

	virtual void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo) = 0;
    
	virtual void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction) = 0; // ���һ����ɫ��������

	virtual void removeRole(UIN_t uin) = 0; // �ӳ������Ƴ�һ����ɫ

	virtual void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction) = 0; // ����һ����ɫ��λ�ã��ƶ���ָ��λ��

	virtual void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction) = 0; // ͨ�������ƶ���ɫ

	virtual void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction) = 0; // �ƶ���ɫ��Ŀ�ĵ�

    //ս��ͬ�����
    virtual void setPlayerStartAtkStat(const UIN_t uin, const vmsg::CSPlayerStartAtk& stStartAtk){} // ���ý�ɫ���״̬
    virtual void setPlayerStopAtkStat(const UIN_t uin, const vmsg::CSPlayerStopAtk& stStopAtk){} // ���ý�ɫֹͣ���״̬
    virtual void addMonster(const vmsg::CSOSDMonstersInfo& monsterInfo){} // ���һ�����ﵽ������
    virtual void setPlayerSkillInScene(const UIN_t uin, const vmsg::CSPlayerSkillInScene& stSkillInScene){} // ���ý�ɫ�ͷż���

};

#endif // OSDSceneInf_h__
