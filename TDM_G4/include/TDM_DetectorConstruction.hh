#ifndef TDM_DetectorConstruction_h
#define TDM_DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"


class G4LogicalVolume;

class TDM_DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  // constructor and destructor.
  TDM_DetectorConstruction();
  virtual ~TDM_DetectorConstruction();

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
  G4double H2CX;
  G4double D1CX;


  G4double H1CY;
  G4double H2CY;
  G4double D1CY;


};
#endif
