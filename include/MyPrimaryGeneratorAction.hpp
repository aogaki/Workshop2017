#ifndef MyPrimaryGeneratorAction_h
#define MyPrimaryGeneratorAction_h 1

#include <TH2.h>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Threading.hh>
#include <G4Event.hh>
#include <G4GenericMessenger.hh>


class MyPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   MyPrimaryGeneratorAction();
   virtual ~MyPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ParticleGun *fParticleGun;
   G4double fZPosition;
   G4bool fFirstFlag;
   
   // Ion parameters
   void SetIon();
   G4int fZ;
   G4int fA;
   G4double fIonCharge;
   G4double fExcitEnergy;

   // Commands
   void DefineCommands();
   G4GenericMessenger *fMessenger;
   // Changing Ion 
   void SetIonCmd(G4String name);
   
   TH2D *fHisSource2D;
   TH1D *fHisSource1D;

   G4ThreeVector GetDirection(G4double limit);
};

#endif
