#include "myDetectorSD.hh"
#include "myDetectorHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>
#include <string.h>
#include <fstream>

#define SIZE_DATA_BUFFER 65536

myDetectorSD::myDetectorSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
	G4String HCname = "scintillator"; // Hit Collection Name
	collectionName.insert(HCname);

  fgInstance = this;	
  
	char outfile[50];
	sprintf(outfile, "Hits.out");
  fout.open(outfile);
	fout << setw(12) << "event" 
       << setw(18) << "Edep (MeV)" << endl;
  
  // allocate memory for data buffers 
  fcount = 0;
  fEdepEvt = new G4double[SIZE_DATA_BUFFER];
  fevtNb = new G4int[SIZE_DATA_BUFFER];

}

myDetectorSD::~myDetectorSD()
{
  // write data buffer to file
  if (fcount > 0){
    for (G4int k=0; k < fcount; k++){
      fout << setw(12) << fevtNb[k]
           << setw(18) << fEdepEvt[k]
           << endl;
    }
    fcount = 0;
  }

  // delete data buffers
  delete[] fevtNb;
  delete[] fEdepEvt;

	fout.close();
}

void myDetectorSD::Initialize(G4HCofThisEvent* hce)
{
	fHitsCollection = new myDetectorHitsCollection
	(SensitiveDetectorName, collectionName[0]);
	if (fHCID<0)
	{ fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);}
	hce->AddHitsCollection(fHCID,fHitsCollection);
}


G4bool myDetectorSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // Get Energy Deposited in volume
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0) return false;
	
  // add hit to collection
	myDetectorHit* newHit = new myDetectorHit();
  newHit->SetEdep(edep);
	fHitsCollection->insert(newHit);

	return true;
}

void myDetectorSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nofHits = fHitsCollection->entries();
  G4double Edep;
  G4int evtNb = Instance()->GetCurrentEventID();

  // track total energy deposited in volume for event
  G4double EdepEvt = 0;
  
  // if TrackHits = 1 
  // track edep at hit level, else
  // track edep at event level
  G4int TrackHits = 0;
  
  for ( G4int i=0; i<nofHits; i++ ){
    myDetectorHit *aHit = (*fHitsCollection)[i]; 
   
    // get energy deposited for hit
    Edep = aHit->GetEdep();
    
    // track energy deposited for this event
    EdepEvt += Edep;
	}

  // save event information to data buffer
  if (!TrackHits){
    if (EdepEvt > 0){
      fEdepEvt[fcount] = EdepEvt/MeV;
      fevtNb[fcount] = evtNb;
      fcount++;
    }
  }

  // if data buffer is full, write contents to file
  if (fcount >= SIZE_DATA_BUFFER){
    for (G4int k=0; k < fcount; k++){
      fout << setw(12) << fevtNb[k]
           << setw(18) << fEdepEvt[k]
           << endl;
    }
    fcount = 0;
  }

} 

myDetectorSD* myDetectorSD::fgInstance = 0;

myDetectorSD* myDetectorSD::Instance()
{
	return fgInstance;
}

