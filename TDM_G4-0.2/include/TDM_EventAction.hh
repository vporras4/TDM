#ifndef TDM_EventAction_h
#define TDM_EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4THitsMap.hh"

/// Event action class
///

class TDM_EventAction : public G4UserEventAction
{
  public:
    TDM_EventAction();
    virtual ~TDM_EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    /***************** Primitive Score ******************/

  private:
    // methods
    G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
    G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
//    void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
//                              G4double gapEdep, G4double gapTrackLength) const;
    void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,G4double absoDodep) const;

    // data members
    G4int  fAbsoEdepHCID;
    G4int  fAbsoTrackLengthHCID;
    G4int fAbsoDodepHCID;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
