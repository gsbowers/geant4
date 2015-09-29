#ifndef G4CMOSHit_HH
#define G4CMOSHit_HH

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4ParticleDefinition.hh"
#include "G4VHit.hh"

class g4CMOSHit : public G4VHit 
{

	public:
	
		g4CMOSHit();
		g4CMOSHit(const g4CMOSHit&);
		virtual ~g4CMOSHit();

		// operators
		const g4CMOSHit& operator=(const g4CMOSHit&);
		G4int operator==(const g4CMOSHit&) const;
	
		inline void *operator new(size_t);
		inline void operator delete(void*);
	
		// methods from base class
		virtual void Draw();
		virtual void Print();

		// set methods
		void SetParticleDefinition (G4ParticleDefinition *pd) {fPD=pd;};
		void SetEdep (G4double de) {fEdep = de;};
		void SetKE   (G4double e) {fKE = e;};
		void SetPos  (G4ThreeVector xyz) {fPos = xyz;};
		void SetProcessName (G4String &name) {fProcessName = name;};

		// get methods
		G4ParticleDefinition* GetParticleDefinition() const {return fPD;};
		G4double GetEdep() const {return fEdep;}; 
		G4double GetKE()   const {return fKE;};
		G4ThreeVector GetPos() const {return fPos;};
		G4String GetProcessName() const {return fProcessName;};
	
	private:
	
		G4ParticleDefinition* fPD;
		G4double fEdep;
		G4double fKE;
		G4ThreeVector fPos; 
		G4String fProcessName;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<g4CMOSHit> g4CMOSHitsCollection;

extern G4Allocator<g4CMOSHit> g4CMOSHitAllocator;		

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* g4CMOSHit::operator new(size_t)
{
	void* hit;
	hit = (void*)g4CMOSHitAllocator.MallocSingle();
	return hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void g4CMOSHit::operator delete(void *hit)
{
	g4CMOSHitAllocator.FreeSingle((g4CMOSHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
