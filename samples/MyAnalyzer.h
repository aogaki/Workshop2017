//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May 13 17:29:49 2017 by ROOT version 6.09/01
// from TChain hits/
//////////////////////////////////////////////////////////

#ifndef MyAnalyzer_h
#define MyAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <TH1.h>

class MyAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> EventID = {fReader, "EventID"};
   TTreeReaderValue<Int_t> TrackID = {fReader, "TrackID"};
   TTreeReaderArray<Char_t> VolumeName = {fReader, "VolumeName"};
   TTreeReaderValue<Double_t> KineticEnergy = {fReader, "KineticEnergy"};
   TTreeReaderValue<Double_t> x = {fReader, "x"};
   TTreeReaderValue<Double_t> y = {fReader, "y"};
   TTreeReaderValue<Double_t> z = {fReader, "z"};
   TTreeReaderValue<Double_t> vx = {fReader, "vx"};
   TTreeReaderValue<Double_t> vy = {fReader, "vy"};
   TTreeReaderValue<Double_t> vz = {fReader, "vz"};
   TTreeReaderValue<Double_t> DepositEnergy = {fReader, "DepositEnergy"};
   TTreeReaderValue<Double_t> IncidentEnergy = {fReader, "IncidentEnergy"};
   TTreeReaderValue<Int_t> IsExit = {fReader, "IsExit"};
   TTreeReaderValue<Double_t> Time = {fReader, "Time"};

   TH1D *fHisNaI;
   
   MyAnalyzer(TTree * /*tree*/ =0) { }
   virtual ~MyAnalyzer() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(MyAnalyzer,0);

};

#endif

#ifdef MyAnalyzer_cxx
void MyAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t MyAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef MyAnalyzer_cxx
