void SaveWaveforms()
{
    string filename = "Diamondoid_muon_n700VC_300VG1_700VG2_CH1_061217_001.root";
    string fOut = filename;
    fOut.replace(fOut.find(".root"),5,"_SelectedWaveforms.pdf");
    
    //TFile *f = new TFile("061716_2600VG2_2600VG1_n2000V_10mm_2um_TEA_Eu154_ch1_001.root","READ");
    //TFile *f = new TFile("/Volumes/Time\ Machine\ Backups/Stanford/result_LXe_Bi207_Shaper1_G50_CAEN_650VG1_1350VG2.root","READ");
    //TFile *f = new TFile("Xe_Bi207/032117/calib/LXe_Bi207_calib_1V_49dB_Shaper2_G50_CAEN_1000VNG_032117_001_fit.root","READ");
    TFile *f = new TFile(filename.c_str());
    
    TTree *t = (TTree*)f->Get("dataTree");
    
    int event;
    double peak;
    double fit;
    double peak_position;
    double baseline;
    double rms;
    int isNoise;
    
    t->SetBranchAddress("event",&event);
    t->SetBranchAddress("peak",&peak);
    t->SetBranchAddress("fit",&fit);
    t->SetBranchAddress("peak_position",&peak_position);
    t->SetBranchAddress("baseline",&baseline);
    t->SetBranchAddress("rms",&rms);
    t->SetBranchAddress("isNoise",&isNoise);
    
    bool first = true;
    
    TCanvas *c1 = new TCanvas();
    
    c1->Print((fOut+"(").c_str(),"pdf");
    
    double rms_cut_up = 300.0;
    double rms_cut_low = 40.0;
    double fit_cut = 50.0;
    double ROI_low = 1120.0;
    double ROI_up = 1180.0;
    
    for (int i = 0; i < t->GetEntries(); i++) {
        t->GetEntry(i);
        
        if (c1) {delete c1;}
        
        c1 = (TCanvas*)f->Get(Form("h_evt%i_ch0",event));
        
        if (fit < fit_cut && peak_position > ROI_low && peak_position < ROI_up && rms > rms_cut_low && rms < rms_cut_up && isNoise == 0 && (peak - baseline) > 3*rms) {
            c1->Print(fOut.c_str(),"pdf");
        }
        
        /*if (isNoise == 0) {
            c1->Print("output.pdf","pdf");
        }*/
    }
    
    c1 = new TCanvas();
    
    c1->Print((fOut+")").c_str(),"pdf");
}
