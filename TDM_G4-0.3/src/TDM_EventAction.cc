#include "TDM_EventAction.hh"
#include "TDM_Run.hh"
#include "TDM_RunAction.hh"
#include "TDM_PrimaryGeneratorAction.hh"
#include "TDM_Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

TDM_EventAction::TDM_EventAction()
: G4UserEventAction()
{
    fAbsoEdepHCID = -1;
    fAbsoTrackLengthHCID = -1;
    fAbsoDodepHCID = -1;
}

TDM_EventAction::~TDM_EventAction()
{}


G4THitsMap<G4double>*
TDM_EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection
    = static_cast<G4THitsMap<G4double>*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("B4dEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double TDM_EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  for ( auto it : *hitsMap->GetMap() ) {
    // hitsMap->GetMap() returns the map of std::map<G4int, G4double*>
    sumValue += *(it.second);
  }
  return sumValue;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void TDM_EventAction::PrintEventStatistics(
 //                           G4double absoEdep, G4double absoTrackLength,
//                           G4double gapEdep, G4double gapTrackLength) const
void TDM_EventAction::PrintEventStatistics(
                            G4double absoEdep, G4double absoTrackLength,G4double absoDodep) const
{
  // Print event statistics
  //
  G4cout
     << "   Absorber: total energy: "
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: "
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
	 << "       total dose deposit: "
	 << std::setw(7) << G4BestUnit(absoDodep, "Dose")
	 << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void TDM_EventAction::BeginOfEventAction(const G4Event*)
{}

void TDM_EventAction::EndOfEventAction(const G4Event* event)
{
	// Get hist collections IDs
	 if ( fAbsoEdepHCID == -1 ) {
	    fAbsoEdepHCID
	      = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/Edep");
	    fAbsoDodepHCID
	    	      = G4SDManager::GetSDMpointer()->GetCollectionID("Absorber/Dodep");

	  }

	  // Get sum values from hits collections
	  //
	  auto absoEdep = GetSum(GetHitsCollection(fAbsoEdepHCID, event));
	  auto absoDodep = GetSum(GetHitsCollection(fAbsoDodepHCID, event));
	  //auto absoTrackLength
	  //  = GetSum(GetHitsCollection(fAbsoTrackLengthHCID, event));
	  // get analysis manager
	  auto analysisManager = G4AnalysisManager::Instance();


	  // fill ntuple
	  //
	  analysisManager->FillNtupleDColumn(0, absoEdep);
	  analysisManager->FillNtupleDColumn(1, absoDodep);
	  analysisManager->AddNtupleRow();

//	  G4cout << "Dose: " << absoDodep << G4endl;

	  //print per event (modulo n)
	  //
	  auto eventID = event->GetEventID();
//	  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
//	  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
	  G4cout << "\rEvent: " << eventID << std::flush;                       // No. Event
//	    PrintEventStatistics(absoEdep, absoTrackLength,absoDodep);
//	  }
}
