#include "../include/fineCFDScanArg.hpp"

#include "../../DataSelection/include/dataSelection.h"
#include "../../../BetaScopeAnalysis/CustomPrintf/include/customPrintf.h"

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TThread.h>
#include <TCanvas.h>
#include <TImage.h>
#include <TF1.h>

std::tuple<double,double,double,bool,double,double> getTimeDiffSigma(
  int workerID,
  TH1D *histogram,
  int CFDpercentage,
  double mean,
  double sigma,
  double minRange,
  double maxRange,
  bool savePlot,
  std::string plotName
)
{
  int numEvent = histogram->GetEntries();
  if( numEvent < 400 )
  {
    TThread::Lock();
    std::cout << "workerID#" << workerID;
    CUSTOM_PRINTF::warning_printf("\nYour number of events might be too low (<400) =");
    std::cout << numEvent << std::endl;
    //sample_std_vs_fit_std = true;
    TThread::UnLock();
  }

  TThread::Lock();
  TCanvas *c = new TCanvas(Form("gaussFit_canvas_workerID#%d", workerID));
  c->cd();
  TF1 *gaussFit = new TF1( Form("gaussFit_workerID#%d", workerID), "gaus", minRange, maxRange);
  gaussFit->SetParameter(1, mean);
  gaussFit->SetParameter(2, sigma);

  int fitCheck = histogram->Fit(gaussFit, "QR");
  TThread::UnLock();

  double meanAfterFit;
  double sigmaAfterFit;
  double sigmaAfterFitError;
  double minRangeAfterFit;
  double maxRangeAfterFit;
  bool badFit = false;

  if( fitCheck )
  {
    CUSTOM_PRINTF::warning_printf("\nBad fit\n");
    meanAfterFit = histogram->GetXaxis()->GetBinCenter(histogram->GetMaximumBin());
    sigmaAfterFit = histogram->GetStdDev(1);
    sigmaAfterFitError = gaussFit->GetParError(2);
    minRangeAfterFit = -2000.0;
    maxRangeAfterFit = 2000.0;
    badFit = true;
    std::cout << "number of Data " << histogram->GetEntries();
    std::cout << "using sample mean "<< meanAfterFit;
    std::cout << " and std " << sigmaAfterFit << std::endl;
    std::cout << std::endl;
  }
  else
  {
    meanAfterFit = gaussFit->GetParameter(1);
    sigmaAfterFit = gaussFit->GetParameter(2);
    sigmaAfterFitError = gaussFit->GetParError(2);
    minRangeAfterFit = meanAfterFit -5.0*sigmaAfterFit;
    maxRangeAfterFit = meanAfterFit +5.0*sigmaAfterFit;

    //if the fitted sigma and data std have difference of 10 ps,
    //that means the distribution migght have problem.
    //Assign the data std intead of fitted one.
    if( abs(sigmaAfterFit-histogram->GetStdDev(1)) > 10 )
    {
      CUSTOM_PRINTF::warning_printf("\n| Fitted sigma - data std| > 10! \n");
      sigmaAfterFit = histogram->GetStdDev(1);
    }
  }

  if( savePlot )
  {
    TThread::Lock();
    TCanvas *c1 = new TCanvas( Form("c1_workerID#%d", workerID) );
  	c1->cd();
  	//histogram->SetDirectory(0);
    histogram->Draw();
    gaussFit->Draw("same");
    c1->Update();
    TImage *img = TImage::Create();
    img->FromPad(c1);
    img->WriteImage( Form("%s_FineResolution[%d].png", plotName.c_str(), CFDpercentage));
    TThread::UnLock();
    delete img;
    delete gaussFit;
    delete c1;
  }
  else
  {
    delete gaussFit;
  }

  delete c;
  //TThread::UnLock();
  return std::make_tuple( meanAfterFit, sigmaAfterFit, sigmaAfterFitError, badFit, minRangeAfterFit, maxRangeAfterFit);
}
