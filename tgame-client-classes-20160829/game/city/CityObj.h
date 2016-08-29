#ifndef CityObj_h__
#define CityObj_h__

class CityObj 
{
public:
	CityObj();
	~CityObj();

	const unsigned int getArriveNPCID();

	void setArriveNPCID(const unsigned int uArriveNPCID);

private:
	unsigned int m_uArriveNPCID;
};

#endif // CityObj_h__