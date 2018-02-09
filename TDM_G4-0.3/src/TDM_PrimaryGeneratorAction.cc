
#include "TDM_PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ios.hh"
#include "Randomize.hh"
//#include "globals.hh"

#include <cmath>
#include <map>


TDM_PrimaryGeneratorAction::TDM_PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
	//G4cout << "hola todos" << G4endl;
 // G4int n_particle = 1;
  //fParticleGun  = new G4GeneralParticleSource(n_particle);

  fParticleGun = new G4GeneralParticleSource();

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
 // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  fParticleGun->SetParticlePosition(G4ThreeVector(-0.5495*m,0.265*m,0.499*m));
 // fParticleGun->SetParticleEnergy(40.0*keV);
}

TDM_PrimaryGeneratorAction::~TDM_PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void TDM_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

	fParticleGun->GeneratePrimaryVertex(anEvent);

	  /*G4double x = 0.1;
	  G4double y = 0.1;
	  G4double z = 1.0;*/

	 //double xrand = -x+ G4UniformRand()*2*x;
	 //double yrand = y*(-1+2* G4UniformRand());

	//  G4cout << xrand << "\t" << yrand << G4endl; // Posición del haz

	//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xrand,yrand,z));
	 // fParticleGun->SetParticlePosition(G4ThreeVector(0.0*m,0.0*m,1.0*m));



}
