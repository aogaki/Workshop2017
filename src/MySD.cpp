#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4TouchableHistory.hh>
#include <G4Step.hh>
#include <G4ios.hh>
#include <g4root.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4VProcess.hh>

#include "MySD.hpp"
#include "MyHit.hpp"


MySD::MySD(const G4String &name,
             const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

MySD::~MySD()
{}

void MySD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool MySD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   G4Track *track = step->GetTrack();   
   G4int trackID = track->GetTrackID();
   if(trackID != 1) return false; // only the primal particle

   G4int parentID = track->GetParentID();

   G4ParticleDefinition *particle = track->GetDefinition();
   G4int pdgCode = particle->GetPDGEncoding();
   G4String parName = particle->GetParticleName();

   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   G4int isExit = (postStepPoint->GetStepStatus() == fGeomBoundary);
   //if(isExit == 0) return false; // only going out particle
   MyHit *newHit = new MyHit();
   newHit->SetIsExit(isExit);
      
   newHit->SetTrackID(trackID);
   
   G4StepPoint *preStepPoint = step->GetPreStepPoint();
   G4String volumeName = preStepPoint->GetPhysicalVolume()->GetName();
   newHit->SetVolumeName(volumeName);

   if(preStepPoint->GetStepStatus() == fGeomBoundary){
      G4double incidentEnergy = preStepPoint->GetKineticEnergy();
      newHit->SetIncidentEnergy(incidentEnergy);
   }
   else newHit->SetIncidentEnergy(0.);
   
   G4ThreeVector position =  postStepPoint->GetPosition();
   newHit->SetPosition(position);
   G4TouchableHandle theTouchable = postStepPoint->GetTouchableHandle();
   G4ThreeVector localPosition = theTouchable->GetHistory()->
                                 GetTopTransform().TransformPoint(position);

   if(volumeName == "LGSORow"){
      G4TouchableHandle touch = preStepPoint->GetTouchableHandle();
      G4int copyNo = touch->GetCopyNumber();
      G4int motherCopyNo = touch->GetCopyNumber(1);
      //G4cout << motherCopyNo <<"\t"<< copyNo << G4endl;
   }
   
   G4String vertexName = track->GetLogicalVolumeAtVertex()->GetName();
   
   G4double kineticEnergy = postStepPoint->GetKineticEnergy();
   newHit->SetKineticEnergy(kineticEnergy);

   G4ThreeVector momentum =  postStepPoint->GetMomentum();
   newHit->SetMomentum(momentum);

   G4double depositEnergy = step->GetTotalEnergyDeposit();
   newHit->SetDepositEnergy(depositEnergy);

   G4double time = postStepPoint->GetGlobalTime();
   newHit->SetTime(time);

   fHitsCollection->insert(newHit);
   return true;
}
