#include <iostream>
#include "TMath.h"
void hw() {
    TFile* f = new TFile("Geant4.root");
    TTree* t = (TTree*)f->Get("tree");

    const Int_t N = t->GetEntries();
    const Int_t d = 3;
    
    Int_t evtID;
    Double_t p[2][d], edep1, edep2, center1[d], center2[d];
    t->SetBranchAddress("Event_ID",&evtID);
    t->SetBranchAddress("Gamma_momentum",p[0]);
    t->SetBranchAddress("Edep_head",&edep1);
    t->SetBranchAddress("Center_edep_head",center1);
    t->SetBranchAddress("Edep_body",&edep2);
    t->SetBranchAddress("Center_edep_body",center2);
    
    TH1D* hist_p[2][d];
    hist_p[0][0] = new TH1D("hist_p_x","",500,-0.665,0.665);
    hist_p[0][1] = new TH1D("hist_p_y","",500,-0.665,0.665);
    hist_p[0][2] = new TH1D("hist_p_z","",500,-0.665,0.665);
    hist_p[1][0] = new TH1D("hist_p_r","",500,0.660,0.664);
    hist_p[1][1] = new TH1D("hist_p_cos_theta","",500,-1.1,1.1);
    hist_p[1][2] = new TH1D("hist_p_phi","",500,-0.2,6.4);
    TH1D* hist_edep_head = new TH1D("hist_edep_head","",50,-0.02,0.68);
    TH1D* hist_center_head[d];
    hist_center_head[0] = new TH1D("hist_center_head_x","",200,-90,90);
    hist_center_head[1] = new TH1D("hist_center_head_y","",200,340,520);
    hist_center_head[2] = new TH1D("hist_center_head_z","",200,-90,90);
    TH1D* hist_edep_body = new TH1D("hist_edep_body","",50,-0.02,0.68);
    TH1D* hist_center_body[d];
    hist_center_body[0] = new TH1D("hist_center_body_x","",150,-60,60);
    hist_center_body[1] = new TH1D("hist_center_body_y","",550,-250,250);
    hist_center_body[2] = new TH1D("hist_center_body_z","",300,-130,130);
    
    for (Int_t i=0;i<N;i++) {
        t->GetEntry(i);

        p[1][0] = TMath::Power(TMath::Power(p[0][0],2)+TMath::Power(p[0][1],2)+TMath::Power(p[0][2],2),0.5);
        p[1][1] = p[0][2]/p[1][0];
        p[1][2] = TMath::ATan(p[0][1]/p[0][0]);
        if (p[0][0] < 0) {
            p[1][2] += TMath::Pi();
        }
        else if (p[0][0] > 0 && p[0][1] < 0) {
            p[1][2] += 2*TMath::Pi();
        }
        
        if ( edep1 != 0) {
            hist_edep_head->Fill(edep1);
        }
        if (edep2 != 0) {
            hist_edep_body->Fill(edep2);
        }

        //if (edep1 != 0 && edep2 != 0) {
        //    cout<<i<<endl;
        //}

        for (Int_t j=0;j<d;j++) {
                hist_p[0][j]->Fill(p[0][j]);
                hist_p[1][j]->Fill(p[1][j]);
                if (edep1 != 0) {
                    hist_center_head[j]->Fill(center1[j]);
                }
                if (edep2 != 0) {
                    hist_center_body[j]->Fill(center2[j]);
                }
        }
    }

    TCanvas *c1 = new TCanvas("c1","c1",1400,1000);
    c1 -> Divide(3,1);
    hist_p[0][0]->GetXaxis()->SetTitle("P_{#gamma x} (MeV/C)");
    hist_p[0][0]->GetXaxis()->CenterTitle(true);
    hist_p[0][0]->GetXaxis()->SetTitleOffset(1);
    hist_p[0][0]->GetYaxis()->SetTitle("Entries");
    hist_p[0][0]->GetYaxis()->CenterTitle(true);
    hist_p[0][0]->GetYaxis()->SetTitleOffset(1.5);  
    c1 -> cd(1);
    hist_p[0][0]->Draw("E");
    hist_p[0][1]->GetXaxis()->SetTitle("P_{#gamma y} (MeV/C)");
    hist_p[0][1]->GetXaxis()->CenterTitle(true);
    hist_p[0][1]->GetXaxis()->SetTitleOffset(1);
    hist_p[0][1]->GetYaxis()->SetTitle("Entries");
    hist_p[0][1]->GetYaxis()->CenterTitle(true);
    hist_p[0][1]->GetYaxis()->SetTitleOffset(1.5);
    c1 -> cd(2);
    hist_p[0][1]->Draw("E");
    hist_p[0][2]->GetXaxis()->SetTitle("P_{#gamma z} (MeV/C)");
    hist_p[0][2]->GetXaxis()->CenterTitle(true);
    hist_p[0][2]->GetXaxis()->SetTitleOffset(1);
    hist_p[0][2]->GetYaxis()->SetTitle("Entries");
    hist_p[0][2]->GetYaxis()->CenterTitle(true);
    hist_p[0][2]->GetYaxis()->SetTitleOffset(1.5);
    c1 -> cd(3);
    hist_p[0][2]->Draw("E");
    c1 -> Print("Gamma_momentum.png");

    TCanvas *c2 = new TCanvas("c2","c2",1400,1000);
    c2 -> Divide(2,1);
    hist_p[1][1]->GetXaxis()->SetTitle("cos#theta_{P_{#gamma}}");
    hist_p[1][1]->GetXaxis()->CenterTitle(true);
    hist_p[1][1]->GetXaxis()->SetTitleOffset(1.2);
    hist_p[1][1]->GetYaxis()->SetTitle("Entries");
    hist_p[1][1]->GetYaxis()->CenterTitle(true);
    hist_p[1][1]->GetYaxis()->SetTitleOffset(1.5);
    c2 -> cd(1);
    hist_p[1][1]->Draw("E");
    hist_p[1][2]->GetXaxis()->SetTitle("#phi_{P_{#gamma}}");
    hist_p[1][2]->GetXaxis()->CenterTitle(true);
    hist_p[1][2]->GetXaxis()->SetTitleOffset(1.2);
    hist_p[1][2]->GetYaxis()->SetTitle("Entries");
    hist_p[1][2]->GetYaxis()->CenterTitle(true);
    hist_p[1][2]->GetYaxis()->SetTitleOffset(1.5);
    c2 -> cd(2);
    hist_p[1][2]->Draw("E");
    c2 -> Print("Gamma_momentum_direction.png");

    TCanvas *c3 = new TCanvas("c3","c3",1400,1000);
    hist_edep_head->GetXaxis()->SetTitle("Energy deposit in head (MeV)");
    hist_edep_head->GetXaxis()->CenterTitle(true);
    hist_edep_head->GetYaxis()->SetTitle("Entries");
    hist_edep_head->GetYaxis()->CenterTitle(true);
    hist_edep_head->Draw("E");
    c3 -> Print("Energy_deposit_in_head.png");

    TCanvas *c4 = new TCanvas("c4","c4",1400,1000);
    c4 -> Divide(3,1);
    hist_center_head[0]->GetXaxis()->SetTitle("Center_{Energy deposit in head, x} (mm)");
    hist_center_head[0]->GetXaxis()->CenterTitle(true);
    hist_center_head[0]->GetYaxis()->SetTitle("Entries");
    hist_center_head[0]->GetYaxis()->CenterTitle(true);
    c4 -> cd(1);
    hist_center_head[0]->Draw("E");
    hist_center_head[1]->GetXaxis()->SetTitle("Center_{Energy deposit in head, y} (mm)");
    hist_center_head[1]->GetXaxis()->CenterTitle(true);
    hist_center_head[1]->GetYaxis()->SetTitle("Entries");
    hist_center_head[1]->GetYaxis()->CenterTitle(true);
    c4 -> cd(2);
    hist_center_head[1]->Draw("E");
    hist_center_head[2]->GetXaxis()->SetTitle("Center_{Energy deposit in head, z} (mm)");
    hist_center_head[2]->GetXaxis()->CenterTitle(true);
    hist_center_head[2]->GetYaxis()->SetTitle("Entries");
    hist_center_head[2]->GetYaxis()->CenterTitle(true);
    c4 -> cd(3);
    hist_center_head[2]->Draw("E");
    c4 -> Print("Center_of_energy_deposit_in_head.png");

    TCanvas *c5 = new TCanvas("c5","c5",1400,1000);
    hist_edep_body->GetXaxis()->SetTitle("Energy deposit in body (MeV)");
    hist_edep_body->GetXaxis()->CenterTitle(true);
    hist_edep_body->GetYaxis()->SetTitle("Entries");
    hist_edep_body->GetYaxis()->CenterTitle(true);
    hist_edep_body->Draw("E");
    c5 -> Print("Energy_deposit_in_body.png");

    TCanvas *c6 = new TCanvas("c6","c6",1400,1000);
    c6 -> Divide(3,1);
    hist_center_body[0]->GetXaxis()->SetTitle("Center_{Energy deposit in body, x} (mm)");
    hist_center_body[0]->GetXaxis()->CenterTitle(true);
    hist_center_body[0]->GetYaxis()->SetTitle("Entries");
    hist_center_body[0]->GetYaxis()->CenterTitle(true);
    c6 -> cd(1);
    hist_center_body[0]->Draw("E");
    hist_center_body[1]->GetXaxis()->SetTitle("Center_{Energy deposit in body, y} (mm)");
    hist_center_body[1]->GetXaxis()->CenterTitle(true);
    hist_center_body[1]->GetYaxis()->SetTitle("Entries");
    hist_center_body[1]->GetYaxis()->CenterTitle(true);
    c6 -> cd(2);
    hist_center_body[1]->Draw("E");
    hist_center_body[2]->GetXaxis()->SetTitle("Center_{Energy deposit in body, z} (mm)");
    hist_center_body[2]->GetXaxis()->CenterTitle(true);
    hist_center_body[2]->GetYaxis()->SetTitle("Entries");
    hist_center_body[2]->GetYaxis()->CenterTitle(true);
    c6 -> cd(3);
    hist_center_body[2]->Draw("E");
    c6 -> Print("Center_of_energy_deposit_in_body.png");
}

