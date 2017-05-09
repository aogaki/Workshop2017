#ifndef MyHit_h
#define MyHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class MyHit : public G4VHit
{
public:
   MyHit();
   virtual ~MyHit();
   MyHit(const MyHit &right);
   const MyHit &operator=(const MyHit &right);
   int operator==(const MyHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetTrackID(G4int id) {fTrackID = id;};
   G4int GetTrackID() {return fTrackID;};

   void SetKineticEnergy(G4double ene) {fKineticEnergy = ene;};
   G4double GetKineticEnergy() {return fKineticEnergy;};

   void SetIncidentEnergy(G4double ene) {fIncidentEnergy = ene;};
   G4double GetIncidentEnergy() {return fIncidentEnergy;};

   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};

   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};
   
   void SetMomentum(G4ThreeVector p) {fMomentum = p;};
   G4ThreeVector GetMomentum() {return fMomentum;};
   
   void SetIsExit(G4int flag) {fIsExit = flag;};
   G4int GetIsExit() {return fIsExit;};
   
   void SetVolumeName(G4String volumeName) {fVolumeName = volumeName;};
   G4String GetVolumeName() {return fVolumeName;};

   void SetTime(G4double time) {fTime = time;};
   G4double GetTime() {return fTime;};

private:
   G4int fTrackID;
   G4double fKineticEnergy;
   G4double fIncidentEnergy;
   G4double fDepositEnergy;
   G4double fTime;
   G4ThreeVector fPosition;
   G4ThreeVector fMomentum;
   G4int fIsExit;
   G4String fVolumeName;
};

typedef G4THitsCollection<MyHit> MyHitsCollection;

extern G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;

inline void *MyHit::operator new(size_t)
{
   if (!MyHitAllocator)
      MyHitAllocator = new G4Allocator<MyHit>;
   return (void *)MyHitAllocator->MallocSingle();
}

inline void MyHit::operator delete(void *hit)
{
   MyHitAllocator->FreeSingle((MyHit *) hit);
}

#endif
