#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iostream>

void sum_channel_appearances(const std::vector<std::string>& filenames, const char* treename, const char* event_branch_name, const char* channel_branch_name, Double_t channel_value, const char* output_filename) {
    // Open the output file
    std::ofstream output_file(output_filename);

    // Loop over the input files
    long long sum = 0;
    int event_count = -1;
    Double_t channel_count = 0;
    for (const auto& filename : filenames) {
        // Open the input file
        TFile file(filename.c_str(), "READ");

        // Get the input tree
        TTree* tree = dynamic_cast<TTree*>(file.Get(treename));

        // Set up the branch pointers
        int event_num = 0;
        Double_t channel_num = 0;
        tree->SetBranchAddress(event_branch_name, &event_num);
        tree->SetBranchAddress(channel_branch_name, &channel_num);

        // Loop over the entries in the tree
        for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
            // Get the current entry from the tree
            tree->GetEntry(i);

            if (event_num != event_count) {
                if (event_count >= 0) {
                    output_file << event_count << "\t" << sum << std::endl;
                }
                sum = 0;
                event_count = event_num;
            }

            if (channel_num == channel_value) {
                ++channel_count;
                ++sum;
            }
        }

        // Close the input file
        file.Close();
    }

    // Write the output for the last event
    output_file << event_count << "\t" << sum << std::endl;

    // Close the output file
    output_file.close();
}

void compress(std::vector<std::string> input, const char* output){
    sum_channel_appearances(input,"Hits","fEvent","fsensor",0,output);
    std::cout<<"Done"<<std::endl;
}