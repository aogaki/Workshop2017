#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Material.hh>
#include <G4VisAttributes.hh>
#include <G4GenericMessenger.hh>


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   MyDetectorConstruction();
   virtual ~MyDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

private:
   G4bool fCheckOverlap;
   
   // Materials
   void DefineMaterials();
   G4Material *fVacuumMat;
   G4Material *fScinti1Mat;
   G4Material *fScinti2Mat;
   
   G4double fScinti1T;
   G4double fScinti2T;
   
   std::vector<G4VisAttributes *> fVisAttributes;

   // UI commnad
   G4GenericMessenger *fMessenger;
   G4VPhysicalVolume *fScinti1PV;
   G4VPhysicalVolume *fScinti2PV;
   G4VPhysicalVolume *fScinti2ColumnPV;
   G4VPhysicalVolume *fScinti2RowPV;
   void DefineCommands();
   void SetScinti1T(G4double t);
   void SetScinti2T(G4double t);
};

#endif
