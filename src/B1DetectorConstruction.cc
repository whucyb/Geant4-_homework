//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume_Head(0),
  fScoringVolume_Body(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeX = 1000.*mm, env_sizeY = 3000.*mm, env_sizeZ = 3000.*mm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeX = 1.2*env_sizeX;
  G4double world_sizeY = 1.2*env_sizeY;
  G4double world_sizeZ = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeX, 0.5*env_sizeY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //
  //human
  //
  G4Material* human_mat = nist->FindOrBuildMaterial("G4_WATER");
  //
  //Body
  //
  G4ThreeVector posBody = G4ThreeVector(0.*mm, 0.*mm, 0.*mm); 
  G4Box* solidBody = 
    new G4Box("Body",
              60.*mm, 250.*mm, 130.*mm);
  G4LogicalVolume* logicBody = 
    new G4LogicalVolume(solidBody,
                        human_mat,
                        "Body");
  G4VisAttributes* colBody = new G4VisAttributes(G4Colour(1.,0.,0.));
  logicBody->SetVisAttributes(colBody);
  fScoringVolume_Body = new G4PVPlacement(0,
                    posBody,
                    logicBody,
                    "Body",   
                    logicEnv, 
                    false,    
                    0,        
                    checkOverlaps);
  //
  //Lleg
  //
  G4ThreeVector posLleg = G4ThreeVector(0.*mm, -660.*mm, -75.*mm);
  G4RotationMatrix* rotLleg = new G4RotationMatrix();
  rotLleg->rotateX(90.*deg);
  G4Tubs* solidLleg =
    new G4Tubs("Lleg",
               0.*mm, 55.*mm, 410.*mm,
               0.*deg, 360.*deg);
  G4LogicalVolume* logicLleg =
    new G4LogicalVolume(solidLleg,
                        human_mat,
                        "Lleg");
  G4VisAttributes* colLleg = new G4VisAttributes(G4Colour(1.,1.,0.));
  logicLleg->SetVisAttributes(colLleg);
  new G4PVPlacement(rotLleg,
                    posLleg,
                    logicLleg,
                    "Lleg",
                    logicEnv,
                    false,
                    0,
                    checkOverlaps);
  //
  //Rleg
  //
  G4ThreeVector posRleg = G4ThreeVector(0.*mm, -660.*mm, 75.*mm);
  G4RotationMatrix* rotRleg = new G4RotationMatrix();
  rotRleg->rotateX(90.*deg);
  G4Tubs* solidRleg =
    new G4Tubs("Rleg",
               0.*mm, 55.*mm, 410.*mm,
               0.*deg, 360.*deg);
  G4LogicalVolume* logicRleg =
    new G4LogicalVolume(solidRleg,
                        human_mat,
                        "Rleg");
  G4VisAttributes* colRleg = new G4VisAttributes(G4Colour(1.,1.,0.));
  logicRleg->SetVisAttributes(colRleg);
  new G4PVPlacement(rotRleg,
                    posRleg,
                    logicRleg,
                    "Rleg",
                    logicEnv,
                    false,
                    0,
                    checkOverlaps);
  //
  //Neck
  //
  G4ThreeVector posNeck = G4ThreeVector(0.*mm, 295.*mm, 0.*mm);
  G4RotationMatrix* rotNeck = new G4RotationMatrix();
  rotNeck->rotateX(90.*deg);
  G4Tubs* solidNeck =
    new G4Tubs("Neck",
               0.*mm, 50.*mm, 45.*mm,
               0.*deg, 360.*deg);
  G4LogicalVolume* logicNeck =
    new G4LogicalVolume(solidNeck,
                        human_mat,
                        "Neck");
  G4VisAttributes* colNeck = new G4VisAttributes(G4Colour(1.,1.,0.));
  logicNeck->SetVisAttributes(colNeck);
  new G4PVPlacement(rotNeck,
                    posNeck,
                    logicNeck,
                    "Neck",
                    logicEnv,
                    false,
                    0,
                    checkOverlaps);
  //
  //Head
  //
  G4ThreeVector posHead = G4ThreeVector(0.*mm, 430.*mm, 0.*mm);
  G4Orb* solidHead =
    new G4Orb("Head",
              90.*mm);
  G4LogicalVolume* logicHead =
    new G4LogicalVolume(solidHead,
                        human_mat,
                        "Head");
  G4VisAttributes* colHead = new G4VisAttributes(G4Colour(1.,0.,0.));
  logicHead->SetVisAttributes(colHead);
  fScoringVolume_Head = new G4PVPlacement(0,
                    posHead,
                    logicHead,
                    "Head",
                    logicEnv,
                    false,
                    0,
                    checkOverlaps);


  // Set Head and Body as scoring volume
  //
  //fScoringVolume_Body = logicBody;
  //fScoringVolume_Head = logicHead;


  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
