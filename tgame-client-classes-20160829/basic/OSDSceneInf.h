#ifndef OSDSceneInf_h__
#define OSDSceneInf_h__

#include "BasicTypes.h"
#include "CSSenceOSD.pb.h"

/*
同屏类的纯虚接口，在CityProxy里使用
*/
class OSDSceneInf {
public:
    OSDSceneInf(){};
    virtual ~OSDSceneInf(){};

	virtual void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo) = 0;
    
	virtual void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction) = 0; // 添加一个角色到场景中

	virtual void removeRole(UIN_t uin) = 0; // 从场景中移除一个角色

	virtual void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction) = 0; // 设置一个角色的位置，移动到指定位置

	virtual void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction) = 0; // 通过方向移动角色

	virtual void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction) = 0; // 移动角色到目的地

    //战斗同屏相关
    virtual void setPlayerStartAtkStat(const UIN_t uin, const vmsg::CSPlayerStartAtk& stStartAtk){} // 设置角色射击状态
    virtual void setPlayerStopAtkStat(const UIN_t uin, const vmsg::CSPlayerStopAtk& stStopAtk){} // 设置角色停止射击状态
    virtual void addMonster(const vmsg::CSOSDMonstersInfo& monsterInfo){} // 添加一个怪物到场景中
    virtual void setPlayerSkillInScene(const UIN_t uin, const vmsg::CSPlayerSkillInScene& stSkillInScene){} // 设置角色释放技能

};

#endif // OSDSceneInf_h__
