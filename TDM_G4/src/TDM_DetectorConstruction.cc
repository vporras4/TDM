#include "TDM_DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

#include "G4PhysicalConstants.hh"

TDM_DetectorConstruction::TDM_DetectorConstruction()
: G4VUserDetectorConstruction()
{
	// World cube (world)
	WorldCube_SizeHalf	= 2.00*m;

	// Water cube (world)
	WaterCube_SizeHalf	= 0.50*m;

	// Water cylinder

	 innerRadius = 0.*m;

	 outerRadius = 0.25*m;

     hz = 0.9*m;

	 startAngle = 0.*deg;

	 spanningAngle = 360.*deg;

	 //camilla

	 camilla_X = 0.5*m;
	 camilla_Y = 0.8*m;
	 camilla_Z = 0.05*m;

	 ColimadorX_SizeHalf = 0.15*m;
	 ColimadorY_SizeHalf = 0.15*m;
	 ColimadorZ_SizeHalf = 0.025*m;

	 FieldX_SizeHalf = 0.05*m;
	 FieldY_SizeHalf = 0.05*m;

	 H1CX = 0.20;			//distancia fuente-parte final bloque
	 H2CX = 0.75;			//distancia fuente-parte final camilla
	 D1CX = (H1CX/H2CX)*FieldX_SizeHalf; //distancia eje x - bloque


	 H1CY = 0.25;							//distancia fuente-parte final bloque
	 H2CY = 0.75;							//distancia fuente-parte final camilla
	 D1CY = (H1CY/H2CY)*FieldY_SizeHalf;	//distancia eje y - bloque



}

TDM_DetectorConstruction::~TDM_DetectorConstruction()
{ }

G4VPhysicalVolume* TDM_DetectorConstruction::Construct()
{
//rotation

	G4RotationMatrix* myRotation = new G4RotationMatrix();
	 myRotation->rotateX(90.*deg);
	 myRotation->rotateY(0.*deg);
	 myRotation->rotateZ(0.*rad);


  // Some elements
  G4double a, z, density;
  G4int nelements;

  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* C = new G4Element("Carbon", "C",z=12, a = 12.01*g/mole);
  G4Element* Pb = new G4Element("Lead" , "L" , z=82, a=207.2*g/mole);


  // Air
  //

  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

  // Water material
  G4Material* water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  // Carbon Fiber
  G4Material* CarbonFiber = new G4Material("CarbonFiber", density= 0.145*g/cm3, nelements=1);
  CarbonFiber->AddElement(C, 1);

  //Lead
  G4Material* Lead = new G4Material("Lead", density=11.35*g/cm3, nelements=1);
  Lead->AddElement(Pb,1);

//=====================================



   //
   // Volumes definitions
   //



  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = false;

  //
  // World: WorldCube acts as world
  //

  G4Box* solid_WorldCube=
    new G4Box("WorldCube_solid",                       						//its name
    		WorldCube_SizeHalf, WorldCube_SizeHalf, WorldCube_SizeHalf);    //its size

  G4LogicalVolume* logic_WorldCube =
    new G4LogicalVolume(solid_WorldCube,          							//its solid
                        air,           										//its material
                        "WorldCube_logic");    								//its name

  G4VPhysicalVolume* physical_WorldCube =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(),       								//at (0,0,0)
                      logic_WorldCube,			          					//its logical volume
                      "WorldCube_physical",               					//its name
                      0,                     								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);        								//overlaps checking

// camilla

  G4Box* solid_camilla=
      new G4Box("camilla",                       						//its name
      		camilla_X, camilla_Y, camilla_Z);    //its size

  G4LogicalVolume* logic_camilla =
      new G4LogicalVolume(solid_camilla,          							//its solid
                          CarbonFiber,           										//its material
                          "camilla_logic");    								//its name

  //G4VPhysicalVolume* physical_camilla =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(0.0*m,0.0*m,-0.30*m),       			//at (0,0,-0.3)
                      logic_camilla,			          					//its logical volume
                      "camilla_physical",               					//its name
                      logic_WorldCube,                     								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);        								//overlaps checking





  /*G4Box* solid_WaterCube=
    new G4Box("WaterCube_solid",                       						//its name
    		WaterCube_SizeHalf, WaterCube_SizeHalf, WaterCube_SizeHalf);*/    //its size





 /* G4LogicalVolume* logic_WaterCube =
    new G4LogicalVolume(solid_WaterCube,          							//its solid
                        water,           									//its material
                        "WaterCube_logic");    								//its name

  G4VPhysicalVolume* physical_WaterCube =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(),       								//at (0,0,0)
                      logic_WaterCube,			          					//its logical volume
                      "WaterCube_physical",               					//its name
					  logic_WorldCube,         								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);     //overlaps checking*/
/*
 *
 *
 *
 *
 *
 * * * * * * * * Cilindro de agua******************************/
G4Tubs* trackerTube
  = new G4Tubs("Person",
		  innerRadius,
		  outerRadius,
		  hz,
		  startAngle,
		  spanningAngle
		  );

G4LogicalVolume* logic_WaterCylinder =
		new G4LogicalVolume(trackerTube,
				water,
				"WaterCylinder_logic");

//G4VPhysicalVolume* physical_WaterCylinder =
		new G4PVPlacement(myRotation,
				G4ThreeVector(),					//centrado en 0,0,0
				logic_WaterCylinder,
				"WaterCylinder_physical",
				logic_WorldCube,
				false,
				0,
				checkOverlaps
				);






  //                               COLIMADORES
  //				1 y 2 son en X, 3 y 4 son en Y.

 /********************************* COLIMADOR 1 **********************************/

   G4Box* Solid_Colimator1 =
    new G4Box("solid_colimator1 ",                       						//its name
    		ColimadorX_SizeHalf, ColimadorY_SizeHalf, ColimadorZ_SizeHalf);    //its size

  G4LogicalVolume* Logic_Colimator1 =
    new G4LogicalVolume(Solid_Colimator1,          							//its solid
                        water,           									//its material
                        "logic_colimator1 ");    								//its name

 //G4VPhysicalVolume* physical_Colimator1 =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(D1CX+ColimadorX_SizeHalf,0.0*m,-0.825*m),					//at (0,0,0)
                      Logic_Colimator1,			          					//its logical volume
                      "physical_colimator1 ",               					//its name
					  logic_WorldCube,         								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);     //overlaps checking*/


/*************************COLIMADOR 2************************************/

  G4Box* Solid_Colimator2=
   new G4Box("solid_colimator2",                       						//its name
   		ColimadorX_SizeHalf, ColimadorY_SizeHalf, ColimadorZ_SizeHalf);    //its size

 G4LogicalVolume* Logic_Colimator2 =
   new G4LogicalVolume(Solid_Colimator2,          							//its solid
                       Lead,           									//its material
                       "logic_colimator2");    								//its name

 //G4VPhysicalVolume* physical_Colimator2 =
   new G4PVPlacement(0,                 								    //no rotation
                     G4ThreeVector(-D1CX-ColimadorX_SizeHalf,0.0*m,-0.825*m),       								//at (0,0,0)
                     Logic_Colimator2,			          					//its logical volume
                     "physical_colimator2",               					//its name
					  logic_WorldCube,         								//its mother  volume
                     false,                 								//no boolean operation
                     0,                     								//copy number
                     checkOverlaps);     //overlaps checking*/


  /******************************COLIMADOR 3*************************************/


G4Box* Solid_Colimator3=
  new G4Box("solid_colimator3",                       						//its name
  		ColimadorX_SizeHalf, ColimadorY_SizeHalf, ColimadorZ_SizeHalf);    //its size

G4LogicalVolume* Logic_Colimator3 =
  new G4LogicalVolume(Solid_Colimator3,          							//its solid
                      Lead,           									//its material
                      "logic_colimator3");    								//its name

//G4VPhysicalVolume* physical_Colimator3 =
  new G4PVPlacement(0,                 								    //no rotation
                    G4ThreeVector(0.0*m,D1CY+ColimadorY_SizeHalf,-0.775*m),       								//at (0,0,0)
                    Logic_Colimator3,			          					//its logical volume
                    "physical_colimator3",               					//its name
					  logic_WorldCube,         								//its mother  volume
                    false,                 								//no boolean operation
                    0,                     								//copy number
                    checkOverlaps);     //overlaps checking*/


  /********************************COLIMADOR 4****************************/

G4Box* Solid_Colimator4=
 new G4Box("solid_colimator4",                       						//its name
 		ColimadorX_SizeHalf, ColimadorY_SizeHalf, ColimadorZ_SizeHalf);    //its size

G4LogicalVolume* Logic_Colimator4 =
 new G4LogicalVolume(Solid_Colimator4,          							//its solid
                     Lead,           									//its material
                     "logic_colimator4");    								//its name

//G4VPhysicalVolume* physical_Colimator4 =
 new G4PVPlacement(0,                 								    //no rotation
                   G4ThreeVector(0.0*m,-D1CY-ColimadorY_SizeHalf,-0.775*m),       								//at (0,0,0)
                   Logic_Colimator4,			          					//its logical volume
                   "physical_colimator4",               					//its name
					  logic_WorldCube,         								//its mother  volume
                   false,                 								//no boolean operation
                   0,                     								//copy number
                   checkOverlaps);     //overlaps checking*/


  /*************************************************************************/

   return physical_WorldCube;
}
