#include "TMath.h"
void hw() {
    TFile* f = new TFile("Geant4.root");
    TTree* t = (TTree*)f->Get("tree");

    const Int_t N = t->GetEntries();
    const Int_t d = 3;
    
    Int_t evtID;
    Double_t pmd[2][d], edep1, edep2, center1[d], center2[d];
    t->SetBranchAddress("Event_ID",&evtID);
    t->SetBranchAddress("Momentum_direction",pmd[0]);
    t->SetBranchAddress("Edep_head",&edep1);
    t->SetBranchAddress("Center_edep_head",center1);
    t->SetBranchAddress("Edep_body",&edep2);
    t->SetBranchAddress("Center_edep_body",center2);
    
    TH1D* hist_pmd[2][d];
    hist_pmd[0][0] = new TH1D("hist_pmd_x","",500,-1,1);
    hist_pmd[0][1] = new TH1D("hist_pmd_y","",500,-1,1);
    hist_pmd[0][2] = new TH1D("hist_pmd_z","",500,-1,1);
    hist_pmd[1][0] = new TH1D("hist_pmd_r","",500,0.562,0.762);
    hist_pmd[1][1] = new TH1D("hist_pmd_cos_theta","",500,-1,1);
    hist_pmd[1][2] = new TH1D("hist_pmd_phi","",500,0,2*TMath::Pi());
    TH1D* hist_edep_head = new TH1D("hist_edep_head","",50,0,0.662);
    TH1D* hist_center_head[d];
    hist_center_head[0] = new TH1D("hist_center_head_x","",500,-90,90);
    hist_center_head[1] = new TH1D("hist_center_head_y","",500,340,520);
    hist_center_head[2] = new TH1D("hist_center_head_z","",500,-90,90);
    TH1D* hist_edep_body = new TH1D("hist_edep_body","",50,0,0.662);
    TH1D* hist_center_body[d];
    hist_center_body[0] = new TH1D("hist_center_body_x","",500,-60,60);
    hist_center_body[1] = new TH1D("hist_center_body_y","",500,-250,250);
    hist_center_body[2] = new TH1D("hist_center_body_z","",500,-130,130);
    
    for (Int_t i=0;i<N;i++) {
        t->GetEntry(i);

        pmd[1][0] = TMath::Power(TMath::Power(pmd[0][0],2)+TMath::Power(pmd[0][1],2)+TMath::Power(pmd[0][2],2),0.5);
        pmd[1][1] = pmd[0][2]/pmd[1][0];
        pmd[1][2] = TMath::ATan(pmd[0][1]/pmd[0][0]);
        if (pmd[0][0] < 0) {
            pmd[1][2] += TMath::Pi();
        }
        else if (pmd[0][0] > 0 && pmd[0][1] < 0) {
            pmd[1][2] += 2*TMath::Pi();
        }
        
        if ( edep1 != 0) {
            hist_edep_head->Fill(edep1);
        }
        if (edep2 != 0) {
            hist_edep_body->Fill(edep2);
        }

        for (Int_t j=0;j<d;j++) {
                hist_pmd[0][j]->Fill(pmd[0][j]);
                hist_pmd[1][j]->Fill(pmd[1][j]);
                hist_center_head[j]->Fill(center1[j]);
                hist_center_body[j]->Fill(center2[j]);
        }
    }

}

