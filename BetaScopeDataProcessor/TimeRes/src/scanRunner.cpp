#include "../include/fineCFDScanArg.hpp"
#include "../../DataSelection/include/dataSelection.h"
#include "../../../BetaScopeAnalysis/CustomPrintf/include/customPrintf.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <TVectorD.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TThread.h>
#include <TCanvas.h>


void *scanRunner(void *args)
{
  FineResCFDScanPmodeArgs *iargs = ( FineResCFDScanPmodeArgs*) args;

  std::string ifile = iargs->in_file;

  std::vector<double> icut = {iargs->Dut_tmin, iargs->Dut_tmax, iargs->Dut_pmin, iargs->Dut_pmax, iargs->Trig_tmin, iargs->Trig_tmax, iargs->Trig_pmin, iargs->Trig_pmax};
  DataSelection selection = DataSelection(icut, iargs->dut_ch, iargs->trig_ch );

  TFile *infile = TFile::Open(ifile.c_str());
  TTree *loadTree = (TTree*) infile->Get("wfm");

  std::vector<std::pair<double,double>> Res_v = cfdScanner(
    loadTree,
    selection.cuts,
    iargs->workerID,
    iargs->dut_ch,
    iargs->trig_ch,
    iargs->totalStep,
    iargs->in_file
  );

  std::vector<double> tgraph_x;
	std::vector<double> tgraph_x_err;
	std::vector<double> tgraph_y;
	std::vector<double> tgraph_y_err;

  if (true) //saving file as txt
  {
    std::ofstream ofile;
    ofile.open ( Form("%s_res_scan.txt", ifile.substr( 10, ifile.length() - 5).c_str()), std::ios::out );

    if(ofile.is_open())
    {
      printf("Output file is OK. Start writing data.\n");

      ofile << "Sensor: " << iargs->SensorName << std::endl;
      ofile << "Bias: " << iargs->Bias << std::endl;
      ofile << "Fluence: " << iargs->Fluence << std::endl;
      ofile << "Temperature: " << iargs->Temperature << std::endl;
      ofile << "CFD Fraction" << " " << "Sigma [ps]" << " " << "Error [ps]" << std::endl;

      for( unsigned int i = 0; i < Res_v.size(); i++ )
      {
        ofile << iargs->toRealCFD * i << " " << Res_v.at(i).first << " " << Res_v.at(i).second << std::endl;

        tgraph_x.push_back( iargs->toRealCFD * i );
        tgraph_x_err.push_back( 0.0 );
        tgraph_y.push_back( Res_v.at(i).first );
        tgraph_y_err.push_back( Res_v.at(i).second );
      }

      ofile.close();
    }
    else
    {
      printf("Something wrong with the output file. \n");
      //return (void *);
    }
  }

  TVectorD t_tgraph_x( tgraph_x.size(), &tgraph_x[0]);
	TVectorD t_tgraph_x_err( tgraph_x_err.size(), &tgraph_x_err[0]);
	TVectorD t_tgraph_y( tgraph_y.size(), &tgraph_y[0]);
	TVectorD t_tgraph_y_err( tgraph_y_err.size(), &tgraph_y_err[0]);

  TThread::Lock();
	TGraphErrors *cfd_scan_plot = new TGraphErrors( t_tgraph_x, t_tgraph_y, t_tgraph_x_err, t_tgraph_y_err );
	TCanvas *cfd_scan_canvas = new TCanvas( Form("cfd_scan_canvas_workerID#%d", iargs->workerID) , "cfd_scan_canvas", 1600, 1000);
	cfd_scan_canvas->cd();
	cfd_scan_plot->GetXaxis()->SetLimits(0,100);
	cfd_scan_plot->Draw();
	cfd_scan_canvas->SaveAs( Form("%s_res_scan_plot.png", ifile.substr(10,ifile.length() - 5).c_str()) );
	delete cfd_scan_plot;
	delete cfd_scan_canvas;

  tgraph_x.clear();
	tgraph_x_err.clear();
	tgraph_y.clear();
	tgraph_y_err.clear();
  TThread::UnLock();
}
