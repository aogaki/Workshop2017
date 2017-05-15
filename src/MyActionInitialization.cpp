#include "MyActionInitialization.hpp"
#include "MyPrimaryGeneratorAction.hpp"
#include "MyCo60.hpp"
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
   //SetUserAction(new MyPrimaryGeneratorAction());
   SetUserAction(new MyCo60());
   SetUserAction(new MyRunAction());
   SetUserAction(new MyEventAction());
}
