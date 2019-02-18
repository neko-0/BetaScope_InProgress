#include "../include/dataOutputFormat.hpp"


void DataOutputFormat::CreateBetaScopeOutputFile(
  std::string biasVoltage,
  std::map<std::string,std::pair<double,double>> outData[] )
{

  std::string dutHeader = "DUT_"+biasVoltage;
  DataOutputFormat::WriteHeader( dutHeader );

  DataOutputFormat::WriteKey( "PulseArea", 0.0);
  DataOutputFormat::WriteKey( "PulseArea_Error", 0.0 );

  DataOutputFormat::WriteKey( "Pmax", outData[0]["Pmax_Langau"].first );
  DataOutputFormat::WriteKey( "Pmax_Error", outData[0]["Pmax_Langau"].second );

  DataOutputFormat::WriteKey( "RMS", outData[0]["RMS"].first );
  DataOutputFormat::WriteKey( "RMS_Error", outData[0]["RMS"].second );

  DataOutputFormat::WriteKey( "Rise_Time", outData[0]["Rise Time"].first );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData[0]["Rise Time"].second );

  DataOutputFormat::WriteKey( "dvdt", outData[0]["dvdt"].first );
  DataOutputFormat::WriteKey( "dvdt_Error", outData[0]["dvdt"].second );

  DataOutputFormat::WriteKey( "FWHM", outData[0]["FWHM"].first );
  DataOutputFormat::WriteKey( "FWHM_Error", outData[0]["FWHM"].second );

  DataOutputFormat::WriteKey( "jitter_20", outData[0]["jitter_20"].first );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData[0]["jitter_20"].second );

  DataOutputFormat::WriteKey( "NewPulseArea", outData[0]["PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax"].first );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData[0]["PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax"].second );


  std::string triggerHeader = "Trig_"+biasVoltage;
  DataOutputFormat::WriteHeader( triggerHeader );

  DataOutputFormat::WriteKey( "PulseArea", 0.0);
  DataOutputFormat::WriteKey( "PulseArea_Error", 0.0 );

  DataOutputFormat::WriteKey( "Pmax", outData[1]["Pmax_Langau"].first );
  DataOutputFormat::WriteKey( "Pmax_Error", outData[1]["Pmax_Langau"].second );

  DataOutputFormat::WriteKey( "RMS", outData[1]["RMS"].first );
  DataOutputFormat::WriteKey( "RMS_Error", outData[1]["RMS"].second );

  DataOutputFormat::WriteKey( "Rise_Time", outData[1]["Rise Time"].first );
  DataOutputFormat::WriteKey( "Rise_Time_Error", outData[1]["Rise Time"].second );

  DataOutputFormat::WriteKey( "dvdt", outData[1]["dvdt"].first );
  DataOutputFormat::WriteKey( "dvdt_Error", outData[1]["dvdt"].second );

  DataOutputFormat::WriteKey( "FWHM", outData[1]["FWHM"].first );
  DataOutputFormat::WriteKey( "FWHM_Error", outData[1]["FWHM"].second );

  DataOutputFormat::WriteKey( "jitter_20", outData[1]["jitter_20"].first );
  DataOutputFormat::WriteKey( "jitter_20_Error", outData[1]["jitter_20"].second );

  DataOutputFormat::WriteKey( "NewPulseArea", outData[1]["PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax"].first );
  DataOutputFormat::WriteKey( "NewPulseArea_Error", outData[1]["PulseArea with 1ns before zero crossing of interpolated rising edge to 3ns after Tmax"].second );
}
