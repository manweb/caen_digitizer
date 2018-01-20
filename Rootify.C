/////////////////////////////////////////////////////////////////
//
// Script converting raw waveform files in .txt or .xml format
// to .root files. Information from a peak finding algorithm is
// stored in a TTree. Optionally the waveforms of each event is
// stored in the same file including the result of the fit to
// the template waveform
//
// Usage: root -l -b Rootify.C
//
// Manuel Weber - April 2016
//
////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <iostream>
#include "TXMLEngine.h"

void ReadDigitizerAttributes(TXMLEngine *xml, XMLNodePointer_t node);
void ReadSettingsAttributes(TXMLEngine *xml, XMLNodePointer_t node);
void ReadDigitizerParameters(TXMLEngine *xml, XMLNodePointer_t node, bool first = false);
void ReadSettingsParameters(TXMLEngine *xml, XMLNodePointer_t node, bool first = false);
void ReadEvents(TXMLEngine *xml, XMLNodePointer_t node);
void ReadWaveforms(TXMLEngine *xml, XMLNodePointer_t trace, int id);
double GetBaseline(TH1I *h);
double GetRMS(TH1I *h);
int FindPeaks(TH1I *h, double baseline, double rms, double sigma, int channel);
void FitWaveform(TH1I *h, double baseline, int channel);
TF1 *GetFitFunction(double center, double amplitude);
double GetChi2(double min, double max);
void ReadTXT(string filename);
void PrepareTree();
void WriteSpectra();
void NewEvent(int event, int channel);
void WriteHisto(int channel);

int digitizer_resolution;
double digitizer_frequency;
int digitizer_max_samples;
double digitizer_voltrange_low;
double digitizer_voltrange_hi;

int settings_dcoffset_1;
int settings_dcoffset_2;
int settings_dcoffset_3;
int settings_dcoffset_4;
int settings_trigger_1;
int settings_trigger_2;
int settings_trigger_3;
int settings_trigger_4;
int settings_window;
double settings_posttrigger;

int event_timestamp;
int event_number;
int channel_number;
double signal_position;
double signal_position2;
double signal_amplitude;
double signal_chi2;
double signal_baseline;
double signal_rms;
double signal_peak;
double signal_fit;

double fit_p0;
double fit_p1;
double fit_p2;
double fit_p3;

int isNoise;

int numberOfPeaks;

int nSamples;
int preTriggerSamples;

TFile *outFile;
TCanvas *c1;
TH1I *h1;
TLine *l1;
TGraph *polyLine1;
TPolyMarker *p1;
TH1F *h_spectrum1;
TH1F *h_spectrum2;
TH1F *h_spectrum3;
TH1F *h_spectrum4;
TH1F *h_chi2;
TH2I *h_waveforms;
TTree *tree;

TF1 *fit;

std::vector<double> peaks_x;
std::vector<double> peaks_y;

string inputFile;
string outputFile;

// *****************************************************************
//
// Some settings
//
// *****************************************************************
int max_number_events = 10000000;    // Maximum number of events to process
double ROI_low = 1050;              // Lower limit of ROI
double ROI_high = 1200;             // Upper limit of ROI
bool saveWaveforms = true;          // Save all waveforms to the file?
bool performFit = false;             // Perform the fit?
//double shapingTime = 0.6555;        // Shaing time
double shapingTime = 2.1;
double sampleScaling = 4;           // The number of samples is divided by this factor
int verboseLevel = 1;               // 0 = don't print anything, 1 = minimal print, 2 = print everything
bool invertWaveform = false;         // Set to true if waveform should be inverted

// ROIs for the different datasets
//
// Muons:                                   ROI_low = 1000, ROI_high = 1300
// Eu-154:                                  ROI_low = 950, ROI_high = 1150
// Calibration shaper 1:                    ROI_low = 1000, ROI_high = 1150
// Calibration shaper 2:                    ROI_low = 1100, ROI_high = 1200
// Muons (2µs shaping time):                ROI_low = 4000, ROI_high = 5000
// Calibration shaper 2 (2µs shaping time): ROI_low = 1050, ROI_high = 1200 (sampleScaling = 4)
// EU-154 (2µs shaping time):               ROI_low = 950, ROI_high = 1150 (sampleScaling = 4)

// *****************************************************************
//
// Function returning the template waveform for the Ortec 572A
// shaper
//
// *****************************************************************
Double_t fitFunction(Double_t *x, Double_t *par) {
    Double_t value;
    
    if (x[0] < par[1]) {value = par[2];}
    else {value = par[2]+par[3]*(0.323276*TMath::Exp(-3*par[0]*(x[0]-par[1])/100*sampleScaling)/(par[0]*par[0])-0.78125*TMath::Exp(-par[0]*(x[0]-par[1])/100*sampleScaling)/(par[0]*par[0])+2*(0.228987*TMath::Cos(0.8*par[0]*(x[0]-par[1])/100*sampleScaling)+0.404095*TMath::Sin(0.8*par[0]*(x[0]-par[1])/100*sampleScaling))*TMath::Exp(-par[0]*(x[0]-par[1])/100*sampleScaling)/(par[0]*par[0]));}
    
    return value;
}

// *****************************************************************
//
// Main function
//
// *****************************************************************
void Rootify(string fname = "")
{
    if (fname == "") {
        cout << "Enter input file:" << endl;
        getline(cin, inputFile);
    }
    else {inputFile = fname;}
    
    outputFile = inputFile;
    
    if (outputFile.find(".txt") != string::npos) {outputFile.replace(outputFile.find("txt"),3,"root");}
    else if (outputFile.find(".xml") != string::npos) {outputFile.replace(outputFile.find("xml"),3,"root");}
    else {cout << "Wrong input format. Only .txt or .xml are allowed." << endl; exit();}
    
    // Prepare the output tree
    PrepareTree();
    
    // Read the .txt file...
    if (inputFile.find(".txt") != string::npos) {ReadTXT(inputFile); return;}
    
    // ...else read the .xml file
    TXMLEngine *xml = new TXMLEngine();
    
    XMLDocPointer_t xml_doc = xml->ParseFile(inputFile.c_str());
    
    XMLNodePointer_t xml_root = xml->DocGetRootElement(xml_doc);
    
    XMLNodePointer_t xml_digitizer = xml->GetChild(xml_root);
    XMLNodePointer_t xml_settings = xml->GetNext(xml_digitizer);
    XMLNodePointer_t xml_event = xml->GetNext(xml_settings);
    
    ReadDigitizerAttributes(xml, xml_digitizer);
    ReadSettingsAttributes(xml, xml_settings);
    ReadDigitizerParameters(xml, xml_digitizer, true);
    ReadSettingsParameters(xml, xml_settings, true);
    ReadEvents(xml, xml_event);
}

// ***************************************************************
//
// Function that prepares the outpur tree
//
// **************************************************************
void PrepareTree()
{
    if (tree) {delete tree;}
    
    tree = new TTree("dataTree","dataTree");
    
    tree->Branch("resol",&digitizer_resolution,"resol/I");
    tree->Branch("freq",&digitizer_frequency,"freq/D");
    tree->Branch("smax",&digitizer_max_samples,"smax/I");
    tree->Branch("vlow",&digitizer_voltrange_low,"vlow/D");
    tree->Branch("vhi",&digitizer_voltrange_hi,"vhi/D");
    tree->Branch("dcoff_1",&settings_dcoffset_1,"dcoff_1/I");
    tree->Branch("dcoff_2",&settings_dcoffset_2,"dcoff_2/I");
    tree->Branch("dcoff_3",&settings_dcoffset_3,"dcoff_3/I");
    tree->Branch("dcoff_4",&settings_dcoffset_4,"dcoff_4/I");
    tree->Branch("trig_1",&settings_trigger_1,"trig_1/I");
    tree->Branch("trig_2",&settings_trigger_2,"trig_2/I");
    tree->Branch("trig_3",&settings_trigger_3,"trig_3/I");
    tree->Branch("trig_4",&settings_trigger_4,"trig_4/I");
    tree->Branch("window",&settings_window,"window/I");
    tree->Branch("posttrig",&settings_posttrigger,"posttrig/D");
    tree->Branch("timestamp",&event_timestamp,"timestamp/I");
    tree->Branch("event",&event_number,"event/I");
    tree->Branch("channel",&channel_number,"channel/I");
    tree->Branch("signal",&signal_position,"signal/D");
    tree->Branch("peak_position",&signal_position2,"peak_position/D");
    tree->Branch("amplitude",&signal_amplitude,"amplitude/D");
    tree->Branch("chi2",&signal_chi2,"chi2/D");
    tree->Branch("baseline",&signal_baseline,"baseline/D");
    tree->Branch("rms",&signal_rms,"rms/D");
    tree->Branch("peak",&signal_peak,"peak/D");
    tree->Branch("fit",&signal_fit,"fit/D");
    tree->Branch("fit_p0",&fit_p0,"fit_p0/D");
    tree->Branch("fit_p1",&fit_p1,"fit_p1/D");
    tree->Branch("fit_p2",&fit_p2,"fit_p2/D");
    tree->Branch("fit_p3",&fit_p3,"fit_p3/D");
    tree->Branch("isNoise",&isNoise,"isNoise/I");
    tree->Branch("numberOfPeaks",&numberOfPeaks,"numberOfPeaks/I");
}

// *************************************************************
//
// Function that prepares the output histograms. For each
// channel a spectrum is produced. Additionally there is a
// histogram for the chi2 of the fits.
//
// *************************************************************
void PrepareSpectra() {
    if (h_spectrum1) {delete h_spectrum1;}
    if (h_spectrum2) {delete h_spectrum2;}
    if (h_spectrum3) {delete h_spectrum3;}
    if (h_spectrum4) {delete h_spectrum4;}
    if (h_chi2) {delete h_chi2;}
    
    h_spectrum1 = new TH1F("spectrum_ch0","spectrum_ch0",16384,1,16384);
    h_spectrum2 = new TH1F("spectrum_ch1","spectrum_ch1",16384,1,16384);
    h_spectrum3 = new TH1F("spectrum_ch2","spectrum_ch2",16384,1,16384);
    h_spectrum4 = new TH1F("spectrum_ch3","spectrum_ch3",16384,1,16384);
    h_chi2 = new TH1F("chi2","chi2",200,0,200);
}

// ************************************************************
//
// Function that is called at the end of the processing. It
// saves the histograms and output tree to the file.
//
// ************************************************************
void WriteSpectra() {
    if (h_spectrum1->GetEntries() > 0) {h_spectrum1->Write();}
    if (h_spectrum2->GetEntries() > 0) {h_spectrum2->Write();}
    if (h_spectrum3->GetEntries() > 0) {h_spectrum3->Write();}
    if (h_spectrum4->GetEntries() > 0) {h_spectrum4->Write();}
    
    h_chi2->Write();
    
    h_waveforms->Write();
    
    tree->Write();
}

// ***********************************************************
//
// Function that is called at the beginning of each event.
// It resets the canvas and waveform histogram.
//
// ***********************************************************
void NewEvent(int event, int channel) {
    if (c1) {delete c1;}
    if (h1) {delete h1;}
    if (l1) {delete l1;}
    if (polyLine1) {delete polyLine1;}
    
    isNoise = 0;
    
    c1 = new TCanvas(Form("h_evt%i_ch%i",event,channel), Form("h_evt%i_ch%i",event,channel));
    
    h1 = new TH1I(Form("h_evt%i_ch%i",event,channel), Form("h_evt%i_ch%i",event,channel), nSamples, 1, nSamples);
}

// **********************************************************
//
// Function that is called after the event has been read and
// the waveform histogram has been filled. It calls the
// functions that perform the peak search and fitting to the
// template waveform.
//
// *********************************************************
void WriteHisto(int channel) {
    if (invertWaveform) {h1->Scale(-1);}
    
    // Get the baseline and RMS
    double baseline = GetBaseline(h1);
    double rms = GetRMS(h1);
    
    if (verboseLevel > 1) {printf("            Baseline: %.1f RMS: %.2f\n", baseline, rms);}
    
    // Find peaks that are sigma*RMS above the baseline
    int nPeaks = FindPeaks(h1, baseline, rms, 1.0, channel));
    
    numberOfPeaks = peaks_x.size();
    
    if (verboseLevel > 1) {printf("            Number of peaks: %i\n",peaks_x.size());}
    
    signal_baseline = baseline;
    signal_rms = rms;
    
    // Fit the waveform to the template
    FitWaveform(h1, baseline, channel);
    
    l1 = new TLine(1,baseline,nSamples,baseline);
    
    l1->SetLineColor(kBlack);
    l1->SetLineWidth(2);
    
    double x[4] = {1,nSamples,nSamples,1};
    double y[4] = {baseline-rms,baseline-rms,baseline+rms,baseline+rms};
    
    polyLine1 = new TGraph(4,x,y);
    
    polyLine1->SetLineWidth(0);
    polyLine1->SetFillColor(kGreen);
    polyLine1->SetFillStyle(3002);
    
    c1->Clear();
    
    c1->Divide(1,2);
    
    c1->cd(1);
    
    h1->Draw();
    if (fit) {fit->Draw("same");}
    polyLine1->Draw("fsame");
    l1->Draw("same");
    p1->Draw("same");
    
    c1->cd(2);
    
    if (fit) {fit->Draw();}
    
    //if (signal_chi2 < 5) {c1->Write();}
    
    // Write the canvas to the file
    if (saveWaveforms) {c1->Write();}
    
    // Add the waveform to the sum waveforms histogram
    if (!h_waveforms) {h_waveforms = new TH2I("h_waveforms","h_waveworms",nSamples/4,1,nSamples,2048,1,16384);}
    
    for (int i = 1; i < nSamples+1; i++) {
        int binValue = h1->GetBinContent(i);
        if (invertWaveform) {
            binValue = h1->GetBinContent(i) - 2*baseline;
        }
        
        int binX = h_waveforms->GetXaxis()->FindBin(i);
        int binY = h_waveforms->GetYaxis()->FindBin(binValue);
        
        h_waveforms->SetBinContent(binX,binY,h_waveforms->GetBinContent(binX,binY)+1);
    }
}

void ReadDigitizerAttributes(TXMLEngine *xml, XMLNodePointer_t node) {
    if (verboseLevel > 1) {printf("Reading node attributes for: %s\n",xml->GetNodeName(node));}
    
    XMLAttrPointer_t attr = xml->GetFirstAttr(node);
    while (attr != 0) {
        if (verboseLevel > 1) {printf("    attribute: %s value: %s\n", xml->GetAttrName(attr), xml->GetAttrValue(attr));}
        attr = xml->GetNextAttr(attr);
    }
}

void ReadSettingsAttributes(TXMLEngine *xml, XMLNodePointer_t node) {
    if (verboseLevel > 1) {printf("Reading node attributes for: %s\n",xml->GetNodeName(node));}
    
    XMLAttrPointer_t attr = xml->GetFirstAttr(node);
    while (attr != 0) {
        if (verboseLevel > 1) {printf("    attribute: %s value: %s\n", xml->GetAttrName(attr), xml->GetAttrValue(attr));}
        attr = xml->GetNextAttr(attr);
    }
}

void ReadDigitizerParameters(TXMLEngine *xml, XMLNodePointer_t node, bool first) {
    if (first) {if (verboseLevel > 1) {printf("Reading parameters for: %s\n",xml->GetNodeName(node));}}
    
    XMLNodePointer_t child = xml->GetChild(node);
    while (child != 0) {
        XMLAttrPointer_t attr = xml->GetFirstAttr(child);
        const char *space = first ? "    " : "        ";
        if (verboseLevel > 1) {printf("%sparameter: %s value: %s\n", space, xml->GetNodeName(child), xml->GetAttrValue(attr));}
        ReadDigitizerParameters(xml, child);
        child = xml->GetNext(child);
    }
}

void ReadSettingsParameters(TXMLEngine *xml, XMLNodePointer_t node, bool first) {
    if (first) {if (verboseLevel > 1) {printf("Reading parameters for: %s\n",xml->GetNodeName(node));}}
    
    XMLNodePointer_t child = xml->GetChild(node);
    while (child != 0) {
        XMLAttrPointer_t attr = xml->GetFirstAttr(child);
        const char *space = first ? "    " : "        ";
        if (strcmp(xml->GetNodeName(child),"dcoffset") == 0) {
            if (verboseLevel > 1) {printf("%sparameter: %s channel: %s ", space, xml->GetNodeName(child), xml->GetAttrValue(attr));}
            
            attr = xml->GetNextAttr(attr);
            if (verboseLevel > 1) {printf("value: %s\n", xml->GetAttrValue(attr));}
        }
        else if (strcmp(xml->GetNodeName(child),"level") == 0) {
            if (verboseLevel > 1) {printf("%sparameter: %s channel: %s ", space, xml->GetNodeName(child), xml->GetAttrValue(attr));}
            
            attr = xml->GetNextAttr(attr);
            if (verboseLevel > 1) {printf("value: %s\n", xml->GetAttrValue(attr));}
        }
        else {
            if (verboseLevel > 1) {printf("%sparameter: %s value: %s\n", space, xml->GetNodeName(child), xml->GetAttrValue(attr));}
            
            if (strcmp(xml->GetNodeName(child),"window") == 0) {nSamples = atoi(xml->GetAttrValue(attr));}
            if (strcmp(xml->GetNodeName(child),"posttrigger") == 0) {
                const char* postTriggerString = strtok(xml->GetAttrValue(attr),"%");
                
                preTriggerSamples = nSamples*(1-atof(postTriggerString)/100)*0.8;
            }
        }
        
        ReadSettingsParameters(xml, child);
        child = xml->GetNext(child);
    }
}

void ReadEvents(TXMLEngine *xml, XMLNodePointer_t event) {
    if (verboseLevel > 1) {printf("Reading events\n");}
    
    outFile = new TFile(outputFile.c_str(),"RECREATE");
    
    PrepareSpectra();
    
    int id = 0;
    while (event != 0) {
        if (verboseLevel == 1 && id % 1000 == 0) {printf("%i events processed",id);}
        if (verboseLevel > 1) {printf("    event %i ", id);}
        XMLAttrPointer_t attr = xml->GetFirstAttr(event);
        for (int i = 0; i < 3; i++) {
            attr = xml->GetNextAttr(attr);
        }
        
        if (verboseLevel > 1) {printf(" time: %s\n", xml->GetAttrValue(attr));}
        
        XMLNodePointer_t trace = xml->GetChild(event);
        trace = xml->GetNext(trace);
        
        event_timestamp = atoi(xml->GetAttrValue(attr));
        
        ReadWaveforms(xml, trace, id);
        
        event = xml->GetNext(event);
        
        id++;
    }
    
    WriteSpectra();
    
    outFile->Close();
}

void ReadWaveforms(TXMLEngine *xml, XMLNodePointer_t trace, int id) {
    while (trace != 0) {
        XMLAttrPointer_t attr = xml->GetFirstAttr(trace);
        
        if (verboseLevel > 1) {printf("        Reading channel %s\n", xml->GetAttrValue(attr));}
        
        NewEvent(id, atoi(xml->GetAttrValue(attr)));
        
        const char* content = xml->GetNodeContent(trace);
        
        const char* values = strtok(content, " ");
        int sample_count = 1;
        while (values != NULL) {
            //printf("            %s\n", values);
            
            h1->SetBinContent(sample_count, atoi(values));
            
            values = strtok(NULL, " ");
            
            sample_count++;
        }
        
        event_number = id;
        channel_number = atoi(xml->GetAttrValue(attr));
        
        WriteHisto(atoi(xml->GetAttrValue(attr)));
        
        trace = xml->GetNext(trace);
    }
}

// **********************************************************
//
// Get the baseline by averaging the pre-trigger samples.
//
// **********************************************************
double GetBaseline(TH1I *h) {
    double mean = 0;
    
    for (int i = 1; i < preTriggerSamples; i++) {
        mean += h->GetBinContent(i);
    }
    
    mean /= preTriggerSamples;
    
    return mean;
}

// ********************************************************
//
// Get the RMS from the pre-trigger samples.
//
// ********************************************************
double GetRMS(TH1I *h) {
    double rms = 0;
    
    double baseline = GetBaseline(h);
    
    for (int i = 1; i < preTriggerSamples; i++) {
        rms += (h->GetBinContent(i) - baseline)*(h->GetBinContent(i) - baseline);
    }
    
    rms /= preTriggerSamples;
    
    rms = TMath::Sqrt(rms);
    
    return rms;
}

// ********************************************************
//
// Function that finds peaks above sigma*RMS in the window
// defined by ROI_low and ROI_high.
//
// ********************************************************
int FindPeaks(TH1I *h, double baseline, double rms, double sigma, int channel) {
    TSpectrum *spectrum = new TSpectrum();
    
    spectrum->Search(h, 20, "", 0.1);
    
    int nPeaks = spectrum->GetNPeaks();
    
    TList *functions = h->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
    
    Double_t *peaks_xTMP = pm->GetX();
    Double_t *peaks_yTMP = pm->GetY();
    
    if (p1) {delete p1;}
    
    p1 = new TPolyMarker();
    
    p1->SetMarkerStyle(21);
    p1->SetMarkerColor(kGreen);
    
    peaks_x.clear();
    peaks_y.clear();
    
    int k = 0;
    for (int i = 0; i < nPeaks; i++) {
        double peak_x = peaks_xTMP[i];
        double peak_y = peaks_yTMP[i];
        
        if (peak_y > baseline+sigma*rms && peak_x > ROI_low && peak_x < ROI_high) {
            int peakCount = 0;
            for (int j = 0; j < nPeaks; j++) {
                if (peak_y-baseline < 1.5*(peaks_yTMP[j]-baseline) && (peaks_xTMP[j] < ROI_low || peaks_xTMP[j] > ROI_high)) {peakCount++;}
            }
            
            if (peakCount > 2) {isNoise = 1;}
            
            double h_min = h->GetBinContent(h->GetMinimumBin());
            
            if (TMath::Abs(h_min-baseline) > TMath::Abs(peak_y-baseline) && h->GetMinimumBin() < peak_x) {isNoise = 1;}
            
            p1->SetPoint(k, peak_x, peak_y);
            
            /*if (channel == 0) {h_spectrum1->Fill(peak_y);}
            if (channel == 1) {h_spectrum2->Fill(peak_y);}
            if (channel == 2) {h_spectrum3->Fill(peak_y);}
            if (channel == 3) {h_spectrum4->Fill(peak_y);}*/
            
            peaks_x.push_back(peak_x);
            peaks_y.push_back(peak_y);
            
            k++;
        }
    }
    
    return k;
}

// **********************************************************
//
// Function that performs a fit to the template waveform.
// It first sets the position and amplitude of the template
// function to the found peak, using the shaping time
// determined by calibration runs. It calculates the sum of
// residuals as a measure of the shape agreement. It then
// performs a fit to the waveform leaving the shaping time
// as free parameters. The chi2 of the fit is also stored in
// the output tree.
//
// *********************************************************
void FitWaveform(TH1I *h, double baseline, int channel) {
    for (int i = 0; i < peaks_x.size(); i++) {
        if (verboseLevel > 1) {printf("            Fitting peak at %.1f\n", peaks_x.at(i));}
        
        if (fit) {delete fit;}
        
        fit = new TF1("fit",fitFunction,0,1000,4);
        
        fit->SetParameter(0,1/shapingTime);
        fit->SetParameter(1,0);
        fit->FixParameter(2,0);
        fit->SetParameter(3,1);
        
        double offset = fit->GetMaximumX(0,1000);
        double maximum = fit->GetMaximum(0,1000);
        
        double fitRange = 500;
        
        double shapingTimeBox1 = 0.6672;
        
        fit->SetRange(peaks_x.at(i)-offset-100,peaks_x.at(i)-offset+fitRange);
        
        fit->SetParNames("#tau","x_{0}","baseline","amplitude");
        
        fit->SetParameter(0,1/shapingTime);
        fit->SetParameter(1,peaks_x.at(i)-offset);
        fit->FixParameter(2,baseline);
        fit->SetParameter(3,(peaks_y.at(i)-baseline)/maximum);
        
        fit->SetParLimits(0,1/(shapingTime*1.5),1/(shapingTime/1.5));
        fit->SetParLimits(1,peaks_x.at(i)-offset-fitRange,peaks_x.at(i)-offset+fitRange);
        
        signal_fit = GetChi2(peaks_x.at(i)-offset,peaks_x.at(i)-offset+fitRange);
        
        if (performFit) {h->Fit(fit,"rqn","goff");}
        
        double max = fit->GetMaximum(peaks_x.at(i)-offset,peaks_x.at(i)+fitRange-offset);
        double chi2;
        if (performFit) {chi2 = fit->GetChisquare()/fit->GetNDF();}
        else {chi2 = 0;}
        
        if (verboseLevel > 1) {printf("            Result: peak = %.1f amplitude = %.1f chi2/ndf = %.2f\n", fit->GetParameter(1),max,chi2);}
        
        if (chi2 < 30) {
            if (channel == 0) {h_spectrum1->Fill(max);}
            if (channel == 1) {h_spectrum2->Fill(max);}
            if (channel == 2) {h_spectrum3->Fill(max);}
            if (channel == 3) {h_spectrum4->Fill(max);}
        }
        
        h_chi2->Fill(chi2);
        
        fit->SetLineColor(kRed);
        fit->SetLineWidth(2);
        
        signal_position = fit->GetParameter(1);
        signal_position2 = peaks_x.at(i);
        signal_amplitude = max;
        signal_chi2 = chi2;
        signal_peak = peaks_y.at(i);
        
        fit_p0 = fit->GetParameter(0);
        fit_p1 = fit->GetParameter(1);
        fit_p2 = fit->GetParameter(2);
        fit_p3 = fit->GetParameter(3);
        
        tree->Fill();
    }
}

// ******************************************************
//
// Function that returns the sum of residuals between
// the waveform and the template waveform.
//
// ******************************************************
double GetChi2(double min, double max) {
    int binMin = h1->GetXaxis()->FindBin(min);
    int binMax = h1->GetXaxis()->FindBin(max);
    
    int nBins = binMax - binMin + 1;
    
    double chisquare = 0;
    for (int i = binMin; i < binMax+1; i++) {
        chisquare += TMath::Abs((h1->GetBinContent(i)-fit->Eval(h1->GetBinCenter(i)))/h1->GetBinContent(i));
    }
    
    return chisquare;
}

// ************************************************************
//
// Function that reads the .txt file.
//
// ************************************************************
void ReadTXT(string filename) {
    std::ifstream infile(filename.c_str());
    
    if (verboseLevel > 1) {printf("Reading file %s\n", filename.c_str());}
    
    outFile = new TFile(outputFile.c_str(),"RECREATE");
    
    PrepareSpectra();
    
    int channel;
    int eventNumber;
    int timestamp;
    
    int line_count = 0;
    int event_count = 0;
    int bin = 0;
    int binScaled = 0;
    bool readData = false;
    bool newEvent = false;
    string line;
    while (infile >> line) {
        if (line.find("Record") != string::npos) {
            if (readData) {
                event_timestamp = timestamp;
                event_number = eventNumber;
                channel_number = channel;
                
                WriteHisto(channel);
                
                readData = false;
                
                event_count++;
            }
            
            newEvent = true;
            
            line_count = 0;
            
            if (event_count > max_number_events) {break;}
        }
        
        switch (line_count) {
            case 0:
            {
                if (line.find("Record") != string::npos) {
                    infile >> line;
                    infile >> line;
                    
                    nSamples = atoi(line.c_str());
                    
                    nSamples /= sampleScaling;
                    
                    preTriggerSamples = nSamples*0.5;
                    
                    if (verboseLevel > 1) {printf("    Number of samples: %i\n", nSamples);}
                }
            }
                break;
            case 1:
            {
                if (line.find("BoardID") != string::npos) {
                    infile >> line;
                }
            }
                break;
            case 2:
            {
                if (line.find("Channel") != string::npos) {
                    infile >> line;
                    
                    channel = atoi(line.c_str());
                    
                    if (verboseLevel > 1) {printf("    Channel: %i\n", channel);}
                }
            }
                break;
            case 3:
            {
                if (line.find("Event") != string::npos) {
                    infile >> line;
                    infile >> line;
                    
                    eventNumber = atoi(line.c_str());
                    
                    if (verboseLevel == 1 && eventNumber % 1000 == 0) {printf("%i events processed\n",eventNumber);}
                    if (verboseLevel > 1) {printf("    Event: %i\n", eventNumber);}
                }
            }
                break;
            case 4:
            {
                if (line.find("Pattern") != string::npos) {
                    infile >> line;
                }
            }
                break;
            case 5:
            {
                if (line.find("Trigger") != string::npos) {
                    infile >> line;
                    infile >> line;
                    infile >> line;
                    
                    timestamp = atoi(line.substr(0,5).c_str());
                    
                    if (verboseLevel > 1) {printf("    Timestamp: %i\n", timestamp);}
                }
            }
                break;
            case 6:
            {
                if (line.find("DC") != string::npos) {
                    infile >> line;
                    infile >> line;
                    infile >> line;
                    
                    line.replace(0,17,"");
                }
            }
                break;
        }
        
        if (line_count < 7) {line_count++; continue;}
        
        if (newEvent) {NewEvent(eventNumber, channel); newEvent = false; bin = 1;}
        
        if (bin == 1) {h1->SetBinContent(bin, atoi(line.c_str()));}
        else if (binScaled == sampleScaling - 1) {h1->SetBinContent(bin/sampleScaling+1, atoi(line.c_str())); binScaled = -1;}
        
        bin++;
        binScaled++;
        
        readData = true;
    }
    
    WriteSpectra();
    
    outFile->Close();
}
