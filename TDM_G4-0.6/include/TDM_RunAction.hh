#ifndef TDM_RunAction_h
#define TDM_RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <vector>

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class TDM_RunAction : public G4UserRunAction
{
  public:
    TDM_RunAction();
    virtual ~TDM_RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
