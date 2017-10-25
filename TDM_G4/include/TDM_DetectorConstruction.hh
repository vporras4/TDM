#ifndef TDM_DetectorConstruction_h
#define TDM_DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"


class G4LogicalVolume;
class TDMMainVolume;


class TDM_DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  // constructor and destructor.
  TDM_DetectorConstruction();
  virtual ~TDM_DetectorConstruction();
  virtual void ConstructSDandField();
  void SetDefaults();

  // virtual method from G4VUserDetectorConstruction.
  virtual G4VPhysicalVolume* Construct();



  // Data members

  // World cube
  G4int WorldCube_SizeHalf;

  // Water cube
  G4int WaterCube_SizeHalf;

  //Water cylinder
 // G4int WaterCylinder_SizeHalf;
  G4double innerRadius;
  G4double outerRadius;
  G4double hz;
  G4double startAngle;
  G4double spanningAngle;

  //Camilla
  G4double camilla_X;
  G4double camilla_Y;
  G4double camilla_Z;


// Tamaño colimador

  G4double ColimadorX_SizeHalf;
  G4double ColimadorY_SizeHalf;
  G4double ColimadorZ_SizeHalf;

  G4double FieldX_SizeHalf;
  G4double FieldY_SizeHalf;

  G4double H1CX;
  G4double DF; //Distancia camilla-fuente
  G4double Distancia; //
  G4double D1CX;


  G4double H1CY;
  //G4double H2CY;
  G4double D1CY;

  // Blindaje Fuente

  G4double Largo;
  G4double Ancho;
  G4double grosor;
  G4double Xconstante;

  G4double Xfinal;
  G4double Yfinal;

  /**************************  CILINDRO PARA BLINDAJE *********************/

  G4double Radio_interno;
    G4double Radio_externo;
    G4double Altura_cilindro;
    G4double Angulo_Inicial;
    G4double Angulo_Final;
    G4double Grosor_cilindro;
    G4double Diagonal_campo;

    /************************* Tapadera cilindro blindaje ****************/

    G4double Radio_interno_tapadera;
        G4double Radio_externo_tapadera;
        G4double Altura_tapadera;
        G4double Angulo_Inicial_tapadera;
        G4double Angulo_Final_tapadera;
        G4double Campo_Max;
        G4double Corrimiento_colimadores;

     /***************************Detector de prueba *********************/

        G4double DetectorX_SizeHalf;
        G4double DetectorY_SizeHalf;
        G4double DetectorZ_SizeHalf;



       G4LogicalVolume* SensitiveDetector;

};
#endif
