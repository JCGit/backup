#ifndef ItemUtils_h__
#define ItemUtils_h__

#include <string>
#include "BasicTypes.h"
using namespace std;

class ItemUtils
{
public:
	static string getItemIcon(const int iType, const int iCfgID);

	static string getItemName(const int iType, const int iCfgID);

	static string getItemDesc(const int iType, const int iCfgID);

	static unsigned int getItemColor(const int iType, const int iCfgID);

	static UINT64_t getEquipFightPower(const int iCfgID, const int iRefineLv, const int iPart);

	static UINT64_t getWeaponFightPower(const int iCfgID, const int iLv, const int iColor);

    //计算装备的属性值(总属性值，包括基础属性值)
    static unsigned int getEquipAttrVal(const int iEquipCfgID, const int iEquipLv, const int iQuality);

    //获取装备的显示属性 （扣除基础属性，如果是命中还要乘以系数）
    static unsigned int getEquipShowAttrVal(const int iEquipCfgID, const int iEquipLv, const int iQuality);
    static string getColorBgImg(const int iEquipCfgID);
    static string getThingTypeName(const int iThingType);

	static string numbFormateW(int iNumb);
protected:
private:
};

#endif // ItemUtils_h__
