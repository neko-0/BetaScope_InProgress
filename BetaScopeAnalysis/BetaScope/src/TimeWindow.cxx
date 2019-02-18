#include "../include/TimeWindow.hpp"
#include <vector>
ClassImp(TimeWindow);

TimeWindow::TimeWindow( int index, std::vector<double> input_para )
{
  this->window_index = index;
  this->parameter = input_para;
}
