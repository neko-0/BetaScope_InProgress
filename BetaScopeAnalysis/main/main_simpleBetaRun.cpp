#include "../BetaScope/include/betaAnalysis.h"
#include "../BetaScope_ConfigParser/include/BetaConfig.hpp"
#include "../CustomPrintf/include/customPrintf.h"

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>

std::vector<std::string> FindAllFolder(	const char* directory )
{
	DIR *dir;
	struct dirent *ent;

	std::vector<std::string> content = {};
	if( (dir = opendir (directory)) != NULL )
	{
  		/* print all the files and directories within directory */
  		while( (ent = readdir (dir)) != NULL )
  		{
    		content.push_back( ent->d_name );
    		//file_counter += 1;
  		}
  	closedir (dir);
  }
  std::cout << "contents in " << directory << std::endl;
	for( std::size_t i = 0, max = content.size(); i < max; i++)
	{
		std::cout << content[i] << std::endl;
	}
	//dir-> ~DIR();
	//ent-> ~dirent();
	return content;
}


void SimpleBetaRun( const char* beta_ini )
{
	std::string beta_ini_file = beta_ini;
	BetaConfig beta_config;
	beta_config.ReadConfig( beta_ini_file ); // reading the configuration file.

	std::vector<std::string> rootfiles = FindAllFolder( beta_config.directory_location.c_str() );
	std::vector<betaAnalysis> parallel_file_loader = {};
	parallel_file_loader.reserve( beta_config.num_core );
	//int change_dic = chdir(beta_config.directory_location.c_str());

	for( std::size_t item = 0, max = rootfiles.size(); item < max; item++ )
	{
		if( !(rootfiles.at(item).compare(".") == 0) && !(rootfiles.at(item).compare("..") == 0) )
		{
			std::string file_name = beta_config.directory_location + "/" + rootfiles.at(item);
			//std::string file_name = rootfiles.at(item);
			betaAnalysis singleRun( file_name.c_str(), beta_ini);

			if( !beta_config.SLAC_raw_data )
			{
				singleRun.set_SLAC_status( beta_config.SLAC_raw_data );
			}
			else
			{
				singleRun.set_SLAC_status( beta_config.SLAC_raw_data );
			}
			std::cout << "SLAC_raw_dat status = " << singleRun.get_SLAC_status() << std::endl;

			/*
			if( !beta_config.trimTimeWindow )
			{
				singleRun.set_trimTimeWindow_status( beta_config.trimTimeWindow );
			}
			else
			{
				singleRun.set_trimTimeWindow_status( beta_config.trimTimeWindow );
				singleRun.set_trimTimeRangeMin( beta_config.trimTimeRangeMin );
				singleRun.set_trimTimeRangeMax( beta_config.trimTimeRangeMax );
			}
			std::cout << "trimTimeWindow status = " << singleRun.get_trimTimeWindow_status() << std::endl;
			*/

			if( !beta_config.limiting_Pmax_search_window )
			{
				singleRun.set_restricSignalMaxSearchRange_status( beta_config.limiting_Pmax_search_window );
			}
			else
			{
				singleRun.set_restricSignalMaxSearchRange_status( beta_config.limiting_Pmax_search_window );
				singleRun.set_signalMaxSearchRange_Min( beta_config.Pmax_search_MinRange );
				singleRun.set_signalMaxSearchRange_Max( beta_config.Pmax_search_MaxRange );
			}
			std::cout << "signalMaxSearchRange status = " << singleRun.get_restricSignalMaxSearchRange_status() << std::endl;

			if( !beta_config.SSRL_Dynamic_Noise_Enable )
			{
				singleRun.set_ssrl_dynamic_noise( beta_config.SSRL_Dynamic_Noise_Enable );
			}
			else
			{
				singleRun.set_ssrl_dynamic_noise( beta_config.SSRL_Dynamic_Noise_Enable );
			}
			std::cout << "SSRL_Dynamic_Noise_Enable status = " << singleRun.get_ssrl_dynamic_noise() << std::endl;

			if( !beta_config.SSRL_Brute_Forced_Noise_Corr )
			{
				singleRun.set_ssrl_bruteForceBaseline( beta_config.SSRL_Brute_Forced_Noise_Corr );
			}
			else
			{
				singleRun.set_ssrl_bruteForceBaseline( beta_config.SSRL_Brute_Forced_Noise_Corr );
			}
			std::cout << "SSRL_Brute_Forced_Noise_Corr status = " << singleRun.get_ssrl_bruteForceBaseline() << std::endl;

			if( beta_config.enable_SSRL )
			{
				singleRun.set_ssrl_status( beta_config.enable_SSRL );
				singleRun.set_ssrl_assist_threshold( beta_config.SSRL_assist_threshold ); // assis threshold for finding multiple bunch signals.
				if( beta_config.enbale_SSRL_AC_posisiton )
				{
					singleRun.set_ssrl_AC_status( beta_config.enbale_SSRL_AC_posisiton );
				}
				else singleRun.set_ssrl_AC_status( beta_config.enbale_SSRL_AC_posisiton );
			}
			else
			{
				singleRun.set_ssrl_status( beta_config.enable_SSRL );
			}
			std::cout << "SSRL analysis status = " << singleRun.get_ssrl_status() << std::endl;
			std::cout << "SSRL AC position status = " << singleRun.get_ssrl_AC_status() << std::endl;

			singleRun.set_voltageScalar( beta_config.voltage_scalar );
			singleRun.set_timeScalar( beta_config.time_scalar );

			if( beta_config.ScopeTimestamp )
			{
				singleRun.set_timestamp_status( true );
				std::cout<< "Add scope timestamp = " << singleRun.get_timestamp_status() << std::endl;
			}
			else std::cout<< "Add scope timestamp = " << singleRun.get_timestamp_status() << std::endl;

			if(beta_config.enable_channel_1)
			{
				if(beta_config.invert_channel_1) singleRun.enableChannel(1,1);
				else singleRun.enableChannel(1,0);
			}

			if(beta_config.enable_channel_2)
			{
				if(beta_config.invert_channel_2) singleRun.enableChannel(2,1);
				else singleRun.enableChannel(2,0);
			}

			if(beta_config.enable_channel_3)
			{
				if(beta_config.invert_channel_3) singleRun.enableChannel(3,1);
				else singleRun.enableChannel(3,0);
			}

			if(beta_config.enable_channel_4)
			{
				if(beta_config.invert_channel_4) singleRun.enableChannel(4,1);
				else singleRun.enableChannel(4,0);
			}

			if(beta_config.enable_channel_5)
			{
				if(beta_config.invert_channel_5) singleRun.enableChannel(5,1);
				else singleRun.enableChannel(5,0);
			}

			if(beta_config.enable_channel_6)
			{
				if(beta_config.invert_channel_6) singleRun.enableChannel(6,1);
				else singleRun.enableChannel(6,0);
			}

			if(beta_config.enable_channel_7)
			{
				if(beta_config.invert_channel_7) singleRun.enableChannel(7,1);
				else singleRun.enableChannel(7,0);
			}

			if(beta_config.enable_channel_8)
			{
				if(beta_config.invert_channel_8) singleRun.enableChannel(8,1);
				else singleRun.enableChannel(8,0);
			}

			if( beta_config.parallel_switch )
			{
				if( singleRun.get_ifile_status() )
				{
					singleRun.set_parallelStatus( true );
					parallel_file_loader.push_back( singleRun );
				}
			}
			else
			{
				if( singleRun.get_ifile_status() )
				{
					singleRun.AllocateMemory();
					betaAnalyzer( (void*) &singleRun );
				}
				else std::cout << "skipp invalid root file " << file_name << std::endl;
			}

			if( parallel_file_loader.size() == beta_config.num_core )
			{
				parallelBetaAnalyzer( parallel_file_loader );
				parallel_file_loader.clear();
			}
		}
	}

	if( parallel_file_loader.size() != 0 )
	{
		parallelBetaAnalyzer( parallel_file_loader );
		parallel_file_loader.clear();
	}
}


int main( int argc, char **argv )
{
	if( argc != 2 )
	{
		CUSTOM_PRINTF::warning_printf("\n Invalid parameter. Use '--h' for details. \n");
		return 1;
	}
	else
	{
		if( strcmp(argv[1],"--h") == 0 )
		{
			printf( "\nUsage of this function:\n");
			printf("\n Create analysis branches from the raw ROOT file. Output has prefix stats_ .\n");

			CUSTOM_PRINTF::help_printf("\n\t Parameter [1] : takes a configuration file, please use Beta_create_beta_config to create configuration file. (It uses default config when calling from bash function) \n" );
		}

		else SimpleBetaRun( argv[1] );
	}

	return 0;
}
