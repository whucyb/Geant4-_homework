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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
B1SteppingAction::B1SteppingAction(
                        const B1DetectorConstruction* detectorConstruction,
                        B1EventAction* eventAction)
: G4UserSteppingAction(),
  fDetConstruction(detectorConstruction),
  fEventAction(eventAction)
  //fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  if (volume == fDetConstruction->GetScoringVolume_Head()) {
      G4double edep_temp = step->GetTotalEnergyDeposit();
      G4ThreeVector pre_point = step->GetPreStepPoint()->GetPosition();
      G4ThreeVector post_point = step->GetPostStepPoint()->GetPosition();
      fEventAction->edep1_temp += edep_temp;
      fEventAction->center1_temp[0] += (pre_point[0]+post_point[0])/2.*edep_temp;
      fEventAction->center1_temp[1] += (pre_point[1]+post_point[1])/2.*edep_temp;
      fEventAction->center1_temp[2] += (pre_point[2]+post_point[2])/2.*edep_temp;
  }
  
  if (volume == fDetConstruction->GetScoringVolume_Body()) {
      G4double edep_temp = step->GetTotalEnergyDeposit();
      G4ThreeVector pre_point = step->GetPreStepPoint()->GetPosition();
      G4ThreeVector post_point = step->GetPostStepPoint()->GetPosition();
      fEventAction->edep2_temp += edep_temp;
      fEventAction->center2_temp[0] += (pre_point[0]+post_point[0])/2.*edep_temp;
      fEventAction->center2_temp[1] += (pre_point[1]+post_point[1])/2.*edep_temp;
      fEventAction->center2_temp[2] += (pre_point[2]+post_point[2])/2.*edep_temp;
  }

 
/*******************************
  if (!fScoringVolume) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume_Head();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  
***********************/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

