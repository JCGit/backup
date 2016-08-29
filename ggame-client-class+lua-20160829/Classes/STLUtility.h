#pragma once

class DeleteObj_Vec
{
public:
	template<typename T>
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};

template<typename T>
void Clear_Vec(T& vec)
{
	T().swap(vec);
}

#define SAFE_RELEASE_VECTOR(vec) for_each(vec.begin(), vec.end(), DeleteObj_Vec()); Clear_Vec(vec);

class DeleteObj_Map
{
public:
	template<typename T>
	void operator()(const T& ptr) const
	{
		delete ptr.second;
	}
};

#define SAFE_RELEASE_MAP(mMap) for_each(mMap.begin(), mMap.end(), DeleteObj_Map()); mMap.clear();