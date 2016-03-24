#include "myDetectorConstruction.hh"
#include "myActionInitialization.hh"
#include "QGSP_BERT_HP.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char** argv){

	// construct the default run manager
	G4RunManager* runManager = new G4RunManager;

	// set mandatory initialization classes
	runManager->SetUserInitialization(new myDetectorConstruction);
	runManager->SetUserInitialization(new QGSP_BERT_HP);	
	runManager->SetUserInitialization(new myActionInitialization);	

	// initialize G4 kernal
	runManager->Initialize();

  // visualization manager
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
   // get the pointer to the User Interface manager
   G4UImanager* UImanager = G4UImanager::GetUIpointer();

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
     G4String command = "/control/execute vis.mac";
     UImanager->ApplyCommand(command);
     ui->SessionStart();
     delete ui;
#endif
   }

	// job termination
	delete visManager;
	delete runManager;
	return 0;

}
