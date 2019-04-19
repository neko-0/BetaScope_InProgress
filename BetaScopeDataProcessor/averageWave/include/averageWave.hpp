#ifndef AVERAGE_WAVE_H
#define AVERAGE_WAVE_H

#include <string>
#include <TFile.h>

class Average_Wave
{
  std::string file_name;
  double t_min = -25000;//ps
  double t_max = 25000;//ps
  double bin_width = 50;//ps

  int dut_ch = 2;
  int trig_ch = 3;

  int workerID;

  TFile *ofile;


  public:
    Average_Wave( std::string file_name ){
      this->file_name = file_name;
      std::string ofile_name = "averaged_" + file_name;
      this->ofile = new TFile( ofile_name.c_str(), "recreate" );
    }

    void set_dut_ch( int ivalue ){ this->dut_ch = ivalue; }
    void set_trig_ch( int ivalue ){ this->trig_ch = ivalue; }
    void set_t_min( double ivalue ){ this->t_min = ivalue; }
    void set_t_max( double ivalue ){ this->t_max = ivalue; }
    void set_bin_width( double ivalue ){ this->bin_width = ivalue; }

    void averaging();

};

#endif
