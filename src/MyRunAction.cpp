#include <g4root.hh>

#include "MyRunAction.hpp"


MyRunAction::MyRunAction()
   : G4UserRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run *)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->SetVerboseLevel(1);
   G4cout << "Using " << anaMan->GetType()
          << " analysis manager" << G4endl;
   G4String fileName = "result";
   anaMan->OpenFile(fileName);

   // Ntuple
   anaMan->CreateNtuple("hits", "Each hit information");
   anaMan->CreateNtupleIColumn(0, "EventID");
   anaMan->CreateNtupleIColumn(0, "TrackID");
   anaMan->CreateNtupleSColumn(0, "VolumeName");
   
   anaMan->CreateNtupleDColumn(0, "KineticEnergy");
   
   anaMan->CreateNtupleDColumn(0, "x");
   anaMan->CreateNtupleDColumn(0, "y");
   anaMan->CreateNtupleDColumn(0, "z");

   anaMan->CreateNtupleDColumn(0, "vx");
   anaMan->CreateNtupleDColumn(0, "vy");
   anaMan->CreateNtupleDColumn(0, "vz");

   anaMan->CreateNtupleDColumn(0, "DepositEnergy");

   anaMan->CreateNtupleDColumn(0, "IncidentEnergy");
   
   anaMan->CreateNtupleIColumn(0, "IsExit");

   anaMan->CreateNtupleDColumn(0, "Time");

   // Init parameters
   anaMan->CreateNtuple("InitPar", "Initial Parameters");
   anaMan->CreateNtupleIColumn(1, "EventID");
   anaMan->CreateNtupleDColumn(1, "KineticEnergy");
   anaMan->CreateNtupleDColumn(1, "vx");
   anaMan->CreateNtupleDColumn(1, "vy");
   anaMan->CreateNtupleDColumn(1, "vz");

   anaMan->FinishNtuple();
}

void MyRunAction::EndOfRunAction(const G4Run *)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->Write();
   anaMan->CloseFile();
}

