#include "myEventAction.hh"
#include "myRunAction.hh"
#include "myDetectorHit.hh"
#include "myDetectorSD.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

myEventAction::myEventAction(myRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fLgPlHCID(-1)
{
}

myEventAction::~myEventAction()
{}

void myEventAction::BeginOfEventAction(const G4Event* evt)
{
	if (fLgPlHCID==-1) {
		G4SDManager* sdManager = G4SDManager::GetSDMpointer();
		fLgPlHCID = sdManager->GetCollectionID("LgPl/scintillator");
	}

  G4int evtNb = evt->GetEventID();
  myDetectorSD::Instance()->SetCurrentEventID(evtNb);
  if (evtNb % 100000 == 0){
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
  } 
}

void myEventAction::EndOfEventAction(const G4Event* event)
{
	G4HCofThisEvent* hce = event->GetHCofThisEvent();
	if (!hce)	
	{
		G4ExceptionDescription msg;
		msg << "No hits collection of this event found." << G4endl;
		G4Exception("myEventAction::EndofEventAction()", 
								"myCode001", JustWarning, msg);
		return;
	}
	myDetectorHitsCollection* hLgPl = static_cast<myDetectorHitsCollection*>(hce->GetHC(fLgPlHCID));

	// LgPl Detector
	G4int n_hit = hLgPl->entries();
	for (G4int i=0; i<n_hit; i++){
		myDetectorHit* hit = (*hLgPl)[i];
		//hit->Print();
	}
}
