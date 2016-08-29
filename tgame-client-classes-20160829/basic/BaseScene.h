#ifndef BaseScene_h__
#define BaseScene_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "ClientSvrMsg.pb.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

using namespace std;

typedef enum {
	LAYER_ID_GAME, // 游戏对象层
	LAYER_ID_WIDGET, // 场景小部件层，如单独的按钮、头像等
	LAYER_ID_HUD, // 摇杆控制层
	LAYER_ID_FIGHT,	//战斗场景中的按钮层
	LAYER_ID_UI, // UI层，存放面板
	LAYER_ID_POP, // 弹出层，tips和对话框
	LAYER_ID_TOP, // 最上层，独占，限制操作
} ELayerID;

class BaseScene: public CCScene
{
public:
	BaseScene();
	virtual ~BaseScene();

	// override start

    virtual bool init(); // 初始化
    virtual void onExit();

	// override end

	void addLayer(ELayerID eLayerID);		// 添加layer到场景中，不会重复添加

	void removeLayer(ELayerID eLayerID);	// 移除场景中的layer，存在才会移除

	CCLayer* getLayer(ELayerID eLayerID); // 获取场景中的layer，没有返回null

	int getLayerZOrder(ELayerID eLayerID); // 获取layer在场景中的ZOrder

	void loadCfg(unsigned int uSceneID);    // 加载场景配置

	unsigned int getSceneID() const;

	unsigned int getSceneType() const;

    virtual void loadRes(); // 加载资源
    virtual bool loadNextRes() {return true;}

	virtual void build(); // 构建场景

	const set<string>& getResList(){         //获取场景的资源列表
		return m_stRes;
	}

	const set<string>& getEffSoundList(){
		return m_stEffSoudnRes;
	}

	virtual void cleanRes(); // 清理资源

	virtual void addResToList(string str);	//添加到资源(已经在场景中的资源加载，非预加载)

	virtual void addEffSound(string str);	//加入音效资源

	void cleanUI();

    void disableHudLayer();
private:
	CCLayer* createLayer(ELayerID eLayerID); // 创建指定的layer

protected:
	unsigned int m_uSceneID; // 场景ID，必须唯一

	unsigned int m_uSceneType; // 场景类型

	string m_strSceneName; // 场景名称

    set<string> m_stRes;	//资源列表

	set<string> m_stEffSoudnRes;		//音效列表
};

#endif // BaseScene_h__
