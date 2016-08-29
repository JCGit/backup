#ifndef Resources_h__
#define Resources_h__

#include <string>

using namespace std;

namespace resources {
	// paths
	static const std::string UI_PATH = "ui_v2/";
	static const std::string MAP_PATH = "map/";

	// files
	static const char ui_start_bg_start_jpg[] = "ui/start/bg_start.jpg";
    static const char ui_bag_bg_panel_png[] = "ui/bag/bg_panel.png";
    static const std::string FONT_NAME = "fonts/tgamefont.ttf";
    /*
    static const char ui_basic_ui_btn_rect_up_png[] = "share/btn_type_up.png"; //UI_TEX_TYPE_PLIST
    static const char ui_basic_ui_btn_rect_down_png[] = "share/btn_type_down.png";
    static const char ui_basic_ui_btn_rect_disabled_png[] = "share/btn_type_down.png";

	static const char ui_scene_btn_chat_up_png[] = "ui/scene/btn_chat_up.png";
	static const char ui_scene_btn_chat_down_png[] = "ui/scene/btn_chat_down.png";
	static const char ui_scene_btn_chat_disabled_png[] = "ui/scene/btn_chat_disabled.png";
    */
}

//地图
const string MAP_CFG_RES = "map/mapres/";
const string MAP_RES_PATH = "map/mapres/foreres/";
const string MAP_BG_RES_PAHT = "map/mapres/bgres/";
const string MAP_QJ_RES_PAHT = "map/mapres/qjres/";
const string MAP_MID_RES_PATH = "map/mapres/midres/";
const string SCENE_EFF_RES_PATH = "map/effect/";
const string MAP_RES_TP_PATH = "map/mapres/mapres_tp/";

//技能
const string SKILL_RES_PATH = "skill/";
const string SKILLICON_RES_PATH = "skillicon/";

//音效
const string SOUND_RES_PATH = "sound/";
const string BG_SOUND_RES_PATH = "sound/bgsound/";

//buff
const string BUFF_RES_PATH = "buff/";

//地表物件 or buff
const string SCENE_ITEM_RES_PATH = "sceneitem/";

//子弹
const string BULLET_RES_PATH = "bullet/";

//传送点
const string MISC_RES_PATH = "miscellaneous/";

//角色
const string HERO_RES_PATH = "hero/";
const string MONSTER_RES_PATH = "monster/";
const string NPC_RES_PATH = "npc/";

//UI
const string UI_RES_PAHT = "ui/";

//枪械
const string ICON_WEAPON_PATH = "weaponicon/";
const string ICON_BIG_WEAPON_PATH = "weapon_big_icon/";

//文件后缀
const string JSON_TYPE_NAME = ".ExportJson";
const string MAP_RES_TYPE = ".bin";
const string PNG_TYPE_NAME = ".png";
const string EFFECT_SOUND_TYPE_NAME = ".ogg";		//音效文件类型
const string BG_MUSIC_TYPE_NAME = ".map3";			//背景音乐类型
const string PLIST_TYPE_NAME = ".plist";

//item
const string ITEM_RES_PATH = "item/";
const string DEFAULT_ITEM = "item/default.png";

//特效
const string EFFECT_RES_PATH = "effect/";

// 头像
const string HEAD_ICON_PATH = "head/";
const string BIGHEAD_ICON_PATH = "bighead/";
const string LOGIN_HEAD_ICON_PATH = "head/login/";
const string BOSS_HEAD_IMG_PATH = "bosshead/";

//icon
const string ICON_PATH = "icon/";

//数字图片资源
const string NUM_RES_PATH = "ui_v2/numbers/";

#endif // Resources_h__
