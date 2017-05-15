#ifndef MyCo60_h
#define MyCo60_h 1

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Event.hh>

class MyCo60: public G4VUserPrimaryGeneratorAction
{
public:
   MyCo60();
   virtual ~MyCo60();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ParticleGun *fParticleGun;
   G4ThreeVector GetDirection(G4double limit);

};

#endif
