#include "TestStandOffline/EventLoop.hpp"
#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <map>
#include <vector>
#include <TCanvas.h>
using namespace std;
using namespace test_stand;


int main(int argc, char* argv[]) {
    
    auto hit_count = new TH1F("tdc_hit_count", "Number of TDC hits", 10, 0.0, 10.0);
   	auto tdc_channel = new TH1F("adc_channel_number", "channel number", 10,0, 127);
   	TCanvas *canvas = new TCanvas ("c", "canvas1", 200,10,600,400);

   	TH1F* delta_time = new TH1F("delta time", "delta time", 250, 0, 2000);
   	TH1F* delta_t2 = new TH1F("double", "delta time (double)", 250, 0, 2000);
   	TH1F* delta_t3 = new TH1F("triple", "delta time (Triple)",  250,0, 2000);
   	TH1F* delta_t4 = new TH1F("quadruple", "delta time (Qadruple)", 250, 0, 2000);
   	
   	// Adc histograms
   	auto adc_values = new TH1F("adc_channel_0_values", "Value of ADC values for Channel 0; ADC Counts", 100, 100, -100);
   	TH1F* adc_double = new TH1F("adc_2", "adc", 250, 0, 2000);
   	TH1F* adc_triple = new TH1F("adc_3", "adc", 250, 0, 2000);
   	TH1F* adc_quadruple = new TH1F("adc_4", "adc", 250, 0, 2000);

	std::vector <int> dt; 
    
    int counter = 0;
    int event_number = 0;
	int Double =0, Quadruple =0, Triple = 0, five = 0;
    
    //Start of event loop
	traverse_runs(argc, argv, [&](MEvent& e) {
        auto n_hits = e.getAllTDCHits();
        const auto adcs = e.getAllADCReadouts();
		
		auto scint_htis = e.getScintillatiorHits();
		
		
		std::map<int, int> ch_counts;      
        std::map<int,int> ch_t;
        std::map <int, vector<int>> temp;
        
        hit_count->Fill( n_hits.size());   
        adc_values->Fill(adcs[0].adc_events[0].channel_counts[0]);
        
        //Get tdc channel number
        for_each (n_hits.cbegin(),n_hits.cend(), [&](const edm::TDCHit &hit) {		
        	
        	temp[hit.channel_number].push_back(hit.time_measurement);
        	tdc_channel -> Fill (hit.channel_number);        		
        	ch_counts[hit.channel_number]++; // increment the counter for the current value
       
       		for_each (adcs.cbegin(),adcs.cend(), [&](const edm:: ))
       	
       		//set pointers here
			int adc_module;
			int *p_adc_module= &adc_module;
			*p_adc_module = (hit.channel_number-64)/8;
			int adc_channel; 
			int *p_adc_ch = &adc_channel;
			*p_adc_ch = hit.channel_number%8;
			
			int adc;
			int *Padc = &adc
			*Padc = adcs[0].adc_events[0].channel_counts[adc_channel];
			
		    cout << "channel"<< hit.channel_number << " " <<"time measurement" << hit.time_measurement << "adc value" << adc << endl;

        });
           
        
           
           
         for (auto& x: temp) {
    		std::cout << "[" << x.first << ": ";	
         	if (temp.find(x.first)->second.size() == 2) {
    	 		cout << "double----------------------------------------------" <<endl;
    	 		Double++;
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
    	 		
    	 		delta_t2->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
    	 	}else if (temp.find(x.first)->second.size() == 3) {
    	 		cout << "Triple----------------------------------------------!" <<endl;

    	 		Triple ++;
    	 	    dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[2]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[2]));
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 	}else if (temp.find(x.first)->second.size() == 4){
    	 		Quadruple ++;
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[3]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[2]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[3]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[3]));

	            
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[1]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[3]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[2]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[3]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[1] - temp.find(x.first)->second[3]));

			}else if (temp.find(x.first)->second.size() == 5){
    	 	    five ++;
    	 	}
    	 	
    	 	 	
    	 	for (int i =0; i < temp.find(x.first)->second.size() ; i++) {cout << temp.find(x.first)->second[i] << ", ";}
    		
    		cout << "]";
    		cout << "\n";
  		    }
         
         	//printing out the time stored
         	for (auto it = ch_counts .cbegin(); it != ch_counts.cend(); ++it) {
        		std::cout << " [" << it->first << ':' << it->second << ']';
        		std::cout << '\n';
         	}
        
        	ch_counts.erase(ch_counts.begin(), ch_counts.end());
       		ch_t.erase(ch_t.begin(), ch_t.end());
        	temp.erase(temp.begin(), temp.end());
       
			event_number ++;
			cout << "---------event number-------" << event_number << endl;
      
      }); //End of event loop
		
		cout << "printing out the delta time" <<endl;
	
			for (const auto & del_t: dt) {
				delta_time ->Fill (del_t);
				cout << "delta time" << del_t <<endl;
			}
	
			cout << "size of dt" << dt.size () <<endl;
			cout << "Saw " << hit_count->GetEntries() << " events" << endl;
			cout <<"double " << Double << "triple " << Triple << "Quadruple " << Quadruple << "five"<< five << endl;
	 
			//std::vector<int>::iterator mindt = min_element( dt.begin(), dt.end() );
			//std::vector<int>::iterator maxdt = max_element( dt.begin(), dt.end() );
	 
			//cout << "max" << maxdt << "min" << mindt << endl;
	
			// Write the histogram out to a file
    
			delta_t2->SetLineColor(1);
			delta_t2->Draw();
			delta_t3->SetLineColor(2);
			delta_t3->Draw("same");
			delta_t4->SetLineColor(3);
			delta_t4->Draw("same");
			canvas->SetLogy();
    
    
    {
			TFile*f = new TFile("tdc_hit88.root", "RECREATE");
			hit_count->Write();
			adc_values->Write();
			tdc_channel->Write();
			delta_time->Write();
			delta_t2->Write();
			delta_t3->Write();
			delta_t4->Write();
			canvas->Write();
		
			f->Close();
         
    }
    

    return 0;
}
