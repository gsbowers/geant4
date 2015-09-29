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
/// \file eventgenerator/exgps/exgps.cc
/// \brief Main program of the eventgenerator/exgps example
//
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4ANALYSIS_USE
#include "g4AnalysisManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "g4GeometryConstruction.hh"
#include "g4PhysicsList.hh"
#include "g4PrimaryGeneratorAction.hh"
#include "g4RunAction.hh"
#include "g4EventAction.hh"
#include "QGSP_BERT_HP.hh"

int main(int argc,char** argv) {

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

#ifdef G4ANALYSIS_USE
  //constructe the analysis manager (need here to activate the UI)
  g4AnalysisManager::GetInstance();
#endif

  // set mandatory initialization classes
  g4GeometryConstruction* detector = new g4GeometryConstruction;
  runManager->SetUserInitialization(detector);
  //runManager->SetUserInitialization(new g4PhysicsList);

	G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
	runManager->SetUserInitialization(physicsList);
  runManager->SetUserInitialization(new g4PhysicsList);
  
  // Set optional user action classes
  g4EventAction* eventAction = new g4EventAction();
  g4RunAction* runAction = new g4RunAction();

  // set user action classes
  runManager->SetUserAction(new g4PrimaryGeneratorAction);
  runManager->SetUserAction(runAction);
  runManager->SetUserAction(eventAction);
  
  //Initialize G4 kernel
  runManager->Initialize();
    
  // visualization manager
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
    
  // get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  
  // UI->ApplyCommand("/control/execute display.mac");    

  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);    
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
#endif
    }

  // job termination

#ifdef G4ANALYSIS_USE
  g4AnalysisManager::Dispose();
#endif

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  
  return 0;
}
