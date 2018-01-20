void AnalyzeEu154SingleChannel()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    string filename1 = "042116_400V_1mm_Eu154_ch2_5m_001.root";
    string filename2 = "042116_1000V_1mm_Eu154_ch2_5m_001.root";
    string filename3 = "042116_1000V_n1000V_10mm_Eu154_ch2_5m_001.root";
    string filename4 = "042116_2000V_n2000V_10mm_Eu154_ch2_5m_001.root";
    
    string filename1_bg = "042116_1000V_1mm_Eu154_ch2_5m_bg_001.root";
    string filename2_bg = "042116_2000V_n2000V_10mm_Eu154_ch2_5m_bg_001.root";
    
    TFile *f = new TFile(filename1.c_str(),"READ");
    TTree *t = (TTree*)f->Get("dataTree");
    
    TFile *f2 = new TFile(filename2.c_str(),"READ");
    TTree *t2 = (TTree*)f2->Get("dataTree");
    
    TFile *f3 = new TFile(filename3.c_str(),"READ");
    TTree *t3 = (TTree*)f3->Get("dataTree");
    
    TFile *f4 = new TFile(filename4.c_str(),"READ");
    TTree *t4 = (TTree*)f4->Get("dataTree");
    
    TFile *f1_bg = new TFile(filename1_bg.c_str(),"READ");
    TTree *t1_bg = (TTree*)f1_bg->Get("dataTree");
    
    TFile *f2_bg = new TFile(filename2_bg.c_str(),"READ");
    TTree *t2_bg = (TTree*)f2_bg->Get("dataTree");
    
    TH2I *h_waveforms = (TH2I*)f->Get("h_waveforms");
    TH2I *h_waveforms_2 = (TH2I*)f2->Get("h_waveforms");
    TH2I *h_waveforms_3 = (TH2I*)f3->Get("h_waveforms");
    TH2I *h_waveforms_4 = (TH2I*)f4->Get("h_waveforms");
    
    double ROI_low_init = 900;
    double ROI_up_init = 1300;
    
    TH1F *h = new TH1F("h","h",500,0,8000);
    TH1F *h2 = new TH1F("h2","h2",500,0,8000);
    TH1F *h3 = new TH1F("h3","h3",500,0,8000);
    TH1F *h4 = new TH1F("h4","h4",500,0,8000);
    TH1F *h1_bg = new TH1F("h1_bg","h1_bg",500,0,8000);
    TH1F *h2_bg = new TH1F("h2_bg","h2_bg",500,0,8000);
    
    TH1F *h_basline = new TH1F("h_baseline","h_baseline",100,7000,9000);
    
    TH1F *h_rms = new TH1F("h_rms","h_rms",100,100,600);
    
    TH2F *h_rms_peakPosition = new TH2F("h_rms_peakPosition","h_rms_peakPosition",100,ROI_low_init,ROI_up_init,100,0,2000);
    
    t->Draw("baseline>>h_baseline","","goff");
    
    t->Draw("rms>>h_rms","","goff");
    
    double baseline = h_baseline->GetMean();
    
    double rms = h_rms->GetMean();
    
    // **** New preamp, channel 1, shaper1
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.392;
    
    // **** New preamp, channel 2, shaper1
    //double calib_p0_ch2 = 0;
    //double calib_p1_ch2 = 2.235;
    
    // **** New preamp, channel 2, shaper 2 (2µs)
    double calib_p0_ch1 = 0;
    double calib_p1_ch1 = 2.316;
    
    double yMax = 2000;
    
    double threshold = 8650;
    
    double rms_cut = 300.0;
    double fit_cut = 50.0;
    double chi2_cut = 50.0;
    double ROI_low = 1000.0;
    double ROI_up = 1150.0;
    
    cout << "baseline = " << baseline << "  RMS = " << rms << " (" << rms*calib_p1_ch1 << " electrons)" << endl;
    
    t->Draw(Form("(peak-baseline)*%f+%f>>h",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    t->Draw("rms:peak_position>>h_rms_peakPosition","","goff");
    
    t2->Draw(Form("(peak-baseline)*%f+%f>>h2",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    
    t3->Draw(Form("(peak-baseline)*%f+%f>>h3",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    
    t4->Draw(Form("(peak-baseline)*%f+%f>>h4",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    
    t1_bg->Draw(Form("(peak-baseline)*%f+%f>>h1_bg",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    
    t2_bg->Draw(Form("(peak-baseline)*%f+%f>>h2_bg",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    
    h->SetLineColor(kBlack);
    h2->SetLineColor(kBlack);
    h3->SetLineColor(kRed);
    h4->SetLineColor(kRed);
    h1_bg->SetLineColor(kGray);
    h2_bg->SetLineColor(kBlue);
    
    h->SetLineWidth(1);
    h2->SetLineWidth(1);
    h3->SetLineWidth(1);
    h4->SetLineWidth(1);
    h1_bg->SetLineWidth(1);
    h2_bg->SetLineWidth(1);
    
    h2->SetLineStyle(2);
    h4->SetLineStyle(2);
    
    h1_bg->SetFillColor(kGray);
    h2_bg->SetFillColor(kBlue);
    h1_bg->SetFillStyle(3001);
    h2_bg->SetFillStyle(3001);
    
    h->GetXaxis()->SetTitle("number of electrons");
    h->GetYaxis()->SetTitle("counts");
    
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    
    h->GetYaxis()->SetRangeUser(0,yMax);
    
    TH1F *h_difference = h->Clone();
    TH1F *h_difference_2 = h2->Clone();
    
    h_difference->Add(h1_bg,-1);
    h_difference_2->Add(h1_bg,-1);
    
    TH1F *h_difference_3 = h3->Clone();
    TH1F *h_difference_4 = h4->Clone();
    
    h_difference_3->Add(h2_bg,-1);
    h_difference_4->Add(h2_bg,-1);
    
    h_rms_peakPosition->GetXaxis()->SetTitle("peak position");
    h_rms_peakPosition->GetYaxis()->SetTitle("RMS");
    
    h_rms_peakPosition->GetXaxis()->CenterTitle();
    h_rms_peakPosition->GetYaxis()->CenterTitle();
    
    TPolyLine *rms_peakPosition_cut = new TPolyLine(5);
    
    rms_peakPosition_cut->SetPoint(0,ROI_low,0);
    rms_peakPosition_cut->SetPoint(1,ROI_up,0);
    rms_peakPosition_cut->SetPoint(2,ROI_up,rms_cut);
    rms_peakPosition_cut->SetPoint(3,ROI_low,rms_cut);
    rms_peakPosition_cut->SetPoint(4,ROI_low,0);
    
    rms_peakPosition_cut->SetLineColor(kRed);
    rms_peakPosition_cut->SetLineWidth(2);
    rms_peakPosition_cut->SetLineStyle(2);
    
    TGraph *gr = new TGraph(5);
    
    gr->SetPoint(0,0,0);
    gr->SetPoint(1,3*rms*calib_p1_ch1,0);
    gr->SetPoint(2,3*rms*calib_p1_ch1,yMax);
    gr->SetPoint(3,0,yMax);
    gr->SetPoint(4,0,0);
    
    gr->SetFillColor(kGreen);
    gr->SetFillStyle(3003);
    
    TLine *l_rms = new TLine(rms*calib_p1_ch1,0,rms*calib_p1_ch1,yMax);
    
    l_rms->SetLineColor(kRed);
    l_rms->SetLineWidth(1);
    l_rms->SetLineStyle(2);
    
    TLine *l2 = new TLine((threshold-baseline)*calib_p1_ch1,0,(threshold-baseline)*calib_p1_ch1,yMax);
    
    l2->SetLineColor(kBlack);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    
    TLegend *l = new TLegend(0.8,0.8,0.9,0.9);
    
    l->AddEntry(h,"1mm, 4kV/cm","l");
    l->AddEntry(h2,"1mm, 10kV/cm","l");
    l->AddEntry(h3,"10mm, 2kV/cm","l");
    l->AddEntry(h4,"10mm, 4kV/cm","l");
    l->AddEntry(h1_bg,"background (1mm)","fl");
    l->AddEntry(h2_bg,"background (10mm)","fl");
    l->AddEntry(l2,"threshold","l");
    l->AddEntry(l_rms,"RMS","l");
    l->AddEntry(gr,"3 #sigma noise","f");
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    
    c1->Divide(1,2);
    
    c1->cd(1);
    
    h->Draw();
    h2->Draw("same");
    h3->Draw("same");
    h4->Draw("same");
    h1_bg->Draw("same");
    h2_bg->Draw("same");
    gr->Draw("Fsame");
    l_rms->Draw("same");
    l->Draw("same");
    l2->Draw("same");
    
    c1->cd(2);
    
    h_difference_2->Draw("");
    h_difference->Draw("same");
    h_difference_3->Draw("same");
    h_difference_4->Draw("same");
    
    TCanvas *c2 = new TCanvas();
    h_rms_peakPosition->Draw("cont1 z");
    rms_peakPosition_cut->Draw("same");
    
    TCanvas *c3 = new TCanvas();
    
    c3->Divide(1,2);
    
    c3->cd(1);
    
    h_waveforms->Draw("colz");
    
    c3->cd(2);
    
    h_waveforms_2->Draw("colz");
}