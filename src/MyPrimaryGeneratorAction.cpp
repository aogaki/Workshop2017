#include <TFile.h>

#include <G4SystemOfUnits.hh>
//#include <G4Threading.hh>
#include <G4AutoLock.hh>
#include <G4ParticleTable.hh>
#include <G4ChargedGeantino.hh>
#include <G4IonTable.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "MyPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0; // Global variable change to local? 
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;


MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr),
     fMessenger(nullptr)
{
   fFirstFlag = true;
   fZPosition = -300.*mm;
   
   fZ = 1;
   fA = 1;
   fIonCharge   = 1.*eplus;
   fExcitEnergy = 0.*keV;

   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* particle
      = particleTable->FindParticle("gamma");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fParticleGun->SetParticleEnergy(511.*keV);    
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

   DefineCommands();
   
   G4AutoLock lock(&mutexInPGA);
   TFile *file = new TFile("Data/energy.root", "OPEN");
   fHisSource2D = (TH2D*)file->Get("His2D");
   fHisSource2D->SetDirectory(0);
   fHisSource1D = (TH1D*)file->Get("HisEne");
   fHisSource1D->SetDirectory(0);
   file->Close();
   delete file;
   
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
   if(fMessenger != nullptr) {delete fMessenger; fMessenger = nullptr;}
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   if(fFirstFlag){
      fFirstFlag = false;
      //SetIon();
   }

   //G4double ene = G4RandExponential::shoot(100)*MeV;
   G4double ene;
   ene = fHisSource1D->GetRandom()*MeV;
   
   G4double theta;
   fHisSource2D->GetRandom2(theta, ene);
   G4double phi = G4UniformRand() * 2.*CLHEP::pi;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   
   fParticleGun->SetParticleEnergy(ene);
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
   fParticleGun->GeneratePrimaryVertex(event);

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->FillNtupleIColumn(1, 0, event->GetEventID());
   anaMan->FillNtupleDColumn(1, 1, ene);
   anaMan->AddNtupleRow(1);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;
}

void MyPrimaryGeneratorAction::SetIon()
{
   G4ParticleDefinition *ion
      = G4IonTable::GetIonTable()->GetIon(fZ, fA, fExcitEnergy);
   fParticleGun->SetParticleDefinition(ion);
   fParticleGun->SetParticleCharge(fIonCharge);
   G4cout << "Beam changed\tZ = " << fZ
          << ", A = " << fA
          << ", Charge = " << fIonCharge << G4endl;
}

void MyPrimaryGeneratorAction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/My/Primary/", 
                                       "Beam control");

   // z position
   G4GenericMessenger::Command &ionCmd
      = fMessenger->DeclareMethod("ionName",
                                  &MyPrimaryGeneratorAction::SetIonCmd, 
                                  "Set the ion type");

   ionCmd.SetParameterName("ionName", true);
}

void MyPrimaryGeneratorAction::SetIonCmd(G4String ionName)
{
   if(ionName == "test"){
      fZ = 100;
      fA = 200;
      fIonCharge  = 100.*eplus;
   }      
   else if(ionName == "H"){
      fZ = 1;
      fA = 1;
      fIonCharge  = 1.*eplus;
   }
   else if(ionName == "He"){
      fZ = 2;
      fA = 4;
      fIonCharge  = 2.*eplus;
   }
   else if(ionName == "C"){
      fZ = 6;
      fA = 12;
      fIonCharge  = 6.*eplus;
   }
   else if(ionName == "O"){
      fZ = 8;
      fA = 16;
      fIonCharge  = 8.*eplus;
   }
   else if(ionName == "Fe"){
      fZ = 26;
      fA = 56;
      fIonCharge  = 26.*eplus;
   }
   else{
      G4cout << "Now, only H, He, C, O, and Fe are acceptable." << G4endl;
   }
   
   SetIon();
}
