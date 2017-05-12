#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PVReplica.hh>
#include <G4RunManager.hh>

#include "MyDetectorConstruction.hpp"
#include "MySD.hpp"


MyDetectorConstruction::MyDetectorConstruction()
   : fVacuumMat(nullptr),
     fScinti1Mat(nullptr),
     fScinti2Mat(nullptr)
{
   fCheckOverlap = true;

   fScinti1T = 20.*mm;
   fScinti2T = 20.*mm;
   
   DefineMaterials();

   DefineCommands();
}

MyDetectorConstruction::~MyDetectorConstruction()
{
   delete fScinti2Mat;
}

void MyDetectorConstruction::DefineMaterials()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");
   fScinti1Mat = manager->FindOrBuildMaterial("G4_SODIUM_IODIDE");

   // LGSO
   G4Element *Si = manager->FindOrBuildElement("Si");
   G4Element *O = manager->FindOrBuildElement("O");
   G4Element *Lu = manager->FindOrBuildElement("Lu");
   G4Element *Gd = manager->FindOrBuildElement("Gd");

   fScinti2Mat = new G4Material("LGSO", 6.5*g/cm3, 4, kStateSolid);
   fScinti2Mat->AddElement(Lu, 72.86*perCent);
   fScinti2Mat->AddElement(Gd, 3.45*perCent);
   fScinti2Mat->AddElement(Si, 6.16*perCent);
   fScinti2Mat->AddElement(O, 17.53*perCent);
  
   
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
   // world volume
   G4double worldX = 0.1*m;
   G4double worldY = 0.1*m;
   G4double worldZ = 0.6*m;

   G4Box *worldS = new G4Box("World", worldX / 2., worldY / 2., worldZ / 2.);
   G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fVacuumMat, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   worldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", 0,
                          false, 0, fCheckOverlap);

   // NaI
   G4double scinti1W = 16.*mm;
   G4double scinti1H = 16.*mm;

   G4Box *scinti1S = new G4Box("NaI", scinti1W / 2., scinti1H / 2., fScinti1T / 2.);
   G4LogicalVolume *scinti1LV = new G4LogicalVolume(scinti1S, fScinti1Mat, "NaI");
   visAttributes = new G4VisAttributes(G4Colour::Magenta());
   scinti1LV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4ThreeVector scinti1Pos = G4ThreeVector(0., 0., -fScinti1T / 2.);
   fScinti1PV = new G4PVPlacement(nullptr, scinti1Pos, scinti1LV, "NaI", worldLV,
                                  false, 0, fCheckOverlap);

   // LGSO
   G4double scinti2W = 16.*mm;
   G4double scinti2H = 16.*mm;

   G4Box *scinti2S = new G4Box("LGSO", scinti2W / 2., scinti2H / 2., fScinti2T / 2.);
   G4LogicalVolume *scinti2LV = new G4LogicalVolume(scinti2S, fScinti2Mat, "LGSO");
   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   scinti2LV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4ThreeVector scinti2Pos = G4ThreeVector(0., 0., fScinti2T / 2.);
   fScinti2PV = new G4PVPlacement(nullptr, scinti2Pos, scinti2LV, "LGSO", worldLV,
                                  false, 0, fCheckOverlap);

   // Make LGSO matrix
   G4double meshSize = 1.*mm;
   
   G4Box *scinti2ColumnS
      = new G4Box("LGSOColumn", meshSize / 2., scinti2H / 2., fScinti2T / 2.);
   G4LogicalVolume *scinti2ColumnLV
      = new G4LogicalVolume(scinti2ColumnS, fScinti2Mat, "LGSOColumn");
   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   scinti2ColumnLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   fScinti2ColumnPV = new G4PVReplica("LGSOColumn", scinti2ColumnLV,
                                      scinti2LV, kXAxis, 16, meshSize);
   
   G4Box *scinti2RowS
      = new G4Box("LGSORow", meshSize / 2., meshSize / 2., fScinti2T / 2.);
   G4LogicalVolume *scinti2RowLV
      = new G4LogicalVolume(scinti2RowS, fScinti2Mat, "LGSORow");
   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   scinti2RowLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   fScinti2RowPV = new G4PVReplica("LGSORow", scinti2RowLV,
                                   scinti2ColumnLV, kYAxis, 16, meshSize);
  
   return worldPV;
}

void MyDetectorConstruction::ConstructSDandField()
{
   // Sensitive Detectors
   G4VSensitiveDetector *SD = new MySD("SD", "HC");
   G4SDManager::GetSDMpointer()->AddNewDetector(SD);
   
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   for(auto &&lv: *lvStore){
      if(lv->GetName().contains("LGSORow") ||
         lv->GetName().contains("NaI"))
         SetSensitiveDetector(lv->GetName(), SD);
   }
}

void MyDetectorConstruction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/MyCommands/Geo/", 
                                       "Scinti control");
   
   // Scinti 1
   G4GenericMessenger::Command &scinti1TCmd
      = fMessenger->DeclareMethodWithUnit("Scinti1T", "mm",
                                          &MyDetectorConstruction::SetScinti1T, 
                                          "Set the thickness of NaI.");
   scinti1TCmd.SetParameterName("t", true);
   scinti1TCmd.SetRange("t>=0. && t<100.");
   scinti1TCmd.SetDefaultValue("20.0");

   // Scinti 2
   G4GenericMessenger::Command &scinti2TCmd
      = fMessenger->DeclareMethodWithUnit("Scinti2T", "mm",
                                          &MyDetectorConstruction::SetScinti2T, 
                                          "Set the thickness of NaI.");
   scinti2TCmd.SetParameterName("t", true);
   scinti2TCmd.SetRange("t>=0. && t<100.");
   scinti2TCmd.SetDefaultValue("20.0");

}

void MyDetectorConstruction::SetScinti1T(G4double t)
{
   fScinti1T = t;
   G4Box *scinti = (G4Box*)(fScinti1PV->GetLogicalVolume()->GetSolid());
   scinti->SetZHalfLength(fScinti1T / 2.);
   fScinti1PV->SetTranslation(G4ThreeVector(0., 0., -fScinti1T / 2.));
   
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void MyDetectorConstruction::SetScinti2T(G4double t)
{
   fScinti2T = t;
   G4Box *scinti = (G4Box*)(fScinti2PV->GetLogicalVolume()->GetSolid());
   scinti->SetZHalfLength(fScinti2T / 2.);
   G4Box *scintiC = (G4Box*)(fScinti2ColumnPV->GetLogicalVolume()->GetSolid());
   scintiC->SetZHalfLength(fScinti2T / 2.);
   G4Box *scintiR = (G4Box*)(fScinti2RowPV->GetLogicalVolume()->GetSolid());
   scintiR->SetZHalfLength(fScinti2T / 2.);
   
   fScinti2PV->SetTranslation(G4ThreeVector(0., 0., fScinti2T / 2.));
   
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
