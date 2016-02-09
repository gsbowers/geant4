#ifndef g4CMOS_HH
#define g4CMOS_HH 1

#include "G4VSensitiveDetector.hh"
#include "g4CMOSHit.hh"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class G4HCofThisEvent;
class G4Step;

class g4CMOS : public G4VSensitiveDetector 
{
public:

	g4CMOS(const G4String& name,
         const G4String& hitsCollectionName);
	virtual ~g4CMOS();

	// Methods from base class
	virtual void Initialize(G4HCofThisEvent* hitCollection);
	virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);	
	virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
	
	void Print();
	static g4CMOS* Instance();

	// get methods
	G4int GetCurrentEventID() const { return fCurrentEventID; }
	G4double GetTotEdep() const { return fTotEdep; }
	G4double GetTotElectronEdep() const { return fTotElectronEdep; }
	G4double GetTotGammaEdep() const { return fTotGammaEdep; }
	G4double GetTotPositronEdep() const { return fTotPositronEdep; }
	G4double GetNTotElectron() const { return fNTotElectron; }
	G4double GetNTotComptonElectron() const { return fNTotComptonElectron; }
	G4double GetNTotPhotoElectron() const { return fNTotPhotoElectron; }
	G4double GetNTotPair() const { return fNTotPair; }
	G4double GetNTotMscElectron() const { return fNTotMscElectron; }
	G4double GetNTotPositron() const { return fNTotPositron; }
	G4double GetNTotMscPositron() const { return fNTotMscPositron; }
	G4double GetNTotIncidentGamma() const { return fNTotIncidentGamma; }
	G4double GetNTotInteractingGamma() const { return fNTotInteractingGamma; }

	// set methods
	void SetCurrentEventID(G4int val) { fCurrentEventID = val;};
	void Reset();
private:
	static g4CMOS* fgInstance;

	g4CMOSHitsCollection* fHitsCollection;	
	G4int fCurrentEventID;
	G4double fTotEdep;
	G4double fTotElectronEdep;
	G4double fTotGammaEdep;
	G4double fTotPositronEdep;
	G4int fNTotComptonElectron;
	G4int fNTotPhotoElectron;
	G4int fNTotPair;
	G4int fNTotMscElectron;
	G4int fNTotElectron;
	G4int fNTotMscPositron;
	G4int fNTotPositron;
	G4int fNTotIncidentGamma;
	G4int fNTotInteractingGamma;

	ofstream fout;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
