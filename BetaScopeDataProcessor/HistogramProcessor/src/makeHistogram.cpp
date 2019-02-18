#include "../include/histogramProcessor.hpp"

//===========================================================================================
// Make histograms
//===========================================================================================

//(mean, mean err, binNum, range_min, range_max)
std::tuple<double,double,int,double,double> HistogramProcessor::Make_Histogram(
  const char* path,
  TTree* loadTree,
  std::string HistName,
  std::string expression,
  std::string FitFunc,
  std::string xTitle,
  TCut        cuts,
  int         Channel,
  int         binNum,
  double      range_min,
  double      range_max,
  bool        makePlot,
  bool        savePlot,
  bool        confine )
{
      this->entries = loadTree->GetEntries();
      int bins = entries/sqrt(2);

      TH1D *histo = new TH1D( Form("%s%d_h", HistName.c_str(), Channel) , Form("%s%d_h", HistName.c_str(), Channel+1) , binNum, range_min, range_max);
      histo->GetXaxis()->SetTitle( xTitle.c_str() );

      loadTree->Project( Form("%s%d_h", HistName.c_str(), Channel), expression.c_str(), cuts);

      double sampleMean = histo->GetMean();
      double sampleSigma = histo->GetStdDev();

      TF1 *histo_fit = new TF1;

      std::string fitOption1;
      std::string fitOption2;

      if (FitFunc.compare("landau") == 0)
      {
        histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str());

        fitOption1 = "Q 0";
        fitOption2 = "Q";

        histo_fit->SetParameter(1, sampleMean);
        histo_fit->SetParameter(2, sampleSigma);
      }

      else if (FitFunc.compare("gaus") == 0)
      {
        histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str());

        if( confine )
        {
          fitOption1 = "Q 0 B";
          fitOption2 = "Q B";
          histo_fit->SetParameter(1, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) );
          histo_fit->SetParLimits(1, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())-100, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())+100);
          histo_fit->SetParameter(2, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/4.0 );
          histo_fit->SetParLimits(2, 0.1, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/3.0);
        }
        else
        {
          fitOption1 = "Q 0";
          fitOption2 = "Q";
        }
      }

      else if( FitFunc.compare("[0]*gaus(1)+[4]*gaus(5)") == 0 )
      {
        histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str() );

        histo_fit->SetParameters( 0.5, 1.0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()), sampleSigma, 0.5, 1.0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 250.0, sampleSigma );

        histo_fit->SetParNames("c1", "gConst1", "gMean1", "gSig1", "c2", "gConst2", "gMean2", "gSig2" );

        fitOption1 = "B";
        fitOption2 = "B";

        //histo_fit->SetParLimits(2, 0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 100 );
        histo_fit->SetParLimits(3, 0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/5.0 );

        histo_fit->SetParLimits(6,  histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 3.0 * sampleSigma, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 5.0 * sampleSigma );
        histo_fit->SetParLimits(7, 0 , histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/3.0 );
        //histo_fit->SetParLimits(7, 0, sampleSigma );
      }

      else if( FitFunc.compare("[0]*gaus(1)+[4]*landau(5)") == 0 )
      {
        histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str() );

        histo_fit->SetParameters( 0.5, 1.0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()), sampleSigma, 0.5, 1.0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 250.0, sampleSigma );

        histo_fit->SetParNames("c1", "gConst1", "gMean1", "gSig1", "c2", "landau_Const", "landau_MPV", "landau_Sig" );

        fitOption1 = "B";
        fitOption2 = "B";

        //histo_fit->SetParLimits(2, 0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) + 100 );
        histo_fit->SetParLimits(3, 0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/5.0 );

        histo_fit->SetParLimits(7, 0 , histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/3.0 );
        //histo_fit->SetParLimits(7, 0, sampleSigma );
      }

      else
      {
        printf("Invalid fit function! \n");
        return std::make_tuple(-1,-1, -1, -1, -1);
      }

      int emptyFit = histo->Fit(histo_fit, fitOption1.c_str());

      if(emptyFit)
      {
        std::cout << "bad fit: " << HistName << std::endl;
        std::cout << "resetting ranges"  << std::endl;
        std::cout << "histo entries = " << histo->GetEntries() << std::endl;
        std::cout << "range_min = " << range_min << std::endl;
        std::cout << "range_max = " << range_max << std::endl;
        std::cout << "sampleMean = " << histo->GetMean() << std::endl;
        std::cout << "sampleSigma = " << sampleSigma << std::endl;
        range_min= -100.0;
        range_max = 200.0;
        return std::make_tuple( sampleMean, sampleSigma/histo->GetEntries(), binNum, range_min, range_max );
      }

      histo-> SetDirectory(0);

      if(auto_adjust)
      {
        coin_events = histo->GetEntries();

        if( FitFunc.compare("landau") == 0 )
         {
          range_min= histo_fit->GetParameter(1) - landau_left * histo_fit->GetParameter(2);
          range_max = histo_fit->GetParameter(1) + landau_right * histo_fit->GetParameter(2);
        }

        if( FitFunc.compare("gaus") == 0 )
        {
          double FD_width = 2.0 * 1.35 * histo->GetStdDev() * std::pow( coin_events, -1.0/3.0 );
          //binNum = sqrt(coin_events);
          range_min= histo_fit->GetParameter(1) - 5 * histo_fit->GetParameter(2);
          range_max = histo_fit->GetParameter(1) + 5 * histo_fit->GetParameter(2);
          binNum = (range_max - range_min)/FD_width;
        }

        if( FitFunc.compare("[0]*gaus(1)+[4]*gaus(5)") == 0 )
        {
          range_min= sampleMean - 5 * sampleSigma;
          range_max = sampleMean + 7 * sampleSigma;
        }

      }

      double Par = histo_fit->GetParameter(1);

      double ParErr = histo_fit->GetParError(1);

      TCanvas *oCanvas = new TCanvas;

      if( makePlot )
      {
        oCanvas = new TCanvas( Form("%s%d_canvas", HistName.c_str(), Channel+1) );

        oCanvas->cd();

        if( savePlot )
        {
          histo ->Fit(histo_fit, fitOption2.c_str());
          gStyle->SetOptFit(1);
          gSystem->ProcessEvents();
          TImage *img = TImage::Create();
          img->FromPad (oCanvas);
          img->WriteImage( Form("%s_ch%d_%s.png", path, Channel, HistName.c_str()) );
          delete img;

          delete histo_fit;
          delete histo;
          delete oCanvas;
        }
        else
        {
          histo ->Fit(histo_fit, fitOption2.c_str());
          gStyle->SetOptFit(1);
        }
      }

      return std::make_tuple( Par, ParErr, binNum, range_min, range_max );
}
