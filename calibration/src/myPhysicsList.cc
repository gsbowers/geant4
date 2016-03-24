#include "myPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4Material.hh"
#include "G4ios.hh"              

// \file eventgenerator/exgps/src/g4PhysicsList.cc
// \brief Implementation of the myPhysicsList class

myPhysicsList::myPhysicsList(): G4VUserPhysicsList()
{
}

myPhysicsList::~myPhysicsList()
{}

void myPhysicsList::ConstructParticle()
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

void myPhysicsList::ConstructBosons()
{
  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4LeptonConstructor.hh"
void myPhysicsList::ConstructLeptons()
{
  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4MesonConstructor.hh"
void myPhysicsList::ConstructMesons()
{
  //  Construct all mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4BaryonConstructor.hh"
void myPhysicsList::ConstructBaryons()
{
  //  Construct all barions
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle(); 
}

void myPhysicsList::ConstructNuclei()
{
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4He3::He3Definition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();
}

void myPhysicsList::ConstructProcess()
{
  AddTransportation();
	ConstructEM();
}

#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

void myPhysicsList::ConstructEM()
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

void myPhysicsList::SetCuts()
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
