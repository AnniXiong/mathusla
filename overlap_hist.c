#include <stdio.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH1.h>
#include <TAxis.h>

void overlap_hist () {

	// THStack *stack = new THStack("hs","");
    TCanvas *canvas = new TCanvas("canvas","canvas",10,10,600,400);
	canvas->SetLogy();
	TText T; 
	T.SetTextFont(42); T.SetTextAlign(21);
    T.DrawTextNDC(.5,.95, "Delta time");
    
   // TFile* f5 = new TFile ("tdc_hit88.root");
    TFile* f5 = new TFile ("tdc_hit25.root");
    /*
    TH1F* h1 = (TH1F*) gDirectory->Get("single");
    h1->Draw();
    TH1F* h2 = (TH1F*) gDirectory->Get("double");
    h2->Draw("same");
    TH1F* h3 = (TH1F*) gDirectory->Get("triple");
    h3->Draw("same");
    TH1F* h4 = (TH1F*) gDirectory->Get("quadruple");
    h4->Draw("same");*/
    
    
    
    
    TH1F* h1 = (TH1F*) gDirectory->Get("adc_r");
    Double_t scale = 1/h1->Integral();
    h1 -> Scale(scale);
    h1->Draw();
    
    TH1F* h2 = (TH1F*) gDirectory->Get("adc_nr");
    Double_t scale2 = 1/h2->Integral();
    h2 -> Scale(scale2);
    h2->Draw("same");
   
    TH1F* h3 = (TH1F*) gDirectory->Get("adc_1");
    Double_t scale3 = 1/h3->Integral();
    h3 -> Scale(scale3);
    h3->Draw("same");
    
    //TH1F* h4 = (TH1F*) gDirectory->Get("adc_4");
    //h4->Draw("same");
    
	//h2->GetYaxis()->SetRangeUser(0,100000);
	
	

	//h4->GetYaxis()->SetRangeUser(3,40e5);
	//h4->SetTitle("dijet mass distribution of mass point 20TeV");
	
	
	TLegend *legend2 = new TLegend(0.1,0.8,0.6,0.9);
	legend2->AddEntry(h1," DeltaT < 50, reflection, mean = 852.8, 18362 entries");
    legend2->AddEntry(h2," DeltaT > 130, non reflection, mean = 1353, 1412 entries");
    legend2->AddEntry(h3," 1 hit, mean = 809.2, 792247 entries ");
    //legend2->AddEntry(h4,"4-hit in 8 instances");
    

    legend2->Draw();
	canvas->Update();
	
	

}
