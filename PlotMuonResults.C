void PlotMuonResults()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TGraphErrors *gr_run1_ng = new TGraphErrors();
    
    TGraphErrors *gr_run2_ng = new TGraphErrors();
    TGraphErrors *gr_run2_g = new TGraphErrors();
    
    TGraphErrors *gr_run3_ng = new TGraphErrors();
    TGraphErrors *gr_run3_g = new TGraphErrors();
    
    gr_run1_ng->SetPoint(0,3.5,3912.7);
    gr_run1_ng->SetPoint(1,4.5,4074.3);
    
    gr_run1_ng->SetPointError(0,0,136.1);
    gr_run1_ng->SetPointError(1,0,101.4);
    
    gr_run2_ng->SetPoint(0,6,5509.2);
    gr_run2_ng->SetPoint(1,4.5,4658.8);
    gr_run2_ng->SetPoint(2,6.6,5726.0);
    
    gr_run2_ng->SetPointError(0,0,208.7);
    gr_run2_ng->SetPointError(1,0,249.6);
    gr_run2_ng->SetPointError(2,0,103.5);
    
    gr_run2_g->SetPoint(0,4.5,4359.7);
    gr_run2_g->SetPoint(1,5.7,4474.4);
    gr_run2_g->SetPoint(2,6.1,4572.0);
    gr_run2_g->SetPoint(3,6.6,4834.4);
    
    gr_run2_g->SetPointError(0,0,137.8);
    gr_run2_g->SetPointError(1,0,276.9);
    gr_run2_g->SetPointError(2,0,204.1);
    gr_run2_g->SetPointError(3,0,99.1);
    
    gr_run3_ng->SetPoint(0,4,2982.9);
    
    gr_run3_ng->SetPointError(0,0,140.2);
    
    gr_run3_g->SetPoint(0,5.3,2997.7);
    gr_run3_g->SetPoint(1,4,2607.0);
    
    gr_run3_g->SetPointError(0,0,673.8);
    gr_run3_g->SetPointError(1,0,163.3);
    
    gr_run1_ng->SetMarkerStyle(20);
    
    gr_run1_ng->SetMarkerSize(0.5);
    
    gr_run1_ng->SetMarkerColor(kBlack);
    
    gr_run1_ng->SetLineColor(kBlack);
    
    gr_run2_ng->SetMarkerStyle(20);
    gr_run2_g->SetMarkerStyle(24);
    
    gr_run2_ng->SetMarkerSize(0.5);
    gr_run2_g->SetMarkerSize(.5);
    
    gr_run2_ng->SetMarkerColor(kBlue);
    gr_run2_g->SetMarkerColor(kBlue);
    
    gr_run2_ng->SetLineColor(kBlue);
    gr_run2_g->SetLineColor(kBlue);
    
    gr_run3_ng->SetMarkerStyle(20);
    gr_run3_g->SetMarkerStyle(24);
    
    gr_run3_ng->SetMarkerSize(0.5);
    gr_run3_g->SetMarkerSize(.5);
    
    gr_run3_ng->SetMarkerColor(kGreen+1);
    gr_run3_g->SetMarkerColor(kGreen+1);
    
    gr_run3_ng->SetLineColor(kGreen+1);
    gr_run3_g->SetLineColor(kGreen+1);
    
    TH2F *h_empty = new TH2F("h_empty","h_empty",100,2,8,100,1000,8000);
    
    h_empty->GetXaxis()->SetTitle("Bulk field (kV/cm)");
    h_empty->GetYaxis()->SetTitle("muon peak (e^{-})");
    
    h_empty->GetXaxis()->CenterTitle();
    h_empty->GetYaxis()->CenterTitle();
    
    h_empty->GetYaxis()->SetTitleOffset(1.4);
    
    TGraph *gr1 = new TGraph();
    TGraph *gr2 = new TGraph();
    TGraph *gr3 = new TGraph();
    TGraph *gr4 = new TGraph();
    TGraph *gr5 = new TGraph();
    
    gr1->SetMarkerColor(kBlack);
    gr2->SetMarkerColor(kBlack);
    
    gr1->SetMarkerStyle(20);
    gr2->SetMarkerStyle(24);
    
    gr1->SetMarkerSize(0.5);
    gr2->SetMarkerSize(0.5);
    
    gr3->SetLineColor(kBlack);
    gr4->SetLineColor(kBlue);
    gr5->SetLineColor(kGreen+1);
    
    TLegend *l = new TLegend(0.2,0.65,0.4,0.85);
    
    l->AddEntry(gr1,"Unity gain cell","p");
    l->AddEntry(gr2,"Gain cell","p");
    l->AddEntry(gr3,"run 1","l");
    l->AddEntry(gr4,"run 2","l");
    l->AddEntry(gr5,"run 3","l");
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    h_empty->Draw();
    gr_run1_ng->Draw("Psame");
    gr_run2_ng->Draw("Psame");
    gr_run2_g->Draw("Psame");
    gr_run3_ng->Draw("Psame");
    gr_run3_g->Draw("Psame");
    l->Draw("same");
}