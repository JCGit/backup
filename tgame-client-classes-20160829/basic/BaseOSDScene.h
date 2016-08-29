#ifndef BaseOSDScene_h__
#define BaseOSDScene_h__

#include "BaseMapScene.h"
//#include "BasicTypes.h"
#include "game/role/MainHero.h"
//#include "CSSenceOSD.pb.h"
#include "OSDSceneInf.h"

#define DEFAULT_ROLE_POS ccp(300, 100)
#define DEFAULT_ROLE_DIRECTION ccp(1, 0)

class CBgTextureNode;

class BaseOSDScene: public CBaseMapScene, public OSDSceneInf {
public:
	BaseOSDScene();
	virtual ~BaseOSDScene();

	// override start

	virtual bool init();

	/*virtual void loadRes();

	virtual void build();*/

	// override end

	CMainHero* getRole(UIN_t uin); // 根据uin获取场景中的角色

	void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo);

	void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction); // 添加一个角色到场景中

	void removeRole(UIN_t uin); // 从场景中移除一个角色

	void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction); // 设置一个角色的位置，移动到指定位置

	void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction); // 通过方向移动角色

	void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction); // 移动角色到目的地

	//void loadMapRes(); // 加载地图资源

	//void parseMapData(const std::string &strMapCfgFileURL); // 解析地图数据

	//void buildMap(); // 构建地图

	/*virtual void cleanRes();
	virtual void cleanup();*/

	void handleHeroWeaponChange(CCObject* pstObj);  //换枪
    CMainHero* getMainHero() {
        return m_stMainHero;
    }
protected:
	CMainHero* createRole(UIN_t uin, string nick, unsigned int showID, unsigned int weaponCfgID, unsigned int uVIPLv); // 创建一个角色

	//void onEachResLoaded(CCObject *pSender);
protected:
	map<UIN_t, vmsg::CSPlayerPosInScene> m_stRolesQueue;		//等待添加到舞台上的角色信息
	CCDictionary *m_pRolesDict; // 场景中的角色列表 <UIN_t, CMainHero>

	CMainHero* m_stMainHero;	//玩家自己

	CCArray* m_stHeroRes;			//人物资源
};

#endif // BaseOSDScene_h__
