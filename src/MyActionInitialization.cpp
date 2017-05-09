#include "MyActionInitialization.hpp"
#include "MyPrimaryGeneratorAction.hpp"
#include "MyRunAction.hpp"
#include "MyEventAction.hpp"


MyActionInitialization::MyActionInitialization()
   : G4VUserActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
   SetUserAction(new MyRunAction());
}

void MyActionInitialization::Build() const
{
   SetUserAction(new MyPrimaryGeneratorAction());
   SetUserAction(new MyRunAction());
   SetUserAction(new MyEventAction());
}
