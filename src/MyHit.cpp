#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MyHit.hpp"


G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator = 0;


MyHit::MyHit()
   : G4VHit(),
     fTrackID(0),
     fKineticEnergy(0),
     fIncidentEnergy(0),
     fDepositEnergy(0),
     fTime(0),
     fPosition(0),
     fMomentum(0),
     fIsExit(false),
     fVolumeName("")
{}

MyHit::~MyHit()
{}

MyHit::MyHit(const MyHit & /*right*/)
   : G4VHit()
{}

const MyHit &
MyHit::operator=(const MyHit & /*right*/)
{
   return *this;
}

int MyHit::operator==(const MyHit & /*right*/) const
{
   return 0;
}
