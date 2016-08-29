
#include <algorithm>
#include "DelaySortArray.h"
#include <iostream>

DelaySortArray::DelaySortArray()
{
    _sortedMap.clear();
    _nonSortMap.clear();
    //_unMap.clear();
    _count = 0;
    sort_fun = DelaySortArray::NormalSortFun;
//    sort_fun = [](void * a, void * b)
//    {
//        return a < b;
//    };
}

DelaySortArray::~DelaySortArray()
{
    _sortedMap.clear();
    _nonSortMap.clear();
    _sortedVec.clear();
    _count = 0;
}

bool DelaySortArray::Add(void * obj)
{
    if(!obj)
        return false;
    
    if(map_have_key(_sortedMap, obj))
        return false;
    if(map_have_key(_nonSortMap, obj))
        return false;
	int nonsize = _nonSortMap.size();
    _nonSortMap.insert({obj, nonsize + 1});
    _sortedVec.push_back(obj);
    _count ++;
    return true;
}

bool DelaySortArray::Remove(void * obj)
{
    if(!obj)
        return false;
    
    if(map_remove_key(_nonSortMap, obj))
    {
        _count --;
        return true;
    }
    if(map_remove_key(_sortedMap, obj))
    {
        _count --;
        return true;
    }
    return false;
}

unsigned int DelaySortArray::Index(void * obj)
{
    if (!obj)
        return 0;
    
    if(map_have_key(_nonSortMap, obj))
        return _sortedMap.size() + _nonSortMap[obj];
    if (map_have_key(_sortedMap, obj))
        return _sortedMap[obj];
    cout << "sort map << " << _sortedMap[obj] << endl;
    return 0;
}

void DelaySortArray::Flush()
{
    _sortedVec.clear();
    auto sortedIter = _sortedMap.begin();
    for(; sortedIter != _sortedMap.end(); sortedIter ++)
        _sortedVec.push_back(sortedIter->first);
	_sortedMap.clear();
    
    auto nonSortIter = _nonSortMap.begin();
    for(; nonSortIter != _nonSortMap.end(); nonSortIter ++)
        _sortedVec.push_back(nonSortIter->first);
    _nonSortMap.clear();
    
    std::sort(_sortedVec.begin(), _sortedVec.end(), sort_fun);
	for (unsigned int i = 0; i < _sortedVec.size(); i++)
	{
		_sortedMap.insert({ _sortedVec[i], i + 1 });
	}
}

void * DelaySortArray::ObjByIndex(unsigned int index)
{
    if (index > _sortedMap.size())
        return NULL;
    
    return _sortedVec[index - 1];
}








