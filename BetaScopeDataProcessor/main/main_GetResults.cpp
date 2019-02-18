#include "../HistogramProcessor/include/histogramProcessor.hpp"
#include "../DataSelection/include/dataSelection.h"
#include "../DataOutputFormat/include/dataOutputFormat.hpp"
#include "../DataProcessingConfig/include/measurement_info.h"

void GetResults(std::string run_ini, std::string output_folder = "Results", bool logging = false )
{
	gROOT->SetBatch(true);

	MeasurementInfo measurement_info( run_ini );
	int dut_channel = measurement_info.dut_channel;
	int trigger_channel = measurement_info.trigger_channel;

	for( int i = 0; i < measurement_info.number_of_runs; i++ )
	{
		std::cout << measurement_info.file_name[i].at(0) << std::endl;
		std::cout << "start running " << std::endl;
		std::cout << "processing histogram " << std::endl;

		HistogramProcessor TEMP;
		TEMP.set_ch(dut_channel, trigger_channel);
		TEMP.SetupParameters();

		//parsing the cut
		std::string delimiter = " ";
	  std::string temp_str  = measurement_info.cut[dut_channel-1][i].at(0);
		std::vector<std::string> cut_v;
		while( int(temp_str.find( delimiter )) != -1 )
		{
	    cut_v.push_back( temp_str.substr(0, temp_str.find( delimiter ) ) );
			temp_str.erase(0, temp_str.find( delimiter ) + delimiter.length() );
		}
		cut_v.push_back( temp_str );

		DataSelection *selection = new DataSelection( cut_v, dut_channel, trigger_channel );

		TEMP.ProcessHistogram( measurement_info.file_name[i].at(0).c_str(), measurement_info.bias[i].at(0).c_str(), selection->cuts, true, selection->sideband1, selection->sideband2 );

    DataOutputFormat outfile;
    outfile.CreateBetaScopeOutputFile( measurement_info.bias[i].at(0).c_str(), TEMP.oData );

		std::cout << "finished " << measurement_info.file_name[i].at(0)  << std::endl;
		cut_v.clear();
		delete selection;
	}

	printf("Start dumping plots...\n");

	int dir_check;

	dir_check = mkdir( output_folder.c_str(), ACCESSPERMS );

	if(dir_check == 0){printf("Directory is created. \n");}

	else{printf("Directory already exists! Previous data will be replaced...\n");}

	std::string mv_png = "mv *.png " + output_folder;
	std::string mv_results_txt = "mv *_results.txt " + output_folder;
	std::string mv_results_ini = "mv *_results.ini " + output_folder;
	std::string mv_results_xlsx = "mv *_results.xlsx " + output_folder;
	system( "python  ~/Codes/py_pyExcel/parseBetaResultsToExcel.py");
	system( "python  $BETASCOPE_PAHT/parseBetaResultsToExcel.py");
	system( mv_png.c_str() );
	system( mv_results_txt.c_str() );
	system( mv_results_ini.c_str() );
	system( mv_results_xlsx.c_str());

	printf( "output is in the %s directory. \n", output_folder.c_str() );
	printf("\n");

	gROOT->SetBatch(false);
}







int main(int argc, const char *argv[])
{

	if(argc == 1) printf("Error! Runlist Requires\n");
	if(argc == 2) GetResults(argv[1]);
	if(argc == 3) GetResults(argv[1], argv[2]);
	if(argc > 4) printf("Error! Check the input arguments!");
}
