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

	BaseScene* createScene(unsigned int uSceneID); // ����SceneID��������

	void enterScene(unsigned int uSceneID, bool bIsPush = false); // ����һ��������isCleanΪ�Ƿ�����ϸ���������Դ��bIsPushΪtrueʱ��CCDirector::m_pRunningScene��ջ��Ϊfalseʱ���ͷ�CCDirector::m_pRunningScene

	void enterScene(BaseScene *pScene, bool isClean = true, bool bIsPush = false); // ����һ������ɹ����ĳ���

	unsigned int backToPrevScene(); //�ص���һ������

	void addSceneIDToResCachedList(unsigned int uSceneID); // ���һ������Դ�����SceneID��һ����Ԥ������Դ��

	void removeSceneIDFromResCachedList(unsigned int uSceneID); // �Ƴ�һ������Դ�����SceneID��һ���ڴ��ڴ����ͷ���Դ��

	bool isSceneInStack(unsigned int uSceneID); // SceneID��Ӧ�ĳ����Ƿ�����ջ

	bool isSceneResCached(unsigned int uSceneID); // SceneID��Ӧ�ĳ�������Դ�Ƿ��ѻ���

	void compareRes(BaseScene* sceneA, BaseScene* sceneB);		//�Ա�2��������Դ��
	void compareEffSoundRes(BaseScene* sceneA, BaseScene* sceneB);		//�Ա�2��������Դ

	void cleanPreSceneRes(bool isClean = true);		//ɾ�������õ���Դ
    
    void prepareNextSceneRes();
private:
    bool m_bNeedDoLoadNextRes;
    CCArray* m_stSceneLst;
private:
	BaseScene *m_pCurScene; // ��ǰ����

	std::vector<unsigned int> m_stSceneEnterOrderList; // ������볡����ID˳��<SceneID>

	std::vector<unsigned int> m_stSceneResCachedList; // ������Դ��Ԥ���صĳ���ID

	std::vector<unsigned int> m_stSceneIDStackList; // ��������ջ�ĳ���ID

	BaseScene *m_pEnteringScene; // δ��ɽ�����̵ĳ���

	bool m_bIsPushEnteringScene; // �Ƿ�pushδ��ɽ�����̵ĳ���

	set<string> m_stNeedCleanRes;		//��һ��������Ҫ�������Դ�б�
	set<string> m_stNeedCleanEffSoundRes;	//��һ��������Ҫ�������Ч��Դ
};

#endif // SceneManager_h__
