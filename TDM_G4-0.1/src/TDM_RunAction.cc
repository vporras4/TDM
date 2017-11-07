#include "TDM_RunAction.hh"
#include "TDM_PrimaryGeneratorAction.hh"
#include "TDM_DetectorConstruction.hh"
#include "TDM_Run.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <ctime>
#include <string>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TDM_RunAction::TDM_RunAction()
: G4UserRunAction()
{}

TDM_RunAction::~TDM_RunAction()
{}

G4Run* TDM_RunAction::GenerateRun()
{
  return new TDM_Run;
}

void TDM_RunAction::BeginOfRunAction(const G4Run*)
{}

void TDM_RunAction::EndOfRunAction(const G4Run*)
{}
