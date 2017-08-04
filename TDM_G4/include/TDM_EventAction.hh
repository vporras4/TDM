#ifndef TDM_EventAction_h
#define TDM_EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class TDM_EventAction : public G4UserEventAction
{
  public:
    TDM_EventAction();
    virtual ~TDM_EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
