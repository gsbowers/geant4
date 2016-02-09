#include "g4CMOS.hh"
#include "g4EventAction.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"

#include <iostream>
#include <fstream>
using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4CMOS::g4CMOS(const G4String& name,
               const G4String& hitsCollectionName)
	: G4VSensitiveDetector(name),
    fHitsCollection(NULL),
		fCurrentEventID(0),
    fTotEdep(0.),
		fTotElectronEdep(0.),
		fTotGammaEdep(0.),
		fTotPositronEdep(0.),
    fNTotComptonElectron(0),
    fNTotPhotoElectron(0),
		fNTotPair(0),
		fNTotMscElectron(0),
    fNTotElectron(0),
		fNTotMscPositron(0),
		fNTotPositron(0),
    fNTotIncidentGamma(0), 
    fNTotInteractingGamma(0) 
{
	collectionName.insert(hitsCollectionName);	
	fgInstance = this;

	// open data file
	char hits_file[50];
	sprintf(hits_file, "%s_Hits.out", name.data());	
	fout.open(hits_file);
	fout << setw(8) << "event"
       << setw(6) << "hit" 
       << setw(12) << "particle"
       << setw(12) << "KE (keV)"
       << setw(12) << "Edep (keV)"
			 << setw(16) << "Process"
			 << setw(12) << "Position"
       << endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4CMOS::~g4CMOS()
{
	// close data file
	fout.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOS::Initialize(G4HCofThisEvent* hce)
{
	// create hits collection

	fHitsCollection
		= new g4CMOSHitsCollection(SensitiveDetectorName, collectionName[0]);
	
	// Add this collection in hce
	
	G4int hcID
		= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection( hcID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool g4CMOS::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{

	// energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();
	//if (edep==0.) return false;	

	G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
	G4double ke = aStep->GetPreStepPoint()->GetKineticEnergy();
	G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
	G4StepPoint * thePostPoint = aStep->GetPostStepPoint();
	G4String processName = thePostPoint->GetProcessDefinedStep()->GetProcessName();

	//G4int processSubType = thePostPoint->GetProcessDefinedStep()->GetProcessSubType();
	//G4String subProcessName = thePostPoint->GetProcessDefinedStep()->GetProcessTypeName(processSubType);

	//cout << processSubType << endl;

	g4CMOSHit *newHit = new g4CMOSHit();

	newHit->SetEdep(edep);
	newHit->SetKE(ke);
	newHit->SetPos(pos);
	newHit->SetParticleDefinition(pd);
	newHit->SetProcessName(processName);
	
	fHitsCollection->insert( newHit );
	//newHit->Print();

	return true;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOS::EndOfEvent(G4HCofThisEvent*)
{
	G4int nofHits = fHitsCollection->entries();
	G4ParticleDefinition *pd;
	G4double Edep;
	G4double KE;
	G4int evtNb = Instance()->GetCurrentEventID();

	//track total energy deposited for event 
	G4double EdepEvt = 0;

	//if TrackHits = 1, 
	//track edep at hit level, else
	//track edep at event level
	G4int TrackHits = 0;

	for ( G4int i=0; i<nofHits; i++ ){

		g4CMOSHit *aHit = (*fHitsCollection)[i];

		// get energy deposited for hit
		Edep = aHit->GetEdep();
		KE = aHit->GetKE(); 
		fTotEdep += Edep;

		// track energy deposited for this event
		EdepEvt += Edep;

		// get particle definition
		pd = aHit->GetParticleDefinition();

		// get interaction information
		if (aHit->GetProcessName() == "compt")
      fNTotComptonElectron += 1;	
		if (aHit->GetProcessName() == "phot")
      fNTotPhotoElectron += 1;	
		if (aHit->GetProcessName() == "conv")
      fNTotPair += 1;	

		// for electrons
		if ((pd->GetParticleName() == "e-") and (Edep > 0.)){
			fTotElectronEdep += Edep;
			if (aHit->GetProcessName()  == "msc") fNTotMscElectron += 1;
			fNTotElectron += 1;
		}

		// for positrons
		if ((pd->GetParticleName() == "e+") and (Edep > 0.)){
			fTotPositronEdep += Edep;
			if (aHit->GetProcessName()  == "msc") fNTotMscPositron += 1;
			fNTotPositron += 1;
		}

		// for gammas
		if ( pd->GetParticleName() == "gamma"){
			fTotGammaEdep += Edep;
      fNTotIncidentGamma += 1;	
			if (Edep > 0) fNTotInteractingGamma += 1;
		}

		// print hit information to file
		if (TrackHits){
		fout << setw(8) << evtNb 
         << setw(6) << i
         << setw(12) << pd->GetParticleName()
         << setw(12) << KE/keV
         << setw(12) << Edep/keV
				 << setw(16) << aHit->GetProcessName()
				 << setw(12) << aHit->GetPos()
         << endl;
		} 

	}

	// print event information to file
	if (!TrackHits){
		if (EdepEvt > 0){
		fout << setw(8) << evtNb 
   	    << setw(12) << EdepEvt/keV
   	    << endl;
		}
	}

	//if (fTotEdep > 0) cout << endl << fTotEdep << endl;

	if (verboseLevel>1){
		G4cout << "\n--->Hits Collection: in this event there are " << nofHits
           << " hits in the CMOS: " << G4endl;
		for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4CMOS* g4CMOS::fgInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
g4CMOS* g4CMOS::Instance()
{
	return fgInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOS::Reset()
{
	fTotEdep = 0.;
	fTotElectronEdep = 0.;
	fTotGammaEdep = 0.;
	fNTotComptonElectron = 0;
	fNTotPhotoElectron = 0;
	fNTotMscElectron = 0;
	fNTotElectron = 0;
	fNTotIncidentGamma = 0;
	fNTotInteractingGamma = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOS::Print()
{
	if (fTotEdep > 0) cout << endl << fTotEdep << endl;
}


