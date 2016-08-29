#ifndef ToolUtils_h__
#define ToolUtils_h__

#include "cocos2d.h"
#include "cocos-ext.h"

#include <vector>
#include <string>

using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class CToolUtils
{
public:
	static void ArrayChangeOrder(CCArray* pstArr);					//改变数组的排序
    static bool checkResContains(CCString* str, CCArray* arr);		//检查字符串是否在数组中

    static void removeArmatureFile(const char* pstrArmatureFile);
    static void removePlistTexFile(const char* pszPlist);

    static void loadArmatureRGBA4444(const string& strJsonPath); //加载rgba4444格式的资源(人物，怪物，和部分特效)
    static void loadSpriteFrameRGBA4444(const string& strPlistPath); //加载rgba4444格式的纹理

    static bool parseUTFStr(const string& sin, vector<string>& ret);	  //解析utf8文本sin的每个字符到ret中(可以为中英文混杂)

	static void parseUTF8(const string& sin, vector<string>& ret);

	static string splitChStr(const std::string& src, const int iOneLine);  //在中文字符串中间加入换行符(必须为纯中文)

	static string getResType(const std::string& str);		//通关资源名获取资源的类型,如图片资源，动画资源等(.png, .ExportJson)

	static string splitUTFStr(const string& sin, const int iOneLine);		 //在字符串中间加入换行符(可以为中英文混杂)

	static ccColor3B parseColorToCC3(const int color);						//将颜色转换为r g b

	static string SecondToStr(int sec);   //时间转换
	static string AddZero(int time);

	static void setTimeScale(float scale);		//设置全局的加速、减速


    //根据iThingType调整pBgImg的大小
    //返回true表示是(166,88)的大icon
    static bool adjustIconBgSize(const int iThingType, const int iThingCfgID, UIImageView* pBgImg = NULL);
    static void adjustFragIcon(const int iThingType, UIImageView* pBgImg); //调整是否显示碎片图标

	/**
     * @brief 在Widget上添加角标
     * @param pWidget	要添加角标的Widget
	 * @param uBadgeNum	显示在角标上的数字的值，为0时会移除已有的角标，达到上限值时数字会变成"!"
	 * @param offsetX	X轴上的偏移量
	 * @param offsetY	Y轴上的偏移量
     */
	static void badgeWidget(Widget *pWidget, unsigned int uBadgeNum, float offsetX = 0.0f, float offsetY = 0.0f);

	/**
     * @brief 在Widget上添加不需数字的角标
     * @param pWidget	要添加角标的Widget
	 * @param uBadgeNum	大于0则显示角标，否则会移除已有的角标
	 * @param offsetX	X轴上的偏移量
	 * @param offsetY	Y轴上的偏移量
     */
	static void badgeWidgetWithPoint(Widget *pWidget, unsigned int uBadgeNum, float offsetX = 0.0f, float offsetY = 0.0f);

	/**
     * @brief 在入口上添加特效
     * @param pWidget	要添加的入口
	 * @param bIsAdd	是否添加
     */
	static void effectEntryIcon(Widget *pWidget, bool bIsAdd);

protected:
private:
	static float m_fTimeScale;
};

#endif // ToolUtils_h__
