/*
** Lua binding: Gamelua
** Generated automatically by tolua++-1.0.92 on 05/14/15 14:59:55.
*/

/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
 #ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"
#include "tolua++.h"
#include "dfont/dfont_manager.h"
using namespace dfont;
 

/* Exported function */
TOLUA_API int  tolua_Gamelua_open (lua_State* tolua_S);

#include "Gamelua.h"
#include "CCBScriptContainer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
   USING_NS_CC_EXT;
#include "TimeCalculator.h"
#include "AES.h"
#include "GameMaths.h"
#include "inifile.h"
#include "MainFrame.h"
#include "Language.h"
#include "SoundManager.h"
#include "BlackBoard.h"
#include "MessageBoxPage.h"
#include "ServerDateManager.h"
#include "DataTableManager.h"
#include "CCBManager.h"
#include "ScriptContentBase.h"
#include "PacketManager.h"
#include "ScriptMathToLua.h"
#include "GameMessages.h"
#include "MessageManager.h"
#include "libOS.h"
#include "libPlatform.h"
#include "RestrictedWord.h"
#include "GamePrecedure.h"
#include "CShiftToAction.h"
#include "AnimMgr.h"
#include "CCBReader/CCBFileNew.h"
#include "CoverSprite.h"
#include "SpineContainer.h"
#include "ArmatureContainer.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CCPoint (lua_State* tolua_S)
{
 CCPoint* self = (CCPoint*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameRefreshPage (lua_State* tolua_S)
{
 MsgMainFrameRefreshPage* self = (MsgMainFrameRefreshPage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_NetworkStatus (lua_State* tolua_S)
{
 NetworkStatus* self = (NetworkStatus*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFriendsDetailReceivePage (lua_State* tolua_S)
{
 MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgBuyPropPopupPage (lua_State* tolua_S)
{
 MsgBuyPropPopupPage* self = (MsgBuyPropPopupPage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCReViScrollViewFacade (lua_State* tolua_S)
{
 CCReViScrollViewFacade* self = (CCReViScrollViewFacade*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgLoadingFrameEnterGame (lua_State* tolua_S)
{
 MsgLoadingFrameEnterGame* self = (MsgLoadingFrameEnterGame*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_LoginInfo (lua_State* tolua_S)
{
 LoginInfo* self = (LoginInfo*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_AES (lua_State* tolua_S)
{
 AES* self = (AES*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgLeaveMsgReceivePage (lua_State* tolua_S)
{
 MsgLeaveMsgReceivePage* self = (MsgLeaveMsgReceivePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgToTab (lua_State* tolua_S)
{
 MsgToTab* self = (MsgToTab*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgUpdateLeaguaContribution (lua_State* tolua_S)
{
 MsgUpdateLeaguaContribution* self = (MsgUpdateLeaguaContribution*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFriendsDetailDisposePage (lua_State* tolua_S)
{
 MsgFriendsDetailDisposePage* self = (MsgFriendsDetailDisposePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_ccColor3B (lua_State* tolua_S)
{
 ccColor3B* self = (ccColor3B*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgInstructionNext (lua_State* tolua_S)
{
 MsgInstructionNext* self = (MsgInstructionNext*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCRect (lua_State* tolua_S)
{
 CCRect* self = (CCRect*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgTitleStatusChange (lua_State* tolua_S)
{
 MsgTitleStatusChange* self = (MsgTitleStatusChange*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameCoverShow (lua_State* tolua_S)
{
 MsgMainFrameCoverShow* self = (MsgMainFrameCoverShow*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFramePushPage (lua_State* tolua_S)
{
 MsgMainFramePushPage* self = (MsgMainFramePushPage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CoverSprite (lua_State* tolua_S)
{
 CoverSprite* self = (CoverSprite*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFightRoundLoadSkill (lua_State* tolua_S)
{
 MsgFightRoundLoadSkill* self = (MsgFightRoundLoadSkill*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_BUYINFO (lua_State* tolua_S)
{
 BUYINFO* self = (BUYINFO*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MessageScriptHandler (lua_State* tolua_S)
{
 MessageScriptHandler* self = (MessageScriptHandler*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgAdventurePageChange (lua_State* tolua_S)
{
 MsgAdventurePageChange* self = (MsgAdventurePageChange*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgRechargeSuccess (lua_State* tolua_S)
{
 MsgRechargeSuccess* self = (MsgRechargeSuccess*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgSeverInfoUpdate (lua_State* tolua_S)
{
 MsgSeverInfoUpdate* self = (MsgSeverInfoUpdate*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCSize (lua_State* tolua_S)
{
 CCSize* self = (CCSize*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_GameScriptMessage (lua_State* tolua_S)
{
 GameScriptMessage* self = (GameScriptMessage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameGetNewInfo (lua_State* tolua_S)
{
 MsgMainFrameGetNewInfo* self = (MsgMainFrameGetNewInfo*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgGoToPrivateChatPageFromLeague (lua_State* tolua_S)
{
 MsgGoToPrivateChatPageFromLeague* self = (MsgGoToPrivateChatPageFromLeague*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFightPageExit (lua_State* tolua_S)
{
 MsgFightPageExit* self = (MsgFightPageExit*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgButtonPressed (lua_State* tolua_S)
{
 MsgButtonPressed* self = (MsgButtonPressed*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFriendsPageChangePage (lua_State* tolua_S)
{
 MsgFriendsPageChangePage* self = (MsgFriendsPageChangePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCBFileNew (lua_State* tolua_S)
{
 CCBFileNew* self = (CCBFileNew*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgFightEnd (lua_State* tolua_S)
{
 MsgFightEnd* self = (MsgFightEnd*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_GameCheckInfo (lua_State* tolua_S)
{
 GameCheckInfo* self = (GameCheckInfo*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgAdventureRemoveItem (lua_State* tolua_S)
{
 MsgAdventureRemoveItem* self = (MsgAdventureRemoveItem*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgGoToPrivateChatPage (lua_State* tolua_S)
{
 MsgGoToPrivateChatPage* self = (MsgGoToPrivateChatPage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_CCReViSvItemData (lua_State* tolua_S)
{
 CCReViSvItemData* self = (CCReViSvItemData*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgAdventurePageHasChange (lua_State* tolua_S)
{
 MsgAdventurePageHasChange* self = (MsgAdventurePageHasChange*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_ServerDateManager (lua_State* tolua_S)
{
 ServerDateManager* self = (ServerDateManager*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFramePopPage (lua_State* tolua_S)
{
 MsgMainFramePopPage* self = (MsgMainFramePopPage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgCareerChangePage (lua_State* tolua_S)
{
 MsgCareerChangePage* self = (MsgCareerChangePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_ConfigFile (lua_State* tolua_S)
{
 ConfigFile* self = (ConfigFile*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameMSGShow (lua_State* tolua_S)
{
 MsgMainFrameMSGShow* self = (MsgMainFrameMSGShow*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameChangePage (lua_State* tolua_S)
{
 MsgMainFrameChangePage* self = (MsgMainFrameChangePage*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_MsgMainFrameCoverHide (lua_State* tolua_S)
{
 MsgMainFrameCoverHide* self = (MsgMainFrameCoverHide*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}

static int tolua_collect_PacketScriptHandler (lua_State* tolua_S)
{
 PacketScriptHandler* self = (PacketScriptHandler*) tolua_tousertype(tolua_S,1,0);
    Mtolua_delete(self);
    return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GameCheckInfo");
 tolua_usertype(tolua_S,"MsgMainFrameRefreshPage");
 tolua_usertype(tolua_S,"NetworkStatus");
 tolua_usertype(tolua_S,"ServerDateManager");
 tolua_usertype(tolua_S,"MsgBuyPropPopupPage");
 tolua_usertype(tolua_S,"std::stringstream");
 tolua_usertype(tolua_S,"CCMenuItemCCBFile");
 tolua_usertype(tolua_S,"SoundManager");
 tolua_usertype(tolua_S,"ScriptMathToLua");
 tolua_usertype(tolua_S,"ConfigFile");
 tolua_usertype(tolua_S,"CCNode");
 tolua_usertype(tolua_S,"int64_t");
 tolua_usertype(tolua_S,"AnimMgr");
 tolua_usertype(tolua_S,"MsgLeaveMsgReceivePage");
 tolua_usertype(tolua_S,"std::ios::fmtflags");
 tolua_usertype(tolua_S,"MainFrame");
 tolua_usertype(tolua_S,"ScriptContentBase");
 tolua_usertype(tolua_S,"CoverSprite");
 tolua_usertype(tolua_S,"ArmatureEventListener");
 tolua_usertype(tolua_S,"PlatformRoleItem");
 tolua_usertype(tolua_S,"BlackBoard");
 tolua_usertype(tolua_S,"libPlatformManager");
 tolua_usertype(tolua_S,"CCArmature");
 tolua_usertype(tolua_S,"MsgMainFrameCoverShow");
 tolua_usertype(tolua_S,"MsgMainFramePopPage");
 tolua_usertype(tolua_S,"SpineEventListener");
 tolua_usertype(tolua_S,"SpineContainer");
 tolua_usertype(tolua_S,"CCTexture2D");
 tolua_usertype(tolua_S,"std::set<unsigned long>");
 tolua_usertype(tolua_S,"CCBFileListener");
 tolua_usertype(tolua_S,"CCBMemberVariableAssigner");
 tolua_usertype(tolua_S,"CCParticleSystem");
 tolua_usertype(tolua_S,"MsgRechargeSuccess");
 tolua_usertype(tolua_S,"PacketManager");
 tolua_usertype(tolua_S,"CCBSelectorResolver");
 tolua_usertype(tolua_S,"TableReader");
 tolua_usertype(tolua_S,"CShiftObjectToYAction");
 tolua_usertype(tolua_S,"StringConverter");
 tolua_usertype(tolua_S,"TimeCalculator");
 tolua_usertype(tolua_S,"cocos2d::CCObject");
 tolua_usertype(tolua_S,"CCBManager");
 tolua_usertype(tolua_S,"CCBFileNew");
 tolua_usertype(tolua_S,"CShiftMaskToYAction");
 tolua_usertype(tolua_S,"CCLabelTTF");
 tolua_usertype(tolua_S,"cocos2d::CCNode");
 tolua_usertype(tolua_S,"CShiftObjectToXAction");
 tolua_usertype(tolua_S,"MsgInstructionNext");
 tolua_usertype(tolua_S,"MsgAdventurePageHasChange");
 tolua_usertype(tolua_S,"CCScale9Sprite");
 tolua_usertype(tolua_S,"std::map<int,std::string>");
 tolua_usertype(tolua_S,"RestrictedWord");
 tolua_usertype(tolua_S,"CCSprite");
 tolua_usertype(tolua_S,"CShiftMaskToXAction");
 tolua_usertype(tolua_S,"CCBContainer");
 tolua_usertype(tolua_S,"libPlatform");
 tolua_usertype(tolua_S,"MessageHandler");
 tolua_usertype(tolua_S,"CCPoint");
 tolua_usertype(tolua_S,"BUYINFO");
 tolua_usertype(tolua_S,"CCLayer");
 tolua_usertype(tolua_S,"MsgGoToPrivateChatPageFromLeague");
 tolua_usertype(tolua_S,"MsgAdventurePageChange");
 tolua_usertype(tolua_S,"CCMenu");
 tolua_usertype(tolua_S,"libOS");
 tolua_usertype(tolua_S,"CCReViScrollViewFacade");
 tolua_usertype(tolua_S,"MessageManager");
 tolua_usertype(tolua_S,"ShiftToManager");
 tolua_usertype(tolua_S,"GameScriptMessage");
 tolua_usertype(tolua_S,"CCScrollView");
 tolua_usertype(tolua_S,"PacketScriptHandler");
 tolua_usertype(tolua_S,"LoginInfo");
 tolua_usertype(tolua_S,"GameMessage");
 tolua_usertype(tolua_S,"MsgFightEnd");
 tolua_usertype(tolua_S,"AES");
 tolua_usertype(tolua_S,"MsgMainFrameGetNewInfo");
 tolua_usertype(tolua_S,"MsgFightPageExit");
 tolua_usertype(tolua_S,"MsgFightRoundLoadSkill");
 tolua_usertype(tolua_S,"MsgToTab");
 tolua_usertype(tolua_S,"MsgButtonPressed");
 tolua_usertype(tolua_S,"TableAutoReader");
 tolua_usertype(tolua_S,"TableReaderManager");
 tolua_usertype(tolua_S,"MsgUpdateLeaguaContribution");
 tolua_usertype(tolua_S,"MsgFriendsDetailDisposePage");
 tolua_usertype(tolua_S,"CCActionInterval");
 tolua_usertype(tolua_S,"MsgAdventureRemoveItem");
 tolua_usertype(tolua_S,"CCLabelBMFont");
 tolua_usertype(tolua_S,"CCObject");
 tolua_usertype(tolua_S,"MsgTitleStatusChange");
 tolua_usertype(tolua_S,"std::map<std::string,std::string>");
 tolua_usertype(tolua_S,"MsgSeverInfoUpdate");
 tolua_usertype(tolua_S,"MsgMainFramePushPage");
 tolua_usertype(tolua_S,"MsgFriendsDetailReceivePage");
 tolua_usertype(tolua_S,"ccColor3B");
 tolua_usertype(tolua_S,"CCMenuItemImage");
 tolua_usertype(tolua_S,"MsgFriendsPageChangePage");
 tolua_usertype(tolua_S,"CCMenuItemSprite");
 tolua_usertype(tolua_S,"PacketHandler");
 tolua_usertype(tolua_S,"MsgMainFrameCoverHide");
 tolua_usertype(tolua_S,"Singleton<ServerDateManager>");
 tolua_usertype(tolua_S,"CCSize");
 tolua_usertype(tolua_S,"CCMenuItem");
 tolua_usertype(tolua_S,"MessageBoxPage");
 tolua_usertype(tolua_S,"CCBScriptContainer");
 tolua_usertype(tolua_S,"Language");
 tolua_usertype(tolua_S,"BasePage");
 tolua_usertype(tolua_S,"MessageScriptHandler");
 tolua_usertype(tolua_S,"MsgMainFrameMSGShow");
 tolua_usertype(tolua_S,"ArmatureContainer");
 tolua_usertype(tolua_S,"MsgLoadingFrameEnterGame");
 tolua_usertype(tolua_S,"CCRect");
 tolua_usertype(tolua_S,"MsgGoToPrivateChatPage");
 tolua_usertype(tolua_S,"CCReViSvItemData");
 
 tolua_usertype(tolua_S,"CCReViSvItemNodeFacade");
 tolua_usertype(tolua_S,"FontFactory");
 tolua_usertype(tolua_S,"MsgCareerChangePage");
 tolua_usertype(tolua_S,"GamePrecedure");
 tolua_usertype(tolua_S,"PlatformRoleTableManager");
 tolua_usertype(tolua_S,"MsgMainFrameChangePage");
 tolua_usertype(tolua_S,"VaribleManager");
 tolua_usertype(tolua_S,"GameMaths");
}

/* method: create of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_create00
static int tolua_Gamelua_CCBScriptContainer_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string ccbfile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int tag = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   CCBScriptContainer* tolua_ret = (CCBScriptContainer*)  CCBScriptContainer::create(ccbfile,tag);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCBScriptContainer");
   tolua_pushcppstring(tolua_S,(const char*)ccbfile);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: purgeCachedData of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_purgeCachedData00
static int tolua_Gamelua_CCBScriptContainer_purgeCachedData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCBScriptContainer::purgeCachedData();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'purgeCachedData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendPakcet of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_sendPakcet00
static int tolua_Gamelua_CCBScriptContainer_sendPakcet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  char* buff = ((char*)  tolua_tostring(tolua_S,3,0));
  int length = ((int)  tolua_tonumber(tolua_S,4,0));
  bool needWaiting = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendPakcet'", NULL);
#endif
  {
   self->sendPakcet(opcode,buff,length,needWaiting);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendPakcet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerMessage of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_registerMessage00
static int tolua_Gamelua_CCBScriptContainer_registerMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int msgID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerMessage'", NULL);
#endif
  {
   self->registerMessage(msgID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerPacket of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_registerPacket00
static int tolua_Gamelua_CCBScriptContainer_registerPacket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerPacket'", NULL);
#endif
  {
   self->registerPacket(opcode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerPacket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeMessage of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_removeMessage00
static int tolua_Gamelua_CCBScriptContainer_removeMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int msgID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeMessage'", NULL);
#endif
  {
   self->removeMessage(msgID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removePacket of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_removePacket00
static int tolua_Gamelua_CCBScriptContainer_removePacket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removePacket'", NULL);
#endif
  {
   self->removePacket(opcode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removePacket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerLibOS of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_registerLibOS00
static int tolua_Gamelua_CCBScriptContainer_registerLibOS00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerLibOS'", NULL);
#endif
  {
   self->registerLibOS();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerLibOS'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeLibOS of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_removeLibOS00
static int tolua_Gamelua_CCBScriptContainer_removeLibOS00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeLibOS'", NULL);
#endif
  {
   self->removeLibOS();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeLibOS'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInputboxContent of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getInputboxContent00
static int tolua_Gamelua_CCBScriptContainer_getInputboxContent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInputboxContent'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getInputboxContent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInputboxContent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMessageboxTag of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getMessageboxTag00
static int tolua_Gamelua_CCBScriptContainer_getMessageboxTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMessageboxTag'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getMessageboxTag();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMessageboxTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadCcbiFile of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_loadCcbiFile00
static int tolua_Gamelua_CCBScriptContainer_loadCcbiFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool froceLoad = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadCcbiFile'", NULL);
#endif
  {
   self->loadCcbiFile(filename,froceLoad);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadCcbiFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runAnimation of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_runAnimation00
static int tolua_Gamelua_CCBScriptContainer_runAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  const std::string actionname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runAnimation'", NULL);
#endif
  {
   self->runAnimation(actionname);
   tolua_pushcppstring(tolua_S,(const char*)actionname);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_load00
static int tolua_Gamelua_CCBScriptContainer_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarNode of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarNode00
static int tolua_Gamelua_CCBScriptContainer_getVarNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarNode'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getVarNode(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarSprite of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarSprite00
static int tolua_Gamelua_CCBScriptContainer_getVarSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarSprite'", NULL);
#endif
  {
   CCSprite* tolua_ret = (CCSprite*)  self->getVarSprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarLabelBMFont of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarLabelBMFont00
static int tolua_Gamelua_CCBScriptContainer_getVarLabelBMFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarLabelBMFont'", NULL);
#endif
  {
   CCLabelBMFont* tolua_ret = (CCLabelBMFont*)  self->getVarLabelBMFont(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelBMFont");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarLabelBMFont'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarLabelTTF of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarLabelTTF00
static int tolua_Gamelua_CCBScriptContainer_getVarLabelTTF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarLabelTTF'", NULL);
#endif
  {
   CCLabelTTF* tolua_ret = (CCLabelTTF*)  self->getVarLabelTTF(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelTTF");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarLabelTTF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenu of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarMenu00
static int tolua_Gamelua_CCBScriptContainer_getVarMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenu'", NULL);
#endif
  {
   CCMenu* tolua_ret = (CCMenu*)  self->getVarMenu(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItem of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarMenuItem00
static int tolua_Gamelua_CCBScriptContainer_getVarMenuItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItem'", NULL);
#endif
  {
   CCMenuItem* tolua_ret = (CCMenuItem*)  self->getVarMenuItem(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemSprite of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarMenuItemSprite00
static int tolua_Gamelua_CCBScriptContainer_getVarMenuItemSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemSprite'", NULL);
#endif
  {
   CCMenuItemSprite* tolua_ret = (CCMenuItemSprite*)  self->getVarMenuItemSprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemImage of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarMenuItemImage00
static int tolua_Gamelua_CCBScriptContainer_getVarMenuItemImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemImage'", NULL);
#endif
  {
   CCMenuItemImage* tolua_ret = (CCMenuItemImage*)  self->getVarMenuItemImage(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemImage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarScrollView of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarScrollView00
static int tolua_Gamelua_CCBScriptContainer_getVarScrollView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarScrollView'", NULL);
#endif
  {
   CCScrollView* tolua_ret = (CCScrollView*)  self->getVarScrollView(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScrollView");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarScrollView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarScale9Sprite of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarScale9Sprite00
static int tolua_Gamelua_CCBScriptContainer_getVarScale9Sprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarScale9Sprite'", NULL);
#endif
  {
   CCScale9Sprite* tolua_ret = (CCScale9Sprite*)  self->getVarScale9Sprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScale9Sprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarScale9Sprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParticleSystem of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getParticleSystem00
static int tolua_Gamelua_CCBScriptContainer_getParticleSystem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParticleSystem'", NULL);
#endif
  {
   CCParticleSystem* tolua_ret = (CCParticleSystem*)  self->getParticleSystem(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCParticleSystem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParticleSystem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemCCB of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarMenuItemCCB00
static int tolua_Gamelua_CCBScriptContainer_getVarMenuItemCCB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemCCB'", NULL);
#endif
  {
   CCMenuItemCCBFile* tolua_ret = (CCMenuItemCCBFile*)  self->getVarMenuItemCCB(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemCCBFile");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemCCB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarLayer of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getVarLayer00
static int tolua_Gamelua_CCBScriptContainer_getVarLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarLayer'", NULL);
#endif
  {
   CCLayer* tolua_ret = (CCLayer*)  self->getVarLayer(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMessage of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getMessage00
static int tolua_Gamelua_CCBScriptContainer_getMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMessage'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->getMessage();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSendPacketOpcode of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getSendPacketOpcode00
static int tolua_Gamelua_CCBScriptContainer_getSendPacketOpcode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSendPacketOpcode'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getSendPacketOpcode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSendPacketOpcode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRecPacketOpcode of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getRecPacketOpcode00
static int tolua_Gamelua_CCBScriptContainer_getRecPacketOpcode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRecPacketOpcode'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getRecPacketOpcode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRecPacketOpcode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRecPacketBufferLength of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getRecPacketBufferLength00
static int tolua_Gamelua_CCBScriptContainer_getRecPacketBufferLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRecPacketBufferLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getRecPacketBufferLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRecPacketBufferLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRecPacketBuffer of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getRecPacketBuffer00
static int tolua_Gamelua_CCBScriptContainer_getRecPacketBuffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRecPacketBuffer'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getRecPacketBuffer();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRecPacketBuffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTag of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_setTag00
static int tolua_Gamelua_CCBScriptContainer_setTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTag'", NULL);
#endif
  {
   self->setTag(tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTag of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getTag00
static int tolua_Gamelua_CCBScriptContainer_getTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTag'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getTag();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: searchChildByTag of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_searchChildByTag00
static int tolua_Gamelua_CCBScriptContainer_searchChildByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'searchChildByTag'", NULL);
#endif
  {
   CCBScriptContainer* tolua_ret = (CCBScriptContainer*)  self->searchChildByTag(tag);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCBScriptContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'searchChildByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: searchParentByTag of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_searchParentByTag00
static int tolua_Gamelua_CCBScriptContainer_searchParentByTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'searchParentByTag'", NULL);
#endif
  {
   CCBScriptContainer* tolua_ret = (CCBScriptContainer*)  self->searchParentByTag(tag);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCBScriptContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'searchParentByTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerFunctionHandler of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_registerFunctionHandler00
static int tolua_Gamelua_CCBScriptContainer_registerFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerFunctionHandler'", NULL);
#endif
  {
   self->registerFunctionHandler(nHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterFunctionHandler of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_unregisterFunctionHandler00
static int tolua_Gamelua_CCBScriptContainer_unregisterFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterFunctionHandler'", NULL);
#endif
  {
   self->unregisterFunctionHandler();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDynamicIconTex of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_setDynamicIconTex00
static int tolua_Gamelua_CCBScriptContainer_setDynamicIconTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  std::string frameName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string iconName = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string iconStr = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDynamicIconTex'", NULL);
#endif
  {
   self->setDynamicIconTex(frameName,iconName,iconStr);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDynamicIconTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFrameColorByQuality of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_setFrameColorByQuality00
static int tolua_Gamelua_CCBScriptContainer_setFrameColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  std::string frameName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int quality = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFrameColorByQuality'", NULL);
#endif
  {
   self->setFrameColorByQuality(frameName,quality);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFrameColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLabelColorByQuality of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_setLabelColorByQuality00
static int tolua_Gamelua_CCBScriptContainer_setLabelColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  std::string labelName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int quality = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLabelColorByQuality'", NULL);
#endif
  {
   self->setLabelColorByQuality(labelName,quality);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLabelColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dumpInfo of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_dumpInfo00
static int tolua_Gamelua_CCBScriptContainer_dumpInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dumpInfo'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->dumpInfo();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dumpInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: autoAdjustResizeScrollview of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScrollview00
static int tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScrollview00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  CCScrollView* pView = ((CCScrollView*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'autoAdjustResizeScrollview'", NULL);
#endif
  {
   self->autoAdjustResizeScrollview(pView);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'autoAdjustResizeScrollview'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: autoAdjustResizeScale9Sprite of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScale9Sprite00
static int tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScale9Sprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScale9Sprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
  CCScale9Sprite* pscale9Sprite = ((CCScale9Sprite*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'autoAdjustResizeScale9Sprite'", NULL);
#endif
  {
   self->autoAdjustResizeScale9Sprite(pscale9Sprite);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'autoAdjustResizeScale9Sprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurAnimationDoneName of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_getCurAnimationDoneName00
static int tolua_Gamelua_CCBScriptContainer_getCurAnimationDoneName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurAnimationDoneName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getCurAnimationDoneName();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurAnimationDoneName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unload of class  CCBScriptContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBScriptContainer_unload00
static int tolua_Gamelua_CCBScriptContainer_unload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBScriptContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBScriptContainer* self = (CCBScriptContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unload'", NULL);
#endif
  {
   self->unload();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unload'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_create00
static int tolua_Gamelua_CCScrollView_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,3,"CCNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSize size = *((CCSize*)  tolua_tousertype(tolua_S,2,0));
  CCNode* container = ((CCNode*)  tolua_tousertype(tolua_S,3,NULL));
  {
   CCScrollView* tolua_ret = (CCScrollView*)  CCScrollView::create(size,container);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScrollView");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_create01
static int tolua_Gamelua_CCScrollView_create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   CCScrollView* tolua_ret = (CCScrollView*)  CCScrollView::create();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScrollView");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_CCScrollView_create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithViewSize of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_initWithViewSize00
static int tolua_Gamelua_CCScrollView_initWithViewSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,3,"CCNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCSize size = *((CCSize*)  tolua_tousertype(tolua_S,2,0));
  CCNode* container = ((CCNode*)  tolua_tousertype(tolua_S,3,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithViewSize'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithViewSize(size,container);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithViewSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setContentOffset of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setContentOffset00
static int tolua_Gamelua_CCScrollView_setContentOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCPoint offset = *((CCPoint*)  tolua_tousertype(tolua_S,2,0));
  bool animated = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setContentOffset'", NULL);
#endif
  {
   self->setContentOffset(offset,animated);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setContentOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContentOffset of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getContentOffset00
static int tolua_Gamelua_CCScrollView_getContentOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContentOffset'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->getContentOffset();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContentOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setContentOffsetInDuration of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setContentOffsetInDuration00
static int tolua_Gamelua_CCScrollView_setContentOffsetInDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCPoint offset = *((CCPoint*)  tolua_tousertype(tolua_S,2,0));
  float dt = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setContentOffsetInDuration'", NULL);
#endif
  {
   self->setContentOffsetInDuration(offset,dt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setContentOffsetInDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setZoomScale of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setZoomScale00
static int tolua_Gamelua_CCScrollView_setZoomScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  float s = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setZoomScale'", NULL);
#endif
  {
   self->setZoomScale(s);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setZoomScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setZoomScale of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setZoomScale01
static int tolua_Gamelua_CCScrollView_setZoomScale01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  float s = ((float)  tolua_tonumber(tolua_S,2,0));
  bool animated = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setZoomScale'", NULL);
#endif
  {
   self->setZoomScale(s,animated);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_CCScrollView_setZoomScale00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getZoomScale of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getZoomScale00
static int tolua_Gamelua_CCScrollView_getZoomScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getZoomScale'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getZoomScale();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getZoomScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScrollDeaccelRate of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setScrollDeaccelRate00
static int tolua_Gamelua_CCScrollView_setScrollDeaccelRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  float rate = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScrollDeaccelRate'", NULL);
#endif
  {
   self->setScrollDeaccelRate(rate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScrollDeaccelRate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScrollDeaccelRate of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getScrollDeaccelRate00
static int tolua_Gamelua_CCScrollView_getScrollDeaccelRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScrollDeaccelRate'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getScrollDeaccelRate();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScrollDeaccelRate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setZoomScaleInDuration of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setZoomScaleInDuration00
static int tolua_Gamelua_CCScrollView_setZoomScaleInDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  float s = ((float)  tolua_tonumber(tolua_S,2,0));
  float dt = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setZoomScaleInDuration'", NULL);
#endif
  {
   self->setZoomScaleInDuration(s,dt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setZoomScaleInDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: minContainerOffset of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_minContainerOffset00
static int tolua_Gamelua_CCScrollView_minContainerOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'minContainerOffset'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->minContainerOffset();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'minContainerOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: maxContainerOffset of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_maxContainerOffset00
static int tolua_Gamelua_CCScrollView_maxContainerOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'maxContainerOffset'", NULL);
#endif
  {
   CCPoint tolua_ret = (CCPoint)  self->maxContainerOffset();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'maxContainerOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isNodeVisible of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_isNodeVisible00
static int tolua_Gamelua_CCScrollView_isNodeVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCNode* node = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNodeVisible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNodeVisible(node);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNodeVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pause of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_pause00
static int tolua_Gamelua_CCScrollView_pause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCObject* sender = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pause'", NULL);
#endif
  {
   self->pause(sender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pause'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resume of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_resume00
static int tolua_Gamelua_CCScrollView_resume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCObject* sender = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resume'", NULL);
#endif
  {
   self->resume(sender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resume'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isDragging of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_isDragging00
static int tolua_Gamelua_CCScrollView_isDragging00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDragging'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isDragging();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDragging'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isTouchMoved of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_isTouchMoved00
static int tolua_Gamelua_CCScrollView_isTouchMoved00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTouchMoved'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isTouchMoved();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isTouchMoved'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isBounceable of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_isBounceable00
static int tolua_Gamelua_CCScrollView_isBounceable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isBounceable'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isBounceable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isBounceable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBounceable of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setBounceable00
static int tolua_Gamelua_CCScrollView_setBounceable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  bool bBounceable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBounceable'", NULL);
#endif
  {
   self->setBounceable(bBounceable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBounceable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewSize of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getViewSize00
static int tolua_Gamelua_CCScrollView_getViewSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewSize'", NULL);
#endif
  {
   CCSize tolua_ret = (CCSize)  self->getViewSize();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCSize)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCSize));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getViewSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setViewSize of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setViewSize00
static int tolua_Gamelua_CCScrollView_setViewSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCSize",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCSize size = *((CCSize*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setViewSize'", NULL);
#endif
  {
   self->setViewSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setViewSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContainer of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getContainer00
static int tolua_Gamelua_CCScrollView_getContainer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContainer'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getContainer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContainer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setContainer of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setContainer00
static int tolua_Gamelua_CCScrollView_setContainer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCNode* pContainer = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setContainer'", NULL);
#endif
  {
   self->setContainer(pContainer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setContainer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDirection of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_getDirection00
static int tolua_Gamelua_CCScrollView_getDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDirection'", NULL);
#endif
  {
   CCScrollViewDirection tolua_ret = (CCScrollViewDirection)  self->getDirection();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDirection of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setDirection00
static int tolua_Gamelua_CCScrollView_setDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCScrollViewDirection eDirection = ((CCScrollViewDirection) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDirection'", NULL);
#endif
  {
   self->setDirection(eDirection);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isClippingToBounds of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_isClippingToBounds00
static int tolua_Gamelua_CCScrollView_isClippingToBounds00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isClippingToBounds'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isClippingToBounds();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isClippingToBounds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setClippingToBounds of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setClippingToBounds00
static int tolua_Gamelua_CCScrollView_setClippingToBounds00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  bool bClippingToBounds = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClippingToBounds'", NULL);
#endif
  {
   self->setClippingToBounds(bClippingToBounds);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setClippingToBounds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: visit of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_visit00
static int tolua_Gamelua_CCScrollView_visit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'visit'", NULL);
#endif
  {
   self->visit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'visit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addChild of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_addChild00
static int tolua_Gamelua_CCScrollView_addChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCNode* child = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  int zOrder = ((int)  tolua_tonumber(tolua_S,3,0));
  int tag = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChild'", NULL);
#endif
  {
   self->addChild(child,zOrder,tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addChild of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_addChild01
static int tolua_Gamelua_CCScrollView_addChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCNode* child = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  int zOrder = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChild'", NULL);
#endif
  {
   self->addChild(child,zOrder);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_CCScrollView_addChild00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addChild of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_addChild02
static int tolua_Gamelua_CCScrollView_addChild02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  CCNode* child = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChild'", NULL);
#endif
  {
   self->addChild(child);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_CCScrollView_addChild01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTouchEnabled of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_setTouchEnabled00
static int tolua_Gamelua_CCScrollView_setTouchEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
  bool e = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchEnabled'", NULL);
#endif
  {
   self->setTouchEnabled(e);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTouchEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: forceRecaculateChildren of class  CCScrollView */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCScrollView_forceRecaculateChildren00
static int tolua_Gamelua_CCScrollView_forceRecaculateChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* self = (CCScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'forceRecaculateChildren'", NULL);
#endif
  {
   self->forceRecaculateChildren();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'forceRecaculateChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLineCount of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_getLineCount00
static int tolua_Gamelua_TableAutoReader_getLineCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLineCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getLineCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLineCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: seekLine of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_seekLine00
static int tolua_Gamelua_TableAutoReader_seekLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int line = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'seekLine'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->seekLine(line);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'seekLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: seekIndex of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_seekIndex00
static int tolua_Gamelua_TableAutoReader_seekIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'seekIndex'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->seekIndex(index);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'seekIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasRow of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_hasRow00
static int tolua_Gamelua_TableAutoReader_hasRow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int row = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasRow'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasRow(row);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasRow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDataInRow of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_getDataInRow00
static int tolua_Gamelua_TableAutoReader_getDataInRow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int row = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDataInRow'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getDataInRow(row);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDataInRow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getData of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_getData00
static int tolua_Gamelua_TableAutoReader_getData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int line = ((int)  tolua_tonumber(tolua_S,2,0));
  int row = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getData'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getData(line,row);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDataIndex of class  TableAutoReader */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableAutoReader_getDataIndex00
static int tolua_Gamelua_TableAutoReader_getDataIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableAutoReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableAutoReader* self = (TableAutoReader*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int row = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDataIndex'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getDataIndex(index,row);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDataIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  TableReaderManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableReaderManager_getInstance00
static int tolua_Gamelua_TableReaderManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TableReaderManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TableReaderManager* tolua_ret = (TableReaderManager*)  TableReaderManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TableReaderManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTableReader of class  TableReaderManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableReaderManager_getTableReader00
static int tolua_Gamelua_TableReaderManager_getTableReader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableReaderManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableReaderManager* self = (TableReaderManager*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTableReader'", NULL);
#endif
  {
   TableAutoReader* tolua_ret = (TableAutoReader*)  self->getTableReader(filename);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TableAutoReader");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTableReader'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reloadAllReader of class  TableReaderManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TableReaderManager_reloadAllReader00
static int tolua_Gamelua_TableReaderManager_reloadAllReader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TableReaderManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TableReaderManager* self = (TableReaderManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reloadAllReader'", NULL);
#endif
  {
   self->reloadAllReader();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reloadAllReader'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  TimeCalculator */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TimeCalculator_getInstance00
static int tolua_Gamelua_TimeCalculator_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TimeCalculator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TimeCalculator* tolua_ret = (TimeCalculator*)  TimeCalculator::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TimeCalculator");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createTimeCalcultor of class  TimeCalculator */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TimeCalculator_createTimeCalcultor00
static int tolua_Gamelua_TimeCalculator_createTimeCalcultor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeCalculator",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeCalculator* self = (TimeCalculator*)  tolua_tousertype(tolua_S,1,0);
  std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  long timeleft = ((long)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTimeCalcultor'", NULL);
#endif
  {
   self->createTimeCalcultor(key,timeleft);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createTimeCalcultor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeTimeCalcultor of class  TimeCalculator */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TimeCalculator_removeTimeCalcultor00
static int tolua_Gamelua_TimeCalculator_removeTimeCalcultor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeCalculator",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeCalculator* self = (TimeCalculator*)  tolua_tousertype(tolua_S,1,0);
  std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeTimeCalcultor'", NULL);
#endif
  {
   self->removeTimeCalcultor(key);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeTimeCalcultor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTimeLeft of class  TimeCalculator */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TimeCalculator_getTimeLeft00
static int tolua_Gamelua_TimeCalculator_getTimeLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeCalculator",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeCalculator* self = (TimeCalculator*)  tolua_tousertype(tolua_S,1,0);
  std::string tolua_var_1 = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTimeLeft'", NULL);
#endif
  {
   long tolua_ret = (long)  self->getTimeLeft(tolua_var_1);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimeLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasKey of class  TimeCalculator */
#ifndef TOLUA_DISABLE_tolua_Gamelua_TimeCalculator_hasKey00
static int tolua_Gamelua_TimeCalculator_hasKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TimeCalculator",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TimeCalculator* self = (TimeCalculator*)  tolua_tousertype(tolua_S,1,0);
  std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasKey'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasKey(key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  AES */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AES_new00
static int tolua_Gamelua_AES_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"AES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AES* tolua_ret = (AES*)  Mtolua_new((AES)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"AES");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  AES */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AES_new00_local
static int tolua_Gamelua_AES_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"AES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AES* tolua_ret = (AES*)  Mtolua_new((AES)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"AES");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  AES */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AES_delete00
static int tolua_Gamelua_AES_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AES* self = (AES*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Decrypt of class  AES */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AES_Decrypt00
static int tolua_Gamelua_AES_Decrypt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AES",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AES* self = (AES*)  tolua_tousertype(tolua_S,1,0);
  unsigned const char* inStr = ((unsigned const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int count = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned char* outStr = ((unsigned char*)  tolua_tostring(tolua_S,4,0));
  unsigned const char* key = ((unsigned const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Decrypt'", NULL);
#endif
  {
   self->Decrypt(inStr,count,outStr,key);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Decrypt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Encrypt of class  AES */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AES_Encrypt00
static int tolua_Gamelua_AES_Encrypt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AES",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AES* self = (AES*)  tolua_tousertype(tolua_S,1,0);
  unsigned const char* inStr = ((unsigned const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int count = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned char* outStr = ((unsigned char*)  tolua_tostring(tolua_S,4,0));
  unsigned const char* key = ((unsigned const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Encrypt'", NULL);
#endif
  {
   self->Encrypt(inStr,count,outStr,key);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Encrypt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stringAutoReturn of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_stringAutoReturn00
static int tolua_Gamelua_GameMaths_stringAutoReturn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string outString = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int lines = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   GameMaths::stringAutoReturn(inString,outString,width,lines);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushcppstring(tolua_S,(const char*)outString);
   tolua_pushnumber(tolua_S,(lua_Number)lines);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stringAutoReturn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stringAutoReturnForLua of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_stringAutoReturnForLua00
static int tolua_Gamelua_GameMaths_stringAutoReturnForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int width = ((int)  tolua_tonumber(tolua_S,3,0));
  int lines = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::stringAutoReturnForLua(inString,width,lines);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushnumber(tolua_S,(lua_Number)lines);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stringAutoReturnForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateStringCharacters of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_calculateStringCharacters00
static int tolua_Gamelua_GameMaths_calculateStringCharacters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   int tolua_ret = (int)  GameMaths::calculateStringCharacters(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'calculateStringCharacters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStringSubCharacters of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_getStringSubCharacters00
static int tolua_Gamelua_GameMaths_getStringSubCharacters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int offset = ((int)  tolua_tonumber(tolua_S,3,0));
  int lenght = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::getStringSubCharacters(str,offset,lenght);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStringSubCharacters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replaceStringWithBlank of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_replaceStringWithBlank00
static int tolua_Gamelua_GameMaths_replaceStringWithBlank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string outString = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   GameMaths::replaceStringWithBlank(inString,outString);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushcppstring(tolua_S,(const char*)outString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceStringWithBlank'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readStringToMap of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_readStringToMap00
static int tolua_Gamelua_GameMaths_readStringToMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::map<int,std::string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  std::map<int,std::string>* strMap = ((std::map<int,std::string>*)  tolua_tousertype(tolua_S,3,0));
  {
   GameMaths::readStringToMap(inString,*strMap);
   tolua_pushcppstring(tolua_S,(const char*)inString);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readStringToMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replaceStringWithCharacter of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_replaceStringWithCharacter00
static int tolua_Gamelua_GameMaths_replaceStringWithCharacter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  char partten = ((char)  tolua_tonumber(tolua_S,3,0));
  char dest = ((char)  tolua_tonumber(tolua_S,4,0));
  std::string outCharacter = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  {
   GameMaths::replaceStringWithCharacter(inString,partten,dest,outCharacter);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushcppstring(tolua_S,(const char*)outCharacter);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceStringWithCharacter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: formatSecondsToTime of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_formatSecondsToTime00
static int tolua_Gamelua_GameMaths_formatSecondsToTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int seconds = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::formatSecondsToTime(seconds);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'formatSecondsToTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: formatTimeToDate of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_formatTimeToDate00
static int tolua_Gamelua_GameMaths_formatTimeToDate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int seconds = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::formatTimeToDate(seconds);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'formatTimeToDate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isStringHasUTF8mb4 of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_isStringHasUTF8mb400
static int tolua_Gamelua_GameMaths_isStringHasUTF8mb400(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string instring = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  GameMaths::isStringHasUTF8mb4(instring);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)instring);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isStringHasUTF8mb4'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasSubString of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_hasSubString00
static int tolua_Gamelua_GameMaths_hasSubString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* longString = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* subString = ((const char*)  tolua_tostring(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  GameMaths::hasSubString(longString,subString);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasSubString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReverseAuto of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_ReverseAuto00
static int tolua_Gamelua_GameMaths_ReverseAuto00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  GameMaths::ReverseAuto(value);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReverseAuto'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replaceStringOneForLua of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_replaceStringOneForLua00
static int tolua_Gamelua_GameMaths_replaceStringOneForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string param = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::replaceStringOneForLua(inString,param);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushcppstring(tolua_S,(const char*)param);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceStringOneForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replaceStringTwoForLua of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_replaceStringTwoForLua00
static int tolua_Gamelua_GameMaths_replaceStringTwoForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string param1 = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string param2 = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::replaceStringTwoForLua(inString,param1,param2);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)inString);
   tolua_pushcppstring(tolua_S,(const char*)param1);
   tolua_pushcppstring(tolua_S,(const char*)param2);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceStringTwoForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replaceStringWithCharacterAll of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_replaceStringWithCharacterAll00
static int tolua_Gamelua_GameMaths_replaceStringWithCharacterAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string old_value = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string new_value = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::replaceStringWithCharacterAll(str,old_value,new_value);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
   tolua_pushcppstring(tolua_S,(const char*)old_value);
   tolua_pushcppstring(tolua_S,(const char*)new_value);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceStringWithCharacterAll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stringCutWidthLen of class  GameMaths */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMaths_stringCutWidthLen00
static int tolua_Gamelua_GameMaths_stringCutWidthLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameMaths",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string inString = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   std::string tolua_ret = (std::string)  GameMaths::stringCutWidthLen(inString,len);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)inString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stringCutWidthLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_new00
static int tolua_Gamelua_ConfigFile_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ConfigFile* tolua_ret = (ConfigFile*)  Mtolua_new((ConfigFile)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ConfigFile");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_new00_local
static int tolua_Gamelua_ConfigFile_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ConfigFile* tolua_ret = (ConfigFile*)  Mtolua_new((ConfigFile)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ConfigFile");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_delete00
static int tolua_Gamelua_ConfigFile_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigFile* self = (ConfigFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_load00
static int tolua_Gamelua_ConfigFile_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"std::stringstream",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigFile* self = (ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  std::stringstream* stream = ((std::stringstream*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load(*stream);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_load01
static int tolua_Gamelua_ConfigFile_load01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"std::stringstream",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  ConfigFile* self = (ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  std::stringstream* stream = ((std::stringstream*)  tolua_tousertype(tolua_S,2,0));
  const std::string separators = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  bool trimWhitespace = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load(*stream,separators,trimWhitespace);
   tolua_pushcppstring(tolua_S,(const char*)separators);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_ConfigFile_load00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_load02
static int tolua_Gamelua_ConfigFile_load02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  ConfigFile* self = (ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load(filename);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_ConfigFile_load01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_load03
static int tolua_Gamelua_ConfigFile_load03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  ConfigFile* self = (ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string separators = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  bool trimWhitespace = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load(filename,separators,trimWhitespace);
   tolua_pushcppstring(tolua_S,(const char*)filename);
   tolua_pushcppstring(tolua_S,(const char*)separators);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Gamelua_ConfigFile_load02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSetting of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_getSetting00
static int tolua_Gamelua_ConfigFile_getSetting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const ConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ConfigFile* self = (const ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSetting'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSetting(key);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSetting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSetting of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_getSetting01
static int tolua_Gamelua_ConfigFile_getSetting01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const ConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const ConfigFile* self = (const ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSetting'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSetting(key,section);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
   tolua_pushcppstring(tolua_S,(const char*)section);
  }
 }
 return 3;
tolua_lerror:
 return tolua_Gamelua_ConfigFile_getSetting00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSetting of class  ConfigFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ConfigFile_getSetting02
static int tolua_Gamelua_ConfigFile_getSetting02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const ConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const ConfigFile* self = (const ConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string defaultValue = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSetting'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSetting(key,section,defaultValue);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)defaultValue);
  }
 }
 return 4;
tolua_lerror:
 return tolua_Gamelua_ConfigFile_getSetting01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_getInstance00
static int tolua_Gamelua_MainFrame_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MainFrame* tolua_ret = (MainFrame*)  MainFrame::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MainFrame");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showPage of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_showPage00
static int tolua_Gamelua_MainFrame_showPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showPage'", NULL);
#endif
  {
   self->showPage(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pushPage of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_pushPage00
static int tolua_Gamelua_MainFrame_pushPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushPage'", NULL);
#endif
  {
   self->pushPage(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popPage of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_popPage00
static int tolua_Gamelua_MainFrame_popPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popPage'", NULL);
#endif
  {
   self->popPage(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popAllPage of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_popAllPage00
static int tolua_Gamelua_MainFrame_popAllPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popAllPage'", NULL);
#endif
  {
   self->popAllPage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popAllPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showFightPage of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_showFightPage00
static int tolua_Gamelua_MainFrame_showFightPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showFightPage'", NULL);
#endif
  {
   self->showFightPage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showFightPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPageInList of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_isPageInList00
static int tolua_Gamelua_MainFrame_isPageInList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPageInList'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPageInList(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPageInList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showNoTouch of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_showNoTouch00
static int tolua_Gamelua_MainFrame_showNoTouch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showNoTouch'", NULL);
#endif
  {
   self->showNoTouch();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showNoTouch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hideNoTouch of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_hideNoTouch00
static int tolua_Gamelua_MainFrame_hideNoTouch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hideNoTouch'", NULL);
#endif
  {
   self->hideNoTouch();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hideNoTouch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurShowPageName of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_getCurShowPageName00
static int tolua_Gamelua_MainFrame_getCurShowPageName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurShowPageName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getCurShowPageName();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurShowPageName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMsgNodeForLua of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_getMsgNodeForLua00
static int tolua_Gamelua_MainFrame_getMsgNodeForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMsgNodeForLua'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getMsgNodeForLua();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMsgNodeForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setChildVisible of class  MainFrame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MainFrame_setChildVisible00
static int tolua_Gamelua_MainFrame_setChildVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MainFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MainFrame* self = (MainFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* childName = ((const char*)  tolua_tostring(tolua_S,2,0));
  bool visible = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setChildVisible'", NULL);
#endif
  {
   self->setChildVisible(childName,visible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setChildVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_init00
static int tolua_Gamelua_Language_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
  const std::string languagefile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   self->init(languagefile);
   tolua_pushcppstring(tolua_S,(const char*)languagefile);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addLanguageFile of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_addLanguageFile00
static int tolua_Gamelua_Language_addLanguageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
  const std::string languagefile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addLanguageFile'", NULL);
#endif
  {
   self->addLanguageFile(languagefile);
   tolua_pushcppstring(tolua_S,(const char*)languagefile);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addLanguageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasString of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_hasString00
static int tolua_Gamelua_Language_hasString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
  const std::string _key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasString'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasString(_key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)_key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_getString00
static int tolua_Gamelua_Language_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
  const std::string _key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getString(_key);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)_key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateNode of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_updateNode00
static int tolua_Gamelua_Language_updateNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cocos2d::CCNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
  cocos2d::CCNode* _root = ((cocos2d::CCNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateNode'", NULL);
#endif
  {
   self->updateNode(_root);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_clear00
static int tolua_Gamelua_Language_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Language* self = (Language*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'", NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  Language */
#ifndef TOLUA_DISABLE_tolua_Gamelua_Language_getInstance00
static int tolua_Gamelua_Language_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Language",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Language* tolua_ret = (Language*)  Language::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Language");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMusicOn of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_setMusicOn00
static int tolua_Gamelua_SoundManager_setMusicOn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  bool tolua_var_2 = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMusicOn'", NULL);
#endif
  {
   self->setMusicOn(tolua_var_2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMusicOn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEffectOn of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_setEffectOn00
static int tolua_Gamelua_SoundManager_setEffectOn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  bool tolua_var_3 = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEffectOn'", NULL);
#endif
  {
   self->setEffectOn(tolua_var_3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEffectOn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playLoadingMusic of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_playLoadingMusic00
static int tolua_Gamelua_SoundManager_playLoadingMusic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playLoadingMusic'", NULL);
#endif
  {
   self->playLoadingMusic();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playLoadingMusic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playGeneralMusic of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_playGeneralMusic00
static int tolua_Gamelua_SoundManager_playGeneralMusic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playGeneralMusic'", NULL);
#endif
  {
   self->playGeneralMusic();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playGeneralMusic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playFightMusic of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_playFightMusic00
static int tolua_Gamelua_SoundManager_playFightMusic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  std::string music = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playFightMusic'", NULL);
#endif
  {
   self->playFightMusic(music);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playFightMusic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopMusic of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_stopMusic00
static int tolua_Gamelua_SoundManager_stopMusic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopMusic'", NULL);
#endif
  {
   self->stopMusic();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopMusic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playMusic of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_playMusic00
static int tolua_Gamelua_SoundManager_playMusic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  std::string music = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playMusic'", NULL);
#endif
  {
   self->playMusic(music);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playMusic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playEffect of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_playEffect00
static int tolua_Gamelua_SoundManager_playEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  std::string effect = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playEffect'", NULL);
#endif
  {
   int tolua_ret = (int)  self->playEffect(effect);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopAllEffect of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_stopAllEffect00
static int tolua_Gamelua_SoundManager_stopAllEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopAllEffect'", NULL);
#endif
  {
   self->stopAllEffect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopAllEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SoundManager_getInstance00
static int tolua_Gamelua_SoundManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   SoundManager* tolua_ret = (SoundManager*)  SoundManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SoundManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_hasVarible00
static int tolua_Gamelua_BlackBoard_hasVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasVarible(key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_addVarible00
static int tolua_Gamelua_BlackBoard_addVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
   tolua_pushcppstring(tolua_S,(const char*)value);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_addVarible01
static int tolua_Gamelua_BlackBoard_addVarible01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool value = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Gamelua_BlackBoard_addVarible00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_addVarible02
static int tolua_Gamelua_BlackBoard_addVarible02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int value = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Gamelua_BlackBoard_addVarible01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_delVarible00
static int tolua_Gamelua_BlackBoard_delVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delVarible'", NULL);
#endif
  {
   self->delVarible(key);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_setVarible00
static int tolua_Gamelua_BlackBoard_setVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
   tolua_pushcppstring(tolua_S,(const char*)value);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_setVarible01
static int tolua_Gamelua_BlackBoard_setVarible01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool value = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Gamelua_BlackBoard_setVarible00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_setVarible02
static int tolua_Gamelua_BlackBoard_setVarible02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int value = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVarible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setVarible(key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Gamelua_BlackBoard_setVarible01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_getVarible00
static int tolua_Gamelua_BlackBoard_getVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarible'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getVarible(key);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVaribleAsUint of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_getVaribleAsUint00
static int tolua_Gamelua_BlackBoard_getVaribleAsUint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVaribleAsUint'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getVaribleAsUint(key);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVaribleAsUint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVaribleAsBool of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_getVaribleAsBool00
static int tolua_Gamelua_BlackBoard_getVaribleAsBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVaribleAsBool'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getVaribleAsBool(key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVaribleAsBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fetchVarible of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_fetchVarible00
static int tolua_Gamelua_BlackBoard_fetchVarible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fetchVarible'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->fetchVarible(key);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fetchVarible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fetchVaribleAsUint of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_fetchVaribleAsUint00
static int tolua_Gamelua_BlackBoard_fetchVaribleAsUint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fetchVaribleAsUint'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->fetchVaribleAsUint(key);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fetchVaribleAsUint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fetchVaribleAsBool of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_fetchVaribleAsBool00
static int tolua_Gamelua_BlackBoard_fetchVaribleAsBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fetchVaribleAsBool'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->fetchVaribleAsBool(key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fetchVaribleAsBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: PLATFORM_TYPE_FOR_LUA of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_get_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA
static int tolua_get_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA(lua_State* tolua_S)
{
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PLATFORM_TYPE_FOR_LUA'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->PLATFORM_TYPE_FOR_LUA);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: PLATFORM_TYPE_FOR_LUA of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_set_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA
static int tolua_set_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA(lua_State* tolua_S)
{
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PLATFORM_TYPE_FOR_LUA'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->PLATFORM_TYPE_FOR_LUA = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearMap of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_clearMap00
static int tolua_Gamelua_BlackBoard_clearMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BlackBoard* self = (BlackBoard*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearMap'", NULL);
#endif
  {
   self->clearMap();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  BlackBoard */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BlackBoard_getInstance00
static int tolua_Gamelua_BlackBoard_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BlackBoard",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BlackBoard* tolua_ret = (BlackBoard*)  BlackBoard::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BlackBoard");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Msg_Box of class  MessageBoxPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageBoxPage_Msg_Box00
static int tolua_Gamelua_MessageBoxPage_Msg_Box00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageBoxPage",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string msgString = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   MessageBoxPage::Msg_Box(msgString);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Msg_Box'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Msg_Box_Lan of class  MessageBoxPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageBoxPage_Msg_Box_Lan00
static int tolua_Gamelua_MessageBoxPage_Msg_Box_Lan00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageBoxPage",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string msgString = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   MessageBoxPage::Msg_Box_Lan(msgString);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Msg_Box_Lan'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_iPlayerID of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_get_LoginInfo_m_iPlayerID
static int tolua_get_LoginInfo_m_iPlayerID(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iPlayerID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_iPlayerID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_iPlayerID of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_set_LoginInfo_m_iPlayerID
static int tolua_set_LoginInfo_m_iPlayerID(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iPlayerID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_iPlayerID = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_iRoleItemID of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_get_LoginInfo_m_iRoleItemID
static int tolua_get_LoginInfo_m_iRoleItemID(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iRoleItemID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_iRoleItemID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_iRoleItemID of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_set_LoginInfo_m_iRoleItemID
static int tolua_set_LoginInfo_m_iRoleItemID(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iRoleItemID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_iRoleItemID = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_iTimeStamp of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_get_LoginInfo_m_iTimeStamp
static int tolua_get_LoginInfo_m_iTimeStamp(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iTimeStamp'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_iTimeStamp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_iTimeStamp of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_set_LoginInfo_m_iTimeStamp
static int tolua_set_LoginInfo_m_iTimeStamp(lua_State* tolua_S)
{
  LoginInfo* self = (LoginInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iTimeStamp'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_iTimeStamp = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_LoginInfo_new00
static int tolua_Gamelua_LoginInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LoginInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LoginInfo* tolua_ret = (LoginInfo*)  Mtolua_new((LoginInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LoginInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  LoginInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_LoginInfo_new00_local
static int tolua_Gamelua_LoginInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LoginInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LoginInfo* tolua_ret = (LoginInfo*)  Mtolua_new((LoginInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LoginInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mCurrAdventureVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_get_GameCheckInfo_unsigned_mCurrAdventureVersion
static int tolua_get_GameCheckInfo_unsigned_mCurrAdventureVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mCurrAdventureVersion'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mCurrAdventureVersion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mCurrAdventureVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_set_GameCheckInfo_unsigned_mCurrAdventureVersion
static int tolua_set_GameCheckInfo_unsigned_mCurrAdventureVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mCurrAdventureVersion'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mCurrAdventureVersion = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mHeartRecruitDiscipleVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_get_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion
static int tolua_get_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mHeartRecruitDiscipleVersion'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mHeartRecruitDiscipleVersion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mHeartRecruitDiscipleVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_set_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion
static int tolua_set_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mHeartRecruitDiscipleVersion'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mHeartRecruitDiscipleVersion = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mHeartAdventureVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_get_GameCheckInfo_unsigned_mHeartAdventureVersion
static int tolua_get_GameCheckInfo_unsigned_mHeartAdventureVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mHeartAdventureVersion'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mHeartAdventureVersion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mHeartAdventureVersion of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_set_GameCheckInfo_unsigned_mHeartAdventureVersion
static int tolua_set_GameCheckInfo_unsigned_mHeartAdventureVersion(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mHeartAdventureVersion'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mHeartAdventureVersion = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mNeedForceSyncAdventure of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_get_GameCheckInfo_mNeedForceSyncAdventure
static int tolua_get_GameCheckInfo_mNeedForceSyncAdventure(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mNeedForceSyncAdventure'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->mNeedForceSyncAdventure);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mNeedForceSyncAdventure of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_set_GameCheckInfo_mNeedForceSyncAdventure
static int tolua_set_GameCheckInfo_mNeedForceSyncAdventure(lua_State* tolua_S)
{
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mNeedForceSyncAdventure'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mNeedForceSyncAdventure = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: isNeedSyncAdventureList of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameCheckInfo_isNeedSyncAdventureList00
static int tolua_Gamelua_GameCheckInfo_isNeedSyncAdventureList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCheckInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNeedSyncAdventureList'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNeedSyncAdventureList();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNeedSyncAdventureList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setNeedForceSyncAdventure of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameCheckInfo_setNeedForceSyncAdventure00
static int tolua_Gamelua_GameCheckInfo_setNeedForceSyncAdventure00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCheckInfo",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCheckInfo* self = (GameCheckInfo*)  tolua_tousertype(tolua_S,1,0);
  bool need = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNeedForceSyncAdventure'", NULL);
#endif
  {
   self->setNeedForceSyncAdventure(need);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNeedForceSyncAdventure'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameCheckInfo_new00
static int tolua_Gamelua_GameCheckInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameCheckInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameCheckInfo* tolua_ret = (GameCheckInfo*)  Mtolua_new((GameCheckInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameCheckInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GameCheckInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameCheckInfo_new00_local
static int tolua_Gamelua_GameCheckInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameCheckInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GameCheckInfo* tolua_ret = (GameCheckInfo*)  Mtolua_new((GameCheckInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameCheckInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_new00
static int tolua_Gamelua_ServerDateManager_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ServerDateManager* tolua_ret = (ServerDateManager*)  Mtolua_new((ServerDateManager)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ServerDateManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_new00_local
static int tolua_Gamelua_ServerDateManager_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ServerDateManager* tolua_ret = (ServerDateManager*)  Mtolua_new((ServerDateManager)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ServerDateManager");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getInstance00
static int tolua_Gamelua_ServerDateManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ServerDateManager* tolua_ret = (ServerDateManager*)  ServerDateManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ServerDateManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUserBasicInfoForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getUserBasicInfoForLua00
static int tolua_Gamelua_ServerDateManager_getUserBasicInfoForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUserBasicInfoForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getUserBasicInfoForLua();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUserBasicInfoForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUserRoleInfoForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getUserRoleInfoForLua00
static int tolua_Gamelua_ServerDateManager_getUserRoleInfoForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUserRoleInfoForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getUserRoleInfoForLua();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUserRoleInfoForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleInfoTotalSize of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getRoleInfoTotalSize00
static int tolua_Gamelua_ServerDateManager_getRoleInfoTotalSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleInfoTotalSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getRoleInfoTotalSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleInfoTotalSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleInfoByIdForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getRoleInfoByIdForLua00
static int tolua_Gamelua_ServerDateManager_getRoleInfoByIdForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleInfoByIdForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getRoleInfoByIdForLua(id);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleInfoByIdForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleInfoByIndexForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getRoleInfoByIndexForLua00
static int tolua_Gamelua_ServerDateManager_getRoleInfoByIndexForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleInfoByIndexForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getRoleInfoByIndexForLua(index);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleInfoByIndexForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEquipInfoTotalSize of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getEquipInfoTotalSize00
static int tolua_Gamelua_ServerDateManager_getEquipInfoTotalSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEquipInfoTotalSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getEquipInfoTotalSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEquipInfoTotalSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEquipInfoByIdForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getEquipInfoByIdForLua00
static int tolua_Gamelua_ServerDateManager_getEquipInfoByIdForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEquipInfoByIdForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getEquipInfoByIdForLua(id);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEquipInfoByIdForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEquipInfoByIndexForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getEquipInfoByIndexForLua00
static int tolua_Gamelua_ServerDateManager_getEquipInfoByIndexForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEquipInfoByIndexForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getEquipInfoByIndexForLua(index);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEquipInfoByIndexForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemInfoTotalSize of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getItemInfoTotalSize00
static int tolua_Gamelua_ServerDateManager_getItemInfoTotalSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemInfoTotalSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getItemInfoTotalSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemInfoTotalSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemInfoByIdForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getItemInfoByIdForLua00
static int tolua_Gamelua_ServerDateManager_getItemInfoByIdForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemInfoByIdForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getItemInfoByIdForLua(id);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemInfoByIdForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemInfoByIndexForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getItemInfoByIndexForLua00
static int tolua_Gamelua_ServerDateManager_getItemInfoByIndexForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemInfoByIndexForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getItemInfoByIndexForLua(index);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemInfoByIndexForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSkillInfoTotalSize of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getSkillInfoTotalSize00
static int tolua_Gamelua_ServerDateManager_getSkillInfoTotalSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkillInfoTotalSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getSkillInfoTotalSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkillInfoTotalSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSkillInfoByIdForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getSkillInfoByIdForLua00
static int tolua_Gamelua_ServerDateManager_getSkillInfoByIdForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkillInfoByIdForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSkillInfoByIdForLua(id);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkillInfoByIdForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSkillInfoByIndexForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getSkillInfoByIndexForLua00
static int tolua_Gamelua_ServerDateManager_getSkillInfoByIndexForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkillInfoByIndexForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSkillInfoByIndexForLua(index);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkillInfoByIndexForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlayerStateForLua of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ServerDateManager_getPlayerStateForLua00
static int tolua_Gamelua_ServerDateManager_getPlayerStateForLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ServerDateManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlayerStateForLua'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getPlayerStateForLua();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlayerStateForLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mLoginInfo of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_get_ServerDateManager_mLoginInfo
static int tolua_get_ServerDateManager_mLoginInfo(lua_State* tolua_S)
{
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mLoginInfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->mLoginInfo,"LoginInfo");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mLoginInfo of class  ServerDateManager */
#ifndef TOLUA_DISABLE_tolua_set_ServerDateManager_mLoginInfo
static int tolua_set_ServerDateManager_mLoginInfo(lua_State* tolua_S)
{
  ServerDateManager* self = (ServerDateManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mLoginInfo'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"LoginInfo",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mLoginInfo = *((LoginInfo*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* function: getFrameColor */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getFrameColor00
static int tolua_Gamelua_getFrameColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   ccColor3B tolua_ret = (ccColor3B)  getFrameColor(quality);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((ccColor3B)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ccColor3B));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getFrameNormalSpirte */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getFrameNormalSpirte00
static int tolua_Gamelua_getFrameNormalSpirte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  std::string ext = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   CCSprite* tolua_ret = (CCSprite*)  getFrameNormalSpirte(quality,ext);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameNormalSpirte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getFrameSelectedSpirte */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getFrameSelectedSpirte00
static int tolua_Gamelua_getFrameSelectedSpirte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  std::string ext = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   CCSprite* tolua_ret = (CCSprite*)  getFrameSelectedSpirte(quality,ext);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameSelectedSpirte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getFrameDisabledSpirte */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getFrameDisabledSpirte00
static int tolua_Gamelua_getFrameDisabledSpirte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  std::string ext = ((std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   CCSprite* tolua_ret = (CCSprite*)  getFrameDisabledSpirte(quality,ext);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameDisabledSpirte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getContentUnselectedSpirte */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getContentUnselectedSpirte00
static int tolua_Gamelua_getContentUnselectedSpirte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string ext = ((std::string)  tolua_tocppstring(tolua_S,1,""));
  {
   CCSprite* tolua_ret = (CCSprite*)  getContentUnselectedSpirte(ext);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContentUnselectedSpirte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getContentSelectedSpirte */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getContentSelectedSpirte00
static int tolua_Gamelua_getContentSelectedSpirte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string ext = ((std::string)  tolua_tocppstring(tolua_S,1,""));
  {
   CCSprite* tolua_ret = (CCSprite*)  getContentSelectedSpirte(ext);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContentSelectedSpirte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getQualityMaskImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getQualityMaskImageFile00
static int tolua_Gamelua_getQualityMaskImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  getQualityMaskImageFile(quality);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQualityMaskImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getQualityImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getQualityImageFile00
static int tolua_Gamelua_getQualityImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int quality = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  getQualityImageFile(quality);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQualityImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getQualityFramePersonImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getQualityFramePersonImageFile00
static int tolua_Gamelua_getQualityFramePersonImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::string tolua_ret = (std::string)  getQualityFramePersonImageFile();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQualityFramePersonImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getQualityFrameNormalImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getQualityFrameNormalImageFile00
static int tolua_Gamelua_getQualityFrameNormalImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::string tolua_ret = (std::string)  getQualityFrameNormalImageFile();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQualityFrameNormalImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getQualityFrameSelectImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getQualityFrameSelectImageFile00
static int tolua_Gamelua_getQualityFrameSelectImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::string tolua_ret = (std::string)  getQualityFrameSelectImageFile();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQualityFrameSelectImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getVipLevelImageFile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getVipLevelImageFile00
static int tolua_Gamelua_getVipLevelImageFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int level = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   std::string tolua_ret = (std::string)  getVipLevelImageFile(level);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVipLevelImageFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getTeamDefaultPic */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getTeamDefaultPic00
static int tolua_Gamelua_getTeamDefaultPic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::string tolua_ret = (std::string)  getTeamDefaultPic();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTeamDefaultPic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getTeamLockPic */
#ifndef TOLUA_DISABLE_tolua_Gamelua_getTeamLockPic00
static int tolua_Gamelua_getTeamLockPic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::string tolua_ret = (std::string)  getTeamLockPic();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTeamLockPic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  VaribleManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_VaribleManager_getInstance00
static int tolua_Gamelua_VaribleManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"VaribleManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   VaribleManager* tolua_ret = (VaribleManager*)  VaribleManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"VaribleManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: id of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_id
static int tolua_get_PlatformRoleItem_unsigned_id(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_id
static int tolua_set_PlatformRoleItem_unsigned_id(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_name
static int tolua_get_PlatformRoleItem_name(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_name
static int tolua_set_PlatformRoleItem_name(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attentionStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_attentionStatus
static int tolua_get_PlatformRoleItem_unsigned_attentionStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attentionStatus'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attentionStatus);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attentionStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_attentionStatus
static int tolua_set_PlatformRoleItem_unsigned_attentionStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attentionStatus'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attentionStatus = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: loadingFrameSeversConnection of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_loadingFrameSeversConnection
static int tolua_get_PlatformRoleItem_loadingFrameSeversConnection(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loadingFrameSeversConnection'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->loadingFrameSeversConnection);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: loadingFrameSeversConnection of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_loadingFrameSeversConnection
static int tolua_set_PlatformRoleItem_loadingFrameSeversConnection(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loadingFrameSeversConnection'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->loadingFrameSeversConnection = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: rechargeMaxLimit of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_rechargeMaxLimit
static int tolua_get_PlatformRoleItem_rechargeMaxLimit(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rechargeMaxLimit'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->rechargeMaxLimit);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: rechargeMaxLimit of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_rechargeMaxLimit
static int tolua_set_PlatformRoleItem_rechargeMaxLimit(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rechargeMaxLimit'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->rechargeMaxLimit = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bbsOpenStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_bbsOpenStatus
static int tolua_get_PlatformRoleItem_unsigned_bbsOpenStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bbsOpenStatus'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bbsOpenStatus);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bbsOpenStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_bbsOpenStatus
static int tolua_set_PlatformRoleItem_unsigned_bbsOpenStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bbsOpenStatus'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bbsOpenStatus = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: showFeedBack of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_showFeedBack
static int tolua_get_PlatformRoleItem_showFeedBack(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'showFeedBack'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->showFeedBack);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: showFeedBack of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_showFeedBack
static int tolua_set_PlatformRoleItem_showFeedBack(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'showFeedBack'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->showFeedBack = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: webkitOpenStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_webkitOpenStatus
static int tolua_get_PlatformRoleItem_unsigned_webkitOpenStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'webkitOpenStatus'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->webkitOpenStatus);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: webkitOpenStatus of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_webkitOpenStatus
static int tolua_set_PlatformRoleItem_unsigned_webkitOpenStatus(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'webkitOpenStatus'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->webkitOpenStatus = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: aboutSetting of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_aboutSetting
static int tolua_get_PlatformRoleItem_aboutSetting(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'aboutSetting'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->aboutSetting);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: aboutSetting of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_aboutSetting
static int tolua_set_PlatformRoleItem_aboutSetting(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'aboutSetting'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->aboutSetting = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: excludedAboutIds of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_excludedAboutIds
static int tolua_get_PlatformRoleItem_excludedAboutIds(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'excludedAboutIds'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->excludedAboutIds);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: excludedAboutIds of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_excludedAboutIds
static int tolua_set_PlatformRoleItem_excludedAboutIds(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'excludedAboutIds'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->excludedAboutIds = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: excludedAnnoucementIds of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_excludedAnnoucementIds
static int tolua_get_PlatformRoleItem_excludedAnnoucementIds(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'excludedAnnoucementIds'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->excludedAnnoucementIds);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: excludedAnnoucementIds of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_excludedAnnoucementIds
static int tolua_set_PlatformRoleItem_excludedAnnoucementIds(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'excludedAnnoucementIds'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->excludedAnnoucementIds = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isUsePrivateBigVersionUpdate of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate
static int tolua_get_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isUsePrivateBigVersionUpdate'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->isUsePrivateBigVersionUpdate);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isUsePrivateBigVersionUpdate of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate
static int tolua_set_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isUsePrivateBigVersionUpdate'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isUsePrivateBigVersionUpdate = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isUseSDKBBS of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_isUseSDKBBS
static int tolua_get_PlatformRoleItem_unsigned_isUseSDKBBS(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isUseSDKBBS'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->isUseSDKBBS);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isUseSDKBBS of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_isUseSDKBBS
static int tolua_set_PlatformRoleItem_unsigned_isUseSDKBBS(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isUseSDKBBS'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isUseSDKBBS = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: openBBSUrl of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_openBBSUrl
static int tolua_get_PlatformRoleItem_openBBSUrl(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'openBBSUrl'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->openBBSUrl);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: openBBSUrl of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_openBBSUrl
static int tolua_set_PlatformRoleItem_openBBSUrl(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'openBBSUrl'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->openBBSUrl = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: loadinScenceMsg of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_loadinScenceMsg
static int tolua_get_PlatformRoleItem_loadinScenceMsg(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loadinScenceMsg'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->loadinScenceMsg);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: loadinScenceMsg of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_loadinScenceMsg
static int tolua_set_PlatformRoleItem_loadinScenceMsg(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loadinScenceMsg'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->loadinScenceMsg = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: exit2platlogout of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_exit2platlogout
static int tolua_get_PlatformRoleItem_unsigned_exit2platlogout(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'exit2platlogout'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->exit2platlogout);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: exit2platlogout of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_exit2platlogout
static int tolua_set_PlatformRoleItem_unsigned_exit2platlogout(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'exit2platlogout'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->exit2platlogout = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: shareSwitch of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_get_PlatformRoleItem_unsigned_shareSwitch
static int tolua_get_PlatformRoleItem_unsigned_shareSwitch(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shareSwitch'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->shareSwitch);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: shareSwitch of class  PlatformRoleItem */
#ifndef TOLUA_DISABLE_tolua_set_PlatformRoleItem_unsigned_shareSwitch
static int tolua_set_PlatformRoleItem_unsigned_shareSwitch(lua_State* tolua_S)
{
  PlatformRoleItem* self = (PlatformRoleItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shareSwitch'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->shareSwitch = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlatformRoleByName of class  PlatformRoleTableManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PlatformRoleTableManager_getPlatformRoleByName00
static int tolua_Gamelua_PlatformRoleTableManager_getPlatformRoleByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PlatformRoleTableManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PlatformRoleTableManager* self = (PlatformRoleTableManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlatformRoleByName'", NULL);
#endif
  {
   const PlatformRoleItem* tolua_ret = (const PlatformRoleItem*)  self->getPlatformRoleByName(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"const PlatformRoleItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlatformRoleByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  PlatformRoleTableManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PlatformRoleTableManager_getInstance00
static int tolua_Gamelua_PlatformRoleTableManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PlatformRoleTableManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PlatformRoleTableManager* tolua_ret = (PlatformRoleTableManager*)  PlatformRoleTableManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PlatformRoleTableManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerPage of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_registerPage00
static int tolua_Gamelua_CCBManager_registerPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"BasePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  BasePage* page = ((BasePage*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerPage'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->registerPage(name,page);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unRegisterPage of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_unRegisterPage00
static int tolua_Gamelua_CCBManager_unRegisterPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"BasePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  BasePage* page = ((BasePage*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unRegisterPage'", NULL);
#endif
  {
   self->unRegisterPage(name,page);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unRegisterPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPage of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_getPage00
static int tolua_Gamelua_CCBManager_getPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPage'", NULL);
#endif
  {
   BasePage* tolua_ret = (BasePage*)  self->getPage(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BasePage");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createAndLoad of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_createAndLoad00
static int tolua_Gamelua_CCBManager_createAndLoad00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string ccbfile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createAndLoad'", NULL);
#endif
  {
   CCBContainer* tolua_ret = (CCBContainer*)  self->createAndLoad(ccbfile);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBContainer");
   tolua_pushcppstring(tolua_S,(const char*)ccbfile);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createAndLoad'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createAndLoad2 of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_createAndLoad200
static int tolua_Gamelua_CCBManager_createAndLoad200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string ccbfile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createAndLoad2'", NULL);
#endif
  {
   ScriptContentBase* tolua_ret = (ScriptContentBase*)  self->createAndLoad2(ccbfile);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"ScriptContentBase");
   tolua_pushcppstring(tolua_S,(const char*)ccbfile);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createAndLoad2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadCCBFileNew of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_loadCCBFileNew00
static int tolua_Gamelua_CCBManager_loadCCBFileNew00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string ccbfile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CCBFileNew* tolua_ret = (CCBFileNew*)  CCBManager::loadCCBFileNew(ccbfile);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBFileNew");
   tolua_pushcppstring(tolua_S,(const char*)ccbfile);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadCCBFileNew'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: purgeCachedData of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_purgeCachedData00
static int tolua_Gamelua_CCBManager_purgeCachedData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBManager* self = (CCBManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'purgeCachedData'", NULL);
#endif
  {
   self->purgeCachedData();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'purgeCachedData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  CCBManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBManager_getInstance00
static int tolua_Gamelua_CCBManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCBManager* tolua_ret = (CCBManager*)  CCBManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: registerScriptPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_registerScriptPage00
static int tolua_Gamelua_registerScriptPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string ccbfile = ((const std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   registerScriptPage(ccbfile);
   tolua_pushcppstring(tolua_S,(const char*)ccbfile);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerScriptPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_create00
static int tolua_Gamelua_ScriptContentBase_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string fileName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int tag = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   ScriptContentBase* tolua_ret = (ScriptContentBase*)  ScriptContentBase::create(fileName,tag);
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"ScriptContentBase");
   tolua_pushcppstring(tolua_S,(const char*)fileName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarNode of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarNode00
static int tolua_Gamelua_ScriptContentBase_getVarNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarNode'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getVarNode(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarSprite of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarSprite00
static int tolua_Gamelua_ScriptContentBase_getVarSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarSprite'", NULL);
#endif
  {
   CCSprite* tolua_ret = (CCSprite*)  self->getVarSprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarLabelBMFont of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarLabelBMFont00
static int tolua_Gamelua_ScriptContentBase_getVarLabelBMFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarLabelBMFont'", NULL);
#endif
  {
   CCLabelBMFont* tolua_ret = (CCLabelBMFont*)  self->getVarLabelBMFont(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelBMFont");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarLabelBMFont'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarLabelTTF of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarLabelTTF00
static int tolua_Gamelua_ScriptContentBase_getVarLabelTTF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarLabelTTF'", NULL);
#endif
  {
   CCLabelTTF* tolua_ret = (CCLabelTTF*)  self->getVarLabelTTF(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCLabelTTF");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarLabelTTF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenu of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarMenu00
static int tolua_Gamelua_ScriptContentBase_getVarMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenu'", NULL);
#endif
  {
   CCMenu* tolua_ret = (CCMenu*)  self->getVarMenu(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenu");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItem of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarMenuItem00
static int tolua_Gamelua_ScriptContentBase_getVarMenuItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItem'", NULL);
#endif
  {
   CCMenuItem* tolua_ret = (CCMenuItem*)  self->getVarMenuItem(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemSprite of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarMenuItemSprite00
static int tolua_Gamelua_ScriptContentBase_getVarMenuItemSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemSprite'", NULL);
#endif
  {
   CCMenuItemSprite* tolua_ret = (CCMenuItemSprite*)  self->getVarMenuItemSprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemSprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemImage of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarMenuItemImage00
static int tolua_Gamelua_ScriptContentBase_getVarMenuItemImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemImage'", NULL);
#endif
  {
   CCMenuItemImage* tolua_ret = (CCMenuItemImage*)  self->getVarMenuItemImage(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemImage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarScrollView of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarScrollView00
static int tolua_Gamelua_ScriptContentBase_getVarScrollView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarScrollView'", NULL);
#endif
  {
   CCScrollView* tolua_ret = (CCScrollView*)  self->getVarScrollView(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScrollView");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarScrollView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarScale9Sprite of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarScale9Sprite00
static int tolua_Gamelua_ScriptContentBase_getVarScale9Sprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarScale9Sprite'", NULL);
#endif
  {
   CCScale9Sprite* tolua_ret = (CCScale9Sprite*)  self->getVarScale9Sprite(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCScale9Sprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarScale9Sprite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParticleSystem of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getParticleSystem00
static int tolua_Gamelua_ScriptContentBase_getParticleSystem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParticleSystem'", NULL);
#endif
  {
   CCParticleSystem* tolua_ret = (CCParticleSystem*)  self->getParticleSystem(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCParticleSystem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParticleSystem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVarMenuItemCCB of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getVarMenuItemCCB00
static int tolua_Gamelua_ScriptContentBase_getVarMenuItemCCB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  char* name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVarMenuItemCCB'", NULL);
#endif
  {
   CCMenuItemCCBFile* tolua_ret = (CCMenuItemCCBFile*)  self->getVarMenuItemCCB(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCMenuItemCCBFile");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVarMenuItemCCB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerFunctionHandler of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_registerFunctionHandler00
static int tolua_Gamelua_ScriptContentBase_registerFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerFunctionHandler'", NULL);
#endif
  {
   self->registerFunctionHandler(nHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterFunctionHandler of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_unregisterFunctionHandler00
static int tolua_Gamelua_ScriptContentBase_unregisterFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterFunctionHandler'", NULL);
#endif
  {
   self->unregisterFunctionHandler();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemDate of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_getItemDate00
static int tolua_Gamelua_ScriptContentBase_getItemDate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemDate'", NULL);
#endif
  {
   CCReViSvItemData* tolua_ret = (CCReViSvItemData*)  self->getItemDate();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCReViSvItemData");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemDate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runAnimation of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_runAnimation00
static int tolua_Gamelua_ScriptContentBase_runAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  const std::string actionname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runAnimation'", NULL);
#endif
  {
   self->runAnimation(actionname);
   tolua_pushcppstring(tolua_S,(const char*)actionname);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDynamicIconTex of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_setDynamicIconTex00
static int tolua_Gamelua_ScriptContentBase_setDynamicIconTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  std::string frameName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string iconName = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string iconStr = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDynamicIconTex'", NULL);
#endif
  {
   self->setDynamicIconTex(frameName,iconName,iconStr);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDynamicIconTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFrameColorByQuality of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_setFrameColorByQuality00
static int tolua_Gamelua_ScriptContentBase_setFrameColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  std::string frameName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int quality = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFrameColorByQuality'", NULL);
#endif
  {
   self->setFrameColorByQuality(frameName,quality);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFrameColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLabelColorByQuality of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_setLabelColorByQuality00
static int tolua_Gamelua_ScriptContentBase_setLabelColorByQuality00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
  std::string labelName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int quality = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLabelColorByQuality'", NULL);
#endif
  {
   self->setLabelColorByQuality(labelName,quality);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLabelColorByQuality'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dumpInfo of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptContentBase_dumpInfo00
static int tolua_Gamelua_ScriptContentBase_dumpInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptContentBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dumpInfo'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->dumpInfo();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dumpInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __CCReViSvItemNodeFacade__ of class  ScriptContentBase */
#ifndef TOLUA_DISABLE_tolua_get_ScriptContentBase___CCReViSvItemNodeFacade__
static int tolua_get_ScriptContentBase___CCReViSvItemNodeFacade__(lua_State* tolua_S)
{
  ScriptContentBase* self = (ScriptContentBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__CCReViSvItemNodeFacade__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<CCReViSvItemNodeFacade*>(self), "CCReViSvItemNodeFacade");
#else
   tolua_pushusertype(tolua_S,(void*)((CCReViSvItemNodeFacade*)self), "CCReViSvItemNodeFacade");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViSvItemData_new00
static int tolua_Gamelua_CCReViSvItemData_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCReViSvItemData* tolua_ret = (CCReViSvItemData*)  Mtolua_new((CCReViSvItemData)());
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCReViSvItemData");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViSvItemData_new00_local
static int tolua_Gamelua_CCReViSvItemData_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCReViSvItemData* tolua_ret = (CCReViSvItemData*)  Mtolua_new((CCReViSvItemData)());
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCReViSvItemData");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ptPosition of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_get_CCReViSvItemData_m_ptPosition
static int tolua_get_CCReViSvItemData_m_ptPosition(lua_State* tolua_S)
{
  CCReViSvItemData* self = (CCReViSvItemData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ptPosition'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_ptPosition,"CCPoint");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ptPosition of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_set_CCReViSvItemData_m_ptPosition
static int tolua_set_CCReViSvItemData_m_ptPosition(lua_State* tolua_S)
{
  CCReViSvItemData* self = (CCReViSvItemData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ptPosition'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ptPosition = *((CCPoint*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_iIdx of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_get_CCReViSvItemData_unsigned_m_iIdx
static int tolua_get_CCReViSvItemData_unsigned_m_iIdx(lua_State* tolua_S)
{
  CCReViSvItemData* self = (CCReViSvItemData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iIdx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_iIdx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_iIdx of class  CCReViSvItemData */
#ifndef TOLUA_DISABLE_tolua_set_CCReViSvItemData_unsigned_m_iIdx
static int tolua_set_CCReViSvItemData_unsigned_m_iIdx(lua_State* tolua_S)
{
  CCReViSvItemData* self = (CCReViSvItemData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iIdx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_iIdx = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: initItemView of class  CCReViSvItemNodeFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViSvItemNodeFacade_initItemView00
static int tolua_Gamelua_CCReViSvItemNodeFacade_initItemView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViSvItemNodeFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViSvItemNodeFacade* self = (CCReViSvItemNodeFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initItemView'", NULL);
#endif
  {
   self->initItemView();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initItemView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: refreshItemView of class  CCReViSvItemNodeFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViSvItemNodeFacade_refreshItemView00
static int tolua_Gamelua_CCReViSvItemNodeFacade_refreshItemView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViSvItemNodeFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViSvItemNodeFacade* self = (CCReViSvItemNodeFacade*)  tolua_tousertype(tolua_S,1,0);
  CCReViSvItemData* pItem = ((CCReViSvItemData*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'refreshItemView'", NULL);
#endif
  {
   self->refreshItemView(pItem);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'refreshItemView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_new00
static int tolua_Gamelua_CCReViScrollViewFacade_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* pView = ((CCScrollView*)  tolua_tousertype(tolua_S,2,0));
  {
   CCReViScrollViewFacade* tolua_ret = (CCReViScrollViewFacade*)  Mtolua_new((CCReViScrollViewFacade)(pView));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCReViScrollViewFacade");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_new00_local
static int tolua_Gamelua_CCReViScrollViewFacade_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* pView = ((CCScrollView*)  tolua_tousertype(tolua_S,2,0));
  {
   CCReViScrollViewFacade* tolua_ret = (CCReViScrollViewFacade*)  Mtolua_new((CCReViScrollViewFacade)(pView));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCReViScrollViewFacade");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_delete00
static int tolua_Gamelua_CCReViScrollViewFacade_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_init00
static int tolua_Gamelua_CCReViScrollViewFacade_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  unsigned int iNum = ((unsigned int)  tolua_tonumber(tolua_S,2,4));
  unsigned int iSize = ((unsigned int)  tolua_tonumber(tolua_S,3,4));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init(iNum,iSize);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaxDynamicControledItemViewsNum of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_getMaxDynamicControledItemViewsNum00
static int tolua_Gamelua_CCReViScrollViewFacade_getMaxDynamicControledItemViewsNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaxDynamicControledItemViewsNum'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getMaxDynamicControledItemViewsNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaxDynamicControledItemViewsNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addItem of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_addItem00
static int tolua_Gamelua_CCReViScrollViewFacade_addItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCReViSvItemNodeFacade",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  CCReViSvItemData* item = ((CCReViSvItemData*)  tolua_tousertype(tolua_S,2,0));
  CCReViSvItemNodeFacade* node = ((CCReViSvItemNodeFacade*)  tolua_tousertype(tolua_S,3,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addItem'", NULL);
#endif
  {
   self->addItem(item,node);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: insertItemAtIdx of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_insertItemAtIdx00
static int tolua_Gamelua_CCReViScrollViewFacade_insertItemAtIdx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCReViSvItemNodeFacade",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  CCReViSvItemData* item = ((CCReViSvItemData*)  tolua_tousertype(tolua_S,2,0));
  int idx = ((int)  tolua_tonumber(tolua_S,3,0));
  CCReViSvItemNodeFacade* node = ((CCReViSvItemNodeFacade*)  tolua_tousertype(tolua_S,4,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'insertItemAtIdx'", NULL);
#endif
  {
   self->insertItemAtIdx(item,idx,node);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'insertItemAtIdx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeItem of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_removeItem00
static int tolua_Gamelua_CCReViScrollViewFacade_removeItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  CCReViSvItemData* item = ((CCReViSvItemData*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeItem'", NULL);
#endif
  {
   self->removeItem(item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeItemAtIdx of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_removeItemAtIdx00
static int tolua_Gamelua_CCReViScrollViewFacade_removeItemAtIdx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  int idx = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeItemAtIdx'", NULL);
#endif
  {
   self->removeItemAtIdx(idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeItemAtIdx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearAllItems of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_clearAllItems00
static int tolua_Gamelua_CCReViScrollViewFacade_clearAllItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearAllItems'", NULL);
#endif
  {
   self->clearAllItems();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearAllItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDynamicItemsStartPosition of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_setDynamicItemsStartPosition00
static int tolua_Gamelua_CCReViScrollViewFacade_setDynamicItemsStartPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  unsigned int idx = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDynamicItemsStartPosition'", NULL);
#endif
  {
   self->setDynamicItemsStartPosition(idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDynamicItemsStartPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStartPosition of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_getStartPosition00
static int tolua_Gamelua_CCReViScrollViewFacade_getStartPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStartPosition'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getStartPosition();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStartPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: refreshDynamicScrollView of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_refreshDynamicScrollView00
static int tolua_Gamelua_CCReViScrollViewFacade_refreshDynamicScrollView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'refreshDynamicScrollView'", NULL);
#endif
  {
   self->refreshDynamicScrollView();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'refreshDynamicScrollView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findItemNodeByItemData of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByItemData00
static int tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByItemData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCReViSvItemData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  CCReViSvItemData* pItem = ((CCReViSvItemData*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findItemNodeByItemData'", NULL);
#endif
  {
   CCReViSvItemNodeFacade* tolua_ret = (CCReViSvItemNodeFacade*)  self->findItemNodeByItemData(pItem);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCReViSvItemNodeFacade");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findItemNodeByItemData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findItemNodeByIndex of class  CCReViScrollViewFacade */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByIndex00
static int tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCReViScrollViewFacade",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCReViScrollViewFacade* self = (CCReViScrollViewFacade*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findItemNodeByIndex'", NULL);
#endif
  {
   CCReViSvItemNodeFacade* tolua_ret = (CCReViSvItemNodeFacade*)  self->findItemNodeByIndex(index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCReViSvItemNodeFacade");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findItemNodeByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString00
static int tolua_Gamelua_StringConverter_toString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float val = ((float)  tolua_tonumber(tolua_S,2,0));
  unsigned short precision = ((unsigned short)  tolua_tonumber(tolua_S,3,6));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,4,0));
  char fill = ((char)  tolua_tonumber(tolua_S,5,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,precision,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString01
static int tolua_Gamelua_StringConverter_toString01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float val = ((float)  tolua_tonumber(tolua_S,2,0));
  unsigned short precision = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,4,0));
  char fill = ((char)  tolua_tonumber(tolua_S,5,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,6,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,precision,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString02
static int tolua_Gamelua_StringConverter_toString02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int val = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString03
static int tolua_Gamelua_StringConverter_toString03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int val = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,5,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString04
static int tolua_Gamelua_StringConverter_toString04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int val = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString05
static int tolua_Gamelua_StringConverter_toString05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned int val = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,5,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString06
static int tolua_Gamelua_StringConverter_toString06(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned long val = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString05(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString07
static int tolua_Gamelua_StringConverter_toString07(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  unsigned long val = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,5,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString06(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString08
static int tolua_Gamelua_StringConverter_toString08(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long val = ((long)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString07(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString09
static int tolua_Gamelua_StringConverter_toString09(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long val = ((long)  tolua_tonumber(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,5,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString08(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString10
static int tolua_Gamelua_StringConverter_toString10(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"int64_t",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64_t val = *((int64_t*)  tolua_tousertype(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,' '));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString09(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString11
static int tolua_Gamelua_StringConverter_toString11(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"int64_t",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"std::ios::fmtflags",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64_t val = *((int64_t*)  tolua_tousertype(tolua_S,2,0));
  unsigned short width = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  char fill = ((char)  tolua_tonumber(tolua_S,4,0));
  std::ios::fmtflags flags = *((std::ios::fmtflags*)  tolua_tousertype(tolua_S,5,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,width,fill,flags);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString10(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString12
static int tolua_Gamelua_StringConverter_toString12(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  bool val = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool yesNo = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(val,yesNo);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString11(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString13
static int tolua_Gamelua_StringConverter_toString13(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const CCRect",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const CCRect* val = ((const CCRect*)  tolua_tousertype(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(*val);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString12(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString14
static int tolua_Gamelua_StringConverter_toString14(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const CCSize",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const CCSize* val = ((const CCSize*)  tolua_tousertype(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(*val);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString13(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString15
static int tolua_Gamelua_StringConverter_toString15(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const CCPoint",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const CCPoint* val = ((const CCPoint*)  tolua_tousertype(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(*val);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString14(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toString of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_toString16
static int tolua_Gamelua_StringConverter_toString16(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const ccColor3B",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const ccColor3B* val = ((const ccColor3B*)  tolua_tousertype(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  StringConverter::toString(*val);
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_StringConverter_toString15(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: parseRect of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_parseRect00
static int tolua_Gamelua_StringConverter_parseRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string val = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CCRect tolua_ret = (CCRect)  StringConverter::parseRect(val);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCRect)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCRect");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCRect));
     tolua_pushusertype(tolua_S,tolua_obj,"CCRect");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)val);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: parsePoint of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_parsePoint00
static int tolua_Gamelua_StringConverter_parsePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string val = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CCPoint tolua_ret = (CCPoint)  StringConverter::parsePoint(val);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCPoint));
     tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)val);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parsePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: parseSize of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_parseSize00
static int tolua_Gamelua_StringConverter_parseSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string val = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CCSize tolua_ret = (CCSize)  StringConverter::parseSize(val);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CCSize)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CCSize));
     tolua_pushusertype(tolua_S,tolua_obj,"CCSize");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)val);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: parseColor3B of class  StringConverter */
#ifndef TOLUA_DISABLE_tolua_Gamelua_StringConverter_parseColor3B00
static int tolua_Gamelua_StringConverter_parseColor3B00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"StringConverter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string val = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   ccColor3B tolua_ret = (ccColor3B)  StringConverter::parseColor3B(val);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((ccColor3B)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ccColor3B));
     tolua_pushusertype(tolua_S,tolua_obj,"ccColor3B");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)val);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseColor3B'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_new00
static int tolua_Gamelua_PacketScriptHandler_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,3,0));
  {
   PacketScriptHandler* tolua_ret = (PacketScriptHandler*)  Mtolua_new((PacketScriptHandler)(opcode,nHandler));
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"PacketScriptHandler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_new00_local
static int tolua_Gamelua_PacketScriptHandler_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,3,0));
  {
   PacketScriptHandler* tolua_ret = (PacketScriptHandler*)  Mtolua_new((PacketScriptHandler)(opcode,nHandler));
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"PacketScriptHandler");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_delete00
static int tolua_Gamelua_PacketScriptHandler_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketScriptHandler* self = (PacketScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRecPacketBufferLength of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_getRecPacketBufferLength00
static int tolua_Gamelua_PacketScriptHandler_getRecPacketBufferLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketScriptHandler* self = (PacketScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRecPacketBufferLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getRecPacketBufferLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRecPacketBufferLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRecPacketBuffer of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_getRecPacketBuffer00
static int tolua_Gamelua_PacketScriptHandler_getRecPacketBuffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketScriptHandler* self = (PacketScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRecPacketBuffer'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getRecPacketBuffer();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRecPacketBuffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerFunctionHandler of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketScriptHandler_registerFunctionHandler00
static int tolua_Gamelua_PacketScriptHandler_registerFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketScriptHandler",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketScriptHandler* self = (PacketScriptHandler*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerFunctionHandler'", NULL);
#endif
  {
   self->registerFunctionHandler(nHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __PacketHandler__ of class  PacketScriptHandler */
#ifndef TOLUA_DISABLE_tolua_get_PacketScriptHandler___PacketHandler__
static int tolua_get_PacketScriptHandler___PacketHandler__(lua_State* tolua_S)
{
  PacketScriptHandler* self = (PacketScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__PacketHandler__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<PacketHandler*>(self), "PacketHandler");
#else
   tolua_pushusertype(tolua_S,(void*)((PacketHandler*)self), "PacketHandler");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_getInstance00
static int tolua_Gamelua_PacketManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   PacketManager* tolua_ret = (PacketManager*)  PacketManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"PacketManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_disconnect00
static int tolua_Gamelua_PacketManager_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'", NULL);
#endif
  {
   self->disconnect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerPacketHandler of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_registerPacketHandler00
static int tolua_Gamelua_PacketManager_registerPacketHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"PacketHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  PacketHandler* tolua_var_4 = ((PacketHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerPacketHandler'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->registerPacketHandler(opcode,tolua_var_4);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerPacketHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: nameToOpcode of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_nameToOpcode00
static int tolua_Gamelua_PacketManager_nameToOpcode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'nameToOpcode'", NULL);
#endif
  {
   int tolua_ret = (int)  self->nameToOpcode(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'nameToOpcode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removePacketHandler of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_removePacketHandler00
static int tolua_Gamelua_PacketManager_removePacketHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"PacketHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  PacketHandler* packethandler = ((PacketHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removePacketHandler'", NULL);
#endif
  {
   self->removePacketHandler(opcode,packethandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removePacketHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removePacketHandler of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_removePacketHandler01
static int tolua_Gamelua_PacketManager_removePacketHandler01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"PacketHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
  PacketHandler* packethandler = ((PacketHandler*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removePacketHandler'", NULL);
#endif
  {
   self->removePacketHandler(packethandler);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_PacketManager_removePacketHandler00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendPakcet of class  PacketManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_PacketManager_sendPakcet00
static int tolua_Gamelua_PacketManager_sendPakcet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"PacketManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  PacketManager* self = (PacketManager*)  tolua_tousertype(tolua_S,1,0);
  int opcode = ((int)  tolua_tonumber(tolua_S,2,0));
  char* buff = ((char*)  tolua_tostring(tolua_S,3,0));
  int length = ((int)  tolua_tonumber(tolua_S,4,0));
  bool needWaiting = ((bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendPakcet'", NULL);
#endif
  {
   self->sendPakcet(opcode,buff,length,needWaiting);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendPakcet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showWXChat of class  ScriptMathToLua */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptMathToLua_showWXChat00
static int tolua_Gamelua_ScriptMathToLua_showWXChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ScriptMathToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ScriptMathToLua::showWXChat();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showWXChat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSwallowsTouches of class  ScriptMathToLua */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ScriptMathToLua_setSwallowsTouches00
static int tolua_Gamelua_ScriptMathToLua_setSwallowsTouches00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ScriptMathToLua",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCScrollView* _scroll = ((CCScrollView*)  tolua_tousertype(tolua_S,2,0));
  {
   ScriptMathToLua::setSwallowsTouches(_scroll);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSwallowsTouches'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgLoadingFrameEnterGame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLoadingFrameEnterGame_getTrueType00
static int tolua_Gamelua_MsgLoadingFrameEnterGame_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLoadingFrameEnterGame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgLoadingFrameEnterGame* tolua_ret = (MsgLoadingFrameEnterGame*)  MsgLoadingFrameEnterGame::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLoadingFrameEnterGame");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgLoadingFrameEnterGame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLoadingFrameEnterGame_new00
static int tolua_Gamelua_MsgLoadingFrameEnterGame_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLoadingFrameEnterGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgLoadingFrameEnterGame* tolua_ret = (MsgLoadingFrameEnterGame*)  Mtolua_new((MsgLoadingFrameEnterGame)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLoadingFrameEnterGame");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgLoadingFrameEnterGame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLoadingFrameEnterGame_new00_local
static int tolua_Gamelua_MsgLoadingFrameEnterGame_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLoadingFrameEnterGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgLoadingFrameEnterGame* tolua_ret = (MsgLoadingFrameEnterGame*)  Mtolua_new((MsgLoadingFrameEnterGame)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLoadingFrameEnterGame");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgLoadingFrameEnterGame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLoadingFrameEnterGame_getTypeId00
static int tolua_Gamelua_MsgLoadingFrameEnterGame_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgLoadingFrameEnterGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgLoadingFrameEnterGame* self = (const MsgLoadingFrameEnterGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgLoadingFrameEnterGame */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLoadingFrameEnterGame_clone00
static int tolua_Gamelua_MsgLoadingFrameEnterGame_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgLoadingFrameEnterGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgLoadingFrameEnterGame* self = (const MsgLoadingFrameEnterGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameChangePage_getTrueType00
static int tolua_Gamelua_MsgMainFrameChangePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameChangePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameChangePage* tolua_ret = (MsgMainFrameChangePage*)  MsgMainFrameChangePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameChangePage_new00
static int tolua_Gamelua_MsgMainFrameChangePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameChangePage* tolua_ret = (MsgMainFrameChangePage*)  Mtolua_new((MsgMainFrameChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameChangePage_new00_local
static int tolua_Gamelua_MsgMainFrameChangePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameChangePage* tolua_ret = (MsgMainFrameChangePage*)  Mtolua_new((MsgMainFrameChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameChangePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameChangePage_pageName
static int tolua_get_MsgMainFrameChangePage_pageName(lua_State* tolua_S)
{
  MsgMainFrameChangePage* self = (MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameChangePage_pageName
static int tolua_set_MsgMainFrameChangePage_pageName(lua_State* tolua_S)
{
  MsgMainFrameChangePage* self = (MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: needPopAllPage of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameChangePage_needPopAllPage
static int tolua_get_MsgMainFrameChangePage_needPopAllPage(lua_State* tolua_S)
{
  MsgMainFrameChangePage* self = (MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'needPopAllPage'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->needPopAllPage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: needPopAllPage of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameChangePage_needPopAllPage
static int tolua_set_MsgMainFrameChangePage_needPopAllPage(lua_State* tolua_S)
{
  MsgMainFrameChangePage* self = (MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'needPopAllPage'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->needPopAllPage = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameChangePage_getTypeId00
static int tolua_Gamelua_MsgMainFrameChangePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameChangePage* self = (const MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameChangePage_clone00
static int tolua_Gamelua_MsgMainFrameChangePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameChangePage* self = (const MsgMainFrameChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePushPage_getTrueType00
static int tolua_Gamelua_MsgMainFramePushPage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePushPage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFramePushPage* tolua_ret = (MsgMainFramePushPage*)  MsgMainFramePushPage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePushPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePushPage_new00
static int tolua_Gamelua_MsgMainFramePushPage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePushPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFramePushPage* tolua_ret = (MsgMainFramePushPage*)  Mtolua_new((MsgMainFramePushPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePushPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePushPage_new00_local
static int tolua_Gamelua_MsgMainFramePushPage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePushPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFramePushPage* tolua_ret = (MsgMainFramePushPage*)  Mtolua_new((MsgMainFramePushPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePushPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFramePushPage_pageName
static int tolua_get_MsgMainFramePushPage_pageName(lua_State* tolua_S)
{
  MsgMainFramePushPage* self = (MsgMainFramePushPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFramePushPage_pageName
static int tolua_set_MsgMainFramePushPage_pageName(lua_State* tolua_S)
{
  MsgMainFramePushPage* self = (MsgMainFramePushPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePushPage_getTypeId00
static int tolua_Gamelua_MsgMainFramePushPage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFramePushPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFramePushPage* self = (const MsgMainFramePushPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFramePushPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePushPage_clone00
static int tolua_Gamelua_MsgMainFramePushPage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFramePushPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFramePushPage* self = (const MsgMainFramePushPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePopPage_getTrueType00
static int tolua_Gamelua_MsgMainFramePopPage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePopPage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFramePopPage* tolua_ret = (MsgMainFramePopPage*)  MsgMainFramePopPage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePopPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePopPage_new00
static int tolua_Gamelua_MsgMainFramePopPage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePopPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFramePopPage* tolua_ret = (MsgMainFramePopPage*)  Mtolua_new((MsgMainFramePopPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePopPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePopPage_new00_local
static int tolua_Gamelua_MsgMainFramePopPage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFramePopPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFramePopPage* tolua_ret = (MsgMainFramePopPage*)  Mtolua_new((MsgMainFramePopPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFramePopPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFramePopPage_pageName
static int tolua_get_MsgMainFramePopPage_pageName(lua_State* tolua_S)
{
  MsgMainFramePopPage* self = (MsgMainFramePopPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFramePopPage_pageName
static int tolua_set_MsgMainFramePopPage_pageName(lua_State* tolua_S)
{
  MsgMainFramePopPage* self = (MsgMainFramePopPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePopPage_getTypeId00
static int tolua_Gamelua_MsgMainFramePopPage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFramePopPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFramePopPage* self = (const MsgMainFramePopPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFramePopPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFramePopPage_clone00
static int tolua_Gamelua_MsgMainFramePopPage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFramePopPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFramePopPage* self = (const MsgMainFramePopPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameMSGShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameMSGShow_getTrueType00
static int tolua_Gamelua_MsgMainFrameMSGShow_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameMSGShow",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameMSGShow* tolua_ret = (MsgMainFrameMSGShow*)  MsgMainFrameMSGShow::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameMSGShow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameMSGShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameMSGShow_new00
static int tolua_Gamelua_MsgMainFrameMSGShow_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameMSGShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameMSGShow* tolua_ret = (MsgMainFrameMSGShow*)  Mtolua_new((MsgMainFrameMSGShow)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameMSGShow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameMSGShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameMSGShow_new00_local
static int tolua_Gamelua_MsgMainFrameMSGShow_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameMSGShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameMSGShow* tolua_ret = (MsgMainFrameMSGShow*)  Mtolua_new((MsgMainFrameMSGShow)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameMSGShow");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameMSGShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameMSGShow_getTypeId00
static int tolua_Gamelua_MsgMainFrameMSGShow_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameMSGShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameMSGShow* self = (const MsgMainFrameMSGShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameMSGShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameMSGShow_clone00
static int tolua_Gamelua_MsgMainFrameMSGShow_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameMSGShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameMSGShow* self = (const MsgMainFrameMSGShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverShow_getTrueType00
static int tolua_Gamelua_MsgMainFrameCoverShow_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverShow",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameCoverShow* tolua_ret = (MsgMainFrameCoverShow*)  MsgMainFrameCoverShow::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverShow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverShow_new00
static int tolua_Gamelua_MsgMainFrameCoverShow_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameCoverShow* tolua_ret = (MsgMainFrameCoverShow*)  Mtolua_new((MsgMainFrameCoverShow)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverShow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverShow_new00_local
static int tolua_Gamelua_MsgMainFrameCoverShow_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameCoverShow* tolua_ret = (MsgMainFrameCoverShow*)  Mtolua_new((MsgMainFrameCoverShow)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverShow");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverShow_getTypeId00
static int tolua_Gamelua_MsgMainFrameCoverShow_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameCoverShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameCoverShow* self = (const MsgMainFrameCoverShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameCoverShow_pageName
static int tolua_get_MsgMainFrameCoverShow_pageName(lua_State* tolua_S)
{
  MsgMainFrameCoverShow* self = (MsgMainFrameCoverShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameCoverShow_pageName
static int tolua_set_MsgMainFrameCoverShow_pageName(lua_State* tolua_S)
{
  MsgMainFrameCoverShow* self = (MsgMainFrameCoverShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameCoverShow */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverShow_clone00
static int tolua_Gamelua_MsgMainFrameCoverShow_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameCoverShow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameCoverShow* self = (const MsgMainFrameCoverShow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverHide_getTrueType00
static int tolua_Gamelua_MsgMainFrameCoverHide_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverHide",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameCoverHide* tolua_ret = (MsgMainFrameCoverHide*)  MsgMainFrameCoverHide::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverHide");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverHide_new00
static int tolua_Gamelua_MsgMainFrameCoverHide_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverHide",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameCoverHide* tolua_ret = (MsgMainFrameCoverHide*)  Mtolua_new((MsgMainFrameCoverHide)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverHide");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverHide_new00_local
static int tolua_Gamelua_MsgMainFrameCoverHide_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameCoverHide",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameCoverHide* tolua_ret = (MsgMainFrameCoverHide*)  Mtolua_new((MsgMainFrameCoverHide)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameCoverHide");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverHide_getTypeId00
static int tolua_Gamelua_MsgMainFrameCoverHide_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameCoverHide",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameCoverHide* self = (const MsgMainFrameCoverHide*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameCoverHide_pageName
static int tolua_get_MsgMainFrameCoverHide_pageName(lua_State* tolua_S)
{
  MsgMainFrameCoverHide* self = (MsgMainFrameCoverHide*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameCoverHide_pageName
static int tolua_set_MsgMainFrameCoverHide_pageName(lua_State* tolua_S)
{
  MsgMainFrameCoverHide* self = (MsgMainFrameCoverHide*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameCoverHide */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameCoverHide_clone00
static int tolua_Gamelua_MsgMainFrameCoverHide_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameCoverHide",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameCoverHide* self = (const MsgMainFrameCoverHide*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_getTrueType00
static int tolua_Gamelua_MsgCareerChangePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgCareerChangePage* tolua_ret = (MsgCareerChangePage*)  MsgCareerChangePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgCareerChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_new00
static int tolua_Gamelua_MsgCareerChangePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgCareerChangePage* tolua_ret = (MsgCareerChangePage*)  Mtolua_new((MsgCareerChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgCareerChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_new00_local
static int tolua_Gamelua_MsgCareerChangePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgCareerChangePage* tolua_ret = (MsgCareerChangePage*)  Mtolua_new((MsgCareerChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgCareerChangePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageID of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgCareerChangePage_pageID
static int tolua_get_MsgCareerChangePage_pageID(lua_State* tolua_S)
{
  MsgCareerChangePage* self = (MsgCareerChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pageID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageID of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgCareerChangePage_pageID
static int tolua_set_MsgCareerChangePage_pageID(lua_State* tolua_S)
{
  MsgCareerChangePage* self = (MsgCareerChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageID = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParameters of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_getParameters00
static int tolua_Gamelua_MsgCareerChangePage_getParameters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgCareerChangePage* self = (const MsgCareerChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParameters'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getParameters();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParameters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_getTypeId00
static int tolua_Gamelua_MsgCareerChangePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgCareerChangePage* self = (const MsgCareerChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgCareerChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgCareerChangePage_clone00
static int tolua_Gamelua_MsgCareerChangePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgCareerChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgCareerChangePage* self = (const MsgCareerChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsPageChangePage_getTrueType00
static int tolua_Gamelua_MsgFriendsPageChangePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsPageChangePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFriendsPageChangePage* tolua_ret = (MsgFriendsPageChangePage*)  MsgFriendsPageChangePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsPageChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsPageChangePage_new00
static int tolua_Gamelua_MsgFriendsPageChangePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsPageChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsPageChangePage* tolua_ret = (MsgFriendsPageChangePage*)  Mtolua_new((MsgFriendsPageChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsPageChangePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsPageChangePage_new00_local
static int tolua_Gamelua_MsgFriendsPageChangePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsPageChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsPageChangePage* tolua_ret = (MsgFriendsPageChangePage*)  Mtolua_new((MsgFriendsPageChangePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsPageChangePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsPageChangePage_index
static int tolua_get_MsgFriendsPageChangePage_index(lua_State* tolua_S)
{
  MsgFriendsPageChangePage* self = (MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsPageChangePage_index
static int tolua_set_MsgFriendsPageChangePage_index(lua_State* tolua_S)
{
  MsgFriendsPageChangePage* self = (MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: oprType of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsPageChangePage_oprType
static int tolua_get_MsgFriendsPageChangePage_oprType(lua_State* tolua_S)
{
  MsgFriendsPageChangePage* self = (MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oprType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->oprType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: oprType of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsPageChangePage_oprType
static int tolua_set_MsgFriendsPageChangePage_oprType(lua_State* tolua_S)
{
  MsgFriendsPageChangePage* self = (MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oprType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->oprType = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsPageChangePage_getTypeId00
static int tolua_Gamelua_MsgFriendsPageChangePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsPageChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsPageChangePage* self = (const MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFriendsPageChangePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsPageChangePage_clone00
static int tolua_Gamelua_MsgFriendsPageChangePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsPageChangePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsPageChangePage* self = (const MsgFriendsPageChangePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgGoToPrivateChatPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPage_getTrueType00
static int tolua_Gamelua_MsgGoToPrivateChatPage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgGoToPrivateChatPage* tolua_ret = (MsgGoToPrivateChatPage*)  MsgGoToPrivateChatPage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgGoToPrivateChatPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPage_new00
static int tolua_Gamelua_MsgGoToPrivateChatPage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgGoToPrivateChatPage* tolua_ret = (MsgGoToPrivateChatPage*)  Mtolua_new((MsgGoToPrivateChatPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgGoToPrivateChatPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPage_new00_local
static int tolua_Gamelua_MsgGoToPrivateChatPage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgGoToPrivateChatPage* tolua_ret = (MsgGoToPrivateChatPage*)  Mtolua_new((MsgGoToPrivateChatPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgGoToPrivateChatPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPage_getTypeId00
static int tolua_Gamelua_MsgGoToPrivateChatPage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgGoToPrivateChatPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgGoToPrivateChatPage* self = (const MsgGoToPrivateChatPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgGoToPrivateChatPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPage_clone00
static int tolua_Gamelua_MsgGoToPrivateChatPage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgGoToPrivateChatPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgGoToPrivateChatPage* self = (const MsgGoToPrivateChatPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgGoToPrivateChatPageFromLeague */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTrueType00
static int tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPageFromLeague",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgGoToPrivateChatPageFromLeague* tolua_ret = (MsgGoToPrivateChatPageFromLeague*)  MsgGoToPrivateChatPageFromLeague::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPageFromLeague");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgGoToPrivateChatPageFromLeague */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00
static int tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPageFromLeague",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgGoToPrivateChatPageFromLeague* tolua_ret = (MsgGoToPrivateChatPageFromLeague*)  Mtolua_new((MsgGoToPrivateChatPageFromLeague)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPageFromLeague");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgGoToPrivateChatPageFromLeague */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00_local
static int tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgGoToPrivateChatPageFromLeague",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgGoToPrivateChatPageFromLeague* tolua_ret = (MsgGoToPrivateChatPageFromLeague*)  Mtolua_new((MsgGoToPrivateChatPageFromLeague)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgGoToPrivateChatPageFromLeague");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgGoToPrivateChatPageFromLeague */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTypeId00
static int tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgGoToPrivateChatPageFromLeague",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgGoToPrivateChatPageFromLeague* self = (const MsgGoToPrivateChatPageFromLeague*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgGoToPrivateChatPageFromLeague */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_clone00
static int tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgGoToPrivateChatPageFromLeague",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgGoToPrivateChatPageFromLeague* self = (const MsgGoToPrivateChatPageFromLeague*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLeaveMsgReceivePage_getTrueType00
static int tolua_Gamelua_MsgLeaveMsgReceivePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLeaveMsgReceivePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgLeaveMsgReceivePage* tolua_ret = (MsgLeaveMsgReceivePage*)  MsgLeaveMsgReceivePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLeaveMsgReceivePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLeaveMsgReceivePage_new00
static int tolua_Gamelua_MsgLeaveMsgReceivePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLeaveMsgReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgLeaveMsgReceivePage* tolua_ret = (MsgLeaveMsgReceivePage*)  Mtolua_new((MsgLeaveMsgReceivePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLeaveMsgReceivePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLeaveMsgReceivePage_new00_local
static int tolua_Gamelua_MsgLeaveMsgReceivePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgLeaveMsgReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgLeaveMsgReceivePage* tolua_ret = (MsgLeaveMsgReceivePage*)  Mtolua_new((MsgLeaveMsgReceivePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgLeaveMsgReceivePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: uID of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgLeaveMsgReceivePage_uID
static int tolua_get_MsgLeaveMsgReceivePage_uID(lua_State* tolua_S)
{
  MsgLeaveMsgReceivePage* self = (MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->uID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: uID of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgLeaveMsgReceivePage_uID
static int tolua_set_MsgLeaveMsgReceivePage_uID(lua_State* tolua_S)
{
  MsgLeaveMsgReceivePage* self = (MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->uID = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: uName of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgLeaveMsgReceivePage_uName
static int tolua_get_MsgLeaveMsgReceivePage_uName(lua_State* tolua_S)
{
  MsgLeaveMsgReceivePage* self = (MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->uName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: uName of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgLeaveMsgReceivePage_uName
static int tolua_set_MsgLeaveMsgReceivePage_uName(lua_State* tolua_S)
{
  MsgLeaveMsgReceivePage* self = (MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->uName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLeaveMsgReceivePage_getTypeId00
static int tolua_Gamelua_MsgLeaveMsgReceivePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgLeaveMsgReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgLeaveMsgReceivePage* self = (const MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgLeaveMsgReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgLeaveMsgReceivePage_clone00
static int tolua_Gamelua_MsgLeaveMsgReceivePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgLeaveMsgReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgLeaveMsgReceivePage* self = (const MsgLeaveMsgReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailReceivePage_getTrueType00
static int tolua_Gamelua_MsgFriendsDetailReceivePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailReceivePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFriendsDetailReceivePage* tolua_ret = (MsgFriendsDetailReceivePage*)  MsgFriendsDetailReceivePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailReceivePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailReceivePage_new00
static int tolua_Gamelua_MsgFriendsDetailReceivePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsDetailReceivePage* tolua_ret = (MsgFriendsDetailReceivePage*)  Mtolua_new((MsgFriendsDetailReceivePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailReceivePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailReceivePage_new00_local
static int tolua_Gamelua_MsgFriendsDetailReceivePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsDetailReceivePage* tolua_ret = (MsgFriendsDetailReceivePage*)  Mtolua_new((MsgFriendsDetailReceivePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailReceivePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailReceivePage_index
static int tolua_get_MsgFriendsDetailReceivePage_index(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailReceivePage_index
static int tolua_set_MsgFriendsDetailReceivePage_index(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: uID of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailReceivePage_uID
static int tolua_get_MsgFriendsDetailReceivePage_uID(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->uID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: uID of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailReceivePage_uID
static int tolua_set_MsgFriendsDetailReceivePage_uID(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->uID = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: type of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailReceivePage_type
static int tolua_get_MsgFriendsDetailReceivePage_type(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: type of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailReceivePage_type
static int tolua_set_MsgFriendsDetailReceivePage_type(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->type = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isAttention of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailReceivePage_isAttention
static int tolua_get_MsgFriendsDetailReceivePage_isAttention(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isAttention'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isAttention);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isAttention of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailReceivePage_isAttention
static int tolua_set_MsgFriendsDetailReceivePage_isAttention(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isAttention'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isAttention = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: uName of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailReceivePage_uName
static int tolua_get_MsgFriendsDetailReceivePage_uName(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->uName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: uName of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailReceivePage_uName
static int tolua_set_MsgFriendsDetailReceivePage_uName(lua_State* tolua_S)
{
  MsgFriendsDetailReceivePage* self = (MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->uName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailReceivePage_getTypeId00
static int tolua_Gamelua_MsgFriendsDetailReceivePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsDetailReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsDetailReceivePage* self = (const MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFriendsDetailReceivePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailReceivePage_clone00
static int tolua_Gamelua_MsgFriendsDetailReceivePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsDetailReceivePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsDetailReceivePage* self = (const MsgFriendsDetailReceivePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailDisposePage_getTrueType00
static int tolua_Gamelua_MsgFriendsDetailDisposePage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailDisposePage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFriendsDetailDisposePage* tolua_ret = (MsgFriendsDetailDisposePage*)  MsgFriendsDetailDisposePage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailDisposePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailDisposePage_new00
static int tolua_Gamelua_MsgFriendsDetailDisposePage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailDisposePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsDetailDisposePage* tolua_ret = (MsgFriendsDetailDisposePage*)  Mtolua_new((MsgFriendsDetailDisposePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailDisposePage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailDisposePage_new00_local
static int tolua_Gamelua_MsgFriendsDetailDisposePage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFriendsDetailDisposePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFriendsDetailDisposePage* tolua_ret = (MsgFriendsDetailDisposePage*)  Mtolua_new((MsgFriendsDetailDisposePage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFriendsDetailDisposePage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailDisposePage_index
static int tolua_get_MsgFriendsDetailDisposePage_index(lua_State* tolua_S)
{
  MsgFriendsDetailDisposePage* self = (MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailDisposePage_index
static int tolua_set_MsgFriendsDetailDisposePage_index(lua_State* tolua_S)
{
  MsgFriendsDetailDisposePage* self = (MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: oprType of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_get_MsgFriendsDetailDisposePage_oprType
static int tolua_get_MsgFriendsDetailDisposePage_oprType(lua_State* tolua_S)
{
  MsgFriendsDetailDisposePage* self = (MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oprType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->oprType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: oprType of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_set_MsgFriendsDetailDisposePage_oprType
static int tolua_set_MsgFriendsDetailDisposePage_oprType(lua_State* tolua_S)
{
  MsgFriendsDetailDisposePage* self = (MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'oprType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->oprType = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailDisposePage_getTypeId00
static int tolua_Gamelua_MsgFriendsDetailDisposePage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsDetailDisposePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsDetailDisposePage* self = (const MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFriendsDetailDisposePage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFriendsDetailDisposePage_clone00
static int tolua_Gamelua_MsgFriendsDetailDisposePage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFriendsDetailDisposePage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFriendsDetailDisposePage* self = (const MsgFriendsDetailDisposePage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgBuyPropPopupPage_getTrueType00
static int tolua_Gamelua_MsgBuyPropPopupPage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgBuyPropPopupPage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgBuyPropPopupPage* tolua_ret = (MsgBuyPropPopupPage*)  MsgBuyPropPopupPage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgBuyPropPopupPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgBuyPropPopupPage_new00
static int tolua_Gamelua_MsgBuyPropPopupPage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgBuyPropPopupPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgBuyPropPopupPage* tolua_ret = (MsgBuyPropPopupPage*)  Mtolua_new((MsgBuyPropPopupPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgBuyPropPopupPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgBuyPropPopupPage_new00_local
static int tolua_Gamelua_MsgBuyPropPopupPage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgBuyPropPopupPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgBuyPropPopupPage* tolua_ret = (MsgBuyPropPopupPage*)  Mtolua_new((MsgBuyPropPopupPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgBuyPropPopupPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: toolId of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgBuyPropPopupPage_unsigned_toolId
static int tolua_get_MsgBuyPropPopupPage_unsigned_toolId(lua_State* tolua_S)
{
  MsgBuyPropPopupPage* self = (MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'toolId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->toolId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: toolId of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgBuyPropPopupPage_unsigned_toolId
static int tolua_set_MsgBuyPropPopupPage_unsigned_toolId(lua_State* tolua_S)
{
  MsgBuyPropPopupPage* self = (MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'toolId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->toolId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: panelIndex of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgBuyPropPopupPage_unsigned_panelIndex
static int tolua_get_MsgBuyPropPopupPage_unsigned_panelIndex(lua_State* tolua_S)
{
  MsgBuyPropPopupPage* self = (MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'panelIndex'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->panelIndex);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: panelIndex of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgBuyPropPopupPage_unsigned_panelIndex
static int tolua_set_MsgBuyPropPopupPage_unsigned_panelIndex(lua_State* tolua_S)
{
  MsgBuyPropPopupPage* self = (MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'panelIndex'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->panelIndex = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgBuyPropPopupPage_getTypeId00
static int tolua_Gamelua_MsgBuyPropPopupPage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgBuyPropPopupPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgBuyPropPopupPage* self = (const MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgBuyPropPopupPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgBuyPropPopupPage_clone00
static int tolua_Gamelua_MsgBuyPropPopupPage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgBuyPropPopupPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgBuyPropPopupPage* self = (const MsgBuyPropPopupPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgSeverInfoUpdate_getTrueType00
static int tolua_Gamelua_MsgSeverInfoUpdate_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgSeverInfoUpdate",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgSeverInfoUpdate* tolua_ret = (MsgSeverInfoUpdate*)  MsgSeverInfoUpdate::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgSeverInfoUpdate");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgSeverInfoUpdate_new00
static int tolua_Gamelua_MsgSeverInfoUpdate_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgSeverInfoUpdate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgSeverInfoUpdate* tolua_ret = (MsgSeverInfoUpdate*)  Mtolua_new((MsgSeverInfoUpdate)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgSeverInfoUpdate");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgSeverInfoUpdate_new00_local
static int tolua_Gamelua_MsgSeverInfoUpdate_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgSeverInfoUpdate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgSeverInfoUpdate* tolua_ret = (MsgSeverInfoUpdate*)  Mtolua_new((MsgSeverInfoUpdate)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgSeverInfoUpdate");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: opcode of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_get_MsgSeverInfoUpdate_unsigned_opcode
static int tolua_get_MsgSeverInfoUpdate_unsigned_opcode(lua_State* tolua_S)
{
  MsgSeverInfoUpdate* self = (MsgSeverInfoUpdate*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'opcode'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->opcode);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: opcode of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_set_MsgSeverInfoUpdate_unsigned_opcode
static int tolua_set_MsgSeverInfoUpdate_unsigned_opcode(lua_State* tolua_S)
{
  MsgSeverInfoUpdate* self = (MsgSeverInfoUpdate*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'opcode'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->opcode = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgSeverInfoUpdate_getTypeId00
static int tolua_Gamelua_MsgSeverInfoUpdate_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgSeverInfoUpdate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgSeverInfoUpdate* self = (const MsgSeverInfoUpdate*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgSeverInfoUpdate */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgSeverInfoUpdate_clone00
static int tolua_Gamelua_MsgSeverInfoUpdate_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgSeverInfoUpdate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgSeverInfoUpdate* self = (const MsgSeverInfoUpdate*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageChange_getTrueType00
static int tolua_Gamelua_MsgAdventurePageChange_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageChange",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgAdventurePageChange* tolua_ret = (MsgAdventurePageChange*)  MsgAdventurePageChange::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageChange_new00
static int tolua_Gamelua_MsgAdventurePageChange_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventurePageChange* tolua_ret = (MsgAdventurePageChange*)  Mtolua_new((MsgAdventurePageChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageChange_new00_local
static int tolua_Gamelua_MsgAdventurePageChange_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventurePageChange* tolua_ret = (MsgAdventurePageChange*)  Mtolua_new((MsgAdventurePageChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageChange");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_get_MsgAdventurePageChange_unsigned_index
static int tolua_get_MsgAdventurePageChange_unsigned_index(lua_State* tolua_S)
{
  MsgAdventurePageChange* self = (MsgAdventurePageChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_set_MsgAdventurePageChange_unsigned_index
static int tolua_set_MsgAdventurePageChange_unsigned_index(lua_State* tolua_S)
{
  MsgAdventurePageChange* self = (MsgAdventurePageChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageChange_getTypeId00
static int tolua_Gamelua_MsgAdventurePageChange_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventurePageChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventurePageChange* self = (const MsgAdventurePageChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgAdventurePageChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageChange_clone00
static int tolua_Gamelua_MsgAdventurePageChange_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventurePageChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventurePageChange* self = (const MsgAdventurePageChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameRefreshPage_getTrueType00
static int tolua_Gamelua_MsgMainFrameRefreshPage_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameRefreshPage",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameRefreshPage* tolua_ret = (MsgMainFrameRefreshPage*)  MsgMainFrameRefreshPage::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameRefreshPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameRefreshPage_new00
static int tolua_Gamelua_MsgMainFrameRefreshPage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameRefreshPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameRefreshPage* tolua_ret = (MsgMainFrameRefreshPage*)  Mtolua_new((MsgMainFrameRefreshPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameRefreshPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameRefreshPage_new00_local
static int tolua_Gamelua_MsgMainFrameRefreshPage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameRefreshPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameRefreshPage* tolua_ret = (MsgMainFrameRefreshPage*)  Mtolua_new((MsgMainFrameRefreshPage)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameRefreshPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pageName of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameRefreshPage_pageName
static int tolua_get_MsgMainFrameRefreshPage_pageName(lua_State* tolua_S)
{
  MsgMainFrameRefreshPage* self = (MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->pageName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pageName of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameRefreshPage_pageName
static int tolua_set_MsgMainFrameRefreshPage_pageName(lua_State* tolua_S)
{
  MsgMainFrameRefreshPage* self = (MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pageName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pageName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extraParam of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameRefreshPage_extraParam
static int tolua_get_MsgMainFrameRefreshPage_extraParam(lua_State* tolua_S)
{
  MsgMainFrameRefreshPage* self = (MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extraParam'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->extraParam);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extraParam of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameRefreshPage_extraParam
static int tolua_set_MsgMainFrameRefreshPage_extraParam(lua_State* tolua_S)
{
  MsgMainFrameRefreshPage* self = (MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extraParam'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extraParam = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameRefreshPage_getTypeId00
static int tolua_Gamelua_MsgMainFrameRefreshPage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameRefreshPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameRefreshPage* self = (const MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameRefreshPage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameRefreshPage_clone00
static int tolua_Gamelua_MsgMainFrameRefreshPage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameRefreshPage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameRefreshPage* self = (const MsgMainFrameRefreshPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameGetNewInfo_getTrueType00
static int tolua_Gamelua_MsgMainFrameGetNewInfo_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameGetNewInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgMainFrameGetNewInfo* tolua_ret = (MsgMainFrameGetNewInfo*)  MsgMainFrameGetNewInfo::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameGetNewInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameGetNewInfo_new00
static int tolua_Gamelua_MsgMainFrameGetNewInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameGetNewInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameGetNewInfo* tolua_ret = (MsgMainFrameGetNewInfo*)  Mtolua_new((MsgMainFrameGetNewInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameGetNewInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameGetNewInfo_new00_local
static int tolua_Gamelua_MsgMainFrameGetNewInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgMainFrameGetNewInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgMainFrameGetNewInfo* tolua_ret = (MsgMainFrameGetNewInfo*)  Mtolua_new((MsgMainFrameGetNewInfo)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgMainFrameGetNewInfo");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: type of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_get_MsgMainFrameGetNewInfo_type
static int tolua_get_MsgMainFrameGetNewInfo_type(lua_State* tolua_S)
{
  MsgMainFrameGetNewInfo* self = (MsgMainFrameGetNewInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: type of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_set_MsgMainFrameGetNewInfo_type
static int tolua_set_MsgMainFrameGetNewInfo_type(lua_State* tolua_S)
{
  MsgMainFrameGetNewInfo* self = (MsgMainFrameGetNewInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->type = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameGetNewInfo_getTypeId00
static int tolua_Gamelua_MsgMainFrameGetNewInfo_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameGetNewInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameGetNewInfo* self = (const MsgMainFrameGetNewInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgMainFrameGetNewInfo */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgMainFrameGetNewInfo_clone00
static int tolua_Gamelua_MsgMainFrameGetNewInfo_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgMainFrameGetNewInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgMainFrameGetNewInfo* self = (const MsgMainFrameGetNewInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventureRemoveItem_getTrueType00
static int tolua_Gamelua_MsgAdventureRemoveItem_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventureRemoveItem",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgAdventureRemoveItem* tolua_ret = (MsgAdventureRemoveItem*)  MsgAdventureRemoveItem::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventureRemoveItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventureRemoveItem_new00
static int tolua_Gamelua_MsgAdventureRemoveItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventureRemoveItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventureRemoveItem* tolua_ret = (MsgAdventureRemoveItem*)  Mtolua_new((MsgAdventureRemoveItem)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventureRemoveItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventureRemoveItem_new00_local
static int tolua_Gamelua_MsgAdventureRemoveItem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventureRemoveItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventureRemoveItem* tolua_ret = (MsgAdventureRemoveItem*)  Mtolua_new((MsgAdventureRemoveItem)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventureRemoveItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_get_MsgAdventureRemoveItem_unsigned_index
static int tolua_get_MsgAdventureRemoveItem_unsigned_index(lua_State* tolua_S)
{
  MsgAdventureRemoveItem* self = (MsgAdventureRemoveItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_set_MsgAdventureRemoveItem_unsigned_index
static int tolua_set_MsgAdventureRemoveItem_unsigned_index(lua_State* tolua_S)
{
  MsgAdventureRemoveItem* self = (MsgAdventureRemoveItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventureRemoveItem_getTypeId00
static int tolua_Gamelua_MsgAdventureRemoveItem_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventureRemoveItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventureRemoveItem* self = (const MsgAdventureRemoveItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgAdventureRemoveItem */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventureRemoveItem_clone00
static int tolua_Gamelua_MsgAdventureRemoveItem_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventureRemoveItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventureRemoveItem* self = (const MsgAdventureRemoveItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgInstructionNext_getTrueType00
static int tolua_Gamelua_MsgInstructionNext_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgInstructionNext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgInstructionNext* tolua_ret = (MsgInstructionNext*)  MsgInstructionNext::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgInstructionNext");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgInstructionNext_new00
static int tolua_Gamelua_MsgInstructionNext_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgInstructionNext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgInstructionNext* tolua_ret = (MsgInstructionNext*)  Mtolua_new((MsgInstructionNext)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgInstructionNext");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgInstructionNext_new00_local
static int tolua_Gamelua_MsgInstructionNext_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgInstructionNext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgInstructionNext* tolua_ret = (MsgInstructionNext*)  Mtolua_new((MsgInstructionNext)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgInstructionNext");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tag of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_get_MsgInstructionNext_unsigned_tag
static int tolua_get_MsgInstructionNext_unsigned_tag(lua_State* tolua_S)
{
  MsgInstructionNext* self = (MsgInstructionNext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tag'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tag of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_set_MsgInstructionNext_unsigned_tag
static int tolua_set_MsgInstructionNext_unsigned_tag(lua_State* tolua_S)
{
  MsgInstructionNext* self = (MsgInstructionNext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tag'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tag = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgInstructionNext_getTypeId00
static int tolua_Gamelua_MsgInstructionNext_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgInstructionNext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgInstructionNext* self = (const MsgInstructionNext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgInstructionNext */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgInstructionNext_clone00
static int tolua_Gamelua_MsgInstructionNext_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgInstructionNext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgInstructionNext* self = (const MsgInstructionNext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightEnd_getTrueType00
static int tolua_Gamelua_MsgFightEnd_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightEnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFightEnd* tolua_ret = (MsgFightEnd*)  MsgFightEnd::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightEnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightEnd_new00
static int tolua_Gamelua_MsgFightEnd_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightEnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightEnd* tolua_ret = (MsgFightEnd*)  Mtolua_new((MsgFightEnd)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightEnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightEnd_new00_local
static int tolua_Gamelua_MsgFightEnd_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightEnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightEnd* tolua_ret = (MsgFightEnd*)  Mtolua_new((MsgFightEnd)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightEnd");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gotItem of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_get_MsgFightEnd_gotItem
static int tolua_get_MsgFightEnd_gotItem(lua_State* tolua_S)
{
  MsgFightEnd* self = (MsgFightEnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gotItem'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->gotItem);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gotItem of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_set_MsgFightEnd_gotItem
static int tolua_set_MsgFightEnd_gotItem(lua_State* tolua_S)
{
  MsgFightEnd* self = (MsgFightEnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gotItem'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gotItem = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightEnd_getTypeId00
static int tolua_Gamelua_MsgFightEnd_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightEnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightEnd* self = (const MsgFightEnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFightEnd */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightEnd_clone00
static int tolua_Gamelua_MsgFightEnd_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightEnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightEnd* self = (const MsgFightEnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgButtonPressed_getTrueType00
static int tolua_Gamelua_MsgButtonPressed_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgButtonPressed",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgButtonPressed* tolua_ret = (MsgButtonPressed*)  MsgButtonPressed::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgButtonPressed");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgButtonPressed_new00
static int tolua_Gamelua_MsgButtonPressed_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgButtonPressed",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgButtonPressed* tolua_ret = (MsgButtonPressed*)  Mtolua_new((MsgButtonPressed)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgButtonPressed");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgButtonPressed_new00_local
static int tolua_Gamelua_MsgButtonPressed_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgButtonPressed",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgButtonPressed* tolua_ret = (MsgButtonPressed*)  Mtolua_new((MsgButtonPressed)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgButtonPressed");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ccb of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_get_MsgButtonPressed_ccb
static int tolua_get_MsgButtonPressed_ccb(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ccb'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ccb);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ccb of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_set_MsgButtonPressed_ccb
static int tolua_set_MsgButtonPressed_ccb(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ccb'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ccb = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: func of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_get_MsgButtonPressed_func
static int tolua_get_MsgButtonPressed_func(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'func'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->func);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: func of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_set_MsgButtonPressed_func
static int tolua_set_MsgButtonPressed_func(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'func'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->func = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tag of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_get_MsgButtonPressed_tag
static int tolua_get_MsgButtonPressed_tag(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tag'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tag of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_set_MsgButtonPressed_tag
static int tolua_set_MsgButtonPressed_tag(lua_State* tolua_S)
{
  MsgButtonPressed* self = (MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tag'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tag = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgButtonPressed_getTypeId00
static int tolua_Gamelua_MsgButtonPressed_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgButtonPressed",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgButtonPressed* self = (const MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgButtonPressed */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgButtonPressed_clone00
static int tolua_Gamelua_MsgButtonPressed_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgButtonPressed",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgButtonPressed* self = (const MsgButtonPressed*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgToTab_getTrueType00
static int tolua_Gamelua_MsgToTab_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgToTab",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgToTab* tolua_ret = (MsgToTab*)  MsgToTab::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgToTab");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgToTab_new00
static int tolua_Gamelua_MsgToTab_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgToTab",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgToTab* tolua_ret = (MsgToTab*)  Mtolua_new((MsgToTab)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgToTab");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgToTab_new00_local
static int tolua_Gamelua_MsgToTab_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgToTab",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgToTab* tolua_ret = (MsgToTab*)  Mtolua_new((MsgToTab)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgToTab");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tabIndex of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_get_MsgToTab_unsigned_tabIndex
static int tolua_get_MsgToTab_unsigned_tabIndex(lua_State* tolua_S)
{
  MsgToTab* self = (MsgToTab*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tabIndex'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tabIndex);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tabIndex of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_set_MsgToTab_unsigned_tabIndex
static int tolua_set_MsgToTab_unsigned_tabIndex(lua_State* tolua_S)
{
  MsgToTab* self = (MsgToTab*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tabIndex'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tabIndex = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgToTab_getTypeId00
static int tolua_Gamelua_MsgToTab_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgToTab",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgToTab* self = (const MsgToTab*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgToTab */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgToTab_clone00
static int tolua_Gamelua_MsgToTab_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgToTab",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgToTab* self = (const MsgToTab*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFightRoundLoadSkill */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightRoundLoadSkill_getTrueType00
static int tolua_Gamelua_MsgFightRoundLoadSkill_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightRoundLoadSkill",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFightRoundLoadSkill* tolua_ret = (MsgFightRoundLoadSkill*)  MsgFightRoundLoadSkill::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightRoundLoadSkill");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFightRoundLoadSkill */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightRoundLoadSkill_new00
static int tolua_Gamelua_MsgFightRoundLoadSkill_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightRoundLoadSkill",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightRoundLoadSkill* tolua_ret = (MsgFightRoundLoadSkill*)  Mtolua_new((MsgFightRoundLoadSkill)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightRoundLoadSkill");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFightRoundLoadSkill */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightRoundLoadSkill_new00_local
static int tolua_Gamelua_MsgFightRoundLoadSkill_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightRoundLoadSkill",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightRoundLoadSkill* tolua_ret = (MsgFightRoundLoadSkill*)  Mtolua_new((MsgFightRoundLoadSkill)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightRoundLoadSkill");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFightRoundLoadSkill */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightRoundLoadSkill_getTypeId00
static int tolua_Gamelua_MsgFightRoundLoadSkill_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightRoundLoadSkill",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightRoundLoadSkill* self = (const MsgFightRoundLoadSkill*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFightRoundLoadSkill */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightRoundLoadSkill_clone00
static int tolua_Gamelua_MsgFightRoundLoadSkill_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightRoundLoadSkill",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightRoundLoadSkill* self = (const MsgFightRoundLoadSkill*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgRechargeSuccess */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgRechargeSuccess_getTrueType00
static int tolua_Gamelua_MsgRechargeSuccess_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgRechargeSuccess",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgRechargeSuccess* tolua_ret = (MsgRechargeSuccess*)  MsgRechargeSuccess::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgRechargeSuccess");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgRechargeSuccess */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgRechargeSuccess_new00
static int tolua_Gamelua_MsgRechargeSuccess_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgRechargeSuccess",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgRechargeSuccess* tolua_ret = (MsgRechargeSuccess*)  Mtolua_new((MsgRechargeSuccess)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgRechargeSuccess");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgRechargeSuccess */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgRechargeSuccess_new00_local
static int tolua_Gamelua_MsgRechargeSuccess_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgRechargeSuccess",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgRechargeSuccess* tolua_ret = (MsgRechargeSuccess*)  Mtolua_new((MsgRechargeSuccess)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgRechargeSuccess");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgRechargeSuccess */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgRechargeSuccess_getTypeId00
static int tolua_Gamelua_MsgRechargeSuccess_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgRechargeSuccess",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgRechargeSuccess* self = (const MsgRechargeSuccess*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgRechargeSuccess */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgRechargeSuccess_clone00
static int tolua_Gamelua_MsgRechargeSuccess_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgRechargeSuccess",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgRechargeSuccess* self = (const MsgRechargeSuccess*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgFightPageExit */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightPageExit_getTrueType00
static int tolua_Gamelua_MsgFightPageExit_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightPageExit",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgFightPageExit* tolua_ret = (MsgFightPageExit*)  MsgFightPageExit::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightPageExit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgFightPageExit */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightPageExit_new00
static int tolua_Gamelua_MsgFightPageExit_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightPageExit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightPageExit* tolua_ret = (MsgFightPageExit*)  Mtolua_new((MsgFightPageExit)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightPageExit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgFightPageExit */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightPageExit_new00_local
static int tolua_Gamelua_MsgFightPageExit_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgFightPageExit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgFightPageExit* tolua_ret = (MsgFightPageExit*)  Mtolua_new((MsgFightPageExit)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgFightPageExit");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgFightPageExit */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightPageExit_getTypeId00
static int tolua_Gamelua_MsgFightPageExit_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightPageExit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightPageExit* self = (const MsgFightPageExit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgFightPageExit */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgFightPageExit_clone00
static int tolua_Gamelua_MsgFightPageExit_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgFightPageExit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgFightPageExit* self = (const MsgFightPageExit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgTitleStatusChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgTitleStatusChange_getTrueType00
static int tolua_Gamelua_MsgTitleStatusChange_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgTitleStatusChange",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgTitleStatusChange* tolua_ret = (MsgTitleStatusChange*)  MsgTitleStatusChange::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgTitleStatusChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgTitleStatusChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgTitleStatusChange_new00
static int tolua_Gamelua_MsgTitleStatusChange_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgTitleStatusChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgTitleStatusChange* tolua_ret = (MsgTitleStatusChange*)  Mtolua_new((MsgTitleStatusChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgTitleStatusChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgTitleStatusChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgTitleStatusChange_new00_local
static int tolua_Gamelua_MsgTitleStatusChange_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgTitleStatusChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgTitleStatusChange* tolua_ret = (MsgTitleStatusChange*)  Mtolua_new((MsgTitleStatusChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgTitleStatusChange");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgTitleStatusChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgTitleStatusChange_getTypeId00
static int tolua_Gamelua_MsgTitleStatusChange_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgTitleStatusChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgTitleStatusChange* self = (const MsgTitleStatusChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgTitleStatusChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgTitleStatusChange_clone00
static int tolua_Gamelua_MsgTitleStatusChange_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgTitleStatusChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgTitleStatusChange* self = (const MsgTitleStatusChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageHasChange_getTrueType00
static int tolua_Gamelua_MsgAdventurePageHasChange_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageHasChange",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgAdventurePageHasChange* tolua_ret = (MsgAdventurePageHasChange*)  MsgAdventurePageHasChange::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageHasChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageHasChange_new00
static int tolua_Gamelua_MsgAdventurePageHasChange_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageHasChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventurePageHasChange* tolua_ret = (MsgAdventurePageHasChange*)  Mtolua_new((MsgAdventurePageHasChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageHasChange");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageHasChange_new00_local
static int tolua_Gamelua_MsgAdventurePageHasChange_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgAdventurePageHasChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgAdventurePageHasChange* tolua_ret = (MsgAdventurePageHasChange*)  Mtolua_new((MsgAdventurePageHasChange)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgAdventurePageHasChange");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: index of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_get_MsgAdventurePageHasChange_unsigned_index
static int tolua_get_MsgAdventurePageHasChange_unsigned_index(lua_State* tolua_S)
{
  MsgAdventurePageHasChange* self = (MsgAdventurePageHasChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: index of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_set_MsgAdventurePageHasChange_unsigned_index
static int tolua_set_MsgAdventurePageHasChange_unsigned_index(lua_State* tolua_S)
{
  MsgAdventurePageHasChange* self = (MsgAdventurePageHasChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->index = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageHasChange_getTypeId00
static int tolua_Gamelua_MsgAdventurePageHasChange_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventurePageHasChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventurePageHasChange* self = (const MsgAdventurePageHasChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgAdventurePageHasChange */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgAdventurePageHasChange_clone00
static int tolua_Gamelua_MsgAdventurePageHasChange_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgAdventurePageHasChange",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgAdventurePageHasChange* self = (const MsgAdventurePageHasChange*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTrueType of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgUpdateLeaguaContribution_getTrueType00
static int tolua_Gamelua_MsgUpdateLeaguaContribution_getTrueType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgUpdateLeaguaContribution",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* msg = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
  {
   MsgUpdateLeaguaContribution* tolua_ret = (MsgUpdateLeaguaContribution*)  MsgUpdateLeaguaContribution::getTrueType(msg);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgUpdateLeaguaContribution");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTrueType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgUpdateLeaguaContribution_new00
static int tolua_Gamelua_MsgUpdateLeaguaContribution_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgUpdateLeaguaContribution",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgUpdateLeaguaContribution* tolua_ret = (MsgUpdateLeaguaContribution*)  Mtolua_new((MsgUpdateLeaguaContribution)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgUpdateLeaguaContribution");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgUpdateLeaguaContribution_new00_local
static int tolua_Gamelua_MsgUpdateLeaguaContribution_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MsgUpdateLeaguaContribution",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MsgUpdateLeaguaContribution* tolua_ret = (MsgUpdateLeaguaContribution*)  Mtolua_new((MsgUpdateLeaguaContribution)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MsgUpdateLeaguaContribution");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: leftContribution of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_get_MsgUpdateLeaguaContribution_unsigned_leftContribution
static int tolua_get_MsgUpdateLeaguaContribution_unsigned_leftContribution(lua_State* tolua_S)
{
  MsgUpdateLeaguaContribution* self = (MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'leftContribution'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->leftContribution);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: leftContribution of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_set_MsgUpdateLeaguaContribution_unsigned_leftContribution
static int tolua_set_MsgUpdateLeaguaContribution_unsigned_leftContribution(lua_State* tolua_S)
{
  MsgUpdateLeaguaContribution* self = (MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'leftContribution'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->leftContribution = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: totalContribution of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_get_MsgUpdateLeaguaContribution_unsigned_totalContribution
static int tolua_get_MsgUpdateLeaguaContribution_unsigned_totalContribution(lua_State* tolua_S)
{
  MsgUpdateLeaguaContribution* self = (MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'totalContribution'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->totalContribution);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: totalContribution of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_set_MsgUpdateLeaguaContribution_unsigned_totalContribution
static int tolua_set_MsgUpdateLeaguaContribution_unsigned_totalContribution(lua_State* tolua_S)
{
  MsgUpdateLeaguaContribution* self = (MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'totalContribution'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->totalContribution = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgUpdateLeaguaContribution_getTypeId00
static int tolua_Gamelua_MsgUpdateLeaguaContribution_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgUpdateLeaguaContribution",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgUpdateLeaguaContribution* self = (const MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  MsgUpdateLeaguaContribution */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MsgUpdateLeaguaContribution_clone00
static int tolua_Gamelua_MsgUpdateLeaguaContribution_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MsgUpdateLeaguaContribution",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const MsgUpdateLeaguaContribution* self = (const MsgUpdateLeaguaContribution*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  GameMessage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameMessage_getTypeId00
static int tolua_Gamelua_GameMessage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameMessage* self = (GameMessage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameScriptMessage_new00
static int tolua_Gamelua_GameScriptMessage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameScriptMessage",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   GameScriptMessage* tolua_ret = (GameScriptMessage*)  Mtolua_new((GameScriptMessage)(type));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameScriptMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameScriptMessage_new00_local
static int tolua_Gamelua_GameScriptMessage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GameScriptMessage",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   GameScriptMessage* tolua_ret = (GameScriptMessage*)  Mtolua_new((GameScriptMessage)(type));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameScriptMessage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeId of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameScriptMessage_getTypeId00
static int tolua_Gamelua_GameScriptMessage_getTypeId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GameScriptMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GameScriptMessage* self = (const GameScriptMessage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getTypeId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GameScriptMessage_clone00
static int tolua_Gamelua_GameScriptMessage_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GameScriptMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GameScriptMessage* self = (const GameScriptMessage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'", NULL);
#endif
  {
   GameMessage* tolua_ret = (GameMessage*)  self->clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameMessage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mTypeId of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_get_GameScriptMessage_unsigned_mTypeId
static int tolua_get_GameScriptMessage_unsigned_mTypeId(lua_State* tolua_S)
{
  GameScriptMessage* self = (GameScriptMessage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mTypeId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mTypeId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mTypeId of class  GameScriptMessage */
#ifndef TOLUA_DISABLE_tolua_set_GameScriptMessage_unsigned_mTypeId
static int tolua_set_GameScriptMessage_unsigned_mTypeId(lua_State* tolua_S)
{
  GameScriptMessage* self = (GameScriptMessage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mTypeId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mTypeId = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_new00
static int tolua_Gamelua_MessageScriptHandler_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
  {
   MessageScriptHandler* tolua_ret = (MessageScriptHandler*)  Mtolua_new((MessageScriptHandler)(nHandler));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MessageScriptHandler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_new00_local
static int tolua_Gamelua_MessageScriptHandler_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LUA_FUNCTION nHandler = (  toluafix_ref_function(tolua_S,2,0));
  {
   MessageScriptHandler* tolua_ret = (MessageScriptHandler*)  Mtolua_new((MessageScriptHandler)(nHandler));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MessageScriptHandler");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_delete00
static int tolua_Gamelua_MessageScriptHandler_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageScriptHandler* self = (MessageScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerFunctionHandler of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_registerFunctionHandler00
static int tolua_Gamelua_MessageScriptHandler_registerFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageScriptHandler* self = (MessageScriptHandler*)  tolua_tousertype(tolua_S,1,0);
  int nHandler = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerFunctionHandler'", NULL);
#endif
  {
   self->registerFunctionHandler(nHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterFunctionHandler of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_unregisterFunctionHandler00
static int tolua_Gamelua_MessageScriptHandler_unregisterFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageScriptHandler* self = (MessageScriptHandler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterFunctionHandler'", NULL);
#endif
  {
   self->unregisterFunctionHandler();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onReceiveMassage of class  MessageScriptHandler */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageScriptHandler_onReceiveMassage00
static int tolua_Gamelua_MessageScriptHandler_onReceiveMassage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageScriptHandler",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageScriptHandler* self = (MessageScriptHandler*)  tolua_tousertype(tolua_S,1,0);
  GameMessage* message = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onReceiveMassage'", NULL);
#endif
  {
   self->onReceiveMassage(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onReceiveMassage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_getInstance00
static int tolua_Gamelua_MessageManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MessageManager* tolua_ret = (MessageManager*)  MessageManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MessageManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: update of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_update00
static int tolua_Gamelua_MessageManager_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageManager* self = (MessageManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendMessageForScript of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_sendMessageForScript00
static int tolua_Gamelua_MessageManager_sendMessageForScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GameMessage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageManager* self = (MessageManager*)  tolua_tousertype(tolua_S,1,0);
  GameMessage* message = ((GameMessage*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendMessageForScript'", NULL);
#endif
  {
   self->sendMessageForScript(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendMessageForScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: regisiterMessageHandler of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_regisiterMessageHandler00
static int tolua_Gamelua_MessageManager_regisiterMessageHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"MessageHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageManager* self = (MessageManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  MessageHandler* handler = ((MessageHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'regisiterMessageHandler'", NULL);
#endif
  {
   self->regisiterMessageHandler(id,handler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'regisiterMessageHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeMessageHandler of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_removeMessageHandler00
static int tolua_Gamelua_MessageManager_removeMessageHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"MessageHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MessageManager* self = (MessageManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  MessageHandler* messagehandler = ((MessageHandler*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeMessageHandler'", NULL);
#endif
  {
   self->removeMessageHandler(id,messagehandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeMessageHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeMessageHandler of class  MessageManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_MessageManager_removeMessageHandler01
static int tolua_Gamelua_MessageManager_removeMessageHandler01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MessageManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"MessageHandler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  MessageManager* self = (MessageManager*)  tolua_tousertype(tolua_S,1,0);
  MessageHandler* messagehandler = ((MessageHandler*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeMessageHandler'", NULL);
#endif
  {
   self->removeMessageHandler(messagehandler);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_MessageManager_removeMessageHandler00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_getInstance00
static int tolua_Gamelua_libOS_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   libOS* tolua_ret = (libOS*)  libOS::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"libOS");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: avalibleMemory of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_avalibleMemory00
static int tolua_Gamelua_libOS_avalibleMemory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'avalibleMemory'", NULL);
#endif
  {
   long tolua_ret = (long)  self->avalibleMemory();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'avalibleMemory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rmdir of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_rmdir00
static int tolua_Gamelua_libOS_rmdir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rmdir'", NULL);
#endif
  {
   self->rmdir(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rmdir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: generateSerial of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_generateSerial00
static int tolua_Gamelua_libOS_generateSerial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'generateSerial'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->generateSerial();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'generateSerial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showInputbox of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_showInputbox00
static int tolua_Gamelua_libOS_showInputbox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  bool multiline = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showInputbox'", NULL);
#endif
  {
   self->showInputbox(multiline);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showInputbox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showMessagebox of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_showMessagebox00
static int tolua_Gamelua_libOS_showMessagebox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string msg = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int tag = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showMessagebox'", NULL);
#endif
  {
   self->showMessagebox(msg,tag);
   tolua_pushcppstring(tolua_S,(const char*)msg);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showMessagebox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openURL of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_openURL00
static int tolua_Gamelua_libOS_openURL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string url = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openURL'", NULL);
#endif
  {
   self->openURL(url);
   tolua_pushcppstring(tolua_S,(const char*)url);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openURL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: emailTo of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_emailTo00
static int tolua_Gamelua_libOS_emailTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string mailto = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string cc = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string title = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  const std::string body = ((const std::string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'emailTo'", NULL);
#endif
  {
   self->emailTo(mailto,cc,title,body);
   tolua_pushcppstring(tolua_S,(const char*)mailto);
   tolua_pushcppstring(tolua_S,(const char*)cc);
   tolua_pushcppstring(tolua_S,(const char*)title);
   tolua_pushcppstring(tolua_S,(const char*)body);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'emailTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWaiting of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_setWaiting00
static int tolua_Gamelua_libOS_setWaiting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  bool tolua_var_5 = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWaiting'", NULL);
#endif
  {
   self->setWaiting(tolua_var_5);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWaiting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFreeSpace of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_getFreeSpace00
static int tolua_Gamelua_libOS_getFreeSpace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFreeSpace'", NULL);
#endif
  {
   long long tolua_ret = (long long)  self->getFreeSpace();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFreeSpace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNetWork of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_getNetWork00
static int tolua_Gamelua_libOS_getNetWork00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNetWork'", NULL);
#endif
  {
   NetworkStatus tolua_ret = (NetworkStatus)  self->getNetWork();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((NetworkStatus)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"NetworkStatus");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(NetworkStatus));
     tolua_pushusertype(tolua_S,tolua_obj,"NetworkStatus");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNetWork'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearNotification of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_clearNotification00
static int tolua_Gamelua_libOS_clearNotification00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearNotification'", NULL);
#endif
  {
   self->clearNotification();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearNotification'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addNotification of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_addNotification00
static int tolua_Gamelua_libOS_addNotification00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string msg = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int secondsdelay = ((int)  tolua_tonumber(tolua_S,3,0));
  bool daily = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addNotification'", NULL);
#endif
  {
   self->addNotification(msg,secondsdelay,daily);
   tolua_pushcppstring(tolua_S,(const char*)msg);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addNotification'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDeviceID of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_getDeviceID00
static int tolua_Gamelua_libOS_getDeviceID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDeviceID'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getDeviceID();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDeviceID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlatformInfo of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_getPlatformInfo00
static int tolua_Gamelua_libOS_getPlatformInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlatformInfo'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getPlatformInfo();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlatformInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: analyticsLogEvent of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_analyticsLogEvent00
static int tolua_Gamelua_libOS_analyticsLogEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string event = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'analyticsLogEvent'", NULL);
#endif
  {
   self->analyticsLogEvent(event);
   tolua_pushcppstring(tolua_S,(const char*)event);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'analyticsLogEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: analyticsLogEvent of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_analyticsLogEvent01
static int tolua_Gamelua_libOS_analyticsLogEvent01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<std::string,std::string>",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string event = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<std::string,std::string>* dictionary = ((const std::map<std::string,std::string>*)  tolua_tousertype(tolua_S,3,0));
  bool timed = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'analyticsLogEvent'", NULL);
#endif
  {
   self->analyticsLogEvent(event,*dictionary,timed);
   tolua_pushcppstring(tolua_S,(const char*)event);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Gamelua_libOS_analyticsLogEvent00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: analyticsLogEndTimeEvent of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_analyticsLogEndTimeEvent00
static int tolua_Gamelua_libOS_analyticsLogEndTimeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string event = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'analyticsLogEndTimeEvent'", NULL);
#endif
  {
   self->analyticsLogEndTimeEvent(event);
   tolua_pushcppstring(tolua_S,(const char*)event);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'analyticsLogEndTimeEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WeChatInit of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_WeChatInit00
static int tolua_Gamelua_libOS_WeChatInit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
  const std::string appID = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WeChatInit'", NULL);
#endif
  {
   self->WeChatInit(appID);
   tolua_pushcppstring(tolua_S,(const char*)appID);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WeChatInit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WeChatIsInstalled of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_WeChatIsInstalled00
static int tolua_Gamelua_libOS_WeChatIsInstalled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WeChatIsInstalled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->WeChatIsInstalled();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WeChatIsInstalled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WeChatInstall of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_WeChatInstall00
static int tolua_Gamelua_libOS_WeChatInstall00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WeChatInstall'", NULL);
#endif
  {
   self->WeChatInstall();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WeChatInstall'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WeChatOpen of class  libOS */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libOS_WeChatOpen00
static int tolua_Gamelua_libOS_WeChatOpen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libOS",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libOS* self = (libOS*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WeChatOpen'", NULL);
#endif
  {
   self->WeChatOpen();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WeChatOpen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cooOrderSerial of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_cooOrderSerial
static int tolua_get_BUYINFO_cooOrderSerial(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cooOrderSerial'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->cooOrderSerial);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cooOrderSerial of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_cooOrderSerial
static int tolua_set_BUYINFO_cooOrderSerial(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cooOrderSerial'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cooOrderSerial = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: productId of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_productId
static int tolua_get_BUYINFO_productId(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productId'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->productId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: productId of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_productId
static int tolua_set_BUYINFO_productId(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productId'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->productId = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: productName of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_productName
static int tolua_get_BUYINFO_productName(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->productName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: productName of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_productName
static int tolua_set_BUYINFO_productName(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->productName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: productPrice of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_productPrice
static int tolua_get_BUYINFO_productPrice(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productPrice'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->productPrice);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: productPrice of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_productPrice
static int tolua_set_BUYINFO_productPrice(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productPrice'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->productPrice = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: productOrignalPrice of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_productOrignalPrice
static int tolua_get_BUYINFO_productOrignalPrice(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productOrignalPrice'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->productOrignalPrice);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: productOrignalPrice of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_productOrignalPrice
static int tolua_set_BUYINFO_productOrignalPrice(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productOrignalPrice'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->productOrignalPrice = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: productCount of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_unsigned_productCount
static int tolua_get_BUYINFO_unsigned_productCount(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productCount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->productCount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: productCount of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_unsigned_productCount
static int tolua_set_BUYINFO_unsigned_productCount(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'productCount'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->productCount = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: description of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_get_BUYINFO_description
static int tolua_get_BUYINFO_description(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'description'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->description);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: description of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_set_BUYINFO_description
static int tolua_set_BUYINFO_description(lua_State* tolua_S)
{
  BUYINFO* self = (BUYINFO*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'description'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->description = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BUYINFO_new00
static int tolua_Gamelua_BUYINFO_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BUYINFO",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BUYINFO* tolua_ret = (BUYINFO*)  Mtolua_new((BUYINFO)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BUYINFO");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BUYINFO */
#ifndef TOLUA_DISABLE_tolua_Gamelua_BUYINFO_new00_local
static int tolua_Gamelua_BUYINFO_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BUYINFO",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BUYINFO* tolua_ret = (BUYINFO*)  Mtolua_new((BUYINFO)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BUYINFO");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openBBS of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_openBBS00
static int tolua_Gamelua_libPlatform_openBBS00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openBBS'", NULL);
#endif
  {
   self->openBBS();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openBBS'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: userFeedBack of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_userFeedBack00
static int tolua_Gamelua_libPlatform_userFeedBack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'userFeedBack'", NULL);
#endif
  {
   self->userFeedBack();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'userFeedBack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: gamePause of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_gamePause00
static int tolua_Gamelua_libPlatform_gamePause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'gamePause'", NULL);
#endif
  {
   self->gamePause();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gamePause'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onShare of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_onShare00
static int tolua_Gamelua_libPlatform_onShare00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
  std::string tittle = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string content = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string path = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onShare'", NULL);
#endif
  {
   self->onShare(tittle,content,path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onShare'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onBinding of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_onBinding00
static int tolua_Gamelua_libPlatform_onBinding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onBinding'", NULL);
#endif
  {
   self->onBinding();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onBinding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onGuest of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_onGuest00
static int tolua_Gamelua_libPlatform_onGuest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onGuest'", NULL);
#endif
  {
   self->onGuest();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onGuest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLogined of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_getLogined00
static int tolua_Gamelua_libPlatform_getLogined00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLogined'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getLogined();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLogined'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loginUin of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_loginUin00
static int tolua_Gamelua_libPlatform_loginUin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loginUin'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->loginUin();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loginUin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sessionID of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_sessionID00
static int tolua_Gamelua_libPlatform_sessionID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sessionID'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->sessionID();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sessionID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: nickName of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_nickName00
static int tolua_Gamelua_libPlatform_nickName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'nickName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->nickName();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'nickName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlatformInfo of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_getPlatformInfo00
static int tolua_Gamelua_libPlatform_getPlatformInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlatformInfo'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getPlatformInfo();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlatformInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClientChannel of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_getClientChannel00
static int tolua_Gamelua_libPlatform_getClientChannel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClientChannel'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getClientChannel();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClientChannel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlatformId of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_getPlatformId00
static int tolua_Gamelua_libPlatform_getPlatformId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlatformId'", NULL);
#endif
  {
   unsigned const int tolua_ret = ( unsigned const int)  self->getPlatformId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlatformId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buyGoods of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_buyGoods00
static int tolua_Gamelua_libPlatform_buyGoods00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"BUYINFO",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
  BUYINFO* tolua_var_6 = ((BUYINFO*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buyGoods'", NULL);
#endif
  {
   self->buyGoods(*tolua_var_6);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buyGoods'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: switchUsers of class  libPlatform */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatform_switchUsers00
static int tolua_Gamelua_libPlatform_switchUsers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatform",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatform* self = (libPlatform*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'switchUsers'", NULL);
#endif
  {
   self->switchUsers();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'switchUsers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlatform of class  libPlatformManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatformManager_setPlatform00
static int tolua_Gamelua_libPlatformManager_setPlatform00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"libPlatformManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  libPlatformManager* self = (libPlatformManager*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlatform'", NULL);
#endif
  {
   self->setPlatform(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPlatform'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlatform of class  libPlatformManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatformManager_getPlatform00
static int tolua_Gamelua_libPlatformManager_getPlatform00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"libPlatformManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   libPlatform* tolua_ret = (libPlatform*)  libPlatformManager::getPlatform();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"libPlatform");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlatform'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  libPlatformManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_libPlatformManager_getInstance00
static int tolua_Gamelua_libPlatformManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"libPlatformManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   libPlatformManager* tolua_ret = (libPlatformManager*)  libPlatformManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"libPlatformManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  RestrictedWord */
#ifndef TOLUA_DISABLE_tolua_Gamelua_RestrictedWord_getInstance00
static int tolua_Gamelua_RestrictedWord_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RestrictedWord",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RestrictedWord* tolua_ret = (RestrictedWord*)  RestrictedWord::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RestrictedWord");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isStringOK of class  RestrictedWord */
#ifndef TOLUA_DISABLE_tolua_Gamelua_RestrictedWord_isStringOK00
static int tolua_Gamelua_RestrictedWord_isStringOK00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RestrictedWord",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RestrictedWord* self = (RestrictedWord*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isStringOK'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isStringOK(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isStringOK'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSecretOK of class  RestrictedWord */
#ifndef TOLUA_DISABLE_tolua_Gamelua_RestrictedWord_isSecretOK00
static int tolua_Gamelua_RestrictedWord_isSecretOK00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RestrictedWord",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RestrictedWord* self = (RestrictedWord*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSecretOK'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isSecretOK(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSecretOK'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isChatWorldStringOK of class  RestrictedWord */
#ifndef TOLUA_DISABLE_tolua_Gamelua_RestrictedWord_isChatWorldStringOK00
static int tolua_Gamelua_RestrictedWord_isChatWorldStringOK00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RestrictedWord",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RestrictedWord* self = (RestrictedWord*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isChatWorldStringOK'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isChatWorldStringOK(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isChatWorldStringOK'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: filterWordSentence of class  RestrictedWord */
#ifndef TOLUA_DISABLE_tolua_Gamelua_RestrictedWord_filterWordSentence00
static int tolua_Gamelua_RestrictedWord_filterWordSentence00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RestrictedWord",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RestrictedWord* self = (RestrictedWord*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'filterWordSentence'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->filterWordSentence(str);
   tolua_pushcpplstring(tolua_S,tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'filterWordSentence'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getInstance00
static int tolua_Gamelua_GamePrecedure_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GamePrecedure* tolua_ret = (GamePrecedure*)  GamePrecedure::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"GamePrecedure");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_init00
static int tolua_Gamelua_GamePrecedure_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   self->init();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unInit of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_unInit00
static int tolua_Gamelua_GamePrecedure_unInit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unInit'", NULL);
#endif
  {
   self->unInit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unInit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enterLoading of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_enterLoading00
static int tolua_Gamelua_GamePrecedure_enterLoading00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enterLoading'", NULL);
#endif
  {
   self->enterLoading();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enterLoading'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reEnterLoading of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_reEnterLoading00
static int tolua_Gamelua_GamePrecedure_reEnterLoading00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reEnterLoading'", NULL);
#endif
  {
   self->reEnterLoading();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reEnterLoading'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: preEnterMainMenu of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_preEnterMainMenu00
static int tolua_Gamelua_GamePrecedure_preEnterMainMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'preEnterMainMenu'", NULL);
#endif
  {
   self->preEnterMainMenu();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'preEnterMainMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readTables of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_readTables00
static int tolua_Gamelua_GamePrecedure_readTables00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readTables'", NULL);
#endif
  {
   self->readTables();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readTables'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enterMainMenu of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_enterMainMenu00
static int tolua_Gamelua_GamePrecedure_enterMainMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enterMainMenu'", NULL);
#endif
  {
   self->enterMainMenu();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enterMainMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enterGame of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_enterGame00
static int tolua_Gamelua_GamePrecedure_enterGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enterGame'", NULL);
#endif
  {
   self->enterGame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enterGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enterInstruction of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_enterInstruction00
static int tolua_Gamelua_GamePrecedure_enterInstruction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enterInstruction'", NULL);
#endif
  {
   self->enterInstruction(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enterInstruction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exitGame of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_exitGame00
static int tolua_Gamelua_GamePrecedure_exitGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exitGame'", NULL);
#endif
  {
   self->exitGame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exitGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInLoadingSceneAndNeedExit of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_isInLoadingSceneAndNeedExit00
static int tolua_Gamelua_GamePrecedure_isInLoadingSceneAndNeedExit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInLoadingSceneAndNeedExit'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInLoadingSceneAndNeedExit();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInLoadingSceneAndNeedExit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInLoadingScene of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_isInLoadingScene00
static int tolua_Gamelua_GamePrecedure_isInLoadingScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInLoadingScene'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInLoadingScene();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInLoadingScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFrameTime of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getFrameTime00
static int tolua_Gamelua_GamePrecedure_getFrameTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFrameTime'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getFrameTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTotalTime of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getTotalTime00
static int tolua_Gamelua_GamePrecedure_getTotalTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTotalTime'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getTotalTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTotalTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setServerID of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_setServerID00
static int tolua_Gamelua_GamePrecedure_setServerID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
  int serverID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setServerID'", NULL);
#endif
  {
   self->setServerID(serverID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setServerID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerID of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getServerID00
static int tolua_Gamelua_GamePrecedure_getServerID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerID'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getServerID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerTime of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getServerTime00
static int tolua_Gamelua_GamePrecedure_getServerTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerTime'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getServerTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setServerTime of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_setServerTime00
static int tolua_Gamelua_GamePrecedure_setServerTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
  int serverTime = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setServerTime'", NULL);
#endif
  {
   self->setServerTime(serverTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setServerTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUin of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_setUin00
static int tolua_Gamelua_GamePrecedure_setUin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
  const std::string uin = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setUin'", NULL);
#endif
  {
   self->setUin(uin);
   tolua_pushcppstring(tolua_S,(const char*)uin);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUin of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_getUin00
static int tolua_Gamelua_GamePrecedure_getUin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUin'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getUin();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasAlertServerUpdating of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_hasAlertServerUpdating00
static int tolua_Gamelua_GamePrecedure_hasAlertServerUpdating00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAlertServerUpdating'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAlertServerUpdating();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasAlertServerUpdating'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAlertServerUpdating of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_setAlertServerUpdating00
static int tolua_Gamelua_GamePrecedure_setAlertServerUpdating00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
  bool hasAlert = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlertServerUpdating'", NULL);
#endif
  {
   self->setAlertServerUpdating(hasAlert);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlertServerUpdating'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: gameSnapshot of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_gameSnapshot00
static int tolua_Gamelua_GamePrecedure_gameSnapshot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'gameSnapshot'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->gameSnapshot();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gameSnapshot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showBulletin of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_showBulletin00
static int tolua_Gamelua_GamePrecedure_showBulletin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showBulletin'", NULL);
#endif
  {
   self->showBulletin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showBulletin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeBulletin of class  GamePrecedure */
#ifndef TOLUA_DISABLE_tolua_Gamelua_GamePrecedure_closeBulletin00
static int tolua_Gamelua_GamePrecedure_closeBulletin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GamePrecedure",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GamePrecedure* self = (GamePrecedure*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeBulletin'", NULL);
#endif
  {
   self->closeBulletin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeBulletin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CShiftMaskToXAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftMaskToXAction_create00
static int tolua_Gamelua_CShiftMaskToXAction_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CShiftMaskToXAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftMaskToXAction* tolua_ret = (CShiftMaskToXAction*)  CShiftMaskToXAction::create(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftMaskToXAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithDuration of class  CShiftMaskToXAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftMaskToXAction_initWithDuration00
static int tolua_Gamelua_CShiftMaskToXAction_initWithDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CShiftMaskToXAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CShiftMaskToXAction* self = (CShiftMaskToXAction*)  tolua_tousertype(tolua_S,1,0);
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithDuration'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithDuration(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CShiftObjectToXAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftObjectToXAction_create00
static int tolua_Gamelua_CShiftObjectToXAction_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CShiftObjectToXAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftObjectToXAction* tolua_ret = (CShiftObjectToXAction*)  CShiftObjectToXAction::create(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftObjectToXAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithDuration of class  CShiftObjectToXAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftObjectToXAction_initWithDuration00
static int tolua_Gamelua_CShiftObjectToXAction_initWithDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CShiftObjectToXAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CShiftObjectToXAction* self = (CShiftObjectToXAction*)  tolua_tousertype(tolua_S,1,0);
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithDuration'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithDuration(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CShiftMaskToYAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftMaskToYAction_create00
static int tolua_Gamelua_CShiftMaskToYAction_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CShiftMaskToYAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftMaskToYAction* tolua_ret = (CShiftMaskToYAction*)  CShiftMaskToYAction::create(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftMaskToYAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithDuration of class  CShiftMaskToYAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftMaskToYAction_initWithDuration00
static int tolua_Gamelua_CShiftMaskToYAction_initWithDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CShiftMaskToYAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CShiftMaskToYAction* self = (CShiftMaskToYAction*)  tolua_tousertype(tolua_S,1,0);
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithDuration'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithDuration(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  CShiftObjectToYAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftObjectToYAction_create00
static int tolua_Gamelua_CShiftObjectToYAction_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CShiftObjectToYAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftObjectToYAction* tolua_ret = (CShiftObjectToYAction*)  CShiftObjectToYAction::create(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftObjectToYAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithDuration of class  CShiftObjectToYAction */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CShiftObjectToYAction_initWithDuration00
static int tolua_Gamelua_CShiftObjectToYAction_initWithDuration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CShiftObjectToYAction",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CShiftObjectToYAction* self = (CShiftObjectToYAction*)  tolua_tousertype(tolua_S,1,0);
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  const CCPoint* startPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* destPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  const CCRect oriRec = *((const CCRect*)  tolua_tousertype(tolua_S,5,0));
  int fCutPos = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithDuration'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithDuration(fDuration,*startPosition,*destPosition,oriRec,fCutPos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithDuration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpSetObjectXPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpSetObjectXPos00
static int tolua_Gamelua_ShiftToManager_ExpSetObjectXPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* object = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  float fCurrentPercent = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  ShiftToManager::ExpSetObjectXPos(object,oriRect,fCurrentPercent);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpSetObjectXPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpShiftObjectToXPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpShiftObjectToXPos00
static int tolua_Gamelua_ShiftToManager_ExpShiftObjectToXPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"CCSprite",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  CCSprite* mask = ((CCSprite*)  tolua_tousertype(tolua_S,4,0));
  float fFromPercent = ((float)  tolua_tonumber(tolua_S,5,0));
  float fToPercent = ((float)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftObjectToXAction* tolua_ret = (CShiftObjectToXAction*)  ShiftToManager::ExpShiftObjectToXPos(fDuration,oriRect,mask,fFromPercent,fToPercent);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftObjectToXAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpShiftObjectToXPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpSetMaskYPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpSetMaskYPos00
static int tolua_Gamelua_ShiftToManager_ExpSetMaskYPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* object = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* oriPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  float fCurrentPercent = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  ShiftToManager::ExpSetMaskYPos(object,oriRect,*oriPosition,fCurrentPercent);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpSetMaskYPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpShiftMaskToYPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpShiftMaskToYPos00
static int tolua_Gamelua_ShiftToManager_ExpShiftMaskToYPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCPoint",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  const CCPoint* oriPosition = ((const CCPoint*)  tolua_tousertype(tolua_S,4,0));
  float fFromPercent = ((float)  tolua_tonumber(tolua_S,5,0));
  float fToPercent = ((float)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftMaskToYAction* tolua_ret = (CShiftMaskToYAction*)  ShiftToManager::ExpShiftMaskToYPos(fDuration,oriRect,*oriPosition,fFromPercent,fToPercent);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftMaskToYAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpShiftMaskToYPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpSetObjectYPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpSetObjectYPos00
static int tolua_Gamelua_ShiftToManager_ExpSetObjectYPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCSprite",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCSprite* object = ((CCSprite*)  tolua_tousertype(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  float fCurrentPercent = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  ShiftToManager::ExpSetObjectYPos(object,oriRect,fCurrentPercent);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpSetObjectYPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExpShiftObjectToYPos of class  ShiftToManager */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ShiftToManager_ExpShiftObjectToYPos00
static int tolua_Gamelua_ShiftToManager_ExpShiftObjectToYPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ShiftToManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCRect",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"CCSprite",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fDuration = ((float)  tolua_tonumber(tolua_S,2,0));
  CCRect oriRect = *((CCRect*)  tolua_tousertype(tolua_S,3,0));
  CCSprite* mask = ((CCSprite*)  tolua_tousertype(tolua_S,4,0));
  float fFromPercent = ((float)  tolua_tonumber(tolua_S,5,0));
  float fToPercent = ((float)  tolua_tonumber(tolua_S,6,0));
  {
   CShiftObjectToYAction* tolua_ret = (CShiftObjectToYAction*)  ShiftToManager::ExpShiftObjectToYPos(fDuration,oriRect,mask,fFromPercent,fToPercent);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CShiftObjectToYAction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExpShiftObjectToYPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fadeInAndOut of class  AnimMgr */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AnimMgr_fadeInAndOut00
static int tolua_Gamelua_AnimMgr_fadeInAndOut00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AnimMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AnimMgr* self = (AnimMgr*)  tolua_tousertype(tolua_S,1,0);
  CCNode* node = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
  float time = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fadeInAndOut'", NULL);
#endif
  {
   self->fadeInAndOut(node,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fadeInAndOut'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: scrollViewMoveIn of class  AnimMgr */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AnimMgr_scrollViewMoveIn00
static int tolua_Gamelua_AnimMgr_scrollViewMoveIn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AnimMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCScrollView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AnimMgr* self = (AnimMgr*)  tolua_tousertype(tolua_S,1,0);
  CCScrollView* scrollView = ((CCScrollView*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'scrollViewMoveIn'", NULL);
#endif
  {
   self->scrollViewMoveIn(scrollView);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'scrollViewMoveIn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  AnimMgr */
#ifndef TOLUA_DISABLE_tolua_Gamelua_AnimMgr_getInstance00
static int tolua_Gamelua_AnimMgr_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"AnimMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AnimMgr* tolua_ret = (AnimMgr*)  AnimMgr::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"AnimMgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_new00
static int tolua_Gamelua_CCBFileNew_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCBFileNew* tolua_ret = (CCBFileNew*)  Mtolua_new((CCBFileNew)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBFileNew");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_new00_local
static int tolua_Gamelua_CCBFileNew_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CCBFileNew* tolua_ret = (CCBFileNew*)  Mtolua_new((CCBFileNew)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBFileNew");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_delete00
static int tolua_Gamelua_CCBFileNew_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_init00
static int tolua_Gamelua_CCBFileNew_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCCBFileName of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_setCCBFileName00
static int tolua_Gamelua_CCBFileNew_setCCBFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCCBFileName'", NULL);
#endif
  {
   self->setCCBFileName(filename);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCCBFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCCBFileName of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getCCBFileName00
static int tolua_Gamelua_CCBFileNew_getCCBFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCCBFileName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getCCBFileName();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCCBFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_load00
static int tolua_Gamelua_CCBFileNew_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  bool froceLoad = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'", NULL);
#endif
  {
   self->load(froceLoad);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unload of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_unload00
static int tolua_Gamelua_CCBFileNew_unload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unload'", NULL);
#endif
  {
   self->unload();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unload'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLoaded of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getLoaded00
static int tolua_Gamelua_CCBFileNew_getLoaded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLoaded'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getLoaded();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLoaded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setParentCCBFileNode of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_setParentCCBFileNode00
static int tolua_Gamelua_CCBFileNew_setParentCCBFileNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  CCBFileNew* parent = ((CCBFileNew*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParentCCBFileNode'", NULL);
#endif
  {
   self->setParentCCBFileNode(parent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setParentCCBFileNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParentCCBFileNode of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getParentCCBFileNode00
static int tolua_Gamelua_CCBFileNew_getParentCCBFileNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParentCCBFileNode'", NULL);
#endif
  {
   CCBFileNew* tolua_ret = (CCBFileNew*)  self->getParentCCBFileNode();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBFileNew");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParentCCBFileNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setListener of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_setListener00
static int tolua_Gamelua_CCBFileNew_setListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCBFileListener",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  CCBFileListener* listener = ((CCBFileListener*)  tolua_tousertype(tolua_S,2,0));
  int tag = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setListener'", NULL);
#endif
  {
   self->setListener(listener,tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getListener of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getListener00
static int tolua_Gamelua_CCBFileNew_getListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getListener'", NULL);
#endif
  {
   CCBFileListener* tolua_ret = (CCBFileListener*)  self->getListener();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCBFileListener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runAnimation of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_runAnimation00
static int tolua_Gamelua_CCBFileNew_runAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  const std::string actionname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool hasEffect = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runAnimation'", NULL);
#endif
  {
   self->runAnimation(actionname,hasEffect);
   tolua_pushcppstring(tolua_S,(const char*)actionname);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasAnimation of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_hasAnimation00
static int tolua_Gamelua_CCBFileNew_hasAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  const std::string animation = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAnimation'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAnimation(animation);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)animation);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAnimationLength of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getAnimationLength00
static int tolua_Gamelua_CCBFileNew_getAnimationLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  const std::string animation = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAnimationLength'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getAnimationLength(animation);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)animation);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAnimationLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCompletedAnimationName of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getCompletedAnimationName00
static int tolua_Gamelua_CCBFileNew_getCompletedAnimationName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCompletedAnimationName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getCompletedAnimationName();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCompletedAnimationName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVariable of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getVariable00
static int tolua_Gamelua_CCBFileNew_getVariable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  const std::string variablename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVariable'", NULL);
#endif
  {
   CCObject* tolua_ret = (CCObject*)  self->getVariable(variablename);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCObject");
   tolua_pushcppstring(tolua_S,(const char*)variablename);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVariable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCCBFileNode of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_getCCBFileNode00
static int tolua_Gamelua_CCBFileNew_getCCBFileNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCCBFileNode'", NULL);
#endif
  {
   CCNode* tolua_ret = (CCNode*)  self->getCCBFileNode();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCCBFileNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playAutoPlaySequence of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_playAutoPlaySequence00
static int tolua_Gamelua_CCBFileNew_playAutoPlaySequence00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playAutoPlaySequence'", NULL);
#endif
  {
   self->playAutoPlaySequence();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playAutoPlaySequence'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerFunctionHandler of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_registerFunctionHandler00
static int tolua_Gamelua_CCBFileNew_registerFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
  int nHandler = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerFunctionHandler'", NULL);
#endif
  {
   self->registerFunctionHandler(nHandler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unregisterFunctionHandler of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_unregisterFunctionHandler00
static int tolua_Gamelua_CCBFileNew_unregisterFunctionHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unregisterFunctionHandler'", NULL);
#endif
  {
   self->unregisterFunctionHandler();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unregisterFunctionHandler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dumpInfo of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_dumpInfo00
static int tolua_Gamelua_CCBFileNew_dumpInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dumpInfo'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->dumpInfo();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dumpInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cleanup of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CCBFileNew_cleanup00
static int tolua_Gamelua_CCBFileNew_cleanup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCBFileNew",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cleanup'", NULL);
#endif
  {
   self->cleanup();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cleanup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __CCBSelectorResolver__ of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_get_CCBFileNew___CCBSelectorResolver__
static int tolua_get_CCBFileNew___CCBSelectorResolver__(lua_State* tolua_S)
{
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__CCBSelectorResolver__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<CCBSelectorResolver*>(self), "CCBSelectorResolver");
#else
   tolua_pushusertype(tolua_S,(void*)((CCBSelectorResolver*)self), "CCBSelectorResolver");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __CCBMemberVariableAssigner__ of class  CCBFileNew */
#ifndef TOLUA_DISABLE_tolua_get_CCBFileNew___CCBMemberVariableAssigner__
static int tolua_get_CCBFileNew___CCBMemberVariableAssigner__(lua_State* tolua_S)
{
  CCBFileNew* self = (CCBFileNew*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__CCBMemberVariableAssigner__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<CCBMemberVariableAssigner*>(self), "CCBMemberVariableAssigner");
#else
   tolua_pushusertype(tolua_S,(void*)((CCBMemberVariableAssigner*)self), "CCBMemberVariableAssigner");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_new00
static int tolua_Gamelua_CoverSprite_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CoverSprite* tolua_ret = (CoverSprite*)  Mtolua_new((CoverSprite)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CoverSprite");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_new00_local
static int tolua_Gamelua_CoverSprite_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CoverSprite* tolua_ret = (CoverSprite*)  Mtolua_new((CoverSprite)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CoverSprite");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_delete00
static int tolua_Gamelua_CoverSprite_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CoverSprite* self = (CoverSprite*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initWithTexture of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_initWithTexture00
static int tolua_Gamelua_CoverSprite_initWithTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCTexture2D",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCTexture2D",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const CCRect",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CoverSprite* self = (CoverSprite*)  tolua_tousertype(tolua_S,1,0);
  CCTexture2D* texture = ((CCTexture2D*)  tolua_tousertype(tolua_S,2,0));
  CCTexture2D* targetTex = ((CCTexture2D*)  tolua_tousertype(tolua_S,3,0));
  const CCRect* rect = ((const CCRect*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initWithTexture'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initWithTexture(texture,targetTex,*rect);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initWithTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: draw of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_draw00
static int tolua_Gamelua_CoverSprite_draw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CoverSprite* self = (CoverSprite*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'draw'", NULL);
#endif
  {
   self->draw();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initProgram of class  CoverSprite */
#ifndef TOLUA_DISABLE_tolua_Gamelua_CoverSprite_initProgram00
static int tolua_Gamelua_CoverSprite_initProgram00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CoverSprite",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CoverSprite* self = (CoverSprite*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initProgram'", NULL);
#endif
  {
   self->initProgram();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initProgram'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  SpineContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SpineContainer_create00
static int tolua_Gamelua_SpineContainer_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SpineContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  float scale = ((float)  tolua_tonumber(tolua_S,4,1.0f));
  {
   SpineContainer* tolua_ret = (SpineContainer*)  SpineContainer::create(path,name,scale);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SpineContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runAnimation of class  SpineContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SpineContainer_runAnimation00
static int tolua_Gamelua_SpineContainer_runAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpineContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpineContainer* self = (SpineContainer*)  tolua_tousertype(tolua_S,1,0);
  int trackIndex = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  int loopTimes = ((int)  tolua_tonumber(tolua_S,4,1));
  float delay = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runAnimation'", NULL);
#endif
  {
   self->runAnimation(trackIndex,name,loopTimes,delay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setListener of class  SpineContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SpineContainer_setListener00
static int tolua_Gamelua_SpineContainer_setListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpineContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SpineEventListener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpineContainer* self = (SpineContainer*)  tolua_tousertype(tolua_S,1,0);
  SpineEventListener* eventListener = ((SpineEventListener*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setListener'", NULL);
#endif
  {
   self->setListener(eventListener);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopAllAnimations of class  SpineContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SpineContainer_stopAllAnimations00
static int tolua_Gamelua_SpineContainer_stopAllAnimations00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpineContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpineContainer* self = (SpineContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopAllAnimations'", NULL);
#endif
  {
   self->stopAllAnimations();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopAllAnimations'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopAnimationByIndex of class  SpineContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_SpineContainer_stopAnimationByIndex00
static int tolua_Gamelua_SpineContainer_stopAnimationByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SpineContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SpineContainer* self = (SpineContainer*)  tolua_tousertype(tolua_S,1,0);
  int trackIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopAnimationByIndex'", NULL);
#endif
  {
   self->stopAnimationByIndex(trackIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopAnimationByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_create00
static int tolua_Gamelua_ArmatureContainer_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CCNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  CCNode* parent = ((CCNode*)  tolua_tousertype(tolua_S,4,NULL));
  {
   ArmatureContainer* tolua_ret = (ArmatureContainer*)  ArmatureContainer::create(path,name,parent);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ArmatureContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runAnimation of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_runAnimation00
static int tolua_Gamelua_ArmatureContainer_runAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int loopTimes = ((unsigned int)  tolua_tonumber(tolua_S,3,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runAnimation'", NULL);
#endif
  {
   self->runAnimation(name,loopTimes);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeSkin of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_changeSkin00
static int tolua_Gamelua_ArmatureContainer_changeSkin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* boneName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* skinName = ((const char*)  tolua_tostring(tolua_S,3,0));
  bool force = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeSkin'", NULL);
#endif
  {
   self->changeSkin(boneName,skinName,force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeSkin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeSkin of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_changeSkin01
static int tolua_Gamelua_ArmatureContainer_changeSkin01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CCLabelTTF",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* boneName = ((const char*)  tolua_tostring(tolua_S,2,0));
  CCLabelTTF* label = ((CCLabelTTF*)  tolua_tousertype(tolua_S,3,0));
  bool force = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeSkin'", NULL);
#endif
  {
   self->changeSkin(boneName,label,force);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Gamelua_ArmatureContainer_changeSkin00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setListener of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_setListener00
static int tolua_Gamelua_ArmatureContainer_setListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ArmatureEventListener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  ArmatureEventListener* eventListener = ((ArmatureEventListener*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setListener'", NULL);
#endif
  {
   self->setListener(eventListener);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setResourcePath of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_setResourcePath00
static int tolua_Gamelua_ArmatureContainer_setResourcePath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* resourcePath = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setResourcePath'", NULL);
#endif
  {
   self->setResourcePath(resourcePath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setResourcePath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAnimationTime of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_getAnimationTime00
static int tolua_Gamelua_ArmatureContainer_getAnimationTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAnimationTime'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getAnimationTime(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAnimationTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBone of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_addBone00
static int tolua_Gamelua_ArmatureContainer_addBone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* boneName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* animationName = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBone'", NULL);
#endif
  {
   self->addBone(boneName,animationName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBonePlay of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_addBonePlay00
static int tolua_Gamelua_ArmatureContainer_addBonePlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* boneName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBonePlay'", NULL);
#endif
  {
   self->addBonePlay(boneName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBonePlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResourcePath of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_getResourcePath00
static int tolua_Gamelua_ArmatureContainer_getResourcePath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ArmatureContainer* self = (ArmatureContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResourcePath'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getResourcePath();
   tolua_pushcpplstring(tolua_S,tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResourcePath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearResource of class  ArmatureContainer */
#ifndef TOLUA_DISABLE_tolua_Gamelua_ArmatureContainer_clearResource00
static int tolua_Gamelua_ArmatureContainer_clearResource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ArmatureContainer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string resourcePath = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   ArmatureContainer::clearResource(resourcePath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearResource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_systemfont_path */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_systemfont_path00
static int tolua_Gamelua_get_systemfont_path00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  get_systemfont_path();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_systemfont_path'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_system_default_ppi */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_system_default_ppi00
static int tolua_Gamelua_get_system_default_ppi00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  get_system_default_ppi();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_system_default_ppi'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_prefered_default_fontsize */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_prefered_default_fontsize00
static int tolua_Gamelua_get_prefered_default_fontsize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  get_prefered_default_fontsize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_prefered_default_fontsize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_system_default_fontfile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_system_default_fontfile00
static int tolua_Gamelua_get_system_default_fontfile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  get_system_default_fontfile();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_system_default_fontfile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_system_helvetica_fontfile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_system_helvetica_fontfile00
static int tolua_Gamelua_get_system_helvetica_fontfile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  get_system_helvetica_fontfile();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_system_helvetica_fontfile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_system_helveticabd_fontfile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_system_helveticabd_fontfile00
static int tolua_Gamelua_get_system_helveticabd_fontfile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  get_system_helveticabd_fontfile();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_system_helveticabd_fontfile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_system_default_hacklatin_fontfile */
#ifndef TOLUA_DISABLE_tolua_Gamelua_get_system_default_hacklatin_fontfile00
static int tolua_Gamelua_get_system_default_hacklatin_fontfile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  get_system_default_hacklatin_fontfile();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_system_default_hacklatin_fontfile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: dfont_default_initialize */
#ifndef TOLUA_DISABLE_tolua_Gamelua_dfont_default_initialize00
static int tolua_Gamelua_dfont_default_initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   dfont_default_initialize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dfont_default_initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: latin_charset */
#ifndef TOLUA_DISABLE_tolua_Gamelua_latin_charset00
static int tolua_Gamelua_latin_charset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::set<unsigned long>* tolua_ret = (std::set<unsigned long>*)  latin_charset();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::set<unsigned long>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'latin_charset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: instance of class  FontFactory */
#ifndef TOLUA_DISABLE_tolua_Gamelua_FontFactory_instance00
static int tolua_Gamelua_FontFactory_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"FontFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   FontFactory* tolua_ret = (FontFactory*)  FontFactory::instance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"FontFactory");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create_font_forLua of class  FontFactory */
#ifndef TOLUA_DISABLE_tolua_Gamelua_FontFactory_create_font_forLua00
static int tolua_Gamelua_FontFactory_create_font_forLua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontFactory",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontFactory* self = (FontFactory*)  tolua_tousertype(tolua_S,1,0);
  const char* alias = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* font_name = ((const char*)  tolua_tostring(tolua_S,3,0));
  unsigned int color = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  int size_pt = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create_font_forLua'", NULL);
#endif
  {
   self->create_font_forLua(alias,font_name,color,size_pt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_font_forLua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Gamelua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CCBScriptContainer","CCBScriptContainer","CCNode",NULL);
  tolua_beginmodule(tolua_S,"CCBScriptContainer");
   tolua_function(tolua_S,"create",tolua_Gamelua_CCBScriptContainer_create00);
   tolua_function(tolua_S,"purgeCachedData",tolua_Gamelua_CCBScriptContainer_purgeCachedData00);
   tolua_function(tolua_S,"sendPakcet",tolua_Gamelua_CCBScriptContainer_sendPakcet00);
   tolua_function(tolua_S,"registerMessage",tolua_Gamelua_CCBScriptContainer_registerMessage00);
   tolua_function(tolua_S,"registerPacket",tolua_Gamelua_CCBScriptContainer_registerPacket00);
   tolua_function(tolua_S,"removeMessage",tolua_Gamelua_CCBScriptContainer_removeMessage00);
   tolua_function(tolua_S,"removePacket",tolua_Gamelua_CCBScriptContainer_removePacket00);
   tolua_function(tolua_S,"registerLibOS",tolua_Gamelua_CCBScriptContainer_registerLibOS00);
   tolua_function(tolua_S,"removeLibOS",tolua_Gamelua_CCBScriptContainer_removeLibOS00);
   tolua_function(tolua_S,"getInputboxContent",tolua_Gamelua_CCBScriptContainer_getInputboxContent00);
   tolua_function(tolua_S,"getMessageboxTag",tolua_Gamelua_CCBScriptContainer_getMessageboxTag00);
   tolua_function(tolua_S,"loadCcbiFile",tolua_Gamelua_CCBScriptContainer_loadCcbiFile00);
   tolua_function(tolua_S,"runAnimation",tolua_Gamelua_CCBScriptContainer_runAnimation00);
   tolua_function(tolua_S,"load",tolua_Gamelua_CCBScriptContainer_load00);
   tolua_function(tolua_S,"getVarNode",tolua_Gamelua_CCBScriptContainer_getVarNode00);
   tolua_function(tolua_S,"getVarSprite",tolua_Gamelua_CCBScriptContainer_getVarSprite00);
   tolua_function(tolua_S,"getVarLabelBMFont",tolua_Gamelua_CCBScriptContainer_getVarLabelBMFont00);
   tolua_function(tolua_S,"getVarLabelTTF",tolua_Gamelua_CCBScriptContainer_getVarLabelTTF00);
   tolua_function(tolua_S,"getVarMenu",tolua_Gamelua_CCBScriptContainer_getVarMenu00);
   tolua_function(tolua_S,"getVarMenuItem",tolua_Gamelua_CCBScriptContainer_getVarMenuItem00);
   tolua_function(tolua_S,"getVarMenuItemSprite",tolua_Gamelua_CCBScriptContainer_getVarMenuItemSprite00);
   tolua_function(tolua_S,"getVarMenuItemImage",tolua_Gamelua_CCBScriptContainer_getVarMenuItemImage00);
   tolua_function(tolua_S,"getVarScrollView",tolua_Gamelua_CCBScriptContainer_getVarScrollView00);
   tolua_function(tolua_S,"getVarScale9Sprite",tolua_Gamelua_CCBScriptContainer_getVarScale9Sprite00);
   tolua_function(tolua_S,"getParticleSystem",tolua_Gamelua_CCBScriptContainer_getParticleSystem00);
   tolua_function(tolua_S,"getVarMenuItemCCB",tolua_Gamelua_CCBScriptContainer_getVarMenuItemCCB00);
   tolua_function(tolua_S,"getVarLayer",tolua_Gamelua_CCBScriptContainer_getVarLayer00);
   tolua_function(tolua_S,"getMessage",tolua_Gamelua_CCBScriptContainer_getMessage00);
   tolua_function(tolua_S,"getSendPacketOpcode",tolua_Gamelua_CCBScriptContainer_getSendPacketOpcode00);
   tolua_function(tolua_S,"getRecPacketOpcode",tolua_Gamelua_CCBScriptContainer_getRecPacketOpcode00);
   tolua_function(tolua_S,"getRecPacketBufferLength",tolua_Gamelua_CCBScriptContainer_getRecPacketBufferLength00);
   tolua_function(tolua_S,"getRecPacketBuffer",tolua_Gamelua_CCBScriptContainer_getRecPacketBuffer00);
   tolua_function(tolua_S,"setTag",tolua_Gamelua_CCBScriptContainer_setTag00);
   tolua_function(tolua_S,"getTag",tolua_Gamelua_CCBScriptContainer_getTag00);
   tolua_function(tolua_S,"searchChildByTag",tolua_Gamelua_CCBScriptContainer_searchChildByTag00);
   tolua_function(tolua_S,"searchParentByTag",tolua_Gamelua_CCBScriptContainer_searchParentByTag00);
   tolua_function(tolua_S,"registerFunctionHandler",tolua_Gamelua_CCBScriptContainer_registerFunctionHandler00);
   tolua_function(tolua_S,"unregisterFunctionHandler",tolua_Gamelua_CCBScriptContainer_unregisterFunctionHandler00);
   tolua_function(tolua_S,"setDynamicIconTex",tolua_Gamelua_CCBScriptContainer_setDynamicIconTex00);
   tolua_function(tolua_S,"setFrameColorByQuality",tolua_Gamelua_CCBScriptContainer_setFrameColorByQuality00);
   tolua_function(tolua_S,"setLabelColorByQuality",tolua_Gamelua_CCBScriptContainer_setLabelColorByQuality00);
   tolua_function(tolua_S,"dumpInfo",tolua_Gamelua_CCBScriptContainer_dumpInfo00);
   tolua_function(tolua_S,"autoAdjustResizeScrollview",tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScrollview00);
   tolua_function(tolua_S,"autoAdjustResizeScale9Sprite",tolua_Gamelua_CCBScriptContainer_autoAdjustResizeScale9Sprite00);
   tolua_function(tolua_S,"getCurAnimationDoneName",tolua_Gamelua_CCBScriptContainer_getCurAnimationDoneName00);
   tolua_function(tolua_S,"unload",tolua_Gamelua_CCBScriptContainer_unload00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"kCCScrollViewDirectionNone",kCCScrollViewDirectionNone);
  tolua_constant(tolua_S,"kCCScrollViewDirectionHorizontal",kCCScrollViewDirectionHorizontal);
  tolua_constant(tolua_S,"kCCScrollViewDirectionVertical",kCCScrollViewDirectionVertical);
  tolua_constant(tolua_S,"kCCScrollViewDirectionBoth",kCCScrollViewDirectionBoth);
  tolua_cclass(tolua_S,"CCScrollView","CCScrollView","CCLayer",NULL);
  tolua_beginmodule(tolua_S,"CCScrollView");
   tolua_function(tolua_S,"create",tolua_Gamelua_CCScrollView_create00);
   tolua_function(tolua_S,"create",tolua_Gamelua_CCScrollView_create01);
   tolua_function(tolua_S,"initWithViewSize",tolua_Gamelua_CCScrollView_initWithViewSize00);
   tolua_function(tolua_S,"setContentOffset",tolua_Gamelua_CCScrollView_setContentOffset00);
   tolua_function(tolua_S,"getContentOffset",tolua_Gamelua_CCScrollView_getContentOffset00);
   tolua_function(tolua_S,"setContentOffsetInDuration",tolua_Gamelua_CCScrollView_setContentOffsetInDuration00);
   tolua_function(tolua_S,"setZoomScale",tolua_Gamelua_CCScrollView_setZoomScale00);
   tolua_function(tolua_S,"setZoomScale",tolua_Gamelua_CCScrollView_setZoomScale01);
   tolua_function(tolua_S,"getZoomScale",tolua_Gamelua_CCScrollView_getZoomScale00);
   tolua_function(tolua_S,"setScrollDeaccelRate",tolua_Gamelua_CCScrollView_setScrollDeaccelRate00);
   tolua_function(tolua_S,"getScrollDeaccelRate",tolua_Gamelua_CCScrollView_getScrollDeaccelRate00);
   tolua_function(tolua_S,"setZoomScaleInDuration",tolua_Gamelua_CCScrollView_setZoomScaleInDuration00);
   tolua_function(tolua_S,"minContainerOffset",tolua_Gamelua_CCScrollView_minContainerOffset00);
   tolua_function(tolua_S,"maxContainerOffset",tolua_Gamelua_CCScrollView_maxContainerOffset00);
   tolua_function(tolua_S,"isNodeVisible",tolua_Gamelua_CCScrollView_isNodeVisible00);
   tolua_function(tolua_S,"pause",tolua_Gamelua_CCScrollView_pause00);
   tolua_function(tolua_S,"resume",tolua_Gamelua_CCScrollView_resume00);
   tolua_function(tolua_S,"isDragging",tolua_Gamelua_CCScrollView_isDragging00);
   tolua_function(tolua_S,"isTouchMoved",tolua_Gamelua_CCScrollView_isTouchMoved00);
   tolua_function(tolua_S,"isBounceable",tolua_Gamelua_CCScrollView_isBounceable00);
   tolua_function(tolua_S,"setBounceable",tolua_Gamelua_CCScrollView_setBounceable00);
   tolua_function(tolua_S,"getViewSize",tolua_Gamelua_CCScrollView_getViewSize00);
   tolua_function(tolua_S,"setViewSize",tolua_Gamelua_CCScrollView_setViewSize00);
   tolua_function(tolua_S,"getContainer",tolua_Gamelua_CCScrollView_getContainer00);
   tolua_function(tolua_S,"setContainer",tolua_Gamelua_CCScrollView_setContainer00);
   tolua_function(tolua_S,"getDirection",tolua_Gamelua_CCScrollView_getDirection00);
   tolua_function(tolua_S,"setDirection",tolua_Gamelua_CCScrollView_setDirection00);
   tolua_function(tolua_S,"isClippingToBounds",tolua_Gamelua_CCScrollView_isClippingToBounds00);
   tolua_function(tolua_S,"setClippingToBounds",tolua_Gamelua_CCScrollView_setClippingToBounds00);
   tolua_function(tolua_S,"visit",tolua_Gamelua_CCScrollView_visit00);
   tolua_function(tolua_S,"addChild",tolua_Gamelua_CCScrollView_addChild00);
   tolua_function(tolua_S,"addChild",tolua_Gamelua_CCScrollView_addChild01);
   tolua_function(tolua_S,"addChild",tolua_Gamelua_CCScrollView_addChild02);
   tolua_function(tolua_S,"setTouchEnabled",tolua_Gamelua_CCScrollView_setTouchEnabled00);
   tolua_function(tolua_S,"forceRecaculateChildren",tolua_Gamelua_CCScrollView_forceRecaculateChildren00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TableAutoReader","TableAutoReader","TableReader",NULL);
  tolua_beginmodule(tolua_S,"TableAutoReader");
   tolua_function(tolua_S,"getLineCount",tolua_Gamelua_TableAutoReader_getLineCount00);
   tolua_function(tolua_S,"seekLine",tolua_Gamelua_TableAutoReader_seekLine00);
   tolua_function(tolua_S,"seekIndex",tolua_Gamelua_TableAutoReader_seekIndex00);
   tolua_function(tolua_S,"hasRow",tolua_Gamelua_TableAutoReader_hasRow00);
   tolua_function(tolua_S,"getDataInRow",tolua_Gamelua_TableAutoReader_getDataInRow00);
   tolua_function(tolua_S,"getData",tolua_Gamelua_TableAutoReader_getData00);
   tolua_function(tolua_S,"getDataIndex",tolua_Gamelua_TableAutoReader_getDataIndex00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TableReaderManager","TableReaderManager","",NULL);
  tolua_beginmodule(tolua_S,"TableReaderManager");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_TableReaderManager_getInstance00);
   tolua_function(tolua_S,"getTableReader",tolua_Gamelua_TableReaderManager_getTableReader00);
   tolua_function(tolua_S,"reloadAllReader",tolua_Gamelua_TableReaderManager_reloadAllReader00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TimeCalculator","TimeCalculator","",NULL);
  tolua_beginmodule(tolua_S,"TimeCalculator");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_TimeCalculator_getInstance00);
   tolua_function(tolua_S,"createTimeCalcultor",tolua_Gamelua_TimeCalculator_createTimeCalcultor00);
   tolua_function(tolua_S,"removeTimeCalcultor",tolua_Gamelua_TimeCalculator_removeTimeCalcultor00);
   tolua_function(tolua_S,"getTimeLeft",tolua_Gamelua_TimeCalculator_getTimeLeft00);
   tolua_function(tolua_S,"hasKey",tolua_Gamelua_TimeCalculator_hasKey00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"AES","AES","",tolua_collect_AES);
  #else
  tolua_cclass(tolua_S,"AES","AES","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"AES");
   tolua_function(tolua_S,"new",tolua_Gamelua_AES_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_AES_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_AES_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_AES_delete00);
   tolua_function(tolua_S,"Decrypt",tolua_Gamelua_AES_Decrypt00);
   tolua_function(tolua_S,"Encrypt",tolua_Gamelua_AES_Encrypt00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameMaths","GameMaths","",NULL);
  tolua_beginmodule(tolua_S,"GameMaths");
   tolua_function(tolua_S,"stringAutoReturn",tolua_Gamelua_GameMaths_stringAutoReturn00);
   tolua_function(tolua_S,"stringAutoReturnForLua",tolua_Gamelua_GameMaths_stringAutoReturnForLua00);
   tolua_function(tolua_S,"calculateStringCharacters",tolua_Gamelua_GameMaths_calculateStringCharacters00);
   tolua_function(tolua_S,"getStringSubCharacters",tolua_Gamelua_GameMaths_getStringSubCharacters00);
   tolua_function(tolua_S,"replaceStringWithBlank",tolua_Gamelua_GameMaths_replaceStringWithBlank00);
   tolua_function(tolua_S,"readStringToMap",tolua_Gamelua_GameMaths_readStringToMap00);
   tolua_function(tolua_S,"replaceStringWithCharacter",tolua_Gamelua_GameMaths_replaceStringWithCharacter00);
   tolua_function(tolua_S,"formatSecondsToTime",tolua_Gamelua_GameMaths_formatSecondsToTime00);
   tolua_function(tolua_S,"formatTimeToDate",tolua_Gamelua_GameMaths_formatTimeToDate00);
   tolua_function(tolua_S,"isStringHasUTF8mb4",tolua_Gamelua_GameMaths_isStringHasUTF8mb400);
   tolua_function(tolua_S,"hasSubString",tolua_Gamelua_GameMaths_hasSubString00);
   tolua_function(tolua_S,"ReverseAuto",tolua_Gamelua_GameMaths_ReverseAuto00);
   tolua_function(tolua_S,"replaceStringOneForLua",tolua_Gamelua_GameMaths_replaceStringOneForLua00);
   tolua_function(tolua_S,"replaceStringTwoForLua",tolua_Gamelua_GameMaths_replaceStringTwoForLua00);
   tolua_function(tolua_S,"replaceStringWithCharacterAll",tolua_Gamelua_GameMaths_replaceStringWithCharacterAll00);
   tolua_function(tolua_S,"stringCutWidthLen",tolua_Gamelua_GameMaths_stringCutWidthLen00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ConfigFile","ConfigFile","",tolua_collect_ConfigFile);
  #else
  tolua_cclass(tolua_S,"ConfigFile","ConfigFile","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ConfigFile");
   tolua_function(tolua_S,"new",tolua_Gamelua_ConfigFile_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_ConfigFile_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_ConfigFile_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_ConfigFile_delete00);
   tolua_function(tolua_S,"load",tolua_Gamelua_ConfigFile_load00);
   tolua_function(tolua_S,"load",tolua_Gamelua_ConfigFile_load01);
   tolua_function(tolua_S,"load",tolua_Gamelua_ConfigFile_load02);
   tolua_function(tolua_S,"load",tolua_Gamelua_ConfigFile_load03);
   tolua_function(tolua_S,"getSetting",tolua_Gamelua_ConfigFile_getSetting00);
   tolua_function(tolua_S,"getSetting",tolua_Gamelua_ConfigFile_getSetting01);
   tolua_function(tolua_S,"getSetting",tolua_Gamelua_ConfigFile_getSetting02);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MainFrame","MainFrame","",NULL);
  tolua_beginmodule(tolua_S,"MainFrame");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_MainFrame_getInstance00);
   tolua_function(tolua_S,"showPage",tolua_Gamelua_MainFrame_showPage00);
   tolua_function(tolua_S,"pushPage",tolua_Gamelua_MainFrame_pushPage00);
   tolua_function(tolua_S,"popPage",tolua_Gamelua_MainFrame_popPage00);
   tolua_function(tolua_S,"popAllPage",tolua_Gamelua_MainFrame_popAllPage00);
   tolua_function(tolua_S,"showFightPage",tolua_Gamelua_MainFrame_showFightPage00);
   tolua_function(tolua_S,"isPageInList",tolua_Gamelua_MainFrame_isPageInList00);
   tolua_function(tolua_S,"showNoTouch",tolua_Gamelua_MainFrame_showNoTouch00);
   tolua_function(tolua_S,"hideNoTouch",tolua_Gamelua_MainFrame_hideNoTouch00);
   tolua_function(tolua_S,"getCurShowPageName",tolua_Gamelua_MainFrame_getCurShowPageName00);
   tolua_function(tolua_S,"getMsgNodeForLua",tolua_Gamelua_MainFrame_getMsgNodeForLua00);
   tolua_function(tolua_S,"setChildVisible",tolua_Gamelua_MainFrame_setChildVisible00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Language","Language","",NULL);
  tolua_beginmodule(tolua_S,"Language");
   tolua_function(tolua_S,"init",tolua_Gamelua_Language_init00);
   tolua_function(tolua_S,"addLanguageFile",tolua_Gamelua_Language_addLanguageFile00);
   tolua_function(tolua_S,"hasString",tolua_Gamelua_Language_hasString00);
   tolua_function(tolua_S,"getString",tolua_Gamelua_Language_getString00);
   tolua_function(tolua_S,"updateNode",tolua_Gamelua_Language_updateNode00);
   tolua_function(tolua_S,"clear",tolua_Gamelua_Language_clear00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_Language_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SoundManager","SoundManager","",NULL);
  tolua_beginmodule(tolua_S,"SoundManager");
   tolua_function(tolua_S,"setMusicOn",tolua_Gamelua_SoundManager_setMusicOn00);
   tolua_function(tolua_S,"setEffectOn",tolua_Gamelua_SoundManager_setEffectOn00);
   tolua_function(tolua_S,"playLoadingMusic",tolua_Gamelua_SoundManager_playLoadingMusic00);
   tolua_function(tolua_S,"playGeneralMusic",tolua_Gamelua_SoundManager_playGeneralMusic00);
   tolua_function(tolua_S,"playFightMusic",tolua_Gamelua_SoundManager_playFightMusic00);
   tolua_function(tolua_S,"stopMusic",tolua_Gamelua_SoundManager_stopMusic00);
   tolua_function(tolua_S,"playMusic",tolua_Gamelua_SoundManager_playMusic00);
   tolua_function(tolua_S,"playEffect",tolua_Gamelua_SoundManager_playEffect00);
   tolua_function(tolua_S,"stopAllEffect",tolua_Gamelua_SoundManager_stopAllEffect00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_SoundManager_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"BlackBoard","BlackBoard","",NULL);
  tolua_beginmodule(tolua_S,"BlackBoard");
   tolua_function(tolua_S,"hasVarible",tolua_Gamelua_BlackBoard_hasVarible00);
   tolua_function(tolua_S,"addVarible",tolua_Gamelua_BlackBoard_addVarible00);
   tolua_function(tolua_S,"addVarible",tolua_Gamelua_BlackBoard_addVarible01);
   tolua_function(tolua_S,"addVarible",tolua_Gamelua_BlackBoard_addVarible02);
   tolua_function(tolua_S,"delVarible",tolua_Gamelua_BlackBoard_delVarible00);
   tolua_function(tolua_S,"setVarible",tolua_Gamelua_BlackBoard_setVarible00);
   tolua_function(tolua_S,"setVarible",tolua_Gamelua_BlackBoard_setVarible01);
   tolua_function(tolua_S,"setVarible",tolua_Gamelua_BlackBoard_setVarible02);
   tolua_function(tolua_S,"getVarible",tolua_Gamelua_BlackBoard_getVarible00);
   tolua_function(tolua_S,"getVaribleAsUint",tolua_Gamelua_BlackBoard_getVaribleAsUint00);
   tolua_function(tolua_S,"getVaribleAsBool",tolua_Gamelua_BlackBoard_getVaribleAsBool00);
   tolua_function(tolua_S,"fetchVarible",tolua_Gamelua_BlackBoard_fetchVarible00);
   tolua_function(tolua_S,"fetchVaribleAsUint",tolua_Gamelua_BlackBoard_fetchVaribleAsUint00);
   tolua_function(tolua_S,"fetchVaribleAsBool",tolua_Gamelua_BlackBoard_fetchVaribleAsBool00);
   tolua_variable(tolua_S,"PLATFORM_TYPE_FOR_LUA",tolua_get_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA,tolua_set_BlackBoard_unsigned_PLATFORM_TYPE_FOR_LUA);
   tolua_function(tolua_S,"clearMap",tolua_Gamelua_BlackBoard_clearMap00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_BlackBoard_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MessageBoxPage","MessageBoxPage","",NULL);
  tolua_beginmodule(tolua_S,"MessageBoxPage");
   tolua_function(tolua_S,"Msg_Box",tolua_Gamelua_MessageBoxPage_Msg_Box00);
   tolua_function(tolua_S,"Msg_Box_Lan",tolua_Gamelua_MessageBoxPage_Msg_Box_Lan00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"LoginInfo","LoginInfo","",tolua_collect_LoginInfo);
  #else
  tolua_cclass(tolua_S,"LoginInfo","LoginInfo","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"LoginInfo");
   tolua_variable(tolua_S,"m_iPlayerID",tolua_get_LoginInfo_m_iPlayerID,tolua_set_LoginInfo_m_iPlayerID);
   tolua_variable(tolua_S,"m_iRoleItemID",tolua_get_LoginInfo_m_iRoleItemID,tolua_set_LoginInfo_m_iRoleItemID);
   tolua_variable(tolua_S,"m_iTimeStamp",tolua_get_LoginInfo_m_iTimeStamp,tolua_set_LoginInfo_m_iTimeStamp);
   tolua_function(tolua_S,"new",tolua_Gamelua_LoginInfo_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_LoginInfo_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_LoginInfo_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GameCheckInfo","GameCheckInfo","",tolua_collect_GameCheckInfo);
  #else
  tolua_cclass(tolua_S,"GameCheckInfo","GameCheckInfo","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GameCheckInfo");
   tolua_variable(tolua_S,"mCurrAdventureVersion",tolua_get_GameCheckInfo_unsigned_mCurrAdventureVersion,tolua_set_GameCheckInfo_unsigned_mCurrAdventureVersion);
   tolua_variable(tolua_S,"mHeartRecruitDiscipleVersion",tolua_get_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion,tolua_set_GameCheckInfo_unsigned_mHeartRecruitDiscipleVersion);
   tolua_variable(tolua_S,"mHeartAdventureVersion",tolua_get_GameCheckInfo_unsigned_mHeartAdventureVersion,tolua_set_GameCheckInfo_unsigned_mHeartAdventureVersion);
   tolua_variable(tolua_S,"mNeedForceSyncAdventure",tolua_get_GameCheckInfo_mNeedForceSyncAdventure,tolua_set_GameCheckInfo_mNeedForceSyncAdventure);
   tolua_function(tolua_S,"isNeedSyncAdventureList",tolua_Gamelua_GameCheckInfo_isNeedSyncAdventureList00);
   tolua_function(tolua_S,"setNeedForceSyncAdventure",tolua_Gamelua_GameCheckInfo_setNeedForceSyncAdventure00);
   tolua_function(tolua_S,"new",tolua_Gamelua_GameCheckInfo_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_GameCheckInfo_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_GameCheckInfo_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ServerDateManager","ServerDateManager","Singleton<ServerDateManager>",tolua_collect_ServerDateManager);
  #else
  tolua_cclass(tolua_S,"ServerDateManager","ServerDateManager","Singleton<ServerDateManager>",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ServerDateManager");
   tolua_function(tolua_S,"new",tolua_Gamelua_ServerDateManager_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_ServerDateManager_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_ServerDateManager_new00_local);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_ServerDateManager_getInstance00);
   tolua_function(tolua_S,"getUserBasicInfoForLua",tolua_Gamelua_ServerDateManager_getUserBasicInfoForLua00);
   tolua_function(tolua_S,"getUserRoleInfoForLua",tolua_Gamelua_ServerDateManager_getUserRoleInfoForLua00);
   tolua_function(tolua_S,"getRoleInfoTotalSize",tolua_Gamelua_ServerDateManager_getRoleInfoTotalSize00);
   tolua_function(tolua_S,"getRoleInfoByIdForLua",tolua_Gamelua_ServerDateManager_getRoleInfoByIdForLua00);
   tolua_function(tolua_S,"getRoleInfoByIndexForLua",tolua_Gamelua_ServerDateManager_getRoleInfoByIndexForLua00);
   tolua_function(tolua_S,"getEquipInfoTotalSize",tolua_Gamelua_ServerDateManager_getEquipInfoTotalSize00);
   tolua_function(tolua_S,"getEquipInfoByIdForLua",tolua_Gamelua_ServerDateManager_getEquipInfoByIdForLua00);
   tolua_function(tolua_S,"getEquipInfoByIndexForLua",tolua_Gamelua_ServerDateManager_getEquipInfoByIndexForLua00);
   tolua_function(tolua_S,"getItemInfoTotalSize",tolua_Gamelua_ServerDateManager_getItemInfoTotalSize00);
   tolua_function(tolua_S,"getItemInfoByIdForLua",tolua_Gamelua_ServerDateManager_getItemInfoByIdForLua00);
   tolua_function(tolua_S,"getItemInfoByIndexForLua",tolua_Gamelua_ServerDateManager_getItemInfoByIndexForLua00);
   tolua_function(tolua_S,"getSkillInfoTotalSize",tolua_Gamelua_ServerDateManager_getSkillInfoTotalSize00);
   tolua_function(tolua_S,"getSkillInfoByIdForLua",tolua_Gamelua_ServerDateManager_getSkillInfoByIdForLua00);
   tolua_function(tolua_S,"getSkillInfoByIndexForLua",tolua_Gamelua_ServerDateManager_getSkillInfoByIndexForLua00);
   tolua_function(tolua_S,"getPlayerStateForLua",tolua_Gamelua_ServerDateManager_getPlayerStateForLua00);
   tolua_variable(tolua_S,"mLoginInfo",tolua_get_ServerDateManager_mLoginInfo,tolua_set_ServerDateManager_mLoginInfo);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"getFrameColor",tolua_Gamelua_getFrameColor00);
  tolua_function(tolua_S,"getFrameNormalSpirte",tolua_Gamelua_getFrameNormalSpirte00);
  tolua_function(tolua_S,"getFrameSelectedSpirte",tolua_Gamelua_getFrameSelectedSpirte00);
  tolua_function(tolua_S,"getFrameDisabledSpirte",tolua_Gamelua_getFrameDisabledSpirte00);
  tolua_function(tolua_S,"getContentUnselectedSpirte",tolua_Gamelua_getContentUnselectedSpirte00);
  tolua_function(tolua_S,"getContentSelectedSpirte",tolua_Gamelua_getContentSelectedSpirte00);
  tolua_function(tolua_S,"getQualityMaskImageFile",tolua_Gamelua_getQualityMaskImageFile00);
  tolua_function(tolua_S,"getQualityImageFile",tolua_Gamelua_getQualityImageFile00);
  tolua_function(tolua_S,"getQualityFramePersonImageFile",tolua_Gamelua_getQualityFramePersonImageFile00);
  tolua_function(tolua_S,"getQualityFrameNormalImageFile",tolua_Gamelua_getQualityFrameNormalImageFile00);
  tolua_function(tolua_S,"getQualityFrameSelectImageFile",tolua_Gamelua_getQualityFrameSelectImageFile00);
  tolua_function(tolua_S,"getVipLevelImageFile",tolua_Gamelua_getVipLevelImageFile00);
  tolua_function(tolua_S,"getTeamDefaultPic",tolua_Gamelua_getTeamDefaultPic00);
  tolua_function(tolua_S,"getTeamLockPic",tolua_Gamelua_getTeamLockPic00);
  tolua_cclass(tolua_S,"VaribleManager","VaribleManager","ConfigFile",NULL);
  tolua_beginmodule(tolua_S,"VaribleManager");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_VaribleManager_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PlatformRoleItem","PlatformRoleItem","",NULL);
  tolua_beginmodule(tolua_S,"PlatformRoleItem");
   tolua_variable(tolua_S,"id",tolua_get_PlatformRoleItem_unsigned_id,tolua_set_PlatformRoleItem_unsigned_id);
   tolua_variable(tolua_S,"name",tolua_get_PlatformRoleItem_name,tolua_set_PlatformRoleItem_name);
   tolua_variable(tolua_S,"attentionStatus",tolua_get_PlatformRoleItem_unsigned_attentionStatus,tolua_set_PlatformRoleItem_unsigned_attentionStatus);
   tolua_variable(tolua_S,"loadingFrameSeversConnection",tolua_get_PlatformRoleItem_loadingFrameSeversConnection,tolua_set_PlatformRoleItem_loadingFrameSeversConnection);
   tolua_variable(tolua_S,"rechargeMaxLimit",tolua_get_PlatformRoleItem_rechargeMaxLimit,tolua_set_PlatformRoleItem_rechargeMaxLimit);
   tolua_variable(tolua_S,"bbsOpenStatus",tolua_get_PlatformRoleItem_unsigned_bbsOpenStatus,tolua_set_PlatformRoleItem_unsigned_bbsOpenStatus);
   tolua_variable(tolua_S,"showFeedBack",tolua_get_PlatformRoleItem_showFeedBack,tolua_set_PlatformRoleItem_showFeedBack);
   tolua_variable(tolua_S,"webkitOpenStatus",tolua_get_PlatformRoleItem_unsigned_webkitOpenStatus,tolua_set_PlatformRoleItem_unsigned_webkitOpenStatus);
   tolua_variable(tolua_S,"aboutSetting",tolua_get_PlatformRoleItem_aboutSetting,tolua_set_PlatformRoleItem_aboutSetting);
   tolua_variable(tolua_S,"excludedAboutIds",tolua_get_PlatformRoleItem_excludedAboutIds,tolua_set_PlatformRoleItem_excludedAboutIds);
   tolua_variable(tolua_S,"excludedAnnoucementIds",tolua_get_PlatformRoleItem_excludedAnnoucementIds,tolua_set_PlatformRoleItem_excludedAnnoucementIds);
   tolua_variable(tolua_S,"isUsePrivateBigVersionUpdate",tolua_get_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate,tolua_set_PlatformRoleItem_unsigned_isUsePrivateBigVersionUpdate);
   tolua_variable(tolua_S,"isUseSDKBBS",tolua_get_PlatformRoleItem_unsigned_isUseSDKBBS,tolua_set_PlatformRoleItem_unsigned_isUseSDKBBS);
   tolua_variable(tolua_S,"openBBSUrl",tolua_get_PlatformRoleItem_openBBSUrl,tolua_set_PlatformRoleItem_openBBSUrl);
   tolua_variable(tolua_S,"loadinScenceMsg",tolua_get_PlatformRoleItem_loadinScenceMsg,tolua_set_PlatformRoleItem_loadinScenceMsg);
   tolua_variable(tolua_S,"exit2platlogout",tolua_get_PlatformRoleItem_unsigned_exit2platlogout,tolua_set_PlatformRoleItem_unsigned_exit2platlogout);
   tolua_variable(tolua_S,"shareSwitch",tolua_get_PlatformRoleItem_unsigned_shareSwitch,tolua_set_PlatformRoleItem_unsigned_shareSwitch);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PlatformRoleTableManager","PlatformRoleTableManager","",NULL);
  tolua_beginmodule(tolua_S,"PlatformRoleTableManager");
   tolua_function(tolua_S,"getPlatformRoleByName",tolua_Gamelua_PlatformRoleTableManager_getPlatformRoleByName00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_PlatformRoleTableManager_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCBManager","CCBManager","",NULL);
  tolua_beginmodule(tolua_S,"CCBManager");
   tolua_function(tolua_S,"registerPage",tolua_Gamelua_CCBManager_registerPage00);
   tolua_function(tolua_S,"unRegisterPage",tolua_Gamelua_CCBManager_unRegisterPage00);
   tolua_function(tolua_S,"getPage",tolua_Gamelua_CCBManager_getPage00);
   tolua_function(tolua_S,"createAndLoad",tolua_Gamelua_CCBManager_createAndLoad00);
   tolua_function(tolua_S,"createAndLoad2",tolua_Gamelua_CCBManager_createAndLoad200);
   tolua_function(tolua_S,"loadCCBFileNew",tolua_Gamelua_CCBManager_loadCCBFileNew00);
   tolua_function(tolua_S,"purgeCachedData",tolua_Gamelua_CCBManager_purgeCachedData00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_CCBManager_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"registerScriptPage",tolua_Gamelua_registerScriptPage00);
  tolua_cclass(tolua_S,"ScriptContentBase","ScriptContentBase","CCNode",NULL);
  tolua_beginmodule(tolua_S,"ScriptContentBase");
   tolua_function(tolua_S,"create",tolua_Gamelua_ScriptContentBase_create00);
   tolua_function(tolua_S,"getVarNode",tolua_Gamelua_ScriptContentBase_getVarNode00);
   tolua_function(tolua_S,"getVarSprite",tolua_Gamelua_ScriptContentBase_getVarSprite00);
   tolua_function(tolua_S,"getVarLabelBMFont",tolua_Gamelua_ScriptContentBase_getVarLabelBMFont00);
   tolua_function(tolua_S,"getVarLabelTTF",tolua_Gamelua_ScriptContentBase_getVarLabelTTF00);
   tolua_function(tolua_S,"getVarMenu",tolua_Gamelua_ScriptContentBase_getVarMenu00);
   tolua_function(tolua_S,"getVarMenuItem",tolua_Gamelua_ScriptContentBase_getVarMenuItem00);
   tolua_function(tolua_S,"getVarMenuItemSprite",tolua_Gamelua_ScriptContentBase_getVarMenuItemSprite00);
   tolua_function(tolua_S,"getVarMenuItemImage",tolua_Gamelua_ScriptContentBase_getVarMenuItemImage00);
   tolua_function(tolua_S,"getVarScrollView",tolua_Gamelua_ScriptContentBase_getVarScrollView00);
   tolua_function(tolua_S,"getVarScale9Sprite",tolua_Gamelua_ScriptContentBase_getVarScale9Sprite00);
   tolua_function(tolua_S,"getParticleSystem",tolua_Gamelua_ScriptContentBase_getParticleSystem00);
   tolua_function(tolua_S,"getVarMenuItemCCB",tolua_Gamelua_ScriptContentBase_getVarMenuItemCCB00);
   tolua_function(tolua_S,"registerFunctionHandler",tolua_Gamelua_ScriptContentBase_registerFunctionHandler00);
   tolua_function(tolua_S,"unregisterFunctionHandler",tolua_Gamelua_ScriptContentBase_unregisterFunctionHandler00);
   tolua_function(tolua_S,"getItemDate",tolua_Gamelua_ScriptContentBase_getItemDate00);
   tolua_function(tolua_S,"runAnimation",tolua_Gamelua_ScriptContentBase_runAnimation00);
   tolua_function(tolua_S,"setDynamicIconTex",tolua_Gamelua_ScriptContentBase_setDynamicIconTex00);
   tolua_function(tolua_S,"setFrameColorByQuality",tolua_Gamelua_ScriptContentBase_setFrameColorByQuality00);
   tolua_function(tolua_S,"setLabelColorByQuality",tolua_Gamelua_ScriptContentBase_setLabelColorByQuality00);
   tolua_function(tolua_S,"dumpInfo",tolua_Gamelua_ScriptContentBase_dumpInfo00);
   tolua_variable(tolua_S,"__CCReViSvItemNodeFacade__",tolua_get_ScriptContentBase___CCReViSvItemNodeFacade__,NULL);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CCReViSvItemData","CCReViSvItemData","CCObject",tolua_collect_CCReViSvItemData);
  #else
  tolua_cclass(tolua_S,"CCReViSvItemData","CCReViSvItemData","CCObject",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CCReViSvItemData");
   tolua_function(tolua_S,"new",tolua_Gamelua_CCReViSvItemData_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_CCReViSvItemData_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_CCReViSvItemData_new00_local);
   tolua_variable(tolua_S,"m_ptPosition",tolua_get_CCReViSvItemData_m_ptPosition,tolua_set_CCReViSvItemData_m_ptPosition);
   tolua_variable(tolua_S,"m_iIdx",tolua_get_CCReViSvItemData_unsigned_m_iIdx,tolua_set_CCReViSvItemData_unsigned_m_iIdx);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCReViSvItemNodeFacade","CCReViSvItemNodeFacade","",NULL);
  tolua_beginmodule(tolua_S,"CCReViSvItemNodeFacade");
   tolua_function(tolua_S,"initItemView",tolua_Gamelua_CCReViSvItemNodeFacade_initItemView00);
   tolua_function(tolua_S,"refreshItemView",tolua_Gamelua_CCReViSvItemNodeFacade_refreshItemView00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CCReViScrollViewFacade","CCReViScrollViewFacade","",tolua_collect_CCReViScrollViewFacade);
  #else
  tolua_cclass(tolua_S,"CCReViScrollViewFacade","CCReViScrollViewFacade","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CCReViScrollViewFacade");
   tolua_function(tolua_S,"new",tolua_Gamelua_CCReViScrollViewFacade_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_CCReViScrollViewFacade_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_CCReViScrollViewFacade_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_CCReViScrollViewFacade_delete00);
   tolua_function(tolua_S,"init",tolua_Gamelua_CCReViScrollViewFacade_init00);
   tolua_function(tolua_S,"getMaxDynamicControledItemViewsNum",tolua_Gamelua_CCReViScrollViewFacade_getMaxDynamicControledItemViewsNum00);
   tolua_function(tolua_S,"addItem",tolua_Gamelua_CCReViScrollViewFacade_addItem00);
   tolua_function(tolua_S,"insertItemAtIdx",tolua_Gamelua_CCReViScrollViewFacade_insertItemAtIdx00);
   tolua_function(tolua_S,"removeItem",tolua_Gamelua_CCReViScrollViewFacade_removeItem00);
   tolua_function(tolua_S,"removeItemAtIdx",tolua_Gamelua_CCReViScrollViewFacade_removeItemAtIdx00);
   tolua_function(tolua_S,"clearAllItems",tolua_Gamelua_CCReViScrollViewFacade_clearAllItems00);
   tolua_function(tolua_S,"setDynamicItemsStartPosition",tolua_Gamelua_CCReViScrollViewFacade_setDynamicItemsStartPosition00);
   tolua_function(tolua_S,"getStartPosition",tolua_Gamelua_CCReViScrollViewFacade_getStartPosition00);
   tolua_function(tolua_S,"refreshDynamicScrollView",tolua_Gamelua_CCReViScrollViewFacade_refreshDynamicScrollView00);
   tolua_function(tolua_S,"findItemNodeByItemData",tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByItemData00);
   tolua_function(tolua_S,"findItemNodeByIndex",tolua_Gamelua_CCReViScrollViewFacade_findItemNodeByIndex00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"StringConverter","StringConverter","",NULL);
  tolua_beginmodule(tolua_S,"StringConverter");
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString00);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString01);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString02);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString03);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString04);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString05);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString06);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString07);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString08);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString09);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString10);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString11);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString12);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString13);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString14);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString15);
   tolua_function(tolua_S,"toString",tolua_Gamelua_StringConverter_toString16);
   tolua_function(tolua_S,"parseRect",tolua_Gamelua_StringConverter_parseRect00);
   tolua_function(tolua_S,"parsePoint",tolua_Gamelua_StringConverter_parsePoint00);
   tolua_function(tolua_S,"parseSize",tolua_Gamelua_StringConverter_parseSize00);
   tolua_function(tolua_S,"parseColor3B",tolua_Gamelua_StringConverter_parseColor3B00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PacketHandler","PacketHandler","",NULL);
  tolua_beginmodule(tolua_S,"PacketHandler");
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"PacketScriptHandler","PacketScriptHandler","CCObject",tolua_collect_PacketScriptHandler);
  #else
  tolua_cclass(tolua_S,"PacketScriptHandler","PacketScriptHandler","CCObject",NULL);
  #endif
  tolua_beginmodule(tolua_S,"PacketScriptHandler");
   tolua_function(tolua_S,"new",tolua_Gamelua_PacketScriptHandler_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_PacketScriptHandler_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_PacketScriptHandler_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_PacketScriptHandler_delete00);
   tolua_function(tolua_S,"getRecPacketBufferLength",tolua_Gamelua_PacketScriptHandler_getRecPacketBufferLength00);
   tolua_function(tolua_S,"getRecPacketBuffer",tolua_Gamelua_PacketScriptHandler_getRecPacketBuffer00);
   tolua_function(tolua_S,"registerFunctionHandler",tolua_Gamelua_PacketScriptHandler_registerFunctionHandler00);
   tolua_variable(tolua_S,"__PacketHandler__",tolua_get_PacketScriptHandler___PacketHandler__,NULL);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"PacketManager","PacketManager","",NULL);
  tolua_beginmodule(tolua_S,"PacketManager");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_PacketManager_getInstance00);
   tolua_function(tolua_S,"disconnect",tolua_Gamelua_PacketManager_disconnect00);
   tolua_function(tolua_S,"registerPacketHandler",tolua_Gamelua_PacketManager_registerPacketHandler00);
   tolua_function(tolua_S,"nameToOpcode",tolua_Gamelua_PacketManager_nameToOpcode00);
   tolua_function(tolua_S,"removePacketHandler",tolua_Gamelua_PacketManager_removePacketHandler00);
   tolua_function(tolua_S,"removePacketHandler",tolua_Gamelua_PacketManager_removePacketHandler01);
   tolua_function(tolua_S,"sendPakcet",tolua_Gamelua_PacketManager_sendPakcet00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ScriptMathToLua","ScriptMathToLua","",NULL);
  tolua_beginmodule(tolua_S,"ScriptMathToLua");
   tolua_function(tolua_S,"showWXChat",tolua_Gamelua_ScriptMathToLua_showWXChat00);
   tolua_function(tolua_S,"setSwallowsTouches",tolua_Gamelua_ScriptMathToLua_setSwallowsTouches00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"MSG_LOADINGFRAME_ENTERGAME",MSG_LOADINGFRAME_ENTERGAME);
  tolua_constant(tolua_S,"MSG_MAINFRAME_CHANGEPAGE",MSG_MAINFRAME_CHANGEPAGE);
  tolua_constant(tolua_S,"MSG_MAINFRAME_PUSHPAGE",MSG_MAINFRAME_PUSHPAGE);
  tolua_constant(tolua_S,"MSG_MAINFRAME_POPPAGE",MSG_MAINFRAME_POPPAGE);
  tolua_constant(tolua_S,"MSG_MAINFRAME_COVERSHOW",MSG_MAINFRAME_COVERSHOW);
  tolua_constant(tolua_S,"MSG_MAINFRAME_COVERHIDE",MSG_MAINFRAME_COVERHIDE);
  tolua_constant(tolua_S,"MSG_MAINFRAME_MSGSHOW",MSG_MAINFRAME_MSGSHOW);
  tolua_constant(tolua_S,"MSG_CAREER_CHANGEPAGE",MSG_CAREER_CHANGEPAGE);
  tolua_constant(tolua_S,"MSG_FRIENDSPAGE_CHANGEPAGE",MSG_FRIENDSPAGE_CHANGEPAGE);
  tolua_constant(tolua_S,"MSG_LEAVEMSG_RECIEVEMSG",MSG_LEAVEMSG_RECIEVEMSG);
  tolua_constant(tolua_S,"MSG_FRIEND_DETAIL_RECICEVEMSG",MSG_FRIEND_DETAIL_RECICEVEMSG);
  tolua_constant(tolua_S,"MSG_BUY_PROP_POPUPMSG",MSG_BUY_PROP_POPUPMSG);
  tolua_constant(tolua_S,"MSG_FRIEND_DETAIL_DISPOSE",MSG_FRIEND_DETAIL_DISPOSE);
  tolua_constant(tolua_S,"MSG_CONTINUE_LOGIN_FLIPCARD",MSG_CONTINUE_LOGIN_FLIPCARD);
  tolua_constant(tolua_S,"MSG_ADVENTURE_PAGECHANGE",MSG_ADVENTURE_PAGECHANGE);
  tolua_constant(tolua_S,"MSG_SEVERINFO_UPDATE",MSG_SEVERINFO_UPDATE);
  tolua_constant(tolua_S,"MSG_MAINFRAME_REFRESH",MSG_MAINFRAME_REFRESH);
  tolua_constant(tolua_S,"MSG_GET_NEW_INFO",MSG_GET_NEW_INFO);
  tolua_constant(tolua_S,"MSG_ADVENTURE_REMOVEITEM",MSG_ADVENTURE_REMOVEITEM);
  tolua_constant(tolua_S,"MSG_ADVENTURE_TEACHEXPINIT",MSG_ADVENTURE_TEACHEXPINIT);
  tolua_constant(tolua_S,"MSG_PACKPREVIEW_SOURCE_MSG",MSG_PACKPREVIEW_SOURCE_MSG);
  tolua_constant(tolua_S,"MSG_INSTRUCTION_NEXT",MSG_INSTRUCTION_NEXT);
  tolua_constant(tolua_S,"MSG_LEVEL_INSTRUCTION_NEXT",MSG_LEVEL_INSTRUCTION_NEXT);
  tolua_constant(tolua_S,"MSG_FIGHT_END",MSG_FIGHT_END);
  tolua_constant(tolua_S,"MSG_BUTTON_PRESSED",MSG_BUTTON_PRESSED);
  tolua_constant(tolua_S,"MSG_TO_TAB",MSG_TO_TAB);
  tolua_constant(tolua_S,"MSG_FIGHTROUND_LOADSKILL",MSG_FIGHTROUND_LOADSKILL);
  tolua_constant(tolua_S,"MSG_RECHARGE_SUCCESS",MSG_RECHARGE_SUCCESS);
  tolua_constant(tolua_S,"MSG_FIGHTPAGE_EXIT",MSG_FIGHTPAGE_EXIT);
  tolua_constant(tolua_S,"MSG_TITLESTATUS_CHANGE",MSG_TITLESTATUS_CHANGE);
  tolua_constant(tolua_S,"MSG_ADVENTUREPAGE_HASCHANGE",MSG_ADVENTUREPAGE_HASCHANGE);
  tolua_constant(tolua_S,"MSG_UPDATE_LEAGUA_CONTRIBUTION",MSG_UPDATE_LEAGUA_CONTRIBUTION);
  tolua_constant(tolua_S,"MSG_CHAT_WINDOW_ONSWITCH",MSG_CHAT_WINDOW_ONSWITCH);
  tolua_constant(tolua_S,"MSG_REFRESH_DISCIPLEPAGE",MSG_REFRESH_DISCIPLEPAGE);
  tolua_constant(tolua_S,"MSG_GOTOPRIVATECHATPAGE",MSG_GOTOPRIVATECHATPAGE);
  tolua_constant(tolua_S,"MSG_GOTOPRIVATECHATPAGEFROMLEAGUE",MSG_GOTOPRIVATECHATPAGEFROMLEAGUE);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgLoadingFrameEnterGame","MsgLoadingFrameEnterGame","GameMessage",tolua_collect_MsgLoadingFrameEnterGame);
  #else
  tolua_cclass(tolua_S,"MsgLoadingFrameEnterGame","MsgLoadingFrameEnterGame","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgLoadingFrameEnterGame");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgLoadingFrameEnterGame_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgLoadingFrameEnterGame_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgLoadingFrameEnterGame_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgLoadingFrameEnterGame_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgLoadingFrameEnterGame_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgLoadingFrameEnterGame_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameChangePage","MsgMainFrameChangePage","GameMessage",tolua_collect_MsgMainFrameChangePage);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameChangePage","MsgMainFrameChangePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameChangePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameChangePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameChangePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameChangePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameChangePage_new00_local);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFrameChangePage_pageName,tolua_set_MsgMainFrameChangePage_pageName);
   tolua_variable(tolua_S,"needPopAllPage",tolua_get_MsgMainFrameChangePage_needPopAllPage,tolua_set_MsgMainFrameChangePage_needPopAllPage);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameChangePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameChangePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFramePushPage","MsgMainFramePushPage","GameMessage",tolua_collect_MsgMainFramePushPage);
  #else
  tolua_cclass(tolua_S,"MsgMainFramePushPage","MsgMainFramePushPage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFramePushPage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFramePushPage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFramePushPage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFramePushPage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFramePushPage_new00_local);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFramePushPage_pageName,tolua_set_MsgMainFramePushPage_pageName);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFramePushPage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFramePushPage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFramePopPage","MsgMainFramePopPage","GameMessage",tolua_collect_MsgMainFramePopPage);
  #else
  tolua_cclass(tolua_S,"MsgMainFramePopPage","MsgMainFramePopPage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFramePopPage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFramePopPage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFramePopPage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFramePopPage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFramePopPage_new00_local);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFramePopPage_pageName,tolua_set_MsgMainFramePopPage_pageName);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFramePopPage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFramePopPage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameMSGShow","MsgMainFrameMSGShow","GameMessage",tolua_collect_MsgMainFrameMSGShow);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameMSGShow","MsgMainFrameMSGShow","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameMSGShow");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameMSGShow_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameMSGShow_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameMSGShow_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameMSGShow_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameMSGShow_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameMSGShow_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameCoverShow","MsgMainFrameCoverShow","GameMessage",tolua_collect_MsgMainFrameCoverShow);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameCoverShow","MsgMainFrameCoverShow","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameCoverShow");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameCoverShow_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameCoverShow_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameCoverShow_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameCoverShow_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameCoverShow_getTypeId00);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFrameCoverShow_pageName,tolua_set_MsgMainFrameCoverShow_pageName);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameCoverShow_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameCoverHide","MsgMainFrameCoverHide","GameMessage",tolua_collect_MsgMainFrameCoverHide);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameCoverHide","MsgMainFrameCoverHide","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameCoverHide");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameCoverHide_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameCoverHide_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameCoverHide_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameCoverHide_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameCoverHide_getTypeId00);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFrameCoverHide_pageName,tolua_set_MsgMainFrameCoverHide_pageName);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameCoverHide_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgCareerChangePage","MsgCareerChangePage","GameMessage",tolua_collect_MsgCareerChangePage);
  #else
  tolua_cclass(tolua_S,"MsgCareerChangePage","MsgCareerChangePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgCareerChangePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgCareerChangePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgCareerChangePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgCareerChangePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgCareerChangePage_new00_local);
   tolua_variable(tolua_S,"pageID",tolua_get_MsgCareerChangePage_pageID,tolua_set_MsgCareerChangePage_pageID);
   tolua_function(tolua_S,"getParameters",tolua_Gamelua_MsgCareerChangePage_getParameters00);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgCareerChangePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgCareerChangePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFriendsPageChangePage","MsgFriendsPageChangePage","GameMessage",tolua_collect_MsgFriendsPageChangePage);
  #else
  tolua_cclass(tolua_S,"MsgFriendsPageChangePage","MsgFriendsPageChangePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFriendsPageChangePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFriendsPageChangePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFriendsPageChangePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFriendsPageChangePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFriendsPageChangePage_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgFriendsPageChangePage_index,tolua_set_MsgFriendsPageChangePage_index);
   tolua_variable(tolua_S,"oprType",tolua_get_MsgFriendsPageChangePage_oprType,tolua_set_MsgFriendsPageChangePage_oprType);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFriendsPageChangePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFriendsPageChangePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgGoToPrivateChatPage","MsgGoToPrivateChatPage","GameMessage",tolua_collect_MsgGoToPrivateChatPage);
  #else
  tolua_cclass(tolua_S,"MsgGoToPrivateChatPage","MsgGoToPrivateChatPage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgGoToPrivateChatPage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgGoToPrivateChatPage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgGoToPrivateChatPage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgGoToPrivateChatPage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgGoToPrivateChatPage_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgGoToPrivateChatPage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgGoToPrivateChatPage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgGoToPrivateChatPageFromLeague","MsgGoToPrivateChatPageFromLeague","GameMessage",tolua_collect_MsgGoToPrivateChatPageFromLeague);
  #else
  tolua_cclass(tolua_S,"MsgGoToPrivateChatPageFromLeague","MsgGoToPrivateChatPageFromLeague","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgGoToPrivateChatPageFromLeague");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgGoToPrivateChatPageFromLeague_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgLeaveMsgReceivePage","MsgLeaveMsgReceivePage","GameMessage",tolua_collect_MsgLeaveMsgReceivePage);
  #else
  tolua_cclass(tolua_S,"MsgLeaveMsgReceivePage","MsgLeaveMsgReceivePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgLeaveMsgReceivePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgLeaveMsgReceivePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgLeaveMsgReceivePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgLeaveMsgReceivePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgLeaveMsgReceivePage_new00_local);
   tolua_variable(tolua_S,"uID",tolua_get_MsgLeaveMsgReceivePage_uID,tolua_set_MsgLeaveMsgReceivePage_uID);
   tolua_variable(tolua_S,"uName",tolua_get_MsgLeaveMsgReceivePage_uName,tolua_set_MsgLeaveMsgReceivePage_uName);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgLeaveMsgReceivePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgLeaveMsgReceivePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFriendsDetailReceivePage","MsgFriendsDetailReceivePage","GameMessage",tolua_collect_MsgFriendsDetailReceivePage);
  #else
  tolua_cclass(tolua_S,"MsgFriendsDetailReceivePage","MsgFriendsDetailReceivePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFriendsDetailReceivePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFriendsDetailReceivePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFriendsDetailReceivePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFriendsDetailReceivePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFriendsDetailReceivePage_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgFriendsDetailReceivePage_index,tolua_set_MsgFriendsDetailReceivePage_index);
   tolua_variable(tolua_S,"uID",tolua_get_MsgFriendsDetailReceivePage_uID,tolua_set_MsgFriendsDetailReceivePage_uID);
   tolua_variable(tolua_S,"type",tolua_get_MsgFriendsDetailReceivePage_type,tolua_set_MsgFriendsDetailReceivePage_type);
   tolua_variable(tolua_S,"isAttention",tolua_get_MsgFriendsDetailReceivePage_isAttention,tolua_set_MsgFriendsDetailReceivePage_isAttention);
   tolua_variable(tolua_S,"uName",tolua_get_MsgFriendsDetailReceivePage_uName,tolua_set_MsgFriendsDetailReceivePage_uName);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFriendsDetailReceivePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFriendsDetailReceivePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFriendsDetailDisposePage","MsgFriendsDetailDisposePage","GameMessage",tolua_collect_MsgFriendsDetailDisposePage);
  #else
  tolua_cclass(tolua_S,"MsgFriendsDetailDisposePage","MsgFriendsDetailDisposePage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFriendsDetailDisposePage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFriendsDetailDisposePage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFriendsDetailDisposePage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFriendsDetailDisposePage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFriendsDetailDisposePage_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgFriendsDetailDisposePage_index,tolua_set_MsgFriendsDetailDisposePage_index);
   tolua_variable(tolua_S,"oprType",tolua_get_MsgFriendsDetailDisposePage_oprType,tolua_set_MsgFriendsDetailDisposePage_oprType);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFriendsDetailDisposePage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFriendsDetailDisposePage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgBuyPropPopupPage","MsgBuyPropPopupPage","GameMessage",tolua_collect_MsgBuyPropPopupPage);
  #else
  tolua_cclass(tolua_S,"MsgBuyPropPopupPage","MsgBuyPropPopupPage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgBuyPropPopupPage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgBuyPropPopupPage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgBuyPropPopupPage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgBuyPropPopupPage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgBuyPropPopupPage_new00_local);
   tolua_variable(tolua_S,"toolId",tolua_get_MsgBuyPropPopupPage_unsigned_toolId,tolua_set_MsgBuyPropPopupPage_unsigned_toolId);
   tolua_variable(tolua_S,"panelIndex",tolua_get_MsgBuyPropPopupPage_unsigned_panelIndex,tolua_set_MsgBuyPropPopupPage_unsigned_panelIndex);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgBuyPropPopupPage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgBuyPropPopupPage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgSeverInfoUpdate","MsgSeverInfoUpdate","GameMessage",tolua_collect_MsgSeverInfoUpdate);
  #else
  tolua_cclass(tolua_S,"MsgSeverInfoUpdate","MsgSeverInfoUpdate","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgSeverInfoUpdate");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgSeverInfoUpdate_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgSeverInfoUpdate_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgSeverInfoUpdate_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgSeverInfoUpdate_new00_local);
   tolua_variable(tolua_S,"opcode",tolua_get_MsgSeverInfoUpdate_unsigned_opcode,tolua_set_MsgSeverInfoUpdate_unsigned_opcode);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgSeverInfoUpdate_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgSeverInfoUpdate_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgAdventurePageChange","MsgAdventurePageChange","GameMessage",tolua_collect_MsgAdventurePageChange);
  #else
  tolua_cclass(tolua_S,"MsgAdventurePageChange","MsgAdventurePageChange","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgAdventurePageChange");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgAdventurePageChange_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgAdventurePageChange_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgAdventurePageChange_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgAdventurePageChange_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgAdventurePageChange_unsigned_index,tolua_set_MsgAdventurePageChange_unsigned_index);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgAdventurePageChange_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgAdventurePageChange_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameRefreshPage","MsgMainFrameRefreshPage","GameMessage",tolua_collect_MsgMainFrameRefreshPage);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameRefreshPage","MsgMainFrameRefreshPage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameRefreshPage");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameRefreshPage_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameRefreshPage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameRefreshPage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameRefreshPage_new00_local);
   tolua_variable(tolua_S,"pageName",tolua_get_MsgMainFrameRefreshPage_pageName,tolua_set_MsgMainFrameRefreshPage_pageName);
   tolua_variable(tolua_S,"extraParam",tolua_get_MsgMainFrameRefreshPage_extraParam,tolua_set_MsgMainFrameRefreshPage_extraParam);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameRefreshPage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameRefreshPage_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgMainFrameGetNewInfo","MsgMainFrameGetNewInfo","GameMessage",tolua_collect_MsgMainFrameGetNewInfo);
  #else
  tolua_cclass(tolua_S,"MsgMainFrameGetNewInfo","MsgMainFrameGetNewInfo","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgMainFrameGetNewInfo");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgMainFrameGetNewInfo_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgMainFrameGetNewInfo_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgMainFrameGetNewInfo_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgMainFrameGetNewInfo_new00_local);
   tolua_variable(tolua_S,"type",tolua_get_MsgMainFrameGetNewInfo_type,tolua_set_MsgMainFrameGetNewInfo_type);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgMainFrameGetNewInfo_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgMainFrameGetNewInfo_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgAdventureRemoveItem","MsgAdventureRemoveItem","GameMessage",tolua_collect_MsgAdventureRemoveItem);
  #else
  tolua_cclass(tolua_S,"MsgAdventureRemoveItem","MsgAdventureRemoveItem","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgAdventureRemoveItem");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgAdventureRemoveItem_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgAdventureRemoveItem_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgAdventureRemoveItem_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgAdventureRemoveItem_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgAdventureRemoveItem_unsigned_index,tolua_set_MsgAdventureRemoveItem_unsigned_index);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgAdventureRemoveItem_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgAdventureRemoveItem_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgInstructionNext","MsgInstructionNext","GameMessage",tolua_collect_MsgInstructionNext);
  #else
  tolua_cclass(tolua_S,"MsgInstructionNext","MsgInstructionNext","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgInstructionNext");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgInstructionNext_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgInstructionNext_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgInstructionNext_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgInstructionNext_new00_local);
   tolua_variable(tolua_S,"tag",tolua_get_MsgInstructionNext_unsigned_tag,tolua_set_MsgInstructionNext_unsigned_tag);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgInstructionNext_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgInstructionNext_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFightEnd","MsgFightEnd","GameMessage",tolua_collect_MsgFightEnd);
  #else
  tolua_cclass(tolua_S,"MsgFightEnd","MsgFightEnd","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFightEnd");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFightEnd_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFightEnd_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFightEnd_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFightEnd_new00_local);
   tolua_variable(tolua_S,"gotItem",tolua_get_MsgFightEnd_gotItem,tolua_set_MsgFightEnd_gotItem);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFightEnd_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFightEnd_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgButtonPressed","MsgButtonPressed","GameMessage",tolua_collect_MsgButtonPressed);
  #else
  tolua_cclass(tolua_S,"MsgButtonPressed","MsgButtonPressed","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgButtonPressed");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgButtonPressed_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgButtonPressed_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgButtonPressed_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgButtonPressed_new00_local);
   tolua_variable(tolua_S,"ccb",tolua_get_MsgButtonPressed_ccb,tolua_set_MsgButtonPressed_ccb);
   tolua_variable(tolua_S,"func",tolua_get_MsgButtonPressed_func,tolua_set_MsgButtonPressed_func);
   tolua_variable(tolua_S,"tag",tolua_get_MsgButtonPressed_tag,tolua_set_MsgButtonPressed_tag);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgButtonPressed_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgButtonPressed_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgToTab","MsgToTab","GameMessage",tolua_collect_MsgToTab);
  #else
  tolua_cclass(tolua_S,"MsgToTab","MsgToTab","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgToTab");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgToTab_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgToTab_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgToTab_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgToTab_new00_local);
   tolua_variable(tolua_S,"tabIndex",tolua_get_MsgToTab_unsigned_tabIndex,tolua_set_MsgToTab_unsigned_tabIndex);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgToTab_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgToTab_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFightRoundLoadSkill","MsgFightRoundLoadSkill","GameMessage",tolua_collect_MsgFightRoundLoadSkill);
  #else
  tolua_cclass(tolua_S,"MsgFightRoundLoadSkill","MsgFightRoundLoadSkill","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFightRoundLoadSkill");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFightRoundLoadSkill_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFightRoundLoadSkill_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFightRoundLoadSkill_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFightRoundLoadSkill_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFightRoundLoadSkill_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFightRoundLoadSkill_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgRechargeSuccess","MsgRechargeSuccess","GameMessage",tolua_collect_MsgRechargeSuccess);
  #else
  tolua_cclass(tolua_S,"MsgRechargeSuccess","MsgRechargeSuccess","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgRechargeSuccess");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgRechargeSuccess_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgRechargeSuccess_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgRechargeSuccess_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgRechargeSuccess_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgRechargeSuccess_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgRechargeSuccess_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgFightPageExit","MsgFightPageExit","GameMessage",tolua_collect_MsgFightPageExit);
  #else
  tolua_cclass(tolua_S,"MsgFightPageExit","MsgFightPageExit","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgFightPageExit");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgFightPageExit_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgFightPageExit_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgFightPageExit_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgFightPageExit_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgFightPageExit_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgFightPageExit_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgTitleStatusChange","MsgTitleStatusChange","GameMessage",tolua_collect_MsgTitleStatusChange);
  #else
  tolua_cclass(tolua_S,"MsgTitleStatusChange","MsgTitleStatusChange","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgTitleStatusChange");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgTitleStatusChange_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgTitleStatusChange_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgTitleStatusChange_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgTitleStatusChange_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgTitleStatusChange_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgTitleStatusChange_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgAdventurePageHasChange","MsgAdventurePageHasChange","GameMessage",tolua_collect_MsgAdventurePageHasChange);
  #else
  tolua_cclass(tolua_S,"MsgAdventurePageHasChange","MsgAdventurePageHasChange","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgAdventurePageHasChange");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgAdventurePageHasChange_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgAdventurePageHasChange_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgAdventurePageHasChange_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgAdventurePageHasChange_new00_local);
   tolua_variable(tolua_S,"index",tolua_get_MsgAdventurePageHasChange_unsigned_index,tolua_set_MsgAdventurePageHasChange_unsigned_index);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgAdventurePageHasChange_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgAdventurePageHasChange_clone00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MsgUpdateLeaguaContribution","MsgUpdateLeaguaContribution","GameMessage",tolua_collect_MsgUpdateLeaguaContribution);
  #else
  tolua_cclass(tolua_S,"MsgUpdateLeaguaContribution","MsgUpdateLeaguaContribution","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MsgUpdateLeaguaContribution");
   tolua_function(tolua_S,"getTrueType",tolua_Gamelua_MsgUpdateLeaguaContribution_getTrueType00);
   tolua_function(tolua_S,"new",tolua_Gamelua_MsgUpdateLeaguaContribution_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MsgUpdateLeaguaContribution_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MsgUpdateLeaguaContribution_new00_local);
   tolua_variable(tolua_S,"leftContribution",tolua_get_MsgUpdateLeaguaContribution_unsigned_leftContribution,tolua_set_MsgUpdateLeaguaContribution_unsigned_leftContribution);
   tolua_variable(tolua_S,"totalContribution",tolua_get_MsgUpdateLeaguaContribution_unsigned_totalContribution,tolua_set_MsgUpdateLeaguaContribution_unsigned_totalContribution);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_MsgUpdateLeaguaContribution_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_MsgUpdateLeaguaContribution_clone00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameMessage","GameMessage","cocos2d::CCObject",NULL);
  tolua_beginmodule(tolua_S,"GameMessage");
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_GameMessage_getTypeId00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GameScriptMessage","GameScriptMessage","GameMessage",tolua_collect_GameScriptMessage);
  #else
  tolua_cclass(tolua_S,"GameScriptMessage","GameScriptMessage","GameMessage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GameScriptMessage");
   tolua_function(tolua_S,"new",tolua_Gamelua_GameScriptMessage_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_GameScriptMessage_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_GameScriptMessage_new00_local);
   tolua_function(tolua_S,"getTypeId",tolua_Gamelua_GameScriptMessage_getTypeId00);
   tolua_function(tolua_S,"clone",tolua_Gamelua_GameScriptMessage_clone00);
   tolua_variable(tolua_S,"mTypeId",tolua_get_GameScriptMessage_unsigned_mTypeId,tolua_set_GameScriptMessage_unsigned_mTypeId);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MessageHandler","MessageHandler","",NULL);
  tolua_beginmodule(tolua_S,"MessageHandler");
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MessageScriptHandler","MessageScriptHandler","MessageHandler",tolua_collect_MessageScriptHandler);
  #else
  tolua_cclass(tolua_S,"MessageScriptHandler","MessageScriptHandler","MessageHandler",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MessageScriptHandler");
   tolua_function(tolua_S,"new",tolua_Gamelua_MessageScriptHandler_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_MessageScriptHandler_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_MessageScriptHandler_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_MessageScriptHandler_delete00);
   tolua_function(tolua_S,"registerFunctionHandler",tolua_Gamelua_MessageScriptHandler_registerFunctionHandler00);
   tolua_function(tolua_S,"unregisterFunctionHandler",tolua_Gamelua_MessageScriptHandler_unregisterFunctionHandler00);
   tolua_function(tolua_S,"onReceiveMassage",tolua_Gamelua_MessageScriptHandler_onReceiveMassage00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MessageManager","MessageManager","",NULL);
  tolua_beginmodule(tolua_S,"MessageManager");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_MessageManager_getInstance00);
   tolua_function(tolua_S,"update",tolua_Gamelua_MessageManager_update00);
   tolua_function(tolua_S,"sendMessageForScript",tolua_Gamelua_MessageManager_sendMessageForScript00);
   tolua_function(tolua_S,"regisiterMessageHandler",tolua_Gamelua_MessageManager_regisiterMessageHandler00);
   tolua_function(tolua_S,"removeMessageHandler",tolua_Gamelua_MessageManager_removeMessageHandler00);
   tolua_function(tolua_S,"removeMessageHandler",tolua_Gamelua_MessageManager_removeMessageHandler01);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"libOS","libOS","",NULL);
  tolua_beginmodule(tolua_S,"libOS");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_libOS_getInstance00);
   tolua_function(tolua_S,"avalibleMemory",tolua_Gamelua_libOS_avalibleMemory00);
   tolua_function(tolua_S,"rmdir",tolua_Gamelua_libOS_rmdir00);
   tolua_function(tolua_S,"generateSerial",tolua_Gamelua_libOS_generateSerial00);
   tolua_function(tolua_S,"showInputbox",tolua_Gamelua_libOS_showInputbox00);
   tolua_function(tolua_S,"showMessagebox",tolua_Gamelua_libOS_showMessagebox00);
   tolua_function(tolua_S,"openURL",tolua_Gamelua_libOS_openURL00);
   tolua_function(tolua_S,"emailTo",tolua_Gamelua_libOS_emailTo00);
   tolua_function(tolua_S,"setWaiting",tolua_Gamelua_libOS_setWaiting00);
   tolua_function(tolua_S,"getFreeSpace",tolua_Gamelua_libOS_getFreeSpace00);
   tolua_function(tolua_S,"getNetWork",tolua_Gamelua_libOS_getNetWork00);
   tolua_function(tolua_S,"clearNotification",tolua_Gamelua_libOS_clearNotification00);
   tolua_function(tolua_S,"addNotification",tolua_Gamelua_libOS_addNotification00);
   tolua_function(tolua_S,"getDeviceID",tolua_Gamelua_libOS_getDeviceID00);
   tolua_function(tolua_S,"getPlatformInfo",tolua_Gamelua_libOS_getPlatformInfo00);
   tolua_function(tolua_S,"analyticsLogEvent",tolua_Gamelua_libOS_analyticsLogEvent00);
   tolua_function(tolua_S,"analyticsLogEvent",tolua_Gamelua_libOS_analyticsLogEvent01);
   tolua_function(tolua_S,"analyticsLogEndTimeEvent",tolua_Gamelua_libOS_analyticsLogEndTimeEvent00);
   tolua_function(tolua_S,"WeChatInit",tolua_Gamelua_libOS_WeChatInit00);
   tolua_function(tolua_S,"WeChatIsInstalled",tolua_Gamelua_libOS_WeChatIsInstalled00);
   tolua_function(tolua_S,"WeChatInstall",tolua_Gamelua_libOS_WeChatInstall00);
   tolua_function(tolua_S,"WeChatOpen",tolua_Gamelua_libOS_WeChatOpen00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"BUYINFO","BUYINFO","",tolua_collect_BUYINFO);
  #else
  tolua_cclass(tolua_S,"BUYINFO","BUYINFO","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"BUYINFO");
   tolua_variable(tolua_S,"cooOrderSerial",tolua_get_BUYINFO_cooOrderSerial,tolua_set_BUYINFO_cooOrderSerial);
   tolua_variable(tolua_S,"productId",tolua_get_BUYINFO_productId,tolua_set_BUYINFO_productId);
   tolua_variable(tolua_S,"productName",tolua_get_BUYINFO_productName,tolua_set_BUYINFO_productName);
   tolua_variable(tolua_S,"productPrice",tolua_get_BUYINFO_productPrice,tolua_set_BUYINFO_productPrice);
   tolua_variable(tolua_S,"productOrignalPrice",tolua_get_BUYINFO_productOrignalPrice,tolua_set_BUYINFO_productOrignalPrice);
   tolua_variable(tolua_S,"productCount",tolua_get_BUYINFO_unsigned_productCount,tolua_set_BUYINFO_unsigned_productCount);
   tolua_variable(tolua_S,"description",tolua_get_BUYINFO_description,tolua_set_BUYINFO_description);
   tolua_function(tolua_S,"new",tolua_Gamelua_BUYINFO_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_BUYINFO_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_BUYINFO_new00_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"libPlatform","libPlatform","",NULL);
  tolua_beginmodule(tolua_S,"libPlatform");
   tolua_function(tolua_S,"openBBS",tolua_Gamelua_libPlatform_openBBS00);
   tolua_function(tolua_S,"userFeedBack",tolua_Gamelua_libPlatform_userFeedBack00);
   tolua_function(tolua_S,"gamePause",tolua_Gamelua_libPlatform_gamePause00);
   tolua_function(tolua_S,"onShare",tolua_Gamelua_libPlatform_onShare00);
   tolua_function(tolua_S,"onBinding",tolua_Gamelua_libPlatform_onBinding00);
   tolua_function(tolua_S,"onGuest",tolua_Gamelua_libPlatform_onGuest00);
   tolua_function(tolua_S,"getLogined",tolua_Gamelua_libPlatform_getLogined00);
   tolua_function(tolua_S,"loginUin",tolua_Gamelua_libPlatform_loginUin00);
   tolua_function(tolua_S,"sessionID",tolua_Gamelua_libPlatform_sessionID00);
   tolua_function(tolua_S,"nickName",tolua_Gamelua_libPlatform_nickName00);
   tolua_function(tolua_S,"getPlatformInfo",tolua_Gamelua_libPlatform_getPlatformInfo00);
   tolua_function(tolua_S,"getClientChannel",tolua_Gamelua_libPlatform_getClientChannel00);
   tolua_function(tolua_S,"getPlatformId",tolua_Gamelua_libPlatform_getPlatformId00);
   tolua_function(tolua_S,"buyGoods",tolua_Gamelua_libPlatform_buyGoods00);
   tolua_function(tolua_S,"switchUsers",tolua_Gamelua_libPlatform_switchUsers00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"libPlatformManager","libPlatformManager","",NULL);
  tolua_beginmodule(tolua_S,"libPlatformManager");
   tolua_function(tolua_S,"setPlatform",tolua_Gamelua_libPlatformManager_setPlatform00);
   tolua_function(tolua_S,"getPlatform",tolua_Gamelua_libPlatformManager_getPlatform00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_libPlatformManager_getInstance00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"RestrictedWord","RestrictedWord","",NULL);
  tolua_beginmodule(tolua_S,"RestrictedWord");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_RestrictedWord_getInstance00);
   tolua_function(tolua_S,"isStringOK",tolua_Gamelua_RestrictedWord_isStringOK00);
   tolua_function(tolua_S,"isSecretOK",tolua_Gamelua_RestrictedWord_isSecretOK00);
   tolua_function(tolua_S,"isChatWorldStringOK",tolua_Gamelua_RestrictedWord_isChatWorldStringOK00);
   tolua_function(tolua_S,"filterWordSentence",tolua_Gamelua_RestrictedWord_filterWordSentence00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GamePrecedure","GamePrecedure","",NULL);
  tolua_beginmodule(tolua_S,"GamePrecedure");
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_GamePrecedure_getInstance00);
   tolua_function(tolua_S,"init",tolua_Gamelua_GamePrecedure_init00);
   tolua_function(tolua_S,"unInit",tolua_Gamelua_GamePrecedure_unInit00);
   tolua_function(tolua_S,"enterLoading",tolua_Gamelua_GamePrecedure_enterLoading00);
   tolua_function(tolua_S,"reEnterLoading",tolua_Gamelua_GamePrecedure_reEnterLoading00);
   tolua_function(tolua_S,"preEnterMainMenu",tolua_Gamelua_GamePrecedure_preEnterMainMenu00);
   tolua_function(tolua_S,"readTables",tolua_Gamelua_GamePrecedure_readTables00);
   tolua_function(tolua_S,"enterMainMenu",tolua_Gamelua_GamePrecedure_enterMainMenu00);
   tolua_function(tolua_S,"enterGame",tolua_Gamelua_GamePrecedure_enterGame00);
   tolua_function(tolua_S,"enterInstruction",tolua_Gamelua_GamePrecedure_enterInstruction00);
   tolua_function(tolua_S,"exitGame",tolua_Gamelua_GamePrecedure_exitGame00);
   tolua_function(tolua_S,"isInLoadingSceneAndNeedExit",tolua_Gamelua_GamePrecedure_isInLoadingSceneAndNeedExit00);
   tolua_function(tolua_S,"isInLoadingScene",tolua_Gamelua_GamePrecedure_isInLoadingScene00);
   tolua_function(tolua_S,"getFrameTime",tolua_Gamelua_GamePrecedure_getFrameTime00);
   tolua_function(tolua_S,"getTotalTime",tolua_Gamelua_GamePrecedure_getTotalTime00);
   tolua_function(tolua_S,"setServerID",tolua_Gamelua_GamePrecedure_setServerID00);
   tolua_function(tolua_S,"getServerID",tolua_Gamelua_GamePrecedure_getServerID00);
   tolua_function(tolua_S,"getServerTime",tolua_Gamelua_GamePrecedure_getServerTime00);
   tolua_function(tolua_S,"setServerTime",tolua_Gamelua_GamePrecedure_setServerTime00);
   tolua_function(tolua_S,"setUin",tolua_Gamelua_GamePrecedure_setUin00);
   tolua_function(tolua_S,"getUin",tolua_Gamelua_GamePrecedure_getUin00);
   tolua_function(tolua_S,"hasAlertServerUpdating",tolua_Gamelua_GamePrecedure_hasAlertServerUpdating00);
   tolua_function(tolua_S,"setAlertServerUpdating",tolua_Gamelua_GamePrecedure_setAlertServerUpdating00);
   tolua_function(tolua_S,"gameSnapshot",tolua_Gamelua_GamePrecedure_gameSnapshot00);
   tolua_function(tolua_S,"showBulletin",tolua_Gamelua_GamePrecedure_showBulletin00);
   tolua_function(tolua_S,"closeBulletin",tolua_Gamelua_GamePrecedure_closeBulletin00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CShiftMaskToXAction","CShiftMaskToXAction","CCActionInterval",NULL);
  tolua_beginmodule(tolua_S,"CShiftMaskToXAction");
   tolua_function(tolua_S,"create",tolua_Gamelua_CShiftMaskToXAction_create00);
   tolua_function(tolua_S,"initWithDuration",tolua_Gamelua_CShiftMaskToXAction_initWithDuration00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CShiftObjectToXAction","CShiftObjectToXAction","CCActionInterval",NULL);
  tolua_beginmodule(tolua_S,"CShiftObjectToXAction");
   tolua_function(tolua_S,"create",tolua_Gamelua_CShiftObjectToXAction_create00);
   tolua_function(tolua_S,"initWithDuration",tolua_Gamelua_CShiftObjectToXAction_initWithDuration00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CShiftMaskToYAction","CShiftMaskToYAction","CCActionInterval",NULL);
  tolua_beginmodule(tolua_S,"CShiftMaskToYAction");
   tolua_function(tolua_S,"create",tolua_Gamelua_CShiftMaskToYAction_create00);
   tolua_function(tolua_S,"initWithDuration",tolua_Gamelua_CShiftMaskToYAction_initWithDuration00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CShiftObjectToYAction","CShiftObjectToYAction","CCActionInterval",NULL);
  tolua_beginmodule(tolua_S,"CShiftObjectToYAction");
   tolua_function(tolua_S,"create",tolua_Gamelua_CShiftObjectToYAction_create00);
   tolua_function(tolua_S,"initWithDuration",tolua_Gamelua_CShiftObjectToYAction_initWithDuration00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ShiftToManager","ShiftToManager","",NULL);
  tolua_beginmodule(tolua_S,"ShiftToManager");
   tolua_function(tolua_S,"ExpSetObjectXPos",tolua_Gamelua_ShiftToManager_ExpSetObjectXPos00);
   tolua_function(tolua_S,"ExpShiftObjectToXPos",tolua_Gamelua_ShiftToManager_ExpShiftObjectToXPos00);
   tolua_function(tolua_S,"ExpSetMaskYPos",tolua_Gamelua_ShiftToManager_ExpSetMaskYPos00);
   tolua_function(tolua_S,"ExpShiftMaskToYPos",tolua_Gamelua_ShiftToManager_ExpShiftMaskToYPos00);
   tolua_function(tolua_S,"ExpSetObjectYPos",tolua_Gamelua_ShiftToManager_ExpSetObjectYPos00);
   tolua_function(tolua_S,"ExpShiftObjectToYPos",tolua_Gamelua_ShiftToManager_ExpShiftObjectToYPos00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"AnimMgr","AnimMgr","",NULL);
  tolua_beginmodule(tolua_S,"AnimMgr");
   tolua_function(tolua_S,"fadeInAndOut",tolua_Gamelua_AnimMgr_fadeInAndOut00);
   tolua_function(tolua_S,"scrollViewMoveIn",tolua_Gamelua_AnimMgr_scrollViewMoveIn00);
   tolua_function(tolua_S,"getInstance",tolua_Gamelua_AnimMgr_getInstance00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CCBFileNew","CCBFileNew","CCNode",tolua_collect_CCBFileNew);
  #else
  tolua_cclass(tolua_S,"CCBFileNew","CCBFileNew","CCNode",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CCBFileNew");
   tolua_function(tolua_S,"new",tolua_Gamelua_CCBFileNew_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_CCBFileNew_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_CCBFileNew_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_CCBFileNew_delete00);
   tolua_function(tolua_S,"init",tolua_Gamelua_CCBFileNew_init00);
   tolua_function(tolua_S,"setCCBFileName",tolua_Gamelua_CCBFileNew_setCCBFileName00);
   tolua_function(tolua_S,"getCCBFileName",tolua_Gamelua_CCBFileNew_getCCBFileName00);
   tolua_function(tolua_S,"load",tolua_Gamelua_CCBFileNew_load00);
   tolua_function(tolua_S,"unload",tolua_Gamelua_CCBFileNew_unload00);
   tolua_function(tolua_S,"getLoaded",tolua_Gamelua_CCBFileNew_getLoaded00);
   tolua_function(tolua_S,"setParentCCBFileNode",tolua_Gamelua_CCBFileNew_setParentCCBFileNode00);
   tolua_function(tolua_S,"getParentCCBFileNode",tolua_Gamelua_CCBFileNew_getParentCCBFileNode00);
   tolua_function(tolua_S,"setListener",tolua_Gamelua_CCBFileNew_setListener00);
   tolua_function(tolua_S,"getListener",tolua_Gamelua_CCBFileNew_getListener00);
   tolua_function(tolua_S,"runAnimation",tolua_Gamelua_CCBFileNew_runAnimation00);
   tolua_function(tolua_S,"hasAnimation",tolua_Gamelua_CCBFileNew_hasAnimation00);
   tolua_function(tolua_S,"getAnimationLength",tolua_Gamelua_CCBFileNew_getAnimationLength00);
   tolua_function(tolua_S,"getCompletedAnimationName",tolua_Gamelua_CCBFileNew_getCompletedAnimationName00);
   tolua_function(tolua_S,"getVariable",tolua_Gamelua_CCBFileNew_getVariable00);
   tolua_function(tolua_S,"getCCBFileNode",tolua_Gamelua_CCBFileNew_getCCBFileNode00);
   tolua_function(tolua_S,"playAutoPlaySequence",tolua_Gamelua_CCBFileNew_playAutoPlaySequence00);
   tolua_function(tolua_S,"registerFunctionHandler",tolua_Gamelua_CCBFileNew_registerFunctionHandler00);
   tolua_function(tolua_S,"unregisterFunctionHandler",tolua_Gamelua_CCBFileNew_unregisterFunctionHandler00);
   tolua_function(tolua_S,"dumpInfo",tolua_Gamelua_CCBFileNew_dumpInfo00);
   tolua_function(tolua_S,"cleanup",tolua_Gamelua_CCBFileNew_cleanup00);
   tolua_variable(tolua_S,"__CCBSelectorResolver__",tolua_get_CCBFileNew___CCBSelectorResolver__,NULL);
   tolua_variable(tolua_S,"__CCBMemberVariableAssigner__",tolua_get_CCBFileNew___CCBMemberVariableAssigner__,NULL);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CoverSprite","CoverSprite","CCSprite",tolua_collect_CoverSprite);
  #else
  tolua_cclass(tolua_S,"CoverSprite","CoverSprite","CCSprite",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CoverSprite");
   tolua_function(tolua_S,"new",tolua_Gamelua_CoverSprite_new00);
   tolua_function(tolua_S,"new_local",tolua_Gamelua_CoverSprite_new00_local);
   tolua_function(tolua_S,".call",tolua_Gamelua_CoverSprite_new00_local);
   tolua_function(tolua_S,"delete",tolua_Gamelua_CoverSprite_delete00);
   tolua_function(tolua_S,"initWithTexture",tolua_Gamelua_CoverSprite_initWithTexture00);
   tolua_function(tolua_S,"draw",tolua_Gamelua_CoverSprite_draw00);
   tolua_function(tolua_S,"initProgram",tolua_Gamelua_CoverSprite_initProgram00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SpineContainer","SpineContainer","",NULL);
  tolua_beginmodule(tolua_S,"SpineContainer");
   tolua_function(tolua_S,"create",tolua_Gamelua_SpineContainer_create00);
   tolua_function(tolua_S,"runAnimation",tolua_Gamelua_SpineContainer_runAnimation00);
   tolua_function(tolua_S,"setListener",tolua_Gamelua_SpineContainer_setListener00);
   tolua_function(tolua_S,"stopAllAnimations",tolua_Gamelua_SpineContainer_stopAllAnimations00);
   tolua_function(tolua_S,"stopAnimationByIndex",tolua_Gamelua_SpineContainer_stopAnimationByIndex00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ArmatureContainer","ArmatureContainer","CCArmature",NULL);
  tolua_beginmodule(tolua_S,"ArmatureContainer");
   tolua_function(tolua_S,"create",tolua_Gamelua_ArmatureContainer_create00);
   tolua_function(tolua_S,"runAnimation",tolua_Gamelua_ArmatureContainer_runAnimation00);
   tolua_function(tolua_S,"changeSkin",tolua_Gamelua_ArmatureContainer_changeSkin00);
   tolua_function(tolua_S,"changeSkin",tolua_Gamelua_ArmatureContainer_changeSkin01);
   tolua_function(tolua_S,"setListener",tolua_Gamelua_ArmatureContainer_setListener00);
   tolua_function(tolua_S,"setResourcePath",tolua_Gamelua_ArmatureContainer_setResourcePath00);
   tolua_function(tolua_S,"getAnimationTime",tolua_Gamelua_ArmatureContainer_getAnimationTime00);
   tolua_function(tolua_S,"addBone",tolua_Gamelua_ArmatureContainer_addBone00);
   tolua_function(tolua_S,"addBonePlay",tolua_Gamelua_ArmatureContainer_addBonePlay00);
   tolua_function(tolua_S,"getResourcePath",tolua_Gamelua_ArmatureContainer_getResourcePath00);
   tolua_function(tolua_S,"clearResource",tolua_Gamelua_ArmatureContainer_clearResource00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"get_systemfont_path",tolua_Gamelua_get_systemfont_path00);
  tolua_function(tolua_S,"get_system_default_ppi",tolua_Gamelua_get_system_default_ppi00);
  tolua_function(tolua_S,"get_prefered_default_fontsize",tolua_Gamelua_get_prefered_default_fontsize00);
  tolua_function(tolua_S,"get_system_default_fontfile",tolua_Gamelua_get_system_default_fontfile00);
  tolua_function(tolua_S,"get_system_helvetica_fontfile",tolua_Gamelua_get_system_helvetica_fontfile00);
  tolua_function(tolua_S,"get_system_helveticabd_fontfile",tolua_Gamelua_get_system_helveticabd_fontfile00);
  tolua_function(tolua_S,"get_system_default_hacklatin_fontfile",tolua_Gamelua_get_system_default_hacklatin_fontfile00);
  tolua_function(tolua_S,"dfont_default_initialize",tolua_Gamelua_dfont_default_initialize00);
  tolua_function(tolua_S,"latin_charset",tolua_Gamelua_latin_charset00);
  tolua_cclass(tolua_S,"FontFactory","FontFactory","",NULL);
  tolua_beginmodule(tolua_S,"FontFactory");
   tolua_function(tolua_S,"instance",tolua_Gamelua_FontFactory_instance00);
   tolua_function(tolua_S,"create_font_forLua",tolua_Gamelua_FontFactory_create_font_forLua00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Gamelua (lua_State* tolua_S) {
 return tolua_Gamelua_open(tolua_S);
};
#endif

