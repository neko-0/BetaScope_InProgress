#include "../include/histogramProcessor.hpp"

//==============================================================================
// Roofit version
//==============================================================================
///*
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
  bool        confine,
  std::string useRoo,
  TCut        sideband1,
  TCut        sideband2
)
{
      entries = loadTree->GetEntries();

      gStyle->SetStatW(0.125);
      gStyle->SetStatH(0.125);

      double Par, ParErr;

      RooRealVar x( "x", xTitle.c_str(), range_min, range_max );

      TH1D *histo = new TH1D( Form("%s%d_h", HistName.c_str(), Channel) , Form("%s%d_h", HistName.c_str(), Channel+1) , binNum, range_min, range_max);
      histo->Sumw2();
      histo->GetXaxis()->SetTitle( xTitle.c_str() );
      loadTree->Project( Form("%s%d_h", HistName.c_str(), Channel), expression.c_str(), cuts);

      double sampleMean = histo->GetMean();
      double sampleSigma = histo->GetStdDev();
      int bins = histo->GetEntries();
      double histo_max = histo->GetXaxis()->GetBinCenter( histo->GetMaximumBin() );

      RooDataHist dataHist("dataHist", Form("%s%d_h", HistName.c_str(), Channel+1), x, Import(*histo) );

      if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 || FitFunc.compare("RooLandauGauss") == 0 )
      {
        RooRealVar *Roo_gmean;
        RooRealVar *Roo_gsigma;
        RooGaussian *Roo_gauss;

        RooRealVar *Roo_lmean;
        RooRealVar *Roo_lsigma;
        RooLandau *Roo_landau;

        TH1D *sideband1_histo = new TH1D;
        TH1D *sideband2_histo = new TH1D;

        if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 )
        {
          sideband1_histo = new TH1D( Form("%s%d_sideband1_h", HistName.c_str(), Channel), Form("%s%d_sideband1_h", HistName.c_str(), Channel+1), binNum, range_min, range_max );
          loadTree->Project( Form("%s%d_sideband1_h", HistName.c_str(), Channel), expression.c_str(), sideband1);

          sideband2_histo = new TH1D( Form("%s%d_sideband2_h", HistName.c_str(), Channel), Form("%s%d_sideband2_h", HistName.c_str(), Channel+1), binNum, range_min, range_max );
          loadTree->Project( Form("%s%d_sideband2_h", HistName.c_str(), Channel), expression.c_str(), sideband2);

          //histo->Add( sideband1_histo, -1);
          //histo->Add( sideband2_histo, -1);

          TH1D *frontBaseline_Area = new TH1D( "frontBaseline_Area", "frontBaseline_Area", 100, 1, 1 );
          TF1* baseline_fit = new TF1( "baseline_fit", "gaus");
          loadTree->Project( "frontBaseline_Area", Form("frontBaselineInt%d[0]/1.0E-15", Channel+1), cuts );
          frontBaseline_Area->Fit( baseline_fit, "Q0" );

          double baseline_area_mean = baseline_fit->GetParameter(1); //std::cout << baseline_area_mean << std::endl; std::cin.get();
          double baseline_area_sigma = baseline_fit->GetParameter(2);
          double baseline_area_sigma_error = baseline_fit->GetParError(2);

          TH1D *backBaseline_Area = new TH1D( "backBaseline_Area", "backBaseline_Area", 100, 1, 1 );
          loadTree->Project( "backBaseline_Area", Form("backBaselineInt%d[0]/1.0E-15", Channel+1), cuts );
          backBaseline_Area->Fit( baseline_fit, "Q0" );

          baseline_area_mean = (baseline_fit->GetParameter(1)+baseline_area_mean)/2.0; //std::cout << baseline_area_mean << std::endl; std::cin.get();
          baseline_area_sigma = (baseline_fit->GetParameter(2)+baseline_area_sigma)/2.0;
          baseline_area_sigma_error = (baseline_fit->GetParError(2)+baseline_area_sigma_error)/2.0;

          delete baseline_fit;
          delete frontBaseline_Area;
          delete backBaseline_Area;

          Roo_gmean = new RooRealVar( "Roo_gmean", "Roo_gmean", 0.0);
          //RooRealVar Roo_gsigma( "Roo_gsigma", "Roo_gsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma) );
          //Roo_gsigma = new RooRealVar( "Roo_gsigma", "Roo_gsigma", baseline_area_sigma, 0.1, baseline_area_sigma+3.0*baseline_area_sigma_error );
          Roo_gsigma = new RooRealVar( "Roo_gsigma", "Roo_gsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma) );
          //Roo_gsigma = new RooRealVar( "Roo_gsigma", "Roo_gsigma", baseline_area_sigma);
          Roo_gauss = new RooGaussian( "Roo_gauss", "Roo_gauss", x, *Roo_gmean, *Roo_gsigma );

          Roo_lmean = new RooRealVar( "Roo_lmean", "Roo_lmean", 1.0, 0.0, range_max );
          Roo_lsigma = new RooRealVar( "Roo_lsigma", "Roo_lsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
          Roo_landau = new RooLandau( "Roo_landau", "Roo_landau", x, *Roo_lmean, *Roo_lsigma );
        }

        if( FitFunc.compare("RooLandauGauss") == 0 )
        {
          Roo_gmean = new RooRealVar( "Roo_gmean", "Roo_gmean", 0.0);
          Roo_gsigma = new RooRealVar( "Roo_gsigma", "Roo_gsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma) );
          Roo_gauss = new RooGaussian( "Roo_gauss", "Roo_gauss", x, *Roo_gmean, *Roo_gsigma );

          Roo_lmean = new RooRealVar( "Roo_lmean", "Roo_lmean", sampleMean, 0.0, range_max );
          Roo_lsigma = new RooRealVar( "Roo_lsigma", "Roo_lsigma", std::abs(sampleSigma), 0.1, 3*std::abs(sampleSigma));
          Roo_landau = new RooLandau( "Roo_landau", "Roo_landau", x, *Roo_lmean, *Roo_lsigma );
        }

        x.setBins(10000, "cache");
        RooFFTConvPdf lxg( "lxg", "laudau conv gauss", x, *Roo_landau, *Roo_gauss );

        RooPlot *frame = x.frame( Title( Form("%s%d_h", HistName.c_str(), Channel+1) ) );
        RooFitResult *emptyFit = lxg.fitTo( dataHist, Save() );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );

        double lmean_error = Roo_lmean->getPropagatedError( *emptyFit );

        if(false)//emptyFit)
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

        TF1 * lxg_tf = lxg.asTF( RooArgList(x) );
        double peak = lxg_tf->GetMaximumX();
        double peak_y = lxg_tf->Eval(peak);
        double left_50per = lxg_tf->GetX(peak_y/2.0, -1000, peak);
        double right_50per = lxg_tf->GetX(peak_y/2.0, peak, 10.0*peak);
        double landau_FWHM = right_50per - left_50per;

        if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 )
        {
          Par = Roo_lmean->getVal();
          ParErr = Roo_lmean->getAsymErrorHi();
        }
        else if( FitFunc.compare("RooLandauGauss") == 0 )
        {
          //Par = peak;
          Par = Roo_lmean->getVal();
          //ParErr = lmean_error; //Roo_lmean.getAsymErrorHi();
          ParErr = Roo_lmean->getAsymErrorHi();
        }

        if(auto_adjust)
        {
          coin_events = histo->GetEntries();
          if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 || FitFunc.compare("RooLandauGauss") == 0 )
          {
            range_min= peak - (landau_left * sampleSigma)/2.0; //(landau_left * sampleSigma)/1.5;//Roo_lsigma.getVal();
            range_max = peak + (landau_right * sampleSigma)/3.0;//(landau_right * sampleSigma)/2.0;//Roo_lsigma.getVal();
            //binNum = (range_max-range_min)/(3.49*sampleSigma*pow(coin_events,-1.0/3.0));
          }
        }

        TCanvas *oCanvas = new TCanvas;
        TPaveText *paveText = new TPaveText;
        TF1 *histo_fit = new TF1;
        if( makePlot )
        {
          oCanvas = new TCanvas( Form("%s%d_canvas", HistName.c_str(), Channel+1) );
          oCanvas->cd();

          if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 || FitFunc.compare("RooLandauGauss") == 0 )
          {
            histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), "landau" );
            histo_fit->SetLineColor(kGreen);
            histo->SetLineColor(kRed);
            histo->SetLineWidth(3);
            //histo->Scale(1.0/histo->Integral(), "width");
            histo->Fit( histo_fit, "Q");
            if(histo->GetEntries() > 700 && histo->GetEntries() < 2000 ) histo->GetYaxis()->SetRangeUser(0, 400);
            else if( histo->GetEntries() > 2000 ) histo->GetYaxis()->SetRangeUser(0, 1000);
            else histo->GetYaxis()->SetRangeUser(0, 50);
            histo->Draw("same");

            if( FitFunc.compare("RooLandauGauss_PulseArea") == 0 )
            {
              sideband1_histo->SetLineColor(kYellow);
              sideband1_histo->SetLineWidth(3);
              sideband1_histo->SetLineStyle(2);
              //sideband1_histo->Draw("same");

              sideband2_histo->SetLineColor(kPink);
              sideband2_histo->SetLineWidth(3);
              sideband2_histo->SetLineStyle(2);
              //sideband2_histo->Draw("same");
            }

            dataHist.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
            dataHist.statOn(frame,Layout(0.45,0.90,0.6));
            lxg.plotOn( frame );//, Normalization(histo->GetEntries(), RooAbsReal::NumEvent));
            lxg.paramOn(frame,Layout(0.45, 0.90, 0.4));
            Roo_landau->plotOn( frame, LineStyle(kDashed), LineColor(kOrange) );//, Normalization(histo->GetEntries(),RooAbsReal::NumEvent) );
            Roo_gauss->plotOn( frame, LineStyle(kDashed), LineColor(kPink) );
            frame->Draw("same");

            paveText = new TPaveText(0.7, 0.9, 0.5, 0.7, "brNDC");
            paveText->AddText( Form("Peak = %lf", peak) );
            paveText->AddText( Form("Error = %lf", lmean_error) );
            paveText->AddText( Form("FWHM = %lf", landau_FWHM) );
            paveText->Draw();
          }

          if( savePlot )
          {
            TImage *img = TImage::Create();
            img->FromPad (oCanvas);
            img->WriteImage( Form("%s_ch%d_%s_RooFit.png", path, Channel, HistName.c_str()) );
            delete img;
          }

          if( histo_fit != NULL ) delete histo_fit;
          if( histo != NULL ) delete histo;
          if( sideband1_histo != NULL ) delete sideband1_histo;
          if( sideband2_histo != NULL ) delete sideband2_histo;
          if( paveText != NULL ) delete paveText;
          if( oCanvas != NULL ) delete oCanvas;

          if( Roo_gauss != NULL ) delete Roo_gauss;
          if( Roo_gmean != NULL ) delete Roo_gmean;
          if( Roo_gsigma != NULL ) delete Roo_gsigma;
          if( Roo_landau != NULL ) delete Roo_landau;
          if( Roo_lmean != NULL ) delete Roo_lmean;
          if( Roo_lsigma != NULL ) delete Roo_lsigma;
        }
        else
        {
          if( histo_fit != NULL ) delete histo_fit;
          if( histo != NULL ) delete histo;
          if( sideband1_histo != NULL ) delete sideband1_histo;
          if( sideband2_histo != NULL ) delete sideband2_histo;
          if( paveText != NULL ) delete paveText;
          if( oCanvas != NULL ) delete oCanvas;

          if( Roo_gauss != NULL ) delete Roo_gauss;
          if( Roo_gmean != NULL ) delete Roo_gmean;
          if( Roo_gsigma != NULL ) delete Roo_gsigma;
          if( Roo_landau != NULL ) delete Roo_landau;
          if( Roo_lmean != NULL ) delete Roo_lmean;
          if( Roo_lsigma != NULL ) delete Roo_lsigma;
        }
      }
      else
      {
        printf("Invalid fit function! \n");
        return std::make_tuple(-1,-1, -1, -1, -1);
      }

      return std::make_tuple( Par, ParErr, binNum, range_min, range_max );
}
