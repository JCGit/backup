#include "FightWeaponArrLayout.h"
#include "BaseFightScene.h"
#include "defines.h"
#include "UserObj.h"
#include "FightObj.h"

#include "WeaponCfgMaster.h"
#include "WeaponTypeCfgMaster.h"

const string CFightWeaponArrLayout::HERO_CHANGE_WEAPON_EVENT = "hero_change_weapon_event";

CFightWeaponArrLayout::CFightWeaponArrLayout(){
	m_iIdx = 0;
	m_iMaxIdx = 0;
	m_iPreIdx = 0;
	m_pTempCell = NULL;
	m_pstOnwerHero = NULL;
}

CFightWeaponArrLayout::~CFightWeaponArrLayout(){
	if(m_stCellLst){
		m_stCellLst->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stCellLst);
	}

	CC_SAFE_RELEASE_NULL(m_pTempCell);
}

CFightWeaponArrLayout* CFightWeaponArrLayout::create(UILayout *pRefLayout) {
	CFightWeaponArrLayout *pRet = new CFightWeaponArrLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CFightWeaponArrLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CFightWeaponArrLayout::initUI(){
	m_pTempCell = CFightWeaponArrCell::create(dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("temp_weapon_bg")));
	m_pTempCell->retain();
	m_pTempCell->hide();

	m_stCellLst = CCArray::create();
	m_stCellLst->retain();

	for(unsigned int i = 0; i < MAX_WEAPON_NUM; i++){
		string str = "weapon_bg_" + intToString(i);
		ImageView* pImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName(str.c_str()));
		CFightWeaponArrCell* pCell = CFightWeaponArrCell::create(pImg);
		pCell->getRefImg()->addTouchEventListener(this, toucheventselector(CFightWeaponArrLayout::touchEventHandler));
		m_stCellLst->addObject(pCell);
	}

	updateUI();
}

void CFightWeaponArrLayout::updateUI(){
	vector<CWeaponArrInfo> stArrInfo = P_USER_OBJ->getUserInfo()->stWeaponArr;

	CFightWeaponArrCell* pCell = NULL;
	CWeaponArrInfo stInfo;
	for(unsigned int i = 0; i < stArrInfo.size(); i++){
		if(i > m_stCellLst->count()){
			break;
		}

		stInfo = stArrInfo.at(i);
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(i));
		pCell->setInfo(stInfo.weaponInfo, stInfo.iIDx, stInfo.iState);

		if(stInfo.weaponInfo.weaponguid() != 0){  //表示有枪械
			m_iMaxIdx = i;
		}
	}

	//没有临时枪械，默认为主武器
	setSelectByIdx(0);
}

void CFightWeaponArrLayout::autoChangeNextWeapon(){
	if(m_iIdx == -1){  //当前选择的是临时枪械
		doTouchByIdx(0);
		return;
	}

	int iStartIdx = 0;
	if(m_iIdx >= m_iMaxIdx){
		iStartIdx = 0;
	}else{
		iStartIdx = m_iIdx + 1;
	}

	int iIdx = 0;
	CFightWeaponArrCell* pCell = NULL;
	for(int i = iStartIdx; i < MAX_WEAPON_NUM; i++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(i));
		if(pCell){
			if(pCell->getInfo().weaponguid() != 0 && pCell->getCurBulletVal() > 0){
				iIdx = i;
				break;
			}
		}
	}

	doTouchByIdx(iIdx);
}

void CFightWeaponArrLayout::showTempWeapon(const CSWeapon& stInfo){
	setSelectByIdx(-1);
	m_pTempCell->setInfo(stInfo, -1, vmsg::WEAPON_ARRAY_CELL_UNLOCKED);
}

void CFightWeaponArrLayout::setSelectByGUID(UINT64_t uWeaponGUID){
	CFightWeaponArrCell* pCell = getCellByInfo(uWeaponGUID);
	if(pCell){
		setSelectByIdx(pCell->getIDX());
	}
}

void CFightWeaponArrLayout::setSelectByIdx(int iIDx){
	m_iIdx = iIDx;

	CFightWeaponArrCell* pCell = NULL;
	for(unsigned int idx = 0; idx < MAX_WEAPON_NUM; idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx));
		if(iIDx == -1){  //临时枪械
			pCell->setSelected(false);
		}else{
			if(idx == iIDx){
				pCell->setSelected(true);
			}else{
				pCell->setSelected(false);
			}
		}
	}

	if(iIDx == -1){
		m_pTempCell->show();
	}else{
		m_pTempCell->hide();
	}
}

void CFightWeaponArrLayout::updateCurCell(float fTime){
	CFightWeaponArrCell* pCell = NULL;
	if(m_iIdx == -1){
		pCell = m_pTempCell;
	}else{
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(m_iIdx));
	}

	if(pCell == NULL){
		return;
	}

	pCell->updateCurBulletShow(fTime);
}

float CFightWeaponArrLayout::getCurCellBulletVal(){
	CFightWeaponArrCell* pCell = NULL;
	if(m_iIdx == -1){
		pCell = m_pTempCell;
	}else{
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(m_iIdx));
	}

	if(pCell == NULL){
		return 0;
	}

	return pCell->getCurBulletVal();
}

void CFightWeaponArrLayout::recordAllBulletVal(CVArray<float, 6>& stArr){
	stArr.Clear();

	if(m_pTempCell->getIsShow()){
		stArr.AddOneItem(m_pTempCell->getCurBulletVal());
	}else{
		stArr.AddOneItem(0);
	}

	CFightWeaponArrCell* pCell = NULL;
	for(unsigned int idx = 0; idx < MAX_WEAPON_NUM; idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx));
		if(pCell){
			if(pCell->getIsShow()){
				stArr.AddOneItem(pCell->getCurBulletVal());
			}else{
				stArr.AddOneItem(0);
			}
		}
	}
}

void CFightWeaponArrLayout::updateCellsByVec(CVArray<float, 6> stArr){
	if(m_pTempCell->getIsShow()){
		m_pTempCell->updateCurBulletShow(stArr[0]);
	}

	CFightWeaponArrCell* pCell = NULL;
	for(int idx = 1; idx < stArr.GetCount(); idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx - 1));
		if(pCell && pCell->getIsShow()){
			pCell->updateCurBulletShow(stArr[idx]);
		}
	}
}

bool CFightWeaponArrLayout::IsAllWeaponOutBullet(){
	CFightWeaponArrCell* pCell = NULL;
	for(unsigned int idx = 0; idx < MAX_WEAPON_NUM; idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx));
		if(pCell->getInfo().weaponguid() != 0 && pCell->getCurBulletVal() > 0){
			return false;
		}
	}

	return true;
}

void CFightWeaponArrLayout::fillAllWeaponBullet(){
	if(m_pTempCell->getIsShow()){
		m_pTempCell->setCurBulletVal(m_pTempCell->getMaxBulletVal());
		m_pTempCell->updateCurBulletShow(m_pTempCell->getMaxBulletVal());
	}

	CFightWeaponArrCell* pCell = NULL;
	for(unsigned int idx = 0; idx < MAX_WEAPON_NUM; idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx));
		if(pCell){
			pCell->setCurBulletVal(pCell->getMaxBulletVal());
			pCell->updateCurBulletShow(pCell->getMaxBulletVal());
		}
	}
}

CFightWeaponArrCell* CFightWeaponArrLayout::getCellByInfo(UINT64_t uWeaponGUID){
	if(uWeaponGUID == TEMP_WEAPON_GUID){
		return m_pTempCell;
	}

	CFightWeaponArrCell* pCell = NULL;
	for(unsigned int idx = 0; idx < MAX_WEAPON_NUM; idx++){
		pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(idx));
		if(uWeaponGUID == pCell->getInfo().weaponguid()){
			return pCell;
		}
	}

	return NULL;
}

void CFightWeaponArrLayout::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strWidgetName.find_first_of("weapon_bg_") == 0){
			string sub = strWidgetName.substr(strWidgetName.find_first_not_of("weapon_bg_"));
			int iIdx = atoi(sub.c_str());
			doTouchByIdx(iIdx);
		}
		break;
	default:

		break;
	}
}

void CFightWeaponArrLayout::doTouchByIdx(int iIDx){
	if(iIDx > MAX_WEAPON_NUM){
		return;
	}

	if (m_pstOnwerHero == 0){ 
		return;
	}

	if (!m_pstOnwerHero->IsActionInterruptable() || m_pstOnwerHero->getIsTheOne()){
		return;

	}
	CFightWeaponArrCell* pCell = dynamic_cast<CFightWeaponArrCell*>(m_stCellLst->objectAtIndex(iIDx));
	if(pCell == NULL || pCell->getState() == vmsg::WEAPON_ARRAY_CELL_LOCKED || pCell->getInfo().weaponguid() == 0){
		return;
	}

	//临时枪械消失
	if(m_pTempCell->getIsShow()){
		m_pTempCell->hide();
	}

	m_iPreIdx = m_iIdx;
	//换枪
	setSelectByIdx(iIDx);

	if(pCell->getInfo().weaponguid() == P_USER_OBJ->getUserInfo()->weapon.weaponguid()
		&& pCell->getInfo().cfgid() == P_USER_OBJ->getUserInfo()->weapon.cfgid()){
			return;
	}	

	P_FIGHT_OBJ->setCurBulletVal(pCell->getCurBulletVal());
	P_USER_OBJ->updateWeapon(pCell->getInfo());
	DispatchEvent(HERO_CHANGE_WEAPON_EVENT, this);
}


//////////////////////////////////////////////////////////////////////////
CFightWeaponArrCell::CFightWeaponArrCell(){
	m_pRefImg  = NULL; 
	m_pIconImg = NULL;
	m_pBulletTxt = NULL;

	m_fCurBulletVal = 0;
	m_iIDx = 0;
	
	m_iState= 0;

	m_bSelected = false;
	m_bIsShow = true;
}

CFightWeaponArrCell* CFightWeaponArrCell::create(ImageView *pRefLayout) {
	CFightWeaponArrCell *pRet = new CFightWeaponArrCell();

	if(pRet != NULL && pRet->initWithImg(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CFightWeaponArrCell::initWithImg(ImageView* pImg){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pRefImg = pImg;
		m_pRefImg->setTouchEnabled(true);
		m_pIconImg = dynamic_cast<ImageView*>(m_pRefImg->getChildByName("weapon_icon"));
		m_pBulletBg = dynamic_cast<ImageView*>(m_pRefImg->getChildByName("bullet_bg"));
		m_pBulletTxt = dynamic_cast<Label*>(m_pRefImg->getChildByName("bullet_val_txt"));

		bRet = true;
	} while (0);

	return bRet;
}

void CFightWeaponArrCell::setInfo(const CSWeapon& stInfo, const int iIDx, const int iState){
	m_iIDx = iIDx;
	m_iState = iState;
	m_stInfo = stInfo;

	string urlStr = "";
	bool bIsTouched = true;
	if(iState == vmsg::WEAPON_ARRAY_CELL_LOCKED){			//未开启
		urlStr = "fight/fight_weapon_icon_locked_bg.png";
		bIsTouched = false;
	}else{
		if(m_stInfo.weaponguid() == 0){			//未装备
			urlStr = "fight/fight_weapon_icon_no_equip_bg.png";
			bIsTouched = false;
		}else{
			urlStr = "fight/fight_weapon_icon_bg.png";
			bIsTouched = true;
		}
	}

	m_pRefImg->setTouchEnabled(bIsTouched);

	if(m_iIDx == -1){
		m_pRefImg->loadTexture("fight/temp_weapon_icon_bg.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pRefImg->loadTexture(urlStr.c_str(), UI_TEX_TYPE_PLIST);
	}

	if(!bIsTouched){
		m_pIconImg->setVisible(false);
		m_pBulletBg->setVisible(false);
		m_pBulletTxt->setVisible(false);
		return;
	}

	m_pIconImg->setVisible(true);
	m_pBulletBg->setVisible(true);
	m_pBulletTxt->setVisible(true);

	const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stInfo.cfgid());
	if(NULL == stCfg) {
		return;
	}

	m_fMaxBulletVal = stCfg->bulletval()*1.0f/1000.0f;
	m_fCurBulletVal = stCfg->bulletval()*1.0f/1000.0f;

	m_pBulletTxt->setText(intToString(stCfg->bulletshowval()));

	const WeaponTypeCfg* stTypeCfg = WEAPON_TYPE_CFG->GetWeaponTypeCfgByID(stCfg->weapontype());
	if(stTypeCfg == NULL){
		return;
	}

	string iconUrl = "icon/weapon_icon/" + stTypeCfg->icon() + ".png";
	m_pIconImg->loadTexture(iconUrl.c_str());
}

void CFightWeaponArrCell::updateCurBulletShow(float fCurBulletVal){
	m_fCurBulletVal = fCurBulletVal;

	const WeaponCfg* stCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stInfo.cfgid());
	if(NULL == stCfg) {
		return;
	}

	unsigned int uCurShowVal = ceil(stCfg->bulletshowval()*(m_fCurBulletVal/m_fMaxBulletVal));
	m_pBulletTxt->setText(intToString(uCurShowVal));
}

void CFightWeaponArrCell::setSelected(bool var){
	if(m_iIDx == -1){   //临时枪械直接选中
		return;
	}

	if(m_iState == vmsg::WEAPON_ARRAY_CELL_LOCKED || m_stInfo.weaponguid() == 0){
		return;
	}

	m_bSelected = var;

	string urlStr = "";
	if(m_bSelected){
		urlStr = "fight/fight_weapon_icon_select_bg.png";
	}else{
		urlStr = "fight/fight_weapon_icon_bg.png";
	}

	m_pRefImg->loadTexture(urlStr.c_str(), UI_TEX_TYPE_PLIST);
}

void CFightWeaponArrCell::show(){
	m_bIsShow = true;
	if(m_pRefImg){
		m_pRefImg->setVisible(true);
		m_pRefImg->setTouchEnabled(true);
	}
}

void CFightWeaponArrCell::hide(){
	m_bIsShow = false;
	if(m_pRefImg){
		m_pRefImg->setVisible(false);
		m_pRefImg->setTouchEnabled(false);
	}
}


