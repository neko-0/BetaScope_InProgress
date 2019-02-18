#include "../include/betaAnalysis.h"

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
//------ROOT----------------//
#include <TThread.h>
#include <TStopwatch.h>
#include <TROOT.h>

void *betaAnalyzer( void *iarg )
{
	betaAnalysis *inputArg = (betaAnalysis*) iarg;

	inputArg->set_objectLocation("betaAnalyzer");
	//std::cout<< "get_objectLocation " << inputArg->get_objectLocation() << std::endl;

	TThread::Lock();
	std::cout << std::string( 100, '=' ) << std::endl;
	std::cout << "Processing " << inputArg->get_ifileName() << std::endl;
	std::cout << "Setting branches " << std::endl;
	TThread::UnLock();

	inputArg->SetBranch();

	TThread::Lock();
	std::cout << "Number of events = " << inputArg->get_numEvent() << std::endl;
	std::cout << "Number of points = " << inputArg->get_npoint() << std::endl;
	std::cout << "Start filling data " << std::endl;
	TThread::UnLock();

	inputArg->FillData();

	TThread::Lock();
	std::cout << "Done. Saving file..." <<std::endl;
	TThread::UnLock();

	inputArg->SaveFile();

	TThread::Lock();
	std::cout << "Export to " << inputArg->get_ofileName() <<std::endl;
	TThread::UnLock();

	std::cout << std::string( 100, '=' ) << std::endl;
}

void parallelBetaAnalyzer( std::vector<betaAnalysis> argVector )
{
	TStopwatch StopWatch;

	StopWatch.Start();

	TThread::Initialize();

	ROOT::EnableThreadSafety();

	int size = argVector.size();

	TThread *thread_array [size];

	for( std::size_t loop = 0, max = size; loop < max; loop++)
	{
		thread_array [loop] = new TThread ( "", betaAnalyzer, (void*) &argVector.at(loop) );
	}

	for( std::size_t loop = 0, max = size; loop < max; loop++)
	{
		thread_array [loop] -> Initialize();
	}

	for( std::size_t loop = 0, max = size; loop < max; loop++)
	{
		thread_array [loop] -> Run();
	}

	TThread::Ps();

	for( std::size_t loop = 0, max = size; loop < max; loop++)
	{
		thread_array [loop] -> Join();
	}

	for( std::size_t loop = 0, max = size; loop < max; loop++)
	{
		delete thread_array [loop];
		//thread_array[loop]->Delete();
	}

	TThread::Ps();
	StopWatch.Stop();

	std::cout << " Time : " << StopWatch.RealTime() << "sec " << std::endl;
}
