#include "BaseItem.h"

CBaseItem::CBaseItem()
:_isEnable(true)
,_selected(false)
{

}

CBaseItem::~CBaseItem(){

}

void CBaseItem::setSelected(bool var){
	_selected = var;
}

bool CBaseItem::getSelected(){
	return _selected;
}

void CBaseItem::setIsEnable(bool var){
	_isEnable = var;
}
bool CBaseItem::getIsEnable(){
	return _isEnable;
}