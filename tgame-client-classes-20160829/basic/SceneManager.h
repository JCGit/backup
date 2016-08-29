#ifndef SceneManager_h__
#define SceneManager_h__

#include <vector>

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class BaseScene;

class SceneManager : public CCObject
{
public:
	SceneManager();
	virtual ~SceneManager();

	static SceneManager* sharedSceneManager(); // singleton get

	static void destroyInstance(); // singleton destroy

	bool init();

	BaseScene* getCurScene();

	const std::vector<unsigned int>& getSceneEnterOrderList() const;

	BaseScene* createScene(unsigned int uSceneID); // 根据SceneID创建场景

	void enterScene(unsigned int uSceneID, bool bIsPush = false); // 进入一个场景，isClean为是否清除上个场景的资源，bIsPush为true时，CCDirector::m_pRunningScene入栈，为false时，释放CCDirector::m_pRunningScene

	void enterScene(BaseScene *pScene, bool isClean = true, bool bIsPush = false); // 进入一个已完成构建的场景

	unsigned int backToPrevScene(); //回到上一个场景

	void addSceneIDToResCachedList(unsigned int uSceneID); // 添加一个有资源缓存的SceneID，一般在预加载资源后

	void removeSceneIDFromResCachedList(unsigned int uSceneID); // 移除一个有资源缓存的SceneID，一般在从内存中释放资源后

	bool isSceneInStack(unsigned int uSceneID); // SceneID对应的场景是否已入栈

	bool isSceneResCached(unsigned int uSceneID); // SceneID对应的场景的资源是否已缓存

	void compareRes(BaseScene* sceneA, BaseScene* sceneB);		//对比2个场景资源，
	void compareEffSoundRes(BaseScene* sceneA, BaseScene* sceneB);		//对比2个场景资源

	void cleanPreSceneRes(bool isClean = true);		//删除不重用的资源
    
    void prepareNextSceneRes();
private:
    bool m_bNeedDoLoadNextRes;
    CCArray* m_stSceneLst;
private:
	BaseScene *m_pCurScene; // 当前场景

	std::vector<unsigned int> m_stSceneEnterOrderList; // 保存进入场景的ID顺序<SceneID>

	std::vector<unsigned int> m_stSceneResCachedList; // 保存资源已预加载的场景ID

	std::vector<unsigned int> m_stSceneIDStackList; // 保存已入栈的场景ID

	BaseScene *m_pEnteringScene; // 未完成进入过程的场景

	bool m_bIsPushEnteringScene; // 是否push未完成进入过程的场景

	set<string> m_stNeedCleanRes;		//上一个场景需要清除的资源列表
	set<string> m_stNeedCleanEffSoundRes;	//上一个场景需要清除的音效资源
};

#endif // SceneManager_h__
