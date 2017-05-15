#include <TChain.h>
#include <TProof.h>
#include <TProofLite.h>


void ActivatePROOF(TChain *chain, Int_t nThreads = 0)
{
   TProof *proof = TProof::Open("");
   proof->SetProgressDialog(kFALSE);
   if(nThreads > 0) proof->SetParallel(nThreads);

   chain->SetProof();
}

void runAnalyzer()
{
   TChain *chain = new TChain("hits");
   chain->Add("../result_t*.root");

   ActivatePROOF(chain);
   
   chain->Process("MyAnalyzer.C+O");
}
