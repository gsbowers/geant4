#ifndef myPhysicsList_h
#define myPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class myPhysicsList: public G4VUserPhysicsList
{
	public:
		myPhysicsList();
		virtual ~myPhysicsList();

	protected:
    // Construct particle and physics
		virtual void ConstructParticle();
		virtual void ConstructProcess();
		void SetCuts();

	public:

	protected:
    // these methods Construct particles 
    void ConstructBosons();
		void ConstructLeptons();
		void ConstructMesons();
		void ConstructBaryons();
		void ConstructNuclei();
		void AddStepMax();

		void ConstructEM();

};

#endif
