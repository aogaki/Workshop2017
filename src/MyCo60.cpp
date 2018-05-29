/*
   Making isotope Co-60
*/

#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>

#include "MyCo60.hpp"

MyCo60::MyCo60() : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -300. * mm));
  fParticleGun->SetParticleEnergy(511. * keV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

MyCo60::~MyCo60()
{
  if (fParticleGun != nullptr) {
    delete fParticleGun;
    fParticleGun = nullptr;
  }
}

void MyCo60::GeneratePrimaries(G4Event* event)
{
  G4double divergence = 5. * deg;
  fParticleGun->SetParticleEnergy(1.17 * MeV);
  fParticleGun->SetParticleMomentumDirection(GetDirection(divergence));
  fParticleGun->GeneratePrimaryVertex(event);

  fParticleGun->SetParticleEnergy(1.33 * MeV);
  fParticleGun->SetParticleMomentumDirection(GetDirection(divergence));
  fParticleGun->GeneratePrimaryVertex(event);
}

G4ThreeVector MyCo60::GetDirection(G4double limit)
{
  G4double theta = acos(1. - G4UniformRand() * (1. - cos(limit)));
  G4double phi = G4UniformRand() * CLHEP::twopi;
  G4double vx = sin(theta) * cos(phi);
  G4double vy = sin(theta) * sin(phi);
  G4double vz = cos(theta);
  return G4ThreeVector(vx, vy, vz);
}
