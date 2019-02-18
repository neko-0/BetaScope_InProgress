#include "../include/fineCFDScanArg.hpp"

#include <TTree.h>
#include <TCut.h>
#include <TH1.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TThread.h>

TH1D *make_CFDTdiff_hitogram(
  TTree* itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int CFDpercentage,
  int numBin,
  double minRange,
  double maxRange
)
{

  //delete gROOT->FindObject( Form("cfd_hist_workerID#%d", workerID) );
  TH1D *histogram = new TH1D( Form("cfd_hist_workerID#%d", workerID), "CFD Tdiff", numBin, minRange, maxRange );
  itree->Project( Form("cfd_hist_workerID#%d", workerID), Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", dutChannel, CFDpercentage, trigChannel), icut);

  histogram->SetDirectory(0);
  return histogram;
}
