#ifndef myDetectorSD_h
#define myDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "myDetectorHit.hh"

#include <iostream>
#include <fstream>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// my Sensitive Detector

class myDetectorSD : public G4VSensitiveDetector
{
	public:
		myDetectorSD(G4String name);
		virtual ~myDetectorSD();
	
		virtual void Initialize(G4HCofThisEvent *HCE);
		virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory* ROhist);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
    static myDetectorSD* Instance();

    G4int GetCurrentEventID() const { return fCurrentEventID; }
    void SetCurrentEventID(G4int val) {fCurrentEventID = val; }


	private:
    static myDetectorSD* fgInstance;
    G4int fCurrentEventID;

		myDetectorHitsCollection* fHitsCollection;		
		G4int fHCID;

		ofstream fout;

    // intermediate array storage
    G4int fcount;
		G4double* fEdepEvt;
    G4int* fevtNb;
};

#endif
