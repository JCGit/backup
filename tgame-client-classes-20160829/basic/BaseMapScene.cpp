#include "BaseMapScene.h"
#include "role/Enemy.h"
#include "loading/LoadingScreen.h"
#include "SceneManager.h"
#include "defines.h"
#include "MapBinReader.h"
#include "ToolUtils.h"
#include "control/AudioManager.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Multiresolution.h"
#include "fight/SceneBuff.h"
#include "fight/SceneTrap.h"
#include "instance/InstanceFightUtils.h"

#include "OSDCfgMaster.h"
#include "SceneEffCfgMaster.h"
#include "SceneCfgMaster.h"
#include "FloorBuffCfgMaster.h"
#include "MonsterCfgMaster.h"
#include "SkillCfgMaster.h"
#include "MapCfgMaster.h"
#include "WarMapCfgMaster.h"
#include "SceneItemCfgMaster.h"
#include "BuildItemCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace vmsg;

CBaseMapScene::CBaseMapScene()
:_itemData(NULL)
,_trapData(NULL)
,_monsterData(NULL)
,_floorBuffData(NULL)
,_sceneBackground(NULL)
,_sceneForegound(NULL)
,_sceneFrontgound(NULL)
,_sceneMidground(NULL)
,_sceneMid2ground(NULL)
{
	m_iCheckCleanInterval = 0.0;
    m_stPrepareLoadRes.clear();
}

CBaseMapScene::~CBaseMapScene(){
	CC_SAFE_RELEASE_NULL(_itemData);
	CC_SAFE_RELEASE_NULL(_trapData);
    CC_SAFE_RELEASE_NULL(_monsterData);
	CC_SAFE_RELEASE_NULL(_floorBuffData);
    CC_SAFE_RELEASE_NULL(_sceneBackground);
    CC_SAFE_RELEASE_NULL(_sceneForegound);
    CC_SAFE_RELEASE_NULL(_sceneFrontgound);
    CC_SAFE_RELEASE_NULL(_sceneMidground);
}

bool CBaseMapScene::init(){
	if(!BaseScene::init()){
		return false;
	}

	return true;
}

void CBaseMapScene::onEnterTransitionDidFinish(){
	BaseScene::onEnterTransitionDidFinish();

}

void CBaseMapScene::loadRes() {
	BaseScene::loadRes();

	loadMapRes();
}

void CBaseMapScene::build() {
	BaseScene::build();

	_blocks.clear();

	initMap();
	buildMap();
}

void CBaseMapScene::initMap(){
	initSound();
}

void CBaseMapScene::buildMap(){

}

void CBaseMapScene::loadMapRes(){
	m_uResLoadedNum = 0;
	m_uResTotalNum = 0;

	//获取每层的资源名
	setMapRes();

	//解析数据
	analyseMapInfo();

	//资源列表
	if(!m_stRes.empty()){
		m_stRes.clear();
	}

	if(!m_stResToNum.empty()){
		m_stResToNum.clear();
	}

	//地图其它背景的资源，如中景层、远景层
	loadMapBgRes();

	//加载猪脚、好友的资源
	loadHeroRes();

	//加载怪物相关资源
	loadMonsterRes();

	//加载item资源
	loadItemRes();

	//加载floor buff资源
	loadFloorBuffRes();

	//加载陷阱资源
	loadTrapRes();

	m_uResTotalNum = m_stRes.size();
    m_stPrepareLoadRes = m_stRes;

    //2014-8-23,neilwu,mod
    //安卓下的bug，异步加载资源时不能同时调用同步加载函数。所以这里分开加载
    /*
    set<string>::iterator stIter;
	for(stIter = m_stRes.begin(); stIter != m_stRes.end(); stIter++){
		string strPath = *stIter;
		string typeStr = CToolUtils::getResType(strPath);

        if(strcmp(typeStr.c_str(), PLIST_TYPE_NAME.c_str()) == 0){  //图片资源
            //CCTextureCache::sharedTextureCache()->addImageAsync(strPath.c_str(), this, callfuncO_selector(CBaseMapScene::onEachMapResLoadedCallback));

            CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
            CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);//强制使用该格式

            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strPath.c_str());

            CCTexture2D::setDefaultAlphaPixelFormat(currentFormat); //用完后恢复

            onEachMapResLoadedCallback(NULL);
        }
	}
    for(stIter = m_stRes.begin(); stIter != m_stRes.end(); stIter++){
        string strPath = *stIter;
        string typeStr = CToolUtils::getResType(strPath);

        if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //动画资源
            CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
            CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);//强制使用该格式

            CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(strPath.c_str(), this, schedule_selector(CBaseMapScene::onEachArmatureLoaderCallback));

            CCTexture2D::setDefaultAlphaPixelFormat(currentFormat); //用完后恢复

        } else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //图片资源

            CCTextureCache::sharedTextureCache()->addImageAsync(strPath.c_str(), this, callfuncO_selector(CBaseMapScene::onEachMapResLoadedCallback));

        } 
    }
    */

}

bool CBaseMapScene::loadNextRes() {
    if(m_stPrepareLoadRes.empty()) {
        return true;
    }
    
    string strPath = *(m_stPrepareLoadRes.begin());
    string typeStr = CToolUtils::getResType(strPath);

    if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //动画资源
        CToolUtils::loadArmatureRGBA4444(strPath);
        onEachArmatureLoaderCallback(0.0f);

    } else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //图片资源

        //CCTextureCache::sharedTextureCache()->addImageAsync(strPath.c_str(), this, callfuncO_selector(CBaseMapScene::onEachMapResLoadedCallback));
        CCTextureCache::sharedTextureCache()->addImage(strPath.c_str());
        onEachMapResLoadedCallback(NULL);
        CCLOG("[trace]load res %s", strPath.c_str());
    } else if(strcmp(typeStr.c_str(), PLIST_TYPE_NAME.c_str()) == 0){  //图片资源
         if(strPath.find("ca10001") != string::npos || strPath.find("cb10001") != string::npos) {
//             //ca10001 cb10001
             //第一个场景不压缩
             CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strPath.c_str());
         } else {
            CToolUtils::loadSpriteFrameRGBA4444(strPath);
        }

        onEachMapResLoadedCallback(NULL);
    }

    m_stPrepareLoadRes.erase(strPath);
    return false;
}

void CBaseMapScene::setMapRes(){
	// 根据场景类型获取地图的配置
	if(m_uSceneType == SCENE_OSD){
		_cfgFileName = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->cfgid();  //读配置
		m_stFrontStr = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->frontid();
		m_stForeStr = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->foreid();
		m_stMidStr = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->midid();;
		m_stBackStr = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->backid(); 
		m_stMid2Str =  OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->midid2(); 
	}else{
		_cfgFileName = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->cfgid();  //读配置
		m_stFrontStr = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->frontid();
		m_stForeStr = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->foreid();
		m_stMidStr = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->midid(); 
		m_stBackStr = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->backid(); 
		m_stMid2Str = MAP_CFG_MASTER->GetMapCfgByID(m_uSceneID)->midid2();
	}
}

void CBaseMapScene::loadMapBgRes(){
	//获取所有的地图碎片的路径

	//地表
	for(int idx = 0; idx < 3; idx++){
		//string str = MAP_RES_PATH + m_stForeStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME;
        string str = MAP_RES_TP_PATH + m_stForeStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME + PLIST_TYPE_NAME;
		m_stRes.insert(str);
	}

	//前景
	if(m_stFrontStr != "null"){
		for(unsigned int idx = 0; idx < 3; idx++){
			//string str = MAP_QJ_RES_PAHT + m_stFrontStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME;
            string str = MAP_RES_TP_PATH + m_stFrontStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME  + PLIST_TYPE_NAME;
			m_stRes.insert(str);
		}
	}

	//中景1
	if(m_stMidStr != "null"){
		for(unsigned int idx = 0; idx < 3; idx++){
			//string str = MAP_MID_RES_PATH + m_stMidStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME;
            string str = MAP_RES_TP_PATH + m_stMidStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME + PLIST_TYPE_NAME;
			m_stRes.insert(str);
		}
	}

	//背景
	if(m_stBackStr != "null"){
		for(int idx = 0; idx < 3; idx++){
			//string str = MAP_BG_RES_PAHT + m_stBackStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME;
            string str = MAP_RES_TP_PATH + m_stBackStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME + PLIST_TYPE_NAME;
			m_stRes.insert(str);
		}
	}

	//中景2
	if(m_stMid2Str != "null" && m_stMid2Str.length()){
		for(int idx = 0; idx < 3; idx++){
			//string str = MAP_BG_RES_PAHT + m_stBackStr + "/bgfm" + intToString(idx) + PNG_TYPE_NAME;
			string str = MAP_RES_TP_PATH + m_stMid2Str + "/bgfm" + intToString(idx) + PNG_TYPE_NAME + PLIST_TYPE_NAME;
			m_stRes.insert(str);
		}
	}
}

void CBaseMapScene::loadHeroRes(){

}

void CBaseMapScene::loadMonsterRes(){

}

void CBaseMapScene::loadItemRes(){
	
}

void CBaseMapScene::loadFloorBuffRes(){

}

void CBaseMapScene::loadTrapRes(){

}

void CBaseMapScene::update(float delta){
    m_iCheckCleanInterval += delta;
    if(m_iCheckCleanInterval < 1.0) {
        return; //控制一下更新间隔吧,neilwu,2014-6-19
    }
    m_iCheckCleanInterval = 0.0;

	//map<string, int>::iterator iter;
	//for(iter = m_stResToNum.begin(); iter != m_stResToNum.end(); iter++){
	//	if(iter->second <= 0){  //id对应的资源使用数量为0
	//		string nameStr = iter->first;

	//		set<string>::iterator strIter;
	//		strIter = m_stRes.find(nameStr);
	//		if(strIter != m_stRes.end()){   //资源数组中找不到
	//			string typeStr = CToolUtils::getResType(nameStr);
	//			if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //动画资源
	//				//CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(nameStr.c_str());
 //                   //CToolUtils::removeArmatureFile(nameStr.c_str());
	//			}else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //图片资源
	//				//CCTextureCache::sharedTextureCache()->removeTextureForKey(nameStr.c_str());
	//			}
	//			m_stRes.erase(strIter);

 //               CCLOG("[trace]CBaseMapScene::update clean res %s", nameStr.c_str());
	//		}
	//	}
	//}
}

void CBaseMapScene::onEachMapResLoadedCallback(CCObject *pObject) {
	onLoaderCallback();
}

void CBaseMapScene::onEachArmatureLoaderCallback(float dt){
	onLoaderCallback();
}

void CBaseMapScene::onLoaderCallback(){
	// 加载完一个资源，更新进度
	m_uResLoadedNum++;
    
	float fPercentage = (float)m_uResLoadedNum / m_uResTotalNum * 100;

	//loading界面
	if(CCDirector::sharedDirector()->getRunningScene() != NULL) {
		LoadingScreen::getInstance()->setPercentage(fPercentage);
	}

	// 资源已全部加载，构建并进入场景
	if(m_uResLoadedNum == m_uResTotalNum) { 
		build();
	}
    //CCLOG("[trace]CBaseMapScene::onLoaderCallback,drawScene");
}

bool CBaseMapScene::getIsCached() const {
	return m_bIsCached;
}

void CBaseMapScene::analyseMapInfo(){
	//第一先获取文件的路径  
	string realPath = MAP_CFG_RES + _cfgFileName + MAP_RES_TYPE;
    
	CMapBin stMapBin;
	int iRet = MapBinReader::Init(stMapBin, realPath.c_str());

	if(iRet != 0){
		CV_ERROR(("CBaseMapScene::analyseMapInfo failed" , __PRETTY_FUNCTION__));
		return;
	}

	_mapW = stMapBin.mapw();
	_mapH = stMapBin.maph();
	_screenNum = stMapBin.sceennum();
	_midResNum = stMapBin.midresnum();
	_screenW = stMapBin.sceenw();
	_gridW = stMapBin.gridw();
	_gridH = stMapBin.gridh();
	_fileName = stMapBin.name();

	setWalkHight();
	_gridWNum = ceil(_mapW/_gridW);
	_gridHNum = ceil(m_uWalkHight/_gridH);

	//场景物件
	CC_SAFE_RELEASE_NULL(_itemData);
	_itemData = CCArray::create();
	_itemData->retain();
	CItemObj* itemObj;
	for(int i = 0; i < stMapBin.proplist_size(); ++i){   //如果不是数组，这一行会报错  
		itemObj = CItemObj::create();
		itemObj->id = stMapBin.proplist(i).id();
		itemObj->x = stMapBin.proplist(i).x();
		itemObj->y = stMapBin.proplist(i).y();
		itemObj->stage = (int)(1.0*itemObj->x/(_screenW*FIGHT_STAGE_NUM)) + 1;
		itemObj->touchId = stMapBin.proplist(i).touchid();
		itemObj->dropId = stMapBin.proplist(i).dropid();
		itemObj->dropType = stMapBin.proplist(i).droptype();
		itemObj->dropParty = stMapBin.proplist(i).dropparty();
		_itemData->addObject(itemObj);
	}  

	CC_SAFE_RELEASE_NULL(_monsterData);
	_monsterData = CCArray::create();
	_monsterData->retain();
	CEnemyData* monsterObj;
	for(int i = 0; i < stMapBin.monsterlist_size(); ++i){   //如果不是数组，这一行会报错  
		monsterObj = CEnemyData::create();
		monsterObj->id = stMapBin.monsterlist(i).id();
		monsterObj->x = stMapBin.monsterlist(i).x();
		monsterObj->y = stMapBin.monsterlist(i).y();
		monsterObj->type = stMapBin.monsterlist(i).type();
		monsterObj->time = stMapBin.monsterlist(i).time();
		monsterObj->stage = stMapBin.monsterlist(i).stage();
		//monsterObj->stage = (int)(1.0*monsterObj->x/(_screenW*FIGHT_STAGE_NUM)) + 1;
		//int iStage = (int)(1.0*monsterObj->x/(_screenW*FIGHT_STAGE_NUM)) + 1;
		monsterObj->num = stMapBin.monsterlist(i).num();
		monsterObj->inter = stMapBin.monsterlist(i).inter();
		monsterObj->touchID = stMapBin.monsterlist(i).touchid();
		monsterObj->key = CInstanceFightUtils::MakeKey(monsterObj->id, monsterObj->x, monsterObj->y);
		_monsterData->addObject(monsterObj);

		//CCLOG("the monster id is %d, touch id is %d", monsterObj->id, monsterObj->touchID);
	}  

	//场景buff
	CC_SAFE_RELEASE_NULL(_floorBuffData);
	_floorBuffData = CCArray::create();
	_floorBuffData->retain();
	CFloorBuffObj* stBuffObj;
	for(int i=0; i < stMapBin.bufflist_size(); ++i){		//如果不是数组，这一行会报错  
		stBuffObj = CFloorBuffObj::create();
		stBuffObj->id = stMapBin.bufflist(i).id();
		stBuffObj->x = stMapBin.bufflist(i).x();
		stBuffObj->y = stMapBin.bufflist(i).y();
		stBuffObj->touchId = stMapBin.bufflist(i).touchid();
		stBuffObj->party = stMapBin.bufflist(i).party();
		stBuffObj->stage = (int)(1.0*stBuffObj->x/(_screenW*FIGHT_STAGE_NUM)) + 1;
		_floorBuffData->addObject(stBuffObj);
	}  

	//陷阱信息
	CC_SAFE_RELEASE_NULL(_trapData);
	_trapData = CCArray::create();
	_trapData->retain();
	CTrapObj* stTrapObj;
	for(int i=0; i < stMapBin.traplist_size(); ++i){		//如果不是数组，这一行会报错  
		stTrapObj = CTrapObj::create();
		stTrapObj->id = stMapBin.traplist(i).id();
		stTrapObj->x = stMapBin.traplist(i).x();
		stTrapObj->y = stMapBin.traplist(i).y();
		stTrapObj->touchId = stMapBin.traplist(i).touchid();
		stTrapObj->party = stMapBin.traplist(i).party();
		stTrapObj->stage = (int)(1.0*stTrapObj->x/(_screenW*FIGHT_STAGE_NUM)) + 1;
		_trapData->addObject(stTrapObj);
	}  

	//起始线
	m_stBeginData.clear();
	map<int, BeginData>::iterator stBeginIter;
	BeginData data;
	for(int i=0; i < stMapBin.beginlist_size(); ++i){		//如果不是数组，这一行会报错  
		data.x = stMapBin.beginlist(i).x();
		data.stage = stMapBin.beginlist(i).id();
		data.condition = stMapBin.beginlist(i).condition();
		data.targetType = stMapBin.beginlist(i).type();
		data.targetID = stMapBin.beginlist(i).targetid();
		data.num = stMapBin.beginlist(i).num();
		data.time = stMapBin.beginlist(i).time();

		stBeginIter = m_stBeginData.find(stMapBin.beginlist(i).id());
		if(stBeginIter != m_stBeginData.end()){
			CCLOG("there have stage's begin line repeated, the stage is %d", stMapBin.beginlist(i).id());
			continue;;
		}

		m_stBeginData[data.stage] = data;
	}  

	//触发线
	m_stTouchData.clear();
	map<int, int>::iterator stIter;
	for(int i=0; i < stMapBin.touchlist_size(); ++i){		//如果不是数组，这一行会报错  
		int touchId = stMapBin.touchlist(i).id();
		stIter = m_stTouchData.find(touchId);

		if(stIter != m_stTouchData.end()){
			CCLOG("there have id repeated, the id is %d", touchId);
		}

		m_stTouchData[touchId] =  stMapBin.touchlist(i).x();
	}  

	//npc
	m_stNPCData.clear();
	map<int, CCPoint>::iterator stNPCIter;
	for(int i=0; i < stMapBin.npclist_size(); ++i){		//如果不是数组，这一行会报错  
		int ncpID = stMapBin.npclist(i).id();
		stNPCIter = m_stNPCData.find(ncpID);

		if(stNPCIter != m_stNPCData.end()){
			CCLOG("there have ncp id repeated, the id is %d", ncpID);
		}

		m_stNPCData[ncpID] = ccp(stMapBin.npclist(i).x(), stMapBin.npclist(i).y());
	}  
}

void CBaseMapScene::setWalkHight() {

}

void CBaseMapScene::cleanRes(){
	set<string>::iterator stIter;
	for(stIter = m_stRes.begin(); stIter != m_stRes.end(); stIter++){
		string strPath = *stIter;
		string typeStr = CToolUtils::getResType(strPath);
		if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //动画资源
			//CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(strPath.c_str());
            CToolUtils::removeArmatureFile(strPath.c_str());
		}else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //图片资源
			CCTextureCache::sharedTextureCache()->removeTextureForKey(strPath.c_str());
		}
	}

	m_stRes.clear();
	m_stBeginData.clear();
	m_stTouchData.clear();
	m_stNPCData.clear();
}

CCPoint CBaseMapScene::xyToGrid(const CCPoint& pst){
	int idx = floor((int)(pst.x)/_gridW);
	int idy = floor((int)(pst.y)/_gridH);
	idx = (idx < 0) ? 0 : idx;
	idy = (idy < 0) ? 0 : idy;
	

	return ccp(idx, idy);
}

CCPoint CBaseMapScene::gridToXY(const CCPoint& pst){
	float x = pst.x*_gridW + _gridW/2;
	float y = pst.y*_gridH + _gridH/2;
	return ccp(x, y);
}

void CBaseMapScene::getBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem){
	CCPoint startPst = xyToGrid(ccp(fabs(pst.x - size.width/2), fabs(pst.y - size.height/2)));
	float endx = ((pst.x + size.width/2) > _mapW)?_mapW:(pst.x + size.width/2);
	float endy = ((pst.y + size.height/2) > m_uWalkHight)?m_uWalkHight:(pst.y + size.height/2);
	CCPoint endPst = xyToGrid(ccp(endx, endy));
	int idx;

    const int iMaxIdx = _gridWNum * _gridHNum;
	for(int i = startPst.x; i <= endPst.x; i++){
		for(int j = startPst.y; j <= endPst.y; j++){
			idx = j*_gridWNum + i;
            if(idx >= iMaxIdx ) {
                continue;
            }
            _blocks.insert(idx);
		}
	}

	if(bIsItem){
		CCRect rect = CCRectMake(pst.x - size.width/2, pst.y - size.height/2, size.width, size.height);
		m_stRects.push_back(rect);
	}
}

void CBaseMapScene::removeBlocksInSize(const CCSize& size, const CCPoint& pst, bool bIsItem /* = false */){
	CCPoint startPst = xyToGrid(ccp(fabs(pst.x - size.width/2), fabs(pst.y - size.height/2)));
	float endx = ((pst.x + size.width/2) > _mapW)?_mapW:(pst.x + size.width/2);
	float endy = ((pst.y + size.height/2) > m_uWalkHight)?m_uWalkHight:(pst.y + size.height/2);
	CCPoint endPst = xyToGrid(ccp(endx, endy));
	int idx;

	const int iMaxIdx = _gridWNum * _gridHNum;
	for(int i = startPst.x; i <= endPst.x; i++){
		for(int j = startPst.y; j <= endPst.y; j++){
			idx = j*_gridWNum + i;
			if(idx >= iMaxIdx ) {
				continue;
			}

			set<int>::iterator stIter = _blocks.find(idx);
			if(stIter != _blocks.end()){
				_blocks.erase(stIter);
			}
		}
	}

	if(bIsItem){
		CCRect rect = CCRectMake(pst.x - size.width/2, pst.y - size.height/2, size.width, size.height);
		vector<CCRect>::iterator stRectIter;
		for(stRectIter = m_stRects.begin(); stRectIter != m_stRects.end(); stRectIter++){
			if(rect.equals(*stRectIter)){
				m_stRects.erase(stRectIter);
				break;
			}
		}
	}
}

void CBaseMapScene::removeAllBlocks(){
	m_stRects.clear();
	_blocks.clear();

	updateMapBlocks();
}

bool CBaseMapScene::isPointCanWalk(const CCPoint& pst)
{
    CCPoint stGridPt = xyToGrid(pst);
    int iGridIdx = stGridPt.y * _gridWNum + stGridPt.x;
	if (stGridPt.x > _gridWNum || stGridPt.y > _gridHNum){
		return false;
	}
    return (0 == _blocks.count(iGridIdx));
}


//移动entity 到 坐标stDestPoint
void CBaseMapScene::moveEntity(CQStageEntity* pEntity, const CCPoint& stDestPoint)
{
    if (NULL == pEntity)
    {
        CCLOG("[trace]CBaseMapScene::moveEntity,fail to move,entity is null.");
        return;
    }

	CCPoint stStartGrid = xyToGrid(pEntity->getPosition()); //使用grid寻路
    CCPoint stDestGrid = xyToGrid(stDestPoint);

    std::vector<CCPoint> stPath; //实际坐标值路径
   
    std::vector<void*> stIdxPath;
    bool bIsSolved = m_stPathFinder.FindPath(stStartGrid.x, stStartGrid.y, stDestGrid.x, stDestGrid.y, stIdxPath);
    if(!bIsSolved) {
        CCLOG("[trace]CBaseMapScene::moveEntity, fail to find path,from(%d,%d),to(%d,%d)",
                (int)stStartGrid.x, (int)stStartGrid.y, (int)stDestGrid.x, (int)stDestGrid.y );

        return;
    }
    long iGridIdx = 0;
    CCPoint stPoint;
    for(auto iter = stIdxPath.rbegin(); iter != stIdxPath.rend(); ++iter) {
        iGridIdx = long(*iter);
		
        stPoint.x = iGridIdx % _gridWNum; //grid X坐标
        stPoint.y = iGridIdx / _gridWNum; //grid Y坐标
		if (stStartGrid.x == stPoint.x && stStartGrid.y == stPoint.y){
			continue;
		}
		stPoint = gridToXY(stPoint);
		if (stPath.size() >= 2){
			CCPoint stLastPoint = stPath[stPath.size() - 1];
			CCPoint stNextPoint = stPath[stPath.size() - 2];
			if ((stLastPoint.x == stNextPoint.x && stPoint.x == stLastPoint.x)
				|| (stLastPoint.y == stNextPoint.y && stPoint.y == stLastPoint.y) ){
					stPath.pop_back();					
			}
		}
        //CCLog("(%d,%d)", int(stPoint.x), int(stPoint.y));
        //stPoint = gridToXY(stPoint);
        //stPath.push_back(stPoint);
        stPath.push_back(stPoint);
    }
    pEntity->WalkByPath(stPath);

}

void CBaseMapScene::updateMapBlocks(){
    m_stPathFinder.Init(_gridWNum, _gridHNum, _blocks); //初始化寻路算法。neilwu
    CCLog("[trace]CBaseMapScene::updateMapBlocks, init path finder,w=%d,h=%d,total=%d.blocks size=%d", 
        _gridWNum, _gridHNum, _gridHNum*_gridWNum, _blocks.size());
}

CCRect CBaseMapScene::getPointInRect(const CCPoint& pst){
	CCRect rect;
	for(unsigned int i = 0; i < m_stRects.size(); i++){
		rect = m_stRects[i];
		if(rect.containsPoint(pst)){
			return rect;
		}
	}

	return CCRectZero;
}

void CBaseMapScene::setViewpointCenter(const CCPoint& position){
	float x;
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin(); 
	CCSize screenSize = pDirector->getVisibleSize();
	CCSize innerSize = Multiresolution::getInnerSize();
	//innerSize.width *= 2.0f / 3;
	//innerSize.height *= 2.0f / 3;
	float scale = screenSize.height / innerSize.height;
	float visibleW = screenSize.width/scale;

	if(position.x - walkableRect.getMinX() <= visibleW/2){
		x = walkableRect.getMinX();
	}else if(position.x - walkableRect.getMinX() >= walkableRect.size.width - visibleW/2){
		x = walkableRect.getMaxX() - visibleW;
	}else{
		x = position.x - visibleW/2;
	}

	CCPoint fgViewPoint = ccp(-x, 0);
	_sceneForegound->setPosition(fgViewPoint);

	updateBgViewCenter(fgViewPoint.x);
}

void CBaseMapScene::updateBgViewCenter(float foreX){
	updateBgTextureByRect(_sceneForegound);

	if(_sceneBackground){
		float bgViewX = foreX * BG_RATIO;
		_sceneBackground->setPositionX(bgViewX);
		updateBgTextureByRect(_sceneBackground, true);
	}

	if(_sceneFrontgound){
		float fgViewX = foreX * FG_RATIO;
		_sceneFrontgound->setPositionX(fgViewX);
		updateBgTextureByRect(_sceneFrontgound);
	}

	if(_sceneMidground){
		float midViewX = foreX * MID_RATIO;
		_sceneMidground->setPositionX(midViewX);
		updateBgTextureByRect(_sceneMidground, true);
	}

	if(_sceneMid2ground){
		float midViewX = foreX * MID2_RATIO;
		_sceneMid2ground->setPositionX(midViewX);
		updateBgTextureByRect(_sceneMid2ground, true);
	}
}

void CBaseMapScene::updateBgTextureByRect(CBgTextureNode* pstBgTexture, bool bIsTopAlign){
	CCString* picName = (CCString*)pstBgTexture->getSubTextureNames()->objectAtIndex(0);
	string str = picName->getCString();

	//检查最后一个
	int lastDis = pstBgTexture->getContentSize().width + pstBgTexture->getPositionX();
	if(lastDis < _screenW){
		pstBgTexture->AddNewTexture(str, bIsTopAlign);
		CToolUtils::ArrayChangeOrder(pstBgTexture->getSubTextureNames());
	}
}

void CBaseMapScene::updateResToNum(int id, RESTYPE type, bool isAdd, int num){
	string str;
	if(type == MONSTER_TYPE){
		MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(id);
		str = MONSTER_RES_PATH + monsterCfg->avatar() + "/" + monsterCfg->avatar() + JSON_TYPE_NAME;	

		updateResToNumByStr(str, isAdd, num);
	}else if(type == ITEM_TYPE){
		SceneItemCfg* itemCfg = SCENE_ITEM_CFG->GetSceneItemCfgByID(id);
		str = SCENE_ITEM_RES_PATH + itemCfg->itemres() + "/" + itemCfg->itemres() + JSON_TYPE_NAME;

		updateResToNumByStr(str, isAdd, num);
	}else if(type == SKILL_TYPE){
		const SkillCfg* skillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(id);

		if(skillCfg->castanimation() != "null"){  //验证
			str = SKILL_RES_PATH + skillCfg->castanimation() + "/" + skillCfg->castanimation() + JSON_TYPE_NAME;
			updateResToNumByStr(str, isAdd, num);
		}

		if(skillCfg->flyanimation() != "null"){  //验证
			str = SKILL_RES_PATH + skillCfg->flyanimation() + "/" + skillCfg->flyanimation() + JSON_TYPE_NAME;
			updateResToNumByStr(str, isAdd, num);
		}

		if(skillCfg->hurtanimation() != "null"){  //验证
			str = SKILL_RES_PATH + skillCfg->hurtanimation() + "/" + skillCfg->hurtanimation() + JSON_TYPE_NAME;
			updateResToNumByStr(str, isAdd, num);
		}

		if(skillCfg->floorbuffid() != 0){  //没有地表buff
			const FloorBuffCfg* floorBuffCfg = FLOOR_BUFF_CFG->GetFloorBuffCfgByID(skillCfg->floorbuffid());
			str = SCENE_ITEM_RES_PATH + floorBuffCfg->res() + "/" + floorBuffCfg->res() + JSON_TYPE_NAME;
			updateResToNumByStr(str, isAdd, num);
		}
	}
}

void CBaseMapScene::updateResToNumByStr(string str, bool isAdd, int num){
	map<string, int>::iterator iter = m_stResToNum.find(str);

	if(iter == m_stResToNum.end()){
		if(isAdd){
			m_stResToNum[str] = num;
		}
	}else{
		int number = iter->second;
		if(isAdd){
			number += num;
		}else{
			number -= num;
		}

		m_stResToNum[str] = number;
	}
}


void CBaseMapScene::initSound(){
	_bgSoundName.assign(SCENE_CFG_MASTER->getSceneCfgByID(m_uSceneID)->bgsoundres());
	if (_bgSoundName.length() && _bgSoundName != "null"){
		string preBgSound = AudioManager::getNowBgSound();
		if(preBgSound != _bgSoundName){
			AudioManager::stopBackgroundMusic();
		}
		AudioManager::playBackgroundMusic(_bgSoundName.c_str(), true);
	}
}

