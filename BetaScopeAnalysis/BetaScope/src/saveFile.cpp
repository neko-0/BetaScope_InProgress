#include "../include/betaAnalysis.h"

//------ROOT----------------//
#include <TThread.h>

void betaAnalysis::SaveFile()
{
	//TThread::Lock();
	this->oFile->Write();
	this->oFile->Close();
	//TThread::UnLock();
}
