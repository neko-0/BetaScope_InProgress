#include "../include/averageWave.hpp"

#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>
#include <TH2.h>


void Average_Wave::averaging()
{
  TFile *ifile = TFile::Open( this->file_name.c_str(), "r" );
  TTree *itree = (TTree *) ifile->Get("wfm");

  int numBin = (this->t_max - this->t_min)/this->bin_width;
  TH2D *dutCh = new TH2D("dutCh", "dut Channel", numBin, this->t_min, this->t_max, 2000, -500.0, 500.0 );
  itree->Project( "dutCh", Form("w%d:t%d", this->dut_ch, this->dut_ch) );
  TProfile *dutCh_profx = dutCh->ProfileX("dut_profx", 0, 2000, "");
  dutCh_profx->SetTitle("Average pulse Shapes");
  dutCh_profx->GetXaxis()->SetTitle("Time [ps]");
  dutCh_profx->GetYaxis()->SetTitle("Voltage [mV]");
  dutCh_profx->SetLineColor(2);
  dutCh_profx->SetMarkerColor(2);
  dutCh_profx->SetLineWidth(3);
  dutCh_profx->SetMarkerSize(0);
  dutCh_profx->SetStats(0);

  dutCh_profx->SetDirectory( this->ofile );
  this->ofile->Write();
  this->ofile->Close();
  delete dutCh;
}
