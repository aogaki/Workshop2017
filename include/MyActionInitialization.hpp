#ifndef MyActionInitialization_h
#define MyActionInitialization_h 1

#include <G4VUserActionInitialization.hh>
#include <globals.hh>


class MyActionInitialization : public G4VUserActionInitialization
{
public:
   MyActionInitialization(G4bool monoFlag, G4double ene);
   virtual ~MyActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   G4bool fMonoFlag;
   G4double fEnergy;
      
};

#endif
