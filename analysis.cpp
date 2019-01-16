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
    
    cout <<"changed" <<endl;
    
	int adc_bin = 125, adc_min = 0, adc_max = 4125;   //uncalibrated
	int tdc_bin = 250, tdc_min = 0, tdc_max = 2000;
	//int r_nr = 130;
	
	//int adc_bin = 20, adc_min = -200, adc_max = 0;    //calibrated
	//int tdc_bin = 60, tdc_min = 0; tdc_max = 600;
	//r_nr = 70;
	
	auto hit_count = new TH1F("tdc_hit_count", "Number of TDC hits", 10, 0.0, 10.0);
   	auto tdc_channel = new TH1F("tdc_channel_number", "channel number", 77,50, 127);
	
	TH1F* tdc_ch_r = new TH1F("tdc_ch_r", "ch", 77, 50, 127);
   	TH1F* delta_time = new TH1F("delta time", "delta time", tdc_bin , tdc_min, tdc_max);
   	TH1F* delta_t1 = new TH1F("single", "time", tdc_bin , tdc_min, tdc_max);
   	TH1F* delta_t2 = new TH1F("double", "delta time (double)", tdc_bin ,tdc_min, tdc_max);
   		TH1F* delta_t2_r = new TH1F("double_r", "delta time (double)", tdc_bin , tdc_min, tdc_max);
   		TH1F* delta_t2_nr = new TH1F("double_nr", "delta time (double)", tdc_bin , tdc_min, tdc_max);

   	TH1F* delta_t3 = new TH1F("triple", "delta time (Triple)",  tdc_bin , tdc_min, tdc_max);
   	TH1F* delta_t4 = new TH1F("quadruple", "delta time (Qadruple)", tdc_bin , tdc_min, tdc_max);
   	
   	auto adc_values = new TH1F("adc_values", "Value of ADC values for Channel 0; ADC Counts", 20 , -200, 0);
   	TH1F* adc1 = new TH1F("adc_1", "adc", adc_bin , adc_min, adc_max);
   	TH1F* adc2 = new TH1F("adc_2", "adc", adc_bin , adc_min, adc_max);
   		TH1F* adc2_r = new TH1F("adc_r", "charge", adc_bin , adc_min, adc_max);
   		TH1F* adc2_nr = new TH1F("adc_nr", "charge", adc_bin , adc_min, adc_max);
   	TH1F* adc3 = new TH1F("adc_3", "adc", adc_bin , adc_min, adc_max);
   	TH1F* adc4 = new TH1F("adc_4", "adc", adc_bin , adc_min, adc_max);

	std::vector <int> t; 
	std::vector <int> dt2; 
	std::vector <int> dt3; 
	std::vector <int> dt4; 
	std::vector <int> adcc;
    
    int counter = 0;
    int event_number = 0;
	int Single = 0, Double =0, Triple = 0, Quadruple =0;
    
    //Start of event loop
	traverse_runs(argc, argv, [&](MEvent& e) {
        auto n_hits = e.getAllTDCHits();
        //const auto adcs = e.getAllADCReadouts();
		auto scint_hits = e.getScintillatorHits();
		
		std::map<int, int> ch_counts;      
        std::map<int,vector<int>> temp_adc;
        std::map<int, vector<int>> temp;
        
        //Get tdc channel number
        for_each (scint_hits.cbegin(),scint_hits.cend(), [&](const edm::ScintillatorHit &sh) {		
        	
        	// calibrated_time_measurement; raw_time_measurement
        	// calibrated_charge; raw_adc_count
        	tdc_channel -> Fill (sh.tdc_channel_number); 
        	
        	temp[sh.tdc_channel_number].push_back(sh.raw_time_measurement);
        	adcc.push_back(sh.raw_adc_count);
        	temp[sh.tdc_channel_number].push_back(sh.raw_adc_count);
        	ch_counts[sh.tdc_channel_number]++;                   // count occurrence of channels
       		adc_values ->Fill (sh.raw_adc_count);
		    cout << "channel: "<< sh.tdc_channel_number << ";" <<"time measurement: " << sh.raw_time_measurement <<";" << "adc value: " << sh.raw_adc_count << ";"<< endl;
			
			hit_count->Fill( n_hits.size());
			//cout << "n hits"<<n_hits.size () << endl;
			counter ++;
        });
                
         for (auto& x: temp) {
    		std::cout << "[" << x.first << ": ";	
    		if (temp.find(x.first)->second.size() == 2) {
    			Single ++;
    			t.push_back (temp.find(x.first)->second[0]);
    			    			
				adc1->Fill(temp.find(x.first)->second[1]);
    		} else if (temp.find(x.first)->second.size() == 4) {
    	 		cout << "------------------------------------------------double" <<endl;
    	 		Double++;
    	 		
    	 		
    	 		int del_t_2;
    	 		int *p_del_t_2 = &del_t_2;
    	 		*p_del_t_2 = abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]);
    	 		
    	 		dt2.push_back (del_t_2);
    	 		
    	 		//differentiating the reflection and non reflection part
    	 		if (del_t_2 <= 50) {
					delta_t2_r ->Fill (del_t_2);
					adc2_r ->Fill(temp.find(x.first)->second[1]);
					//adc2_r->Fill(temp.find(x.first)->second[3]);
					tdc_ch_r -> Fill(x.first);
					cout << "deltR" << x.first << " " << del_t_2 << "first adc"<< temp.find(x.first)->second[1] << "second adc"<<temp.find(x.first)->second[3]<<endl;
				} else if (del_t_2 > 130) {
					delta_t2_nr ->Fill (del_t_2);
					adc2_nr ->Fill(temp.find(x.first)->second[1]);
					//adc2_nr->Fill(temp.find(x.first)->second[3]);
					cout << "deltnR" <<x.first << " "<< del_t_2 << "first adc"<< temp.find(x.first)->second[1] << "second adc"<<temp.find(x.first)->second[3]<<endl;

				}
				
    	 		adc2->Fill(temp.find(x.first)->second[1]);
    	 		//adc2->Fill(temp.find(x.first)->second[3]);
    	 	}else if (temp.find(x.first)->second.size() == 6) {
    	 		//cout << "----------------------------------------------Triple" <<endl;

    	 		Triple ++;
    	 	    dt3.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 	    dt3.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 		dt3.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		
    	 		adc3->Fill(temp.find(x.first)->second[1]);
    	 		//adc3->Fill(temp.find(x.first)->second[3]);
    	 		//adc3->Fill(temp.find(x.first)->second[5]);
    	 	}else if (temp.find(x.first)->second.size() == 8){
    	 		//cout << "----------------------------------------------Quadruple"<<endl;
    	 		Quadruple ++;
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[2]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[4]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[0] - temp.find(x.first)->second[6]));
    	 	    dt4.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[4]));
    	 	    dt4.push_back ( abs(temp.find(x.first)->second[2] - temp.find(x.first)->second[6]));
    	 		dt4.push_back ( abs(temp.find(x.first)->second[4] - temp.find(x.first)->second[6]));

	            adc4->Fill(temp.find(x.first)->second[1]);
    	 		//adc4->Fill(temp.find(x.first)->second[3]);
    	 		//adc4->Fill(temp.find(x.first)->second[5]);
    	 		//adc4->Fill(temp.find(x.first)->second[7]);
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
			
			//Filling the delta time plots
			
			for (const auto & del_t1: t) {       //single time
				delta_t1 ->Fill (del_t1);
			}
			
			for (const auto & del_t2: dt2) {     //double time
				delta_t2 ->Fill (del_t2);
			}
			
			for (const auto & del_t3: dt3) {     //triple time
				delta_t3 ->Fill (del_t3);
			}
			
			for (const auto & del_t4: dt4) {     //quadruple time
				delta_t4 ->Fill (del_t4);
			}

			
			//checking max and min of the vectors for plotting purposes
			/*double max_dt = *max_element(dt.begin(), dt.end());
			int min_dt = *min_element(dt.begin(), dt.end());
			
			int max_adc = *max_element(adcc.begin(), adcc.end());
			int min_adc = *min_element(adcc.begin(), adcc.end());
			
			cout << "total number of event loops" << event_number << "counter"<< counter << endl;
			cout<<"Max value of dt: "<<max_dt<< " min value of dt: " <<min_dt<<endl;
			cout<<"Max value of adc: "<<max_adc<< " min value of adc: "<<min_adc<<endl;*/
			
			//printing out statistics
			//cout << "size of dt" << dt.size () <<endl;
			cout << "Saw " << hit_count->GetEntries() << " events" << endl;
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
			TFile*f = new TFile("tdc_hit25.root", "RECREATE");
			hit_count->Write();
			adc_values->Write();
			tdc_channel->Write();
			delta_time->Write();
			
			delta_t1->Write();
			delta_t2->Write();
			delta_t2_r->Write();
			delta_t2_nr->Write();
			delta_t3->Write();
			delta_t4->Write();
			tdc_ch_r->Write();
			
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

