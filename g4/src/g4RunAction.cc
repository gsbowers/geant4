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
/// \file eventgenerator/exgps/src/g4RunAction.cc
/// \brief Implementation of the g4RunAction class
//

#include "g4RunAction.hh"
#include "g4CMOS.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "g4Analysis.hh"

#ifdef G4ANALYSIS_USE
#include "g4AnalysisManager.hh"
#endif

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4RunAction::g4RunAction()
{
	fout.open("CMOS_RunSummary.txt"); 
	fout << setw(6) << "run"
       << setw(13) << "N Events"
       << setw(13) << "Edep_Tot"
       << setw(13) << "Edep_e-"
       << setw(13) << "Edep_e+"
       << setw(13) << "Edep_gamma"
       << setw(10) << "N_e-"
       << setw(10) << "N_compt" 
       << setw(10) << "N_phot"
       << setw(10) << "N_msce-"
       << setw(10) << "N_e+"
       << setw(10) << "N_pair"
       << setw(10) << "N_msce+"
       << setw(14) << "N_gamma"
       << setw(10) << "N_xgamma" << endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4RunAction::~g4RunAction()
{
	fout.close();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4RunAction::BeginOfRunAction(const G4Run* aRun)
{
 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer(); 
      UI->ApplyCommand("/vis/scene/notifyHandlers");
    } 
#endif
#ifdef G4ANALYSIS_USE
  // If analysis is used reset the histograms
  g4AnalysisManager::GetInstance()->BeginOfRun();
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void g4RunAction::EndOfRunAction(const G4Run* aRun)
{

	G4double TotEdep = g4CMOS::Instance()->GetTotEdep(); 
	G4double TotElectronEdep = g4CMOS::Instance()->GetTotElectronEdep();	
	G4double TotGammaEdep = g4CMOS::Instance()->GetTotGammaEdep();	
	G4double NTotElectron = g4CMOS::Instance()->GetNTotElectron();	
	G4double NTotComptonElectron = g4CMOS::Instance()->GetNTotComptonElectron();	
	G4double NTotPhotoElectron = g4CMOS::Instance()->GetNTotPhotoElectron();	
	G4double NTotMscElectron = g4CMOS::Instance()->GetNTotMscElectron();	
	G4double NTotIncidentGamma = g4CMOS::Instance()->GetNTotIncidentGamma();	
	G4double NTotInteractingGamma = g4CMOS::Instance()->GetNTotInteractingGamma();	
	G4int NumberOfEvents = g4CMOS::Instance()->GetCurrentEventID() + 1;

	G4double NTotPositron = g4CMOS::Instance()->GetNTotPositron();	
	G4double NTotPair = g4CMOS::Instance()->GetNTotPair();	
	G4double NTotMscPositron = g4CMOS::Instance()->GetNTotMscPositron();	
	G4double TotPositronEdep = g4CMOS::Instance()->GetTotPositronEdep();	


	G4cout << G4endl 
				 << "### Run " << aRun->GetRunID() << " Summary." 
         << endl << endl
				 << "Number of Events in Run: "
				 << NumberOfEvents << endl
         << "Energy Deposited in CMOS: "
         << TotEdep/MeV << " MeV" << G4endl
				 << "Energy from Electrons: "
         << TotElectronEdep/MeV << " MeV" << G4endl
				 << "Energy from Photons: "
         << TotGammaEdep/MeV << " MeV" << G4endl

				 << "Energy from Positrons: "
         << TotPositronEdep/MeV << " MeV" << G4endl

				 << "Number of Electrons: " 
         << NTotElectron << G4endl
				 << "Number of Compton Electrons: " 
         << NTotComptonElectron << G4endl
				 << "Number of Photo Electrons: " 
         << NTotPhotoElectron << G4endl
				 << "Number of Positrons: "
         << NTotPositron << G4endl

				 << "Number of msc Electrons interactions: " 
         << NTotMscElectron << G4endl

				 << "Number of Incident Photons "
         << NTotIncidentGamma << G4endl

				 << "Number of Interacting Photons "
         << NTotInteractingGamma << G4endl << G4endl;

	G4cout << setw(6) << aRun->GetRunID() 
       << setw(13) << NumberOfEvents 
       << setw(13) << TotEdep/MeV 
       << setw(13) << TotElectronEdep/MeV 
       << setw(13) << TotGammaEdep/MeV 
       << setw(13) << TotPositronEdep/MeV 
       << setw(10) << NTotElectron 
       << setw(10) << NTotComptonElectron 
       << setw(10) << NTotPhotoElectron 
       << setw(10) << NTotMscElectron
       << setw(10) << NTotPositron
       << setw(10) << NTotPair
       << setw(10) << NTotMscPositron
       << setw(14) << NTotIncidentGamma 
       << setw(10) << NTotInteractingGamma << G4endl;

	fout << setw(6) << aRun->GetRunID() 
       << setw(13) << NumberOfEvents 
       << setw(13) << TotEdep/MeV 
       << setw(13) << TotElectronEdep/MeV 
       << setw(14) << TotPositronEdep/MeV 
       << setw(14) << TotGammaEdep/MeV 
       << setw(14) << NTotElectron 
       << setw(14) << NTotComptonElectron 
       << setw(14) << NTotPhotoElectron 
       << setw(14) << NTotMscElectron 
       << setw(14) << NTotPositron
       << setw(14) << NTotPair
       << setw(14) << NTotMscPositron
       << setw(14) << NTotIncidentGamma 
       << setw(12) << NTotInteractingGamma << endl;


	fout << "### Run " << aRun->GetRunID() << " Summary." 
         << endl << endl
				 << "Number of Events in Run: "
				 << NumberOfEvents << endl
         << "Energy Deposited in CMOS: "
         << TotEdep/MeV << " MeV" << endl
				 << "Energy from Electrons: "
         << TotElectronEdep/MeV << " MeV" << endl
				 << "Energy from Photons: "
         << TotGammaEdep/MeV << " MeV" << endl
				 << "Number of Electrons: " 
         << NTotElectron << endl
				 << "Number of Compton Electrons: " 
         << NTotComptonElectron << endl
				 << "Number of Photo Electrons: " 
         << NTotPhotoElectron << endl
				 << "Number of Incident Photons "
         << NTotIncidentGamma << endl
				 << "Number of Interacting Photons "
         << NTotInteractingGamma << endl << endl;

	//reset g4CMOS values
	g4CMOS::Instance()->Reset();


#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance()) {
     G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }
#endif
  // If analysis is used

#ifdef G4ANALYSIS_USE 
  g4AnalysisManager::GetInstance()->EndOfRun();
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
