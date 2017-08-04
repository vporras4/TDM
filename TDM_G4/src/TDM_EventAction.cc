#include "TDM_EventAction.hh"
#include "TDM_Run.hh"
#include "TDM_RunAction.hh"
#include "TDM_PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"

TDM_EventAction::TDM_EventAction()
: G4UserEventAction()
{}

TDM_EventAction::~TDM_EventAction()
{}

void TDM_EventAction::BeginOfEventAction(const G4Event*)
{}

void TDM_EventAction::EndOfEventAction(const G4Event*)
{}
