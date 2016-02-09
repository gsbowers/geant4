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
/// \file eventgenerator/exgps/src/g4PrimaryGeneratorAction.cc
/// \brief Implementation of the g4PrimaryGeneratorAction class
//

#include "g4PrimaryGeneratorAction.hh"

#include "G4Event.hh"
//#include "G4GeneralParticleSource.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Orb.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "myParticle.hh"
#include "myParticleInput.hh"

#include<iostream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4PrimaryGeneratorAction::g4PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
	fParticleGun(0),
	fEnvelopeOrb(0)
{
   //fParticleGun = new G4GeneralParticleSource();

   G4int n_particle = 1;
   fParticleGun = new G4ParticleGun(n_particle);
 
   // default particle kinematic
   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4String particleName;
   G4ParticleDefinition* particle
      = particleTable->FindParticle(particleName="gamma");
   fParticleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4PrimaryGeneratorAction::~g4PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double shell_pRmax = 0; 
  
  if ( !fEnvelopeOrb ) {
		G4LogicalVolume* envLV
			= G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeOrb = dynamic_cast<G4Orb*>(envLV->GetSolid());
  }

  if ( fEnvelopeOrb ) {
    shell_pRmax = fEnvelopeOrb->GetRadius();
  }
  else {
    G4ExceptionDescription msg;
    msg << "Envelope volume of orb shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("g4PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
	}
  
  fParticleGun->GeneratePrimaryVertex(anEvent) ;
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
