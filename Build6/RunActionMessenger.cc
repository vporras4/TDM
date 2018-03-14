//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: PrimaryGeneratorMessenger.cc 68740 2013-04-05 09:56:39Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TDDM_RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAInt.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TDM_RunActionMessenger::TDM_RunActionMessenger(
                                             PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  gunDir = new G4UIdirectory("/TDM/Ramdon");
  gunDir->SetGuidance("Estableciendo la semilla para los numeros aleatorios");
   
  //DefaultCmd = new G4UIcmdWithoutParameter("/ams/gun/setDefault",this);
  //DefaultCmd->SetGuidance("set/reset kinematic defined in PrimaryGenerator");
  //DefaultCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //RndmCmd = new G4UIcmdWithADoubleAndUnit("/ams/gun/beam",this);
  //RndmCmd->SetGuidance("random lateral extension on the beam");
  //RndmCmd->SetGuidance(" max: 0.5*sizeYZ");
  //RndmCmd->SetParameterName("rBeam",false);
  //RndmCmd->SetRange("rBeam>=0.");
  //RndmCmd->SetUnitCategory("Length");  
  //RndmCmd->AvailableForStates(G4State_Idle);

	Cmd = new G4UIcmdWithAInteger("/TDM/Random/Seed",this);
	Cmd->SetParameterName("Semilla",false);
	Cmd->SetRange("Semilla>0.");
	Cmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete DefaultCmd;
  delete RndmCmd;
  delete gunDir;  
  delete Cmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4int newValue)
{ 
   
  if( command == Cmd )
   { Action->SetSemilla(Cmd->GetNewIntValue(newValue));}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
