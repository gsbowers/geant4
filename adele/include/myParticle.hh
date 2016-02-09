#ifndef particle_h
#define particle_h 1

using namespace std;

class myParticle{

	public:
    myParticle(){};
    ~myParticle(){};

    double get_Energy(){ 
      return fEnergy; 
    };

    double* get_StartXYZ(){
      return fStartXYZ;
    };

    double* get_StartDir(){
      return fStartDir;
    };
 
    void set_Energy(double E){
      fEnergy = E;
    };
 
    void set_StartXYZ(double x, double y, double z){
      fStartXYZ[0] = x;
      fStartXYZ[1] = y;
      fStartXYZ[2] = z;
    };
 
    void set_StartDir(double a, double b, double c){
      fStartDir[0] = a;
      fStartDir[1] = b;
      fStartDir[2] = c;
    };

  private:
    double fEnergy; 
    double fStartXYZ[3]; 
    double fStartDir[3];
};

#endif
