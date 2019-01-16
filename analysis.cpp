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
    
    int counter = 0;
    int event_number = 0;
	int Single = 0, Double =0, Triple = 0, Quadruple =0;
	
    //uncalibrated hist arguments
	int adc_bin = 125, adc_min = 0, adc_max = 4250;   
	int tdc_bin = 250, tdc_min = 0, tdc_max = 2250;
	
	// differentiate peak and flat region on delta time plot
	int t_boundary = 50, t_boundary2 = 130;   
	auto hit_count = new TH1F("tdc_hit_count", "Number of TDC hits", 10, 0.0, 10.0); 
   	auto tdc_channel = new TH1F("tdc_channel_number", "channel number", 77,50, 127);
	
	//timing hist
	TH1F* tdc_ch_r = new TH1F("tdc_ch_r", "ch", 77, 50, 127);
   	TH1F* delta_t1 = new TH1F("single", "time", tdc_bin , tdc_min, tdc_max);
   	TH1F* delta_t2 = new TH1F("double", "delta time (double)", tdc_bin ,tdc_min, tdc_max);
   	TH1F* delta_t3 = new TH1F("triple", "delta time (Triple)",  tdc_bin , tdc_min, tdc_max);
   	TH1F* delta_t4 = new TH1F("quadruple", "delta time (Qadruple)", tdc_bin , tdc_min, tdc_max);
   	
   	//ADC hist
   	TH1F* adc1 = new TH1F("adc_1", "adc", adc_bin , adc_min, adc_max);
   	TH1F* adc2 = new TH1F("adc_2", "adc", adc_bin , adc_min, adc_max);
   	TH1F* adc2_r = new TH1F("adc_r", "charge", adc_bin , adc_min, adc_max);
   	TH1F* adc2_nr = new TH1F("adc_nr", "charge", adc_bin , adc_min, adc_max);
   	TH1F* adc3 = new TH1F("adc_3", "adc", adc_bin , adc_min, adc_max);
   	TH1F* adc4 = new TH1F("adc_4", "adc", adc_bin , adc_min, adc_max);


	//storing time or delta time info
	std::vector <int> t; 
	std::vector <int> dt2; 
	std::vector <int> dt3; 
	std::vector <int> dt4; 
    
    //Start of event loop
	traverse_runs(argc, argv, [&](MEvent& e) {
		auto n_hits = e.getAllTDCHits();
		auto scint_hits = e.getScintillatorHits();
		
		std::map<int, int> ch_counts;      
        std::map<int, vector<int>> temp;
        
        //Extract TDC and ADC data from mathusla event data 
        for_each (scint_hits.cbegin(),scint_hits.cend(), [&](const edm::ScintillatorHit &sh) {		
        	
        	hit_count->Fill( n_hits.size());
        	tdc_channel -> Fill (sh.tdc_channel_number); 
        	temp[sh.tdc_channel_number].push_back(sh.raw_time_measurement);
        	temp[sh.tdc_channel_number].push_back(sh.raw_adc_count);
        	ch_counts[sh.tdc_channel_number]++;                   // count occurrence of channels
		    //cout << "channel: "<< sh.tdc_channel_number << ";" <<"time measurement: " << sh.raw_time_measurement <<";" << "adc value: " << sh.raw_adc_count << ";"<< endl;
			counter ++;
        });
                
         //Differentiate single, double, triple and quadruple hits       
         for (auto& x: temp) {
    		//cout << "[" << x.first << ": ";	
    		if (temp.find(x.first)->second.size() == 2) {
    			Single ++;
    			t.push_back (temp.find(x.first)->second[0]);
    			    			
				adc1->Fill(temp.find(x.first)->second[1]);
    		} else if (temp.find(x.first)->second.size() == 4) {
    	 		//cout << "-----------------------------------------------double" <<endl;
    	 		Double++;
    	 		
    	 		int del_t_2;
    	 		int *p_del_t_2 = &del_t_2;
    	 		*p_del_t_2 = abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]);
    	 		
    	 		dt2.push_back (del_t_2);
    	 		adc2->Fill(temp.find(x.first)->second[1]);

    	 		//differentiating the peak and flat part in timing plot
    	 		if (del_t_2 <= t_boundary) {
					adc2_r ->Fill(temp.find(x.first)->second[1]);
					tdc_ch_r -> Fill(x.first);
				
				} else if (del_t_2 > t_boundary2) {
					adc2_nr ->Fill(temp.find(x.first)->second[1]);
					
				}
    	 	} else if (temp.find(x.first)->second.size() == 6) {
    	 		//cout << "----------------------------------------------Triple" <<endl;
    	 		Triple ++;
    	 	    dt3.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 	    dt3.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 		dt3.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		
    	 		adc3->Fill(temp.find(x.first)->second[1]);
    	 		
    	 	} else if (temp.find(x.first)->second.size() == 8) {
    	 		//cout << "----------------------------------------------Quadruple"<<endl;
    	 		Quadruple ++;
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[6]));
    	 	    dt4.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 	    dt4.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[6]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[4] - temp.find(x.first)->second[6]));

	            adc4->Fill(temp.find(x.first)->second[1]);
    	 		
			}
    	 	
    	 	//for (int i =0; i < temp.find(x.first)->second.size() ; i++) {cout << temp.find(x.first)->second[i] << ", ";}
    			//cout << "]";
    			//cout << "\n";
  		    }
         
         	//printing out the time stored
         	
         	//for (auto it = ch_counts .cbegin(); it != ch_counts.cend(); ++it) {
        	//	std::cout << " [" << it->first << ':' << it->second << ']';
        	//	std::cout << '\n';
         	//}
        
        	ch_counts.erase(ch_counts.begin(), ch_counts.end());
        	temp.erase(temp.begin(), temp.end());
       
			event_number ++;
			//cout << "---------event number-------" << event_number << endl;
      
      }); //End of event loop
			
			//Filling the delta time plots

			for (int & del_t1: t) {       //single time
				delta_t1 ->Fill (del_t1);
			}
			
			for (int & del_t2: dt2) {     //double time
				delta_t2 ->Fill (del_t2);
			}
			
			for (int & del_t3: dt3) {     //triple time
				delta_t3 ->Fill (del_t3);
			}
			
			for (int & del_t4: dt4) {     //quadruple time
				delta_t4 ->Fill (del_t4);
			}

			
			//printing out counts
			cout << "Saw " << hit_count->GetEntries() << "hits" << endl;
			cout <<"single"<< Single <<"double " << Double << "triple " << Triple << "Quadruple " << Quadruple << endl;

	
			// Write the histogram out to a file
    		delta_t1->SetLineColor(1);
			delta_t2->SetLineColor(2);
			delta_t3->SetLineColor(3);
			delta_t4->SetLineColor(4);
			
			adc1->SetLineColor(3);
			adc2->SetLineColor(1);
			adc3->SetLineColor(2);
			adc4->SetLineColor(4);
			
			adc2_r->SetLineColor(1);
			adc2_nr->SetLineColor(2);
			
    {
			TFile*f = new TFile("tdc_test.root", "RECREATE");
			tdc_channel->Write();
			tdc_ch_r->Write();
			
			delta_t1->Write();
			delta_t2->Write();
			delta_t3->Write();
			delta_t4->Write();
			
			adc1->Write();
			adc2->Write();
			adc2_r->Write();
			adc2_nr->Write();
			adc3->Write();
			adc4->Write();
					
			f->Close();
         
    }
    
    return 0;
}

