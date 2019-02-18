//////////////////////////////////////
////methods implementation          //
////                                //
//////////////////////////////////////

#include "../hpp/betaAnalysis.h"

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <numeric>
#include <functional>
//------ROOT----------------//
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TThread.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TImage.h>
#include <TCanvas.h>






//==============================================================================

//===================Simple function to give the time at index==================

double betaAnalysis::TimeAtIndex (std::vector<double> t, unsigned int index)
{
  if(index == this->npoint){ index = index - 1 ; }
  return t.at(index);
}

//==============================================================================



//-----------8 21 2017------------------------------------------------
//add dvdt linear fit



int image_counter = 0;

TF1 *linear_fit = new TF1("linear_fit", "[0]*x + [1]");

double betaAnalysis::dvdt_linear(int ndata_pt, int npoints, int fraction, std::vector<double> w, std::vector<double> t, int inoise, int TimeRestriction_OnOff, double TimeRestrictionRange)
{
  std::pair<double, int> pmax_holder = betaAnalysis::Find_Singal_Maximum( npoints, w, t, TimeRestriction_OnOff, -TimeRestrictionRange, TimeRestrictionRange );

  double voltage_at_fraction = double(fraction)/100.0 * pmax_holder.first;

  std::vector<double> x;
  std::vector<double> y;
  int temp_index;
  //int counter = 0;

  for (int j = pmax_holder.second; j > -1; j--)
  {
    if ( w.at(j) < voltage_at_fraction )
    {
      temp_index = j;

      if ( (temp_index - ndata_pt) <= 0 )
      {
        for ( int i = 0; i < 2*ndata_pt; i++ )
        {
          x.push_back( t.at(0 + i) );
          y.push_back( w.at(0 + i) );
        }
        break;
      }

      if ( (temp_index + ndata_pt) > pmax_holder.second )
      {
        for ( int i = 0; i < 2*ndata_pt; i++ )
        {
          x.push_back( t.at(pmax_holder.second - i) );
          y.push_back( w.at(pmax_holder.second - i) );
        }
        break;
      }

      else
      {
        for ( int i = 0; i < 2*ndata_pt; i++)
        {
          x.push_back( t.at(temp_index - ndata_pt + i) );
          y.push_back( w.at(temp_index - ndata_pt + i) );
        }
        break;
      }

    }
  }

    if(x.empty())
    {
      return 0;
    }

  TGraph *dvdt_g = new TGraph( x.size() , &x[0], &y[0]);

  //for(int k=0;k<x.size();k++){std::cout<<y.at(k)<<std::endl;}


  //TF1 *linear_fit = new TF1("", "[0]*x + [1]");//, x.at(0), x.at( x.size()-1 ) );


  //cout<< x.at(0)<<endl;cout<<x.at(x.size()-1)<<endl;

  TThread::Lock();

  dvdt_g->Fit(linear_fit, "Q");

  double dvdt_o = linear_fit->GetParameter(0);

  TThread::UnLock();

  if(0)
  {
    gROOT->SetBatch(true);

    TCanvas *c = new TCanvas;

    dvdt_g->Draw("AC*");

    TImage *img = TImage::Create();

    img->FromPad(c);

    img->WriteImage( Form("%d.png", image_counter) );

    TThread::Lock();

    image_counter++;

    TThread::UnLock();

    delete img;
    delete c;
  }

  delete dvdt_g;
  //delete linear_fit;
  x.clear();
  y.clear();

  return dvdt_o;
}

//----------------------------8 29 2017----------------------------------
//pmax witth ploy fit

TF1 *ploy_2 = new TF1("ploy_2", "[0]*x*x + [1]*x + [2]");

double betaAnalysis::fit_pmax ( int npoints, std::vector<double> w, std::vector<double> t, int inoise, int range, int TimeRestriction_OnOff, double TimeRestrictionRange)
{
  std::pair<double, int> pmax_holder = betaAnalysis::Find_Singal_Maximum ( npoints, w, t, TimeRestriction_OnOff, -TimeRestrictionRange, TimeRestrictionRange ); //locating the pmax and its index.

  std::vector<double> pmax_neighber_v; //getting the points around pmax.

  std::vector<double> pmax_neighber_t;

  int wsize = w.size();

  if( (pmax_holder.second + range) > wsize || (pmax_holder.second - range) < 0 )
  {
    range = range - 1;
  }

  for (int step = 0; step < range; step++) //fill up the vector with points around pmax
  {
    pmax_neighber_v.push_back ( w.at(pmax_holder.second + step) );

    pmax_neighber_t.push_back ( t.at(pmax_holder.second + step) );

    pmax_neighber_v.push_back ( w.at(pmax_holder.second - step) );

    pmax_neighber_t.push_back ( t.at(pmax_holder.second - step) );
  }

  TGraph *pmax_g = new TGraph ( pmax_neighber_v.size() , &(pmax_neighber_t[0]), &(pmax_neighber_v[0]) );

  TThread::Lock();

  pmax_g -> Fit(ploy_2, "Q");

  double a0 = ploy_2->GetParameter(0);

  double a1 = ploy_2->GetParameter(1);

  double a2 = ploy_2->GetParameter(2);

  double pmax_t = -a1/(2*a0);

  double pmax = a0 * pmax_t * pmax_t + a1 * pmax_t + a2;

  TThread::UnLock();

  delete pmax_g;

  pmax_neighber_v.clear();

  pmax_neighber_t.clear();

  return pmax;
}
