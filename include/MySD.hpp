#ifndef MySD_h
#define MySD_h 1

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "MyHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MySD: public G4VSensitiveDetector
{
public:
   MySD(const G4String &name,
            const G4String &hitsCollectionName);
   virtual ~MySD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   MyHitsCollection *fHitsCollection;
};

#endif
