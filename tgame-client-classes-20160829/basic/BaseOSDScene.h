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

	CMainHero* getRole(UIN_t uin); // ����uin��ȡ�����еĽ�ɫ

	void addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo);

	void addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction); // ���һ����ɫ��������

	void removeRole(UIN_t uin); // �ӳ������Ƴ�һ����ɫ

	void setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction); // ����һ����ɫ��λ�ã��ƶ���ָ��λ��

	void moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction); // ͨ�������ƶ���ɫ

	void moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction); // �ƶ���ɫ��Ŀ�ĵ�

	//void loadMapRes(); // ���ص�ͼ��Դ

	//void parseMapData(const std::string &strMapCfgFileURL); // ������ͼ����

	//void buildMap(); // ������ͼ

	/*virtual void cleanRes();
	virtual void cleanup();*/

	void handleHeroWeaponChange(CCObject* pstObj);  //��ǹ
    CMainHero* getMainHero() {
        return m_stMainHero;
    }
protected:
	CMainHero* createRole(UIN_t uin, string nick, unsigned int showID, unsigned int weaponCfgID, unsigned int uVIPLv); // ����һ����ɫ

	//void onEachResLoaded(CCObject *pSender);
protected:
	map<UIN_t, vmsg::CSPlayerPosInScene> m_stRolesQueue;		//�ȴ���ӵ���̨�ϵĽ�ɫ��Ϣ
	CCDictionary *m_pRolesDict; // �����еĽ�ɫ�б� <UIN_t, CMainHero>

	CMainHero* m_stMainHero;	//����Լ�

	CCArray* m_stHeroRes;			//������Դ
};

#endif // BaseOSDScene_h__
