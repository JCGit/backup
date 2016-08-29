#ifndef PlotControl_h__
#define PlotControl_h__

#include "vlib/base/CVSingleton.h"
#include "DialogLayout.h"

class PlotControl 
{
public:
	PlotControl();
	~PlotControl();

	DialogLayout* getDialogLayout();

	void setStoryID(unsigned int uStoryID);

	void onStoryEnd();

	void showDialog();

	void hideDialog();

private:
	DialogLayout *m_pDialogLayout;

	unsigned int m_uStoryID;

};

#define P_PLOT_CTRL CVLib::CVSingleton<PlotControl>::Instance()

#endif // PlotControl_h__