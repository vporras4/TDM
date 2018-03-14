#include "TDM_RunAction.hh"
#include "TDM_PrimaryGeneratorAction.hh"
#include "TDM_DetectorConstruction.hh"
#include "TDM_Run.hh"
#include "TDM_Analysis.hh"
#include "TDM_RunActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4WorkerRunManager.hh"
#include <G4WorkerThread.hh>
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
//#include "CLHEP/Random/Random.h"
#include "Randomize.hh"

#include <ctime>
#include <string>
#include <vector>

#include <stdio.h>      /* puts */
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

 #include "G4Threading.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TDM_RunAction::TDM_RunAction()
: G4UserRunAction()
{

	TheSeed = G4Random::getTheSeed();;
	//create a messenger for this class
	  MessengerAction = new TDM_RunActionMessenger(this);

	 G4RunManager::RMType TType = G4RunManager::GetRunManager()->GetRunManagerType();
	 switch( TType ){
		 case G4RunManager::RMType::sequentialRM:
		 {
			 G4cout << "Corriendo en modo secuencial" << G4endl;
			 break;
		 }
		 case G4RunManager::RMType::masterRM:
		 {

			 G4cout << "Corriendo en el hilo maestro" << G4endl;
			 break;
		 }
		 case G4RunManager::RMType::workerRM:
		 {

			 G4cout << "Corriendo en un hilo esclavo" << G4endl;
			 const TDM_RunAction *MasterRunAction = static_cast<const TDM_RunAction*>(G4MTRunManager::GetMasterRunManager()->GetUserRunAction());
			 //const TDM_RunAction *MasterRunAction = static_cast<const TDM_RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
			 //G4int tmp = 0;
			 G4int tmp = MasterRunAction->GetSemilla();
			 G4cout<<"master seed: "<< tmp << G4endl;
			 G4int TId = G4Threading::G4GetThreadId();
			 G4int tmp2 = tmp*(TId+2);
			 G4Random::setTheSeed(tmp2);
			 break;
		 }
	 }
     //= G4RunManager::GetRunManager();
	//G4cout<<"Thread ID is: "<< G4Threading::G4GetPidId() << G4endl;
	G4cout<<"Thread ID is: "<< G4Threading::G4GetThreadId() << G4endl;

	// Create analysis manager
	  // The choice of analysis technology is done via selectin of a namespace
	  // in B4Analysis.hh

//	G4Random::setTheSeed(TheSeed);
	int startSeed = G4Random::getTheSeed();
	G4cout<<"TDM_RunAction::TDM_RunAction Semilla: " << startSeed << G4endl;

	  //SetSemilla(567);

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

		 	 G4String a = N+"AbsoEdep";
		 	 G4String b = N+"AbsoDodep";

	  analysisManager->CreateNtupleDColumn(a);
	  analysisManager->CreateNtupleDColumn(b);
	  }
	  analysisManager->FinishNtuple();
}

TDM_RunAction::~TDM_RunAction()
{
	  delete G4AnalysisManager::Instance();
	  delete MessengerAction;
	 // delete Action;
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

	  //Definiendo el tiempo actual de maquina

	  time_t rawtime;
	    struct tm * timeinfo;
	    char buffer [80];

	    time (&rawtime);
	    timeinfo = localtime (&rawtime);

	    strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

	    G4String M;
	    std::stringstream convert;
	    convert << buffer;
	    M = convert.str();

	  // Open an output file
	  //
	  G4String fileName = "TDM-output-" + M;
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

void TDM_RunAction::SetSemilla (G4int M){
	TheSeed = M;
	G4cout << "Semilla del archivo: " << M << G4endl;
	//G4Random::setTheSeed(M);
    //int startSeed = G4Random::getTheSeed();
    //G4cout<<"TDM_RunAction::SetSemilla Semilla: " << startSeed << G4endl;
}
