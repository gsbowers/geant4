#ifndef myDetectorHit_h
#define myDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

using namespace std;

class myDetectorHit : public G4VHit
{
	public:
		myDetectorHit();
		virtual ~myDetectorHit();
		
		inline void *operator new(size_t);
		inline void operator delete(void *aHit);

		void Print();

		void SetEdep(G4double val) { fEdep = val; }

		G4double GetEdep() { return fEdep; }

	private:

		G4double fEdep;
};

typedef G4THitsCollection<myDetectorHit> myDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<myDetectorHit>* myDetectorHitAllocator;

inline void* myDetectorHit::operator new(size_t)
{
	if (!myDetectorHitAllocator)
		myDetectorHitAllocator = new G4Allocator<myDetectorHit>;
	return (void*) myDetectorHitAllocator->MallocSingle();
}

inline void myDetectorHit::operator delete(void *aHit)
{
	myDetectorHitAllocator->FreeSingle((myDetectorHit*) aHit);
}

#endif
