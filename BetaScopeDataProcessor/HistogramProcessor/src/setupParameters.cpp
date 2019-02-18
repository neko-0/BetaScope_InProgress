#include "../include/histogramProcessor.hpp"

#include <vector>

void HistogramProcessor::SetupParameters()
{
  std::vector<int> activeChannel;

  activeChannel.push_back( this->DUT + 1 );
  activeChannel.push_back( this->Trigger + 1 );

  for( unsigned int i = 0; i < activeChannel.size(); i++ )
  {
    int ch = activeChannel.at(i);

    plotArg[i].push_back(
      {
        { "Pmax" },
        { Form( "pmax%d[0]", ch ) },
        { "landau" },
        { "Pmax [mV]"}
      }
    );

    plotArg[i].push_back(
      {
        { "Pmax_Langau" },
        { Form( "pmax%d[0]", ch ) },
        { "RooLandauGauss" },
        { "Pmax [mV]"},
        { "Roo" }
      }
    );

    plotArg[i].push_back(
      {
        { "RMS" },
        { Form( "rms%d[0]", ch ) },
        { "gaus" },
        { "RMS [mV]"}
      }
    );

    plotArg[i].push_back(
      {
        { "Rise Time" },
        { Form( "riseTime%d[0]", ch ) },
        { "gaus" },
        { "Rise Time [ps]" }
      }
    );

    plotArg[i].push_back(
      {
        { "dvdt" },
        { Form( "dvdt%d[20]", ch ) },
        { "landau" },
        { "dV/dt [mV/ps]" }
      }
    );

    plotArg[i].push_back(
      {
        { "jitter_20" },
        { Form( "rms%d[0]/dvdt%d[20]", ch, ch) },
        { "RooLandauGauss" },
        { "Jitter_20 [ps]" },
        { "Roo" }
      }
    );

    plotArg[i].push_back(
      {
        { "FWHM" },
        { Form( "cfd%d_fall[50] - cfd%d[50]", ch, ch ) },
        { "gaus" },
        { "FWHM [ps]" }
      }
    );

    /*plotArg[i].push_back( {
                         { "Rise_GausGaus" },
                         { Form( "rise%d_1090[0]", ch ) },
                         { "[0]*gaus(1)+[4]*gaus(5)" }
     } );*/

    /*plotArg[i].push_back( {
                         { "Rise_Gaus_add_Landau" },
                         { Form( "rise%d_1090[0]", ch ) },
                         { "[0]*gaus(1)+[4]*landau(5)" }
     } );*/

    /*plotArg[i].push_back( {
                         { "Rise_Log_normal" },
                         { Form( "log(rise%d_1090[0])", ch ) },
                         { "gaus" }
     } );*/

    /*plotArg[i].push_back( {
     { "Rise Time 20-80" },
     { Form( "rise%d_1090[1]", ch ) },
     { "gaus" }
     } );*/

    /*plotArg[i].push_back( {
     { "Rise Time_20-80_gaus_gaus" },
     { Form( "rise%d_1090[1]", ch ) },
     { "[0]*gaus(1)+[4]*gaus(5)" }
     } );*/

    /*plotArg[i].push_back(
      {
        { "PulseArea-UndershootArea" },
        { Form( "(pulse_area%d[0]+pulse_undershoot_area%d[0])/1E-15", ch, ch ) },
        { "landau" },
        { "corrected Pulse Area [V*s/1E-15]" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "New_PulseArea-UndershootArea" },
        { Form( "(pulseArea_withInterEdge%d[0]+pulse_undershoot_area%d[0])/1E-15", ch, ch ) },
        { "landau" },
        { "corrected Pulse Area [V*s/1E-15]" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "PulseArea with zero crossing of interpolated rising edge to 3ns hard stop" },
        { Form( "pulseArea_withInterEdge_Extended%d[0]/1.0E-15", ch ) },
        { "RooLandauGauss" }, //landau
        { "Pulse Area [V*s/1E-15]" },
        {"Roo"}
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "Undershoot PulseArea extended to 3ns" },
        { Form( "-1.0*pulse_undershoot_area_extended%d[0]/1.0E-15", ch ) },
        { "landau" },
        { "Udershoot Pulse Area [V*s/1E-15]" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "Jitter with Landau" },
        { Form( "rms%d[0]/dvdt%d[20]", ch, ch ) },
        { "landau" },
        { "Jitter [ps]" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "Jitter with Gaussain" },
        { Form( "rms%d[0]/dvdt%d[20]", ch, ch ) },
        { "gaus" },
        { "Jitter [ps]" }
      }
    );*/

    plotArg[i].push_back(
      {
        { "PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax" },
        { Form( "pulseArea_1nsFront_3nsBack%d[0]/1.0E-15", ch ) },
        { "RooLandauGauss_PulseArea" },
        { "Pulse Area [V*s/1E-15]" },
        { "Roo" }
      }
    );

    /*plotArg[i].push_back(
      {
        { "PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax" },
        { Form( "pulseArea_1nsFront_3nsBack%d[0]/1.0E-15/4700.0", ch ) },
        { "RooLandauGauss_PulseArea" },
        { "Charge [fC]" },
        { "Roo" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "PulseArea with zero crossing of interpolated rising edge to 2nd zero crossing of the back" },
        { Form( "(pulseArea_withInterEdge%d[0]+pulse_undershoot_area%d[0])/1.0E-15", ch, ch ) },
        { "RooLandauGauss" },
        { "corrected Pulse Area [V*s/1E-15]" },
        { "Roo" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "PulseArea No Udershoot" },
        { Form( "(pulseArea_withInterEdge%d[0])/1E-15", ch ) },
        { "RooLandauGauss" },
        { "Pulse Area [V*s/1E-15]" },
        { "Roo" }
      }
    );*/

    /*plotArg[i].push_back(
      {
        { "PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax pureLandau" },
        { Form( "pulseArea_1nsFront_3nsBack%d[0]/1.0E-15", ch ) },
        { "landau" },
        { "Pulse Area [V*s/1E-15]" }
      }
    );*/

    plotArg[i].push_back(
      {
        { "Front Baseline Area" },
        { Form( "frontBaselineInt%d[0]/1.0E-15", ch ) },
        { "gaus" },
        { "Pulse Area [V*s/1E-15]" }
      }
    );

    plotArg[i].push_back(
      {
        { "Front Baseline Area independent base corr" },
        { Form( "frontBaselineInt_indepBaseCorr%d[0]/1.0E-15", ch ) },
        { "gaus" },
        { "Pulse Area [V*s/1E-15]" }
      }
    );

    plotArg[i].push_back(
      {
        { "Back Baseline Area" },
        { Form( "backBaselineInt%d[0]/1.0E-15", ch ) },
        { "gaus" },
        { "Pulse Area [V*s/1E-15]" }
      }
    );

    plotArg[i].push_back(
      {
        { "Back Baseline Area independent base corr" },
        { Form( "backBaselineInt_indepBaseCorr%d[0]/1.0E-15", ch ) },
        { "gaus" },
        { "Pulse Area [V*s/1E-15]" }
      }
    );


  }
}
