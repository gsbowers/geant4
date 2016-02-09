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
/// \file eventgenerator/exgps/src/g4EventAction.cc
/// \brief Implementation of the g4EventAction class
//

#include "g4EventAction.hh"

#ifdef G4ANALYSIS_USE
#include "g4AnalysisManager.hh"
#endif

#include "g4EventActionMessenger.hh"
#include "g4CMOSHit.hh"
#include "g4CMOS.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
g4EventAction::g4EventAction()
:fDrawFlag("all"), fPrintModulo(10000000), fEventMessenger(NULL)
{
  fEventMessenger = new g4EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4EventAction::~g4EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4EventAction::BeginOfEventAction(const G4Event* evt)
{
  
  G4int evtNb = evt->GetEventID();
	g4CMOS::Instance()->SetCurrentEventID(evtNb);
	
	//G4cout << GetEventID() << G4endl;

  //if (evtNb == 0){ 
		//G4SDManager *SDman = G4SDManager::GetSDMpointer();
		//fCollID_CMOS = SDman->GetCollectionID("g4CMOSHitsCollection");
  //}
  if (evtNb%fPrintModulo == 0){
      G4cout << "\n---> Begin of event: " << evtNb << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
#ifdef G4ANALYSIS_USE 
void g4EventAction::EndOfEventAction(const G4Event* evt)
#else
void g4EventAction::EndOfEventAction(const G4Event*)
#endif
{
  //  G4int evtNb = evt->GetEventID();
#ifdef G4ANALYSIS_USE 

  G4int nVertex = evt->GetNumberOfPrimaryVertex();
  for ( G4int j = 0; j < nVertex; j++) { 
    G4int nPart =  evt->GetPrimaryVertex(j)->GetNumberOfParticle(); 
    for ( G4int i = 0; i < nPart; i++) {
      G4ThreeVector position=evt->GetPrimaryVertex(j)->GetPosition();
      G4ThreeVector direction=evt->GetPrimaryVertex(j)->GetPrimary(i)
                                 ->GetMomentum();
      G4double P=direction.mag();
      G4double E=evt->GetPrimaryVertex(j)->GetPrimary(i)->GetMass();
      G4double E0=evt->GetPrimaryVertex(j)->GetPrimary(i)->GetG4code()
                                 ->GetPDGMass();
      E=std::sqrt(P*P+E0*E0);   
      E -= E0;
      G4double pid = G4double(evt->GetPrimaryVertex(j)->GetPrimary(i)
                                 ->GetPDGcode());
      //
      direction=direction*(1./direction.mag());                
      direction = -direction;  // reverse the direction
      G4double x,y,z,w;
      G4double Phi=direction.phi();
      if (Phi <0) Phi=Phi+twopi;
      G4double Theta=direction.theta();
      x=position.x();
      y=position.y();
      z=position.z();
      w = evt->GetPrimaryVertex(j)->GetPrimary(i)->GetWeight();
//     g4AnalysisManager::getInstance()->Fill(pname,E,x,y,z,Theta,Phi,w);
//     g4AnalysisManager::GetInstance()->Fill(pid,E,x,y,z,Theta,Phi,w);
    	g4AnalysisManager::GetInstance()->Fill(pid,E,x,y,z,Theta,Phi,w);
    }
  }   
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
