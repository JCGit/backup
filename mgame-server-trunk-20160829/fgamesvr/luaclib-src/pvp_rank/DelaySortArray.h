#pragma once

//auth:diandian
//date:2016.4.18
//延迟排序数组封装


#include <functional>
#include <vector>
#include <unordered_map>
using namespace std;
class DelaySortArray
{
    public:
        DelaySortArray();
        ~DelaySortArray();
        
        bool Add(void * obj);
        bool Remove(void * obj);
        unsigned int Index(void * obj);
        unsigned int Count(void * obj);
        void * ObjByIndex(unsigned int index);
        
        void Flush();
        function<bool (void *a, void * b)> sort_fun;
    protected:
        std::unordered_map<void *, unsigned int>         _sortedMap;
        std::unordered_map<void *, unsigned int>         _nonSortMap;
        vector<void *>                                   _sortedVec;
        //unordered_map<void *, int>                 _unMap;
        signed int                                  _count;
        
        static bool NormalSortFun(void * a, void * b){ return a < b; };
};

template<class M, class K>
bool map_have_key(M & m, K k)
{
    auto iter = m.find(k);
    return iter != m.end();
}

template<class M, class K>
bool map_remove_key(M & m, K k)
{
    auto iter = m.find(k);
    if(iter != m.end())
    {
        m.erase(iter);
        return true;
    }
    return false;
}