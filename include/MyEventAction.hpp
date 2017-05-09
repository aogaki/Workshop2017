#ifndef MyEventAction_h
#define MyEventAction_h 1

#include <G4UserEventAction.hh>

#include "MyHit.hpp"

class MyEventAction : public G4UserEventAction
{
public:
   MyEventAction();
   virtual ~MyEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   MyHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;
   
   G4int fHitsCollectionID;
};

#endif
