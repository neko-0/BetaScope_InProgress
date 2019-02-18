#ifndef __TimeWindow__
#define __TimeWindow__
#include <TObject.h>
#include <vector>

class TimeWindow : public TObject{

	int window_index;
	std::vector<double> parameter;

	public:

		TimeWindow()
		{
			this->window_index = -9;
			this->parameter = {};
		}

		TimeWindow( int index, std::vector<double> input_para );

		int Get_Window_Index(){ return this->window_index; }
		double ValAt( int i ){ return this->parameter.at(i); }
		int GetSize(){ return this->parameter.size(); }

		ClassDef(TimeWindow, 1);
};

#endif
