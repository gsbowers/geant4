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
/// \file eventgenerator/exgps/src/g4PhysicsList.cc
/// \brief Implementation of the g4PhysicsList class
//
#include "g4PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4ios.hh"              

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4PhysicsList::g4PhysicsList():  G4VUserPhysicsList()
{
  SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4PhysicsList::~g4PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructNuclei();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void g4PhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4LeptonConstructor.hh"
void g4PhysicsList::ConstructLeptons()
{
  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4MesonConstructor.hh"
void g4PhysicsList::ConstructMesons()
{
  //  Construct all mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4BaryonConstructor.hh"
void g4PhysicsList::ConstructBaryons()
{
  //  Construct all barions
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4PhysicsList::ConstructNuclei()
{
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4He3::He3Definition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4PhysicsList::ConstructProcess()
{
  AddTransportation();
	ConstructEM();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

/*#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4GammaConversion.hh"

#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"


void g4PhysicsList::ConstructEM(){

	G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();	
	G4ProcessManager* gmanager = gamma->GetProcessManager();

	gmanager->AddDiscreteProcess(new G4ComptonScattering);
	gmanager->AddDiscreteProcess(new G4GammaConversion);
	gmanager->AddDiscreteProcess(new G4PhotoElectricEffect);

	G4ParticleDefinition* e = G4Electron::ElectronDefinition();
	G4ProcessManager* emanager = e->GetProcessManager();

	emanager->AddProcess(new G4eBremsstrahlung);
	emanager->AddProcess(new G4eIonisation);
	emanager->AddProcess(new G4eMultipleScattering);

}

                 WHY DOESN'T THIS WORK!!!!

*/

#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4PhysicsList::ConstructEM()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
          
      ph->RegisterProcess(new G4PhotoElectricEffect, particle);
      ph->RegisterProcess(new G4ComptonScattering,   particle);
      ph->RegisterProcess(new G4GammaConversion,     particle);
      
    } else if (particleName == "e-") {

      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);      

    } else if (particleName == "e+") {

      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);
      ph->RegisterProcess(new G4eplusAnnihilation,   particle);      
    }
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4PhysicsList::SetCuts()
{
  // surppress error messages even in case e/gamma/proton do not exist            
  G4int temp = GetVerboseLevel();
  SetVerboseLevel(0);                                                         
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();   
  
  // Retrieve verbose level
  SetVerboseLevel(temp);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// from example N02

#include "G4StepLimiter.hh"

void g4PhysicsList::AddStepMax()
{
	// Step limition seen as a process 
	G4StepLimiter* stepLimiter = new G4StepLimiter();
		
	theParticleIterator->reset();	
	while ((*theParticleIterator)()){
			G4ParticleDefinition* particle = theParticleIterator->value();
			G4ProcessManager* pmanager = particle->GetProcessManager();
			
			if (particle->GetPDGCharge() != 0.0)
			{
				pmanager->AddDiscreteProcess(stepLimiter);
			}
	}
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
