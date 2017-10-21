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
#include "TDMSD.hh"
#include "G4SDManager.hh"

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

	 ColimadorX_SizeHalf = 0.2*m;
	 ColimadorY_SizeHalf = 0.2*m;
	 ColimadorZ_SizeHalf = 0.01*m;

	 Campo_Max = 0.2*m;
	 FieldX_SizeHalf = 0.05*m;
	 FieldY_SizeHalf = 0.05*m;

	 DF = 1.0*m;			//distancia fuente-parte final camilla
	 Distancia = 0.1*m;    //distancia de los colimadores a la fuente
	 H1CX = Distancia + (2.0*ColimadorZ_SizeHalf);			//distancia fuente-parte final bloque
	 D1CX = (H1CX/DF)*FieldX_SizeHalf; //distancia eje x - bloque


	 H1CY = Distancia + (4.0*ColimadorZ_SizeHalf);							//distancia fuente-parte final bloque
	 D1CY = (H1CY/DF)*FieldY_SizeHalf;	//distancia eje y - bloque


	 //////// Blindaje de la fuente
	 Xconstante = (H1CX/DF)*0.2*m;
	 Largo = ((ColimadorX_SizeHalf/2)+ Xconstante);
	 Ancho = ((ColimadorY_SizeHalf/2)+ Xconstante);
	 grosor = ColimadorZ_SizeHalf;


	 /***********Blindaje de forma cilindrica *******/

	 Grosor_cilindro = 0.02*m ;
	 Diagonal_campo = ((2^(3/2))*(Campo_Max)*Distancia)/DF; //Diagonal del campo en el cilindro
	 Radio_interno = Diagonal_campo/2;
	 Radio_externo = Radio_interno + grosor;
	 Altura_cilindro = 0.1*m;
	 Angulo_Inicial = 0. *deg;
	 Angulo_Final = 360. *deg;
	 Corrimiento_colimadores = ((FieldX_SizeHalf)*Distancia)/DF;

	 /********** Tapadera *********************/
	 Radio_interno_tapadera = 0. *m;
	 Radio_externo_tapadera = Radio_externo;
	 Altura_tapadera = Grosor_cilindro/4;
	 Angulo_Inicial_tapadera = 0. *deg;
	 Angulo_Final_tapadera  = 360.*m;

	 /**************Detector de prueba *****/

	 DetectorX_SizeHalf=50*cm;
	 DetectorY_SizeHalf=50*cm;
	 DetectorZ_SizeHalf=1*mm;
	 SensitiveDetector = 0;
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
/*G4Tubs* trackerTube
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

*/


  //                               COLIMADORES
  //				1 y 2 son en X, 3 y 4 son en Y.

 /********************************* COLIMADOR 1 **********************************/

    G4Box* Solid_Colimator1 =
    new G4Box("solid_colimator1 ",                       						//its name
    		ColimadorX_SizeHalf, ColimadorY_SizeHalf, ColimadorZ_SizeHalf);    //its size

  G4LogicalVolume* Logic_Colimator1 =
    new G4LogicalVolume(Solid_Colimator1,          							//its solid
                        Lead,           									//its material
                        "logic_colimator1 ");    								//its name

 //G4VPhysicalVolume* physical_Colimator1 =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(ColimadorX_SizeHalf+Corrimiento_colimadores,0.0*m,-DF +Distancia+ColimadorZ_SizeHalf),					//at (0,0,0)
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
                     G4ThreeVector(-ColimadorX_SizeHalf-Corrimiento_colimadores,0.0*m,-DF +Distancia +ColimadorZ_SizeHalf),       								//at (0,0,0)
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
                    G4ThreeVector(0.0*m,ColimadorY_SizeHalf+Corrimiento_colimadores,-DF + Distancia + (3*ColimadorZ_SizeHalf)),       								//at (0,0,0)
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
                   G4ThreeVector(0.0*m,-ColimadorY_SizeHalf-Corrimiento_colimadores,-DF +Distancia + (3*ColimadorZ_SizeHalf)),       								//at (0,0,0)
                   Logic_Colimator4,			          					//its logical volume
                   "physical_colimator4",               					//its name
					  logic_WorldCube,         								//its mother  volume
                   false,                 								//no boolean operation
                   0,                     								//copy number
                   checkOverlaps);     //overlaps checking*/


 /*************************************************************************/

 /**************Blindaje de la fuente *******************/
/*
 G4Box* Blindaje1 =
  new G4Box("Blindaje1",                       						//its name
  		grosor,2*Largo, Ancho);    //its size

 G4LogicalVolume* Logic_Blindaje1 =
  new G4LogicalVolume(Blindaje1,          							//its solid
                      Lead,           									//its material
                      "logic_blindaje1");    								//its name

 //G4VPhysicalVolume* physical_Colimator4 =
  new G4PVPlacement(0,                 								    //no rotation
                    G4ThreeVector(-Largo-grosor,0.0*m, -DF+(Distancia - Ancho)),       								//at (0,0,0)
                    Logic_Blindaje1,			          					//its logical volume
                    "physical_blindaje1",               					//its name
 					  logic_WorldCube,         								//its mother  volume
                    false,                 								//no boolean operation
                    0,                     								//copy number
                    checkOverlaps);     //overlaps checking

  G4Box* Blindaje2 =
    new G4Box("Blindaje2",                       						//its name
    		grosor,2*Largo, Ancho);    //its size

   G4LogicalVolume* Logic_Blindaje2 =
    new G4LogicalVolume(Blindaje2,          							//its solid
                        Lead,           									//its material
                        "logic_blindaje2");    								//its name

   //G4VPhysicalVolume* physical_Colimator4 =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(Largo+grosor,0.0*m, -DF+(Distancia-Ancho)),       								//at (0,0,0)
                      Logic_Blindaje2,			          					//its logical volume
                      "physical_blindaje2",               					//its name
   					  logic_WorldCube,         								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);     //overlaps checking


    G4Box* Blindaje3 =
      new G4Box("Blindaje3",                       						//its name
      		Largo,grosor, Ancho);    //its size

     G4LogicalVolume* Logic_Blindaje3 =
      new G4LogicalVolume(Blindaje3,          							//its solid
                          Lead,           									//its material
                          "logic_blindaje1");    								//its name


      new G4PVPlacement(0,                 								    //no rotation
                        G4ThreeVector(0.0*m,-Largo-grosor, -DF+Distancia-Ancho),       								//at (0,0,0)
                        Logic_Blindaje3,			          					//its logical volume
                        "physical_blindaje3",               					//its name
     					  logic_WorldCube,         								//its mother  volume
                        false,                 								//no boolean operation
                        0,                     								//copy number
                        checkOverlaps);     //overlaps checking


      G4Box* Blindaje4 =
            new G4Box("Blindaje4",                       						//its name
            		Largo,grosor, Ancho);    //its size

           G4LogicalVolume* Logic_Blindaje4 =
            new G4LogicalVolume(Blindaje4,          							//its solid
                                Lead,           									//its material
                                "logic_blindaje4");    								//its name


            new G4PVPlacement(0,                 								    //no rotation
                              G4ThreeVector(0.0*m,Largo+grosor, -DF+Distancia-Ancho),       								//at (0,0,0)
                              Logic_Blindaje4,			          					//its logical volume
                              "physical_blindaje4",               					//its name
           					  logic_WorldCube,         								//its mother  volume
                              false,                 								//no boolean operation
                              0,                     								//copy number
                              checkOverlaps);     //overlaps checking


            G4Box* Blindaje5 =
                  new G4Box("Blindaje5",                       						//its name
                  		2*Largo,2*Ancho, grosor);    //its size

                 G4LogicalVolume* Logic_Blindaje5 =
                  new G4LogicalVolume(Blindaje5,          							//its solid
                                      Lead,           									//its material
                                      "logic_blindaje5");    								//its name


                  new G4PVPlacement(0,                 								    //no rotation
                                    G4ThreeVector(0.0*m,0.0*m, -DF-(2*Ancho - Distancia)-grosor),       								//at (0,0,0)
                                    Logic_Blindaje5,			          					//its logical volume
                                    "physical_blindaje5",               					//its name
                 					  logic_WorldCube,         								//its mother  volume
                                    false,                 								//no boolean operation
                                    0,                     								//copy number
                                    checkOverlaps);     //overlaps checking*/

 /***************** Blindaje de forma cilindrica *************/

 G4Tubs* Blindaje_cilindro
   = new G4Tubs("cilindroblindaje",
 		  Radio_interno,
 		  Radio_externo,
 		  Altura_cilindro,
 		  Angulo_Inicial,
 		  Angulo_Final
 		  );

 G4LogicalVolume* logic_cilindroblindaje =
 		new G4LogicalVolume(Blindaje_cilindro,
 				Lead,
 				"cilindroblindaje_logic");

 //G4VPhysicalVolume* physical_WaterCylinder =
 		new G4PVPlacement(0,
 				G4ThreeVector(0.0*m,0.0*m, -DF+Distancia-Altura_cilindro ),					//centrado en 0,0,0
 				logic_cilindroblindaje,
 				"Leadclinder_physical",
 				logic_WorldCube,
 				false,
 				0,
 				checkOverlaps
 				);

/*****************Tapadera Blindaje *********************/

 		G4Tubs* Tapadera
 		   = new G4Tubs("tapaderablindaje",
 		 		  Radio_interno_tapadera,
 		 		  Radio_externo_tapadera,
 		 		  Altura_tapadera,
 		 		  Angulo_Inicial_tapadera,
 		 		  Angulo_Final_tapadera
 		 		  );

 		 G4LogicalVolume* logic_tapadera =
 		 		new G4LogicalVolume(Tapadera,
 		 				Lead,
 		 				"tapadera_logic");

 		 //G4VPhysicalVolume* physical_WaterCylinder =
 		 		new G4PVPlacement(0,
 		 				G4ThreeVector(0.0*m,0.0*m, -DF-Altura_cilindro-Altura_tapadera),					//centrado en 0,0,0
 		 				logic_tapadera,
 		 				"Leadtapadera_physical",
 		 				logic_WorldCube,
 		 				false,
 		 				0,
 		 				checkOverlaps
 		 				);

 /******************* Detector prueba ****************/

 		 		G4Box* Detector_prueba=
 		 		 new G4Box("Detector_prueba",                       						//its name
 		 		 		DetectorX_SizeHalf, DetectorY_SizeHalf, DetectorZ_SizeHalf);    //its size

 		 		G4LogicalVolume* Logic_Detector =
 		 		 new G4LogicalVolume(Detector_prueba,          							//its solid
 		 		                     Lead,           									//its material
 		 		                     "logic_detector");    								//its name
 		 		//G4VPhysicalVolume* physical_Colimator4 =
 		 		 new G4PVPlacement(0,                 								    //no rotation
 		 		                   G4ThreeVector(0.0*m,0.0*m,0.7*m),       								//at (0,0,0)
 		 		                   Logic_Detector,			          					//its logical volume
 		 		                   "physical_detector",               					//its name
 		 							  logic_WorldCube,         								//its mother  volume
 		 		                   false,                 								//no boolean operation
 		 		                   0,                     								//copy number
 		 		                   checkOverlaps);     //overlaps checking*/

 		 		SensitiveDetector = Logic_Detector;
   return physical_WorldCube;
}

//Superficies Sensibles

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void TDM_DetectorConstruction::ConstructSDandField() {

  if (!SensitiveDetector) return;

  TDMSD* test_SD = new TDMSD("/TDM/testSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(test_SD);
  SetSensitiveDetector(SensitiveDetector, test_SD);
}

