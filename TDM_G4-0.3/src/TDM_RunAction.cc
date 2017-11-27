#include "TDM_RunAction.hh"
#include "TDM_PrimaryGeneratorAction.hh"
#include "TDM_DetectorConstruction.hh"
#include "TDM_Run.hh"
#include "TDM_Analysis.hh"

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
{
	// Create analysis manager
	  // The choice of analysis technology is done via selectin of a namespace
	  // in B4Analysis.hh
	  auto analysisManager = G4AnalysisManager::Instance();
	  G4cout << "Using " << analysisManager->GetType() << G4endl;

	  // Create directories
	  //analysisManager->SetHistoDirectoryName("histograms");
	  //analysisManager->SetNtupleDirectoryName("ntuple");
	  analysisManager->SetVerboseLevel(1);
	  analysisManager->SetNtupleMerging(true);
	    // Note: merging ntuples is available only with Root output

	  // Book histograms, ntuple
	  //

	  G4int TLDNumber = 4;

	  // Creating ntuple
	  //
	  analysisManager->CreateNtuple("TDM", "Edep and Dodep");
	  for (G4int n = 0;n<TLDNumber;n++){

		 	 G4String N;
		 	 std::stringstream convert;
		 	 convert << n;
		 	 N = convert.str();

		 	 G4String a = N+"Edep";
		 	 G4String b = N+"Dodep";

	  analysisManager->CreateNtupleDColumn(a);
	  analysisManager->CreateNtupleDColumn(b);
	  }
	  analysisManager->FinishNtuple();
}

TDM_RunAction::~TDM_RunAction()
{
	  delete G4AnalysisManager::Instance();
}

G4Run* TDM_RunAction::GenerateRun()
{
  return new TDM_Run;
}

void TDM_RunAction::BeginOfRunAction(const G4Run*)
{

	  //inform the runManager to save random number seed
	  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	  // Get analysis manager
	  auto analysisManager = G4AnalysisManager::Instance();

	  // Open an output file
	  //
	  G4String fileName = "TDM-output1000";
	  analysisManager->OpenFile(fileName);
}

void TDM_RunAction::EndOfRunAction(const G4Run*)
{

	  // print histogram statistics
	  //
	  auto analysisManager = G4AnalysisManager::Instance();
/*	  if ( analysisManager->GetH1(1) ) {
	    G4cout << G4endl << " ----> print histograms statistic ";
	    if(isMaster) {
	      G4cout << "for the entire run " << G4endl << G4endl;
	    }
	    else {
	      G4cout << "for the local thread " << G4endl << G4endl;
	    }

	    G4cout << " EAbs : mean = "
	       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy")
	       << " rms = "
	       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;

	    G4cout << " EGap : mean = "
	       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
	       << " rms = "
	       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;

	    G4cout << " LAbs : mean = "
	      << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length")
	      << " rms = "
	      << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Length") << G4endl;

	    G4cout << " LGap : mean = "
	      << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length")
	      << " rms = "
	      << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;
	  }
*/
	  // save histograms & ntuple
	  //
	  analysisManager->Write();
	  analysisManager->CloseFile();
}
