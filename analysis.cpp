#include "TestStandOffline/EventLoop.hpp"
#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <map>
#include <vector>
#include <TCanvas.h>
#include <algorithm>
using namespace std;
using namespace test_stand;


int main(int argc, char* argv[]) {
    
    auto hit_count = new TH1F("tdc_hit_count", "Number of TDC hits", 10, 0.0, 10.0);
   	auto tdc_channel = new TH1F("adc_channel_number", "channel number", 10,0, 127);

   	TH1F* delta_time = new TH1F("delta time", "delta time", 250, 0, 2000);
   	TH1F* delta_t1 = new TH1F("single", "time", 250, 0, 2000);
   	TH1F* delta_t2 = new TH1F("double", "delta time (double)", 250, 0, 2000);
   	TH1F* delta_t3 = new TH1F("triple", "delta time (Triple)",  250,0, 2000);
   	TH1F* delta_t4 = new TH1F("quadruple", "delta time (Qadruple)", 250, 0, 2000);
   	
   	// Adc histograms
   	auto adc_values = new TH1F("adc_values", "Value of ADC values for Channel 0; ADC Counts", 125, 0, 4250);
   	TH1F* adc1 = new TH1F("adc_1", "adc", 125, 0, 4125);
   	TH1F* adc2 = new TH1F("adc_2", "adc", 125, 0, 4125);
   	TH1F* adc3 = new TH1F("adc_3", "adc", 125, 0, 4125);
   	TH1F* adc4 = new TH1F("adc_4", "adc", 125, 0, 4125);

	std::vector <int> dt; 
	std::vector <int> adcc;
    
    int counter = 0;
    int event_number = 0;
	int Double =0, Quadruple =0, Triple = 0, five = 0;
    
    //Start of event loop
	traverse_runs(argc, argv, [&](MEvent& e) {
        //auto n_hits = e.getAllTDCHits();
        //const auto adcs = e.getAllADCReadouts();
		auto scint_hits = e.getScintillatorHits();
		
		
		std::map<int, int> ch_counts;      
        std::map<int,vector<int>> temp_adc;
        std::map<int, vector<int>> temp;
        
        //Get tdc channel number
        for_each (scint_hits.cbegin(),scint_hits.cend(), [&](const edm::ScintillatorHit &sh) {		
        	// calibrated_time_measurment
        	// calibrated_charge
        	temp[sh.tdc_channel_number].push_back(sh.raw_time_measurement);
        	adcc.push_back(sh.raw_adc_count);
        	temp[sh.tdc_channel_number].push_back(sh.raw_adc_count);

        	tdc_channel -> Fill (sh.tdc_channel_number);        		
        	ch_counts[sh.tdc_channel_number]++; // increment the counter for the current value
       		adc_values ->Fill (sh.raw_adc_count);
		    cout << "channel"<< sh.tdc_channel_number << " " <<"time measurement" << sh.raw_time_measurement << "adc value" << sh.raw_adc_count<< endl;

        });
                
         for (auto& x: temp) {
    		std::cout << "[" << x.first << ": ";	
    		if (temp.find(x.first)->second.size() == 2) {
    			dt.push_back (temp.find(x.first)->second[0]);
    			delta_t1->Fill(temp.find(x.first)->second[0]);
    			
				adc1->Fill(temp.find(x.first)->second[1]);
    		} else if (temp.find(x.first)->second.size() == 4) {
    	 		cout << "----------------------------------------------double" <<endl;
    	 		Double++;
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		cout << abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]) <<endl;
    	 		delta_t2->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		
    	 		adc2->Fill(temp.find(x.first)->second[1]);
    	 		adc2->Fill(temp.find(x.first)->second[3]);
    	 		cout << temp.find(x.first)->second[3] << endl;
    	 	}else if (temp.find(x.first)->second.size() == 6) {
    	 		cout << "----------------------------------------------Triple" <<endl;

    	 		Triple ++;
    	 	    dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 		delta_t3->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		
    	 		adc3->Fill(temp.find(x.first)->second[1]);
    	 		adc3->Fill(temp.find(x.first)->second[3]);
    	 		adc3->Fill(temp.find(x.first)->second[5]);
    	 		
    	 		cout << temp.find(x.first)->second[5] << endl;
    	 	}else if (temp.find(x.first)->second.size() == 8){
    	 		cout << "----------------------------------------------Quadruple"<<endl;
    	 		Quadruple ++;
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[6]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 	    dt.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[6]));
    	 		dt.push_back ( abs(temp.find(x.first)->second[4] - temp.find(x.first)->second[6]));

	            
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[6]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[6]));
	            delta_t4->Fill(abs(temp.find(x.first)->second[4] - temp.find(x.first)->second[6]));
	            
	            adc4->Fill(temp.find(x.first)->second[1]);
    	 		adc4->Fill(temp.find(x.first)->second[3]);
    	 		adc4->Fill(temp.find(x.first)->second[5]);
    	 		adc4->Fill(temp.find(x.first)->second[7]);
    	 		
    	 		cout << temp.find(x.first)->second[3] <<endl;


			}else if (temp.find(x.first)->second.size() == 10){
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
       		temp_adc.erase(temp_adc.begin(), temp_adc.end());
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
	 
			double max_dt = *max_element(dt.begin(), dt.end());
			int min_dt = *min_element(dt.begin(), dt.end());
			
			int max_adc = *max_element(adcc.begin(), adcc.end());
			int min_adc = *min_element(adcc.begin(), adcc.end());
			
			cout<<"Max value of dt: "<<max_dt<< "min value of dt: " <<min_dt<<endl;
			cout<<"Max value of adc: "<<max_adc<< "min value of adc: "<<min_adc<<endl;

	
			// Write the histogram out to a file
    		delta_t1->SetLineColor(1);
			delta_t2->SetLineColor(2);
			delta_t3->SetLineColor(2);
			delta_t4->SetLineColor(3);
			
			adc1->SetLineColor(1);
			adc2->SetLineColor(2);
			adc3->SetLineColor(3);
			adc4->SetLineColor(4);
			
    
    
    {
			TFile*f = new TFile("tdc_hit15.root", "RECREATE");
			hit_count->Write();
			adc_values->Write();
			tdc_channel->Write();
			delta_time->Write();
			
			delta_t1->Write();
			delta_t2->Write();
			delta_t3->Write();
			delta_t4->Write();
			
			adc1->Write();
			adc2->Write();
			adc3->Write();
			adc4->Write();
		
			f->Close();
         
    }
    

    return 0;
}
