#include <math.h>
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
//#include "TDMSD.hh"
#include "G4SDManager.hh"

#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSTrackLength.hh"

#include "G4PhysicalConstants.hh"


TDM_DetectorConstruction::TDM_DetectorConstruction()
: G4VUserDetectorConstruction()
{
	// World cube (world)
	WorldCube_SizeHalfX	= (4.745/2.0)*m + (0.2/2.0)*m ;
	WorldCube_SizeHalfY	= (3.480/2.0)*m + (0.2/2.0)*m;
	WorldCube_SizeHalfZ	= (2.460/2.0)*m + (0.2/2.0)*m;

	// Water cube (world)
	WaterCube_SizeHalf	= 0.50*m;

	// Water cylinder

	 innerRadius = 0.*m;
	 outerRadius = 0.25*m;
     hz = 0.9*m;
	 startAngle = 0.*deg;
	 spanningAngle = 360.*deg;

	 //camilla

	 camilla_X = (2.2/2.0)*m;
	 camilla_Y = (0.43/2.0)*m;
	 camilla_Z = (0.02/2.0)*m;

	 ColimadorX_SizeHalf = 0.2*m;
	 ColimadorY_SizeHalf = 0.2*m;
	 ColimadorZ_SizeHalf = 0.035*m;

	 Campo_Max = 0.2*m;
	 FieldX_SizeHalf = 0.1*m;
	 FieldY_SizeHalf = 0.1*m;

	 DF = 0.519*m;			//distancia fuente-parte final camilla
	 Distancia = 0.203*m;    //distancia de los colimadores a la fuente
	 H1CX = Distancia + (2.0*ColimadorZ_SizeHalf);			//distancia fuente-parte final bloque
	 D1CX = (H1CX/DF)*FieldX_SizeHalf; //distancia eje x - bloque


	 H1CY = Distancia + (4.0*ColimadorZ_SizeHalf);							//distancia fuente-parte final bloque
	 D1CY = (H1CY/DF)*FieldY_SizeHalf;	//distancia eje y - bloque

	 Xfinal = (2*ColimadorZ_SizeHalf + Distancia)*(FieldX_SizeHalf/DF); //Distancia desde el eje
	 Yfinal = (4*ColimadorZ_SizeHalf + Distancia)*(FieldY_SizeHalf/DF); //Distancia desde el eje

	 //////// Blindaje de la fuente1
	 Xconstante = (H1CX/DF)*0.2*m;
	 Largo = ((ColimadorX_SizeHalf/2)+ Xconstante);
	 Ancho = ((ColimadorY_SizeHalf/2)+ Xconstante);
	 grosor = ColimadorZ_SizeHalf;


	 /***********Blindaje de forma cilindrica *******/

	 Grosor_cilindro = 0.5*cm;
	 //Diagonal_campo = ((2^(3/2))*(Campo_Max)*Distancia)/DF; //Diagonal del campo en el cilindro
	 Diagonal_campo = 8.2*cm;
	 Radio_interno = Diagonal_campo/2;
	 Radio_externo = Radio_interno + Grosor_cilindro;
	 Altura_cilindro = (0.425/2)*m;
	 Angulo_Inicial = 0. *deg;
	 Angulo_Final = 360. *deg;
	 Corrimiento_colimadores = ((FieldX_SizeHalf)*Distancia)/DF;

	 /********** Tapadera *********************/
	 Radio_interno_tapadera = 0. *m;
	 Radio_externo_tapadera = Radio_externo;
	 Altura_tapadera = Grosor_cilindro/2;
	 Angulo_Inicial_tapadera = 0. *deg;
	 Angulo_Final_tapadera  = 360.*m;

	 /**************Detector de prueba *****/

	 DetectorX_SizeHalf=1*m;
	 DetectorY_SizeHalf=1*m;
	 DetectorZ_SizeHalf=0.0005*m;
	 SensitiveDetector = 0;


	 Radio_interno_detector = 0.0*m;
	 Radio_externo_detector = (38.2/2)*cm;
	 Altura_detector = (2.5/2)*cm;
	 Angulo_Inicial_detector = 0*deg;
	 Angulo_Final_detector = 360*deg;



	 /************ Primitive Score *******/

	 TLD_HalfSizeY = 0.5*cm;
	 TLD_HalfSizeX = 1.25*cm;
	 TLD_HalfSizeZ = 0.5*cm;

	 /************ TLDNumber **************/

	 TLDNumber = 4;

	 /********************* PMMA ********************************/
	  PMMAX_SizeHalf=0.203/2*m;
	  PMMAY_SizeHalf=0.203/2*m;
	  PMMAZ_SizeHalf=19.3/2*cm;

	  /**********************Paredes*****************************/

	  Pared_SizeHalfX = (4.745/2.0)*m;
	  Pared_SizeHalfY	= (3.480/2.0)*m ;
	  Pared_SizeHalfZ	= (2.460/2.0)*m ;
	  Pared_GrosorMedio = (0.1/2.0)*m;

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
  G4Element* Li6 = new G4Element("Lithium6", "Li6", z=3, a=6.015*g/mole);
  G4Element* Li7 = new G4Element("Lithium7", "Li7", z=3, a=7.016*g/mole);
  G4Element* F = new G4Element("Fluorine", "F", z=9, a=18.99*g/mole);
  G4Element* Ti = new G4Element("Titanium", "Ti",z=22,a=47.867*g/mole);
  G4Element* Mg = new G4Element("Magnesium", "Mg",z=12,a=24.305*g/mole);
  //G4Element* 6LiF = new G4Element("6LiF", "Li", z=3, a=6*g/mole);
  //G4Element* 7LiF = new G4Element("Lithium", "Li", z=3, a=6*g/mole);


  // Vacuum

    G4Material* vacuum = new G4Material("vacuum", z=1., a=1.01*g/mole, density=universe_mean_density, kStateGas, 0.1*kelvin, 1.e-19*pascal);

    //Fluoruros de Litio

    G4Material* LiF6 = new G4Material("LiF6", density= 2.55*g/cm3, nelements=2);
    LiF6->AddElement(F, 1);
    LiF6->AddElement(Li6, 1);

    G4Material* LiF7 = new G4Material("LiF7", density= 2.65*g/cm3, nelements=2);
    LiF7->AddElement(F, 1);
    LiF7->AddElement(Li7, 1);

    // TLD100

    G4Material* TLD100 = new G4Material("TLD100", density= 2.65*g/cm3, nelements=4);
    TLD100->AddMaterial(LiF6, 7.5995*perCent);
    TLD100->AddMaterial(LiF7, 92.3795*perCent);
    TLD100->AddElement(Mg, 0.02*perCent);
    TLD100->AddElement(Ti, 0.001*perCent);

  // Air

  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

  // Water material
  G4Material* water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  //PMMA material
  G4Material* pmma = new G4Material("pmma", density=1.18*g/cm3, nelements=3);
  pmma->AddElement(C, 5);
  pmma->AddElement(H, 8);
  pmma->AddElement(O, 2);


  // Carbon Fiber
  G4Material* CarbonFiber = new G4Material("CarbonFiber", density= 0.145*g/cm3, nelements=1);
  CarbonFiber->AddElement(C, 1);

  //Lead
  G4Material* Lead = new G4Material("Lead", density=11.35*g/cm3, nelements=1);
  Lead->AddElement(Pb,1);

  //Concreto

  G4NistManager* man = G4NistManager::Instance();
  G4Material* concreto = man->FindOrBuildMaterial("G4_CONCRETE");

  	  	  	  	  	  	  //=====================================

   // Volumes definitions

  // Option to switch on/off checking of volumes overlaps

  G4bool checkOverlaps = false;

  //
  // World: WorldCube acts as world
  //

  G4Box* solid_WorldCube=
    new G4Box("WorldCube_solid",                       						//its name
    		WorldCube_SizeHalfX, WorldCube_SizeHalfY, WorldCube_SizeHalfZ);    //its size

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

  	  	  	  // **************************camilla********************************************//

  G4Box* solid_camilla=
      new G4Box("camilla",                       						//its name
      		camilla_X, camilla_Y, camilla_Z);    //its size

  G4LogicalVolume* logic_camilla =
      new G4LogicalVolume(solid_camilla,          							//its solid
                          CarbonFiber,           										//its material
                          "camilla_logic");    								//its name

  //G4VPhysicalVolume* physical_camilla =
    new G4PVPlacement(0,                 								    //no rotation
                      G4ThreeVector(-0.6945*m,0.297*m,-0.209*m),       			//at (0,0,-0.3)
                      logic_camilla,			          					//its logical volume
                      "camilla_physical",               					//its name
                      logic_WorldCube,                     								//its mother  volume
                      false,                 								//no boolean operation
                      0,                     								//copy number
                      checkOverlaps);        								//overlaps checking


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
 				G4ThreeVector(-0.5495*m,0.265*m, 0.5085*m ),					//centrado en 0,0,0
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
 		 				G4ThreeVector(-0.5495*m,0.265*m, 0.5085*m+Altura_cilindro+Altura_tapadera),					//centrado en 0,0,0
 		 				logic_tapadera,
 		 				"Leadtapadera_physical",
 		 				logic_WorldCube,
 		 				false,
 		 				0,
 		 				checkOverlaps
 		 				);

 /******************* Detector prueba  ****************/

 		 	/*	G4Tubs* Detector_prueba
 		 		 		   = new G4Tubs("Detector",
 		 		 		 		  Radio_interno_detector ,
 		 		 		 		  Radio_externo_detector ,
 		 		 		 		  Altura_detector,
 		 		 		 		  Angulo_Inicial_detector ,
 		 		 		 		  Angulo_Final_detector
 		 		 		 		  );

 		 		 		 G4LogicalVolume* Logic_Detector =
 		 		 		 		new G4LogicalVolume(Detector_prueba,
 		 		 		 				vacuum,
 		 		 		 				"detector_prueba_logic");

 		 		 		 //G4VPhysicalVolume* physical_WaterCylinder =
 		 		 		 		new G4PVPlacement(0,
 		 		 		 				G4ThreeVector(-0.5495*m, 0.265*m, -0.4915*m),					//centrado en 0,0,0
 		 		 		 				Logic_Detector,
 		 		 		 				"Leadtapadera_physical",
 		 		 		 				logic_WorldCube,
 		 		 		 				false,
 		 		 		 				0,
 		 		 		 				checkOverlaps
 		 		 		 				);*/

 		 	/*	G4Box* Detector_prueba=
 		 		 new G4Box("Detector_prueba",                       						//its name
 		 		 		DetectorX_SizeHalf, DetectorY_SizeHalf, DetectorZ_SizeHalf);    //its size

 		 		G4LogicalVolume* Logic_Detector =
 		 		 new G4LogicalVolume(Detector_prueba,          							//its solid
 		 		                     vacuum,           									//its material
 		 		                     "logic_detector");    								//its name
 		 		//G4VPhysicalVolume* physical_Colimator4 =
 		 		 new G4PVPlacement(0,                 								    //no rotation
 		 		                   G4ThreeVector(-0.5495*m,0.265*m,-0.209*m + camilla_Z),       								//at (0,0,0)
 		 		                   Logic_Detector,			          					//its logical volume
 		 		                   "physical_detector",               					//its name
 		 							  logic_WorldCube,         								//its mother  volume
 		 		                   false,                 								//no boolean operation
 		 		                   0,                     								//copy number
 		 		                   checkOverlaps);     //overlaps checking*/

 		/* 	SensitiveDetector = Logic_Detector;*/




 /******************************** Medio Dispersor PMMA *********************************/

 		 	G4Box* PMMA=
 		 		      new G4Box("PMMA",                       						//its name
 		 		    		PMMAX_SizeHalf,PMMAY_SizeHalf, PMMAZ_SizeHalf);    //its size

 		 		  G4LogicalVolume* logic_PMMA =
 		 		      new G4LogicalVolume(PMMA,          							//its solid
 		 		                          pmma,	           										//its material
 		 		                          "pmma_logic");    								//its name

 		 		  //G4VPhysicalVolume* physical_camilla =
 		 		    new G4PVPlacement(0,                 								    //no rotation
 		 		                      G4ThreeVector(-0.55*m,0.265*m,-0.209*m +camilla_Z + PMMAZ_SizeHalf+2*TLD_HalfSizeZ),       			//at (0,0,-0.3)
 		 		                      logic_PMMA,			          					//its logical volume
 		 		                      "PMMA_physical",               					//its name
 		 		                      logic_WorldCube,                     								//its mother  volume
 		 		                      false,                 								//no boolean operation
 		 		                      0,                     								//copy number
 		 		                      checkOverlaps);        								//overlaps checking


 /*************************************Detectors Primitive Score ****************************/

 		 		/****************Detector 0 ************************************/
 		 		TDM_TLD_Cons(water, logic_WorldCube, "0Abso", G4ThreeVector(-0.55*m,0.265*m,-0.209*m + camilla_Z+TLD_HalfSizeZ),TLD_HalfSizeX,TLD_HalfSizeY,TLD_HalfSizeZ); //ABAJO DEL CENTRO DEL PMMA
 		 		//TDM_TLD_Cons(water, logic_WorldCube, "0Abso", G4ThreeVector(-0.55*m,0.265*m,-0.209*m + camilla_Z + 2*PMMAZ_SizeHalf + TLD_HalfSizeZ)); // ARRIBA DEL CENTRO DEL PMMA

 		 		/****************Detector 1 ************************************/
 		 		TDM_TLD_Cons(water, logic_WorldCube, "1Abso", G4ThreeVector(-0.55*m,0.265*m,-0.209*m + camilla_Z + 2*PMMAZ_SizeHalf + 3*TLD_HalfSizeZ),TLD_HalfSizeX,TLD_HalfSizeY,TLD_HalfSizeZ); // ARRIBA DEL CENTRO DEL PMMA
 		 		/****************Detector 2 ************************************/
 		 		TDM_TLD_Cons(water, logic_WorldCube, "2Abso", G4ThreeVector(-0.55*m,1.265*m + PMMAX_SizeHalf - TLD_HalfSizeY,-0.209*m + camilla_Z + PMMAZ_SizeHalf+2*TLD_HalfSizeZ),TLD_HalfSizeX,TLD_HalfSizeY,TLD_HalfSizeZ); // A 1 METRO DEL CENTRO DEL PMMA
 		 		/****************Detector 3 ************************************/
 		 		TDM_TLD_Cons(water, logic_WorldCube, "3Abso", G4ThreeVector(2.3725*m- TLD_HalfSizeX,0.265*m,-0.209*m + camilla_Z + PMMAZ_SizeHalf),TLD_HalfSizeZ,TLD_HalfSizeY,TLD_HalfSizeX); // PUERTA DE LA CLINICA

/******************************************Paredes********************************************/

 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-Puerta1",G4ThreeVector(WorldCube_SizeHalfX-Pared_GrosorMedio,0,0),Pared_GrosorMedio,WorldCube_SizeHalfY,WorldCube_SizeHalfZ);//Pared de la puerta
 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-Puerta2",G4ThreeVector(-WorldCube_SizeHalfX+Pared_GrosorMedio,0,0),Pared_GrosorMedio,WorldCube_SizeHalfY,WorldCube_SizeHalfZ);//Pared de la parte trasera
 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-izquierda",G4ThreeVector(0,WorldCube_SizeHalfY-Pared_GrosorMedio,0),WorldCube_SizeHalfX-2*(Pared_GrosorMedio),Pared_GrosorMedio,WorldCube_SizeHalfZ);//Pared del lado izquierdo
 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-derecho",G4ThreeVector(0,-WorldCube_SizeHalfY+Pared_GrosorMedio,0),WorldCube_SizeHalfX-2*(Pared_GrosorMedio),Pared_GrosorMedio,WorldCube_SizeHalfZ);//Pared del lado derecho
 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-Arriba",G4ThreeVector(0,0,WorldCube_SizeHalfZ-Pared_GrosorMedio),WorldCube_SizeHalfX-2*(Pared_GrosorMedio),WorldCube_SizeHalfY-2*(Pared_GrosorMedio),Pared_GrosorMedio);//Pared de arriba
 		 		Paredes_Cons(concreto,logic_WorldCube,"Pared-Abajo",G4ThreeVector(0,0,-WorldCube_SizeHalfZ+Pared_GrosorMedio),WorldCube_SizeHalfX-2*(Pared_GrosorMedio),WorldCube_SizeHalfY-2*(Pared_GrosorMedio),Pared_GrosorMedio);//Pared de abajo


   return physical_WorldCube;
}

inline void  TDM_DetectorConstruction::TDM_TLD_Cons( G4Material* Material, G4LogicalVolume* MotherVolume, G4String Name, G4ThreeVector Position,G4double X, G4double Y,G4double Z)
{
	/************************** Primitive Score ************************/

	 // Absorber
	  //
	  auto absorberS
		= new G4Box(Name,            // its name
					 X, Y, Z); // its size

	  auto absorberLV
		= new G4LogicalVolume(
					 absorberS,        // its solid
					 Material, // its material
					 Name.append("LV"));          // its name

	  G4cout << "TDM_TLD_Cons Name: " << Name << G4endl;

	   new G4PVPlacement(
					 0,                // no rotation
					 Position, //  its position
					 absorberLV,       // its logical volume
					 Name.append("PL"),           // its name
					 MotherVolume,          // its mother  volume
					 false,            // no boolean operation
					 0,                // copy number
					 false);  // checking overlaps

	 		 		  //
}

inline void TDM_DetectorConstruction::TDM_SetTLD_SD(  G4String Name )
{
	G4SDManager::GetSDMpointer()->SetVerboseLevel(0);
	    //
	    // Scorers

	    // declare Absorber as a MultiFunctionalDetector scorer

	    auto Detector = new G4MultiFunctionalDetector(Name);
	    G4SDManager::GetSDMpointer()->AddNewDetector(Detector);

	    G4VPrimitiveScorer* primitive;
	    G4String Edepname = Name+"Edep";
	    primitive = new G4PSEnergyDeposit(Edepname);
	    Detector->RegisterPrimitive(primitive);
	    G4String Dodepname = Name+"Dodep";
	    primitive = new G4PSDoseDeposit(Dodepname);
	    Detector->RegisterPrimitive(primitive);

	    SetSensitiveDetector(Name.append("LV"),Detector); // AGREGAR AL INCLUIR ALGUN TLD
}


//Superficies Sensibles

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TDM_DetectorConstruction::ConstructSDandField() {

/* if (!SensitiveDetector) return;

  TDMSD* test_SD = new TDMSD("/TDM/testSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(test_SD);
  SetSensitiveDetector(SensitiveDetector, test_SD);*/

  /*************** Primitive Score  *********************/

  G4SDManager::GetSDMpointer()->SetVerboseLevel(0);
    //
    // Scorers
    //

    // declare Absorber as a MultiFunctionalDetector scorer
    //
    auto absDetector = new G4MultiFunctionalDetector("Absorber");
    G4SDManager::GetSDMpointer()->AddNewDetector(absDetector);

    G4VPrimitiveScorer* primitive;
    primitive = new G4PSEnergyDeposit("Edep");
    absDetector->RegisterPrimitive(primitive);
    primitive = new G4PSDoseDeposit("Dodep");
    absDetector->RegisterPrimitive(primitive);

 for (G4int i = 0; i < TLDNumber; i++){

	 	 /* Change i into String */

	 	 G4String n;
	 	 std::stringstream convert;
	 	 convert << i;
	 	 n = convert.str();

	 	 G4String a = n+"Abso";

	 	 TDM_SetTLD_SD(a);
 	 	 	 }

}

inline void  TDM_DetectorConstruction::Paredes_Cons( G4Material* Material, G4LogicalVolume* MotherVolume, G4String Name, G4ThreeVector Position,G4double X, G4double Y,G4double Z)
{
	 // Paredes
	  //
	  auto Pared
		= new G4Box(Name,            // its name
					 X, Y, Z); // its size

	  auto paredLV
		= new G4LogicalVolume(
					 Pared,        // its solid
					 Material, // its material
					 Name.append("LV"));          // its name

	   new G4PVPlacement(
					 0,                // no rotation
					 Position, //  its position
					 paredLV,       // its logical volume
					 Name.append("PL"),           // its name
					 MotherVolume,          // its mother  volume
					 false,            // no boolean operation
					 0,                // copy number
					 false);  // checking overlaps

	 		 		  //
}

