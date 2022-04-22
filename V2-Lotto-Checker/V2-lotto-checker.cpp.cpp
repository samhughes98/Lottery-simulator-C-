#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <filesystem>
#include <map>

//call for outputting results to txt function
void output_file(std::vector<int> numcount, std::vector<int>& numbers);

//call for random number generator function
void num_gen(std::vector<int>& numbers);

int main(){

    clock_t start, end, start1, end1;
    start = clock();
    start1 = clock();

    //random num gen variables
    srand(time(NULL));
    int size = 6;

    std::vector<int> numbers(size);
    
    //fetch and print generated numbers
    num_gen(numbers);
    
    //checking if random nums are unique
    auto it = std::unique(numbers.begin(), numbers.end());
    bool duplicate = (it == numbers.end());
    
    //if not unique, run generator again
    if(duplicate == 0){
      num_gen(numbers);
    }

    //formatting for terminal output of random nums
    std::cout<< std::endl;
    std::cout << "Numbers generated: ";
    for (auto i: numbers)
    std::cout<< i << " ";
    std::cout<< std::endl;
    std::cout<< std::endl;

    std::string line;
    std::vector<int> numcount;

    //open csv file for reading
     std::string path = "C:\\Users\\samhu\\lotterychecker\\Lottery-numbers-csv";
     for (const auto & f : std::filesystem::directory_iterator(path)){

    std::ifstream file(f.path());

    //if csv is successfully open
    if(file.is_open()) 
    {
        //initialising counters
        int found = 0;
        int line_count = 0;

       //run through entire file
        while(getline(file, line, ','))
        {

            //create stream to turn numbers to ints
            std::stringstream linestream(line);
            int item;
            
            //if stream is in int format
            while(linestream >> item)
            {
                //counter for lines in file
                ++line_count;

                //output file contents
                std::cout << item << '\n';
                
                //split into chunks of 7
                if(line_count % 7 == 0){
                std::cout << std::endl;}

                    //check if file contains any of the 6 numbers in 'numbers' vector
                    for(int i = 0; i < size; i++)
                    {
                        if(item == numbers.at(i))
                        {   
                            //counter for number of nums found in file
                            ++found;
                            
                            //output number found and how many nums have been found in the 7 lines so far
                            std::cout << "Number found: " << numbers.at(i) << " - count: " << found << '\n';  
                        }

                         //if linecount is a multiple of 7 (end of section) - send numbers to vector and reset found count to 0
                            if(line_count == 7)
                            {
                                numcount.push_back(found);   
                                found = 0;   
                                line_count = 0;  
                            }                     
                    }

            }
        }        
    }   
    



    }
    
    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by program is : " << std::fixed 
         << time_taken << std::setprecision(5);
    std::cout << " sec " << std::endl;
    
    std::ofstream outfile("time.txt", std::ios_base::app | std::ios_base::in);
     outfile << time_taken << " ";

     
    //send to output file function
    output_file(numcount, numbers);
  
}


//random number generator - stored in vector
void num_gen(std::vector<int>& numbers){
    
    //lambda to generate random vector
    std::generate(numbers.begin(), numbers.end(), []() {
        return 1 + ( std::rand() % ( 50 - 1 + 1 ));
    });

}

//function to output results to txt file
void output_file(std::vector<int> numcount, std::vector<int>& numbers){
    
    //sort results in order
    std::sort(numcount.begin(), numcount.end());
    
    //erase all 0's
    numcount.erase(std::remove(numcount.begin(), numcount.end(), 0), numcount.end());

    //outputting result vector and formatted for easier reading
    std::ofstream output_txt("Winners!.txt");
    output_txt << "         Results of lottery draw" << '\n' << '\n';

    output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Original numbers were " << '\n' << '\n';

    for(auto i: numbers){
    output_txt << " " << i ;}

    output_txt << '\n' << '\n';

    output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 1 number : "
         << count(numcount.begin(), numcount.end(), 1) << '\n' << '\n';

    output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 2 numbers : "
         << count(numcount.begin(), numcount.end(), 2) << '\n' << '\n';

         output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 4 numbers : "
         << count(numcount.begin(), numcount.end(), 3) << '\n' << '\n';

         output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 4 numbers : "
         << count(numcount.begin(), numcount.end(), 4) << '\n' << '\n';

         output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 5 numbers : "
         << count(numcount.begin(), numcount.end(), 5) << '\n' << '\n';

         output_txt << "-------------------------------------------" << '\n' << '\n';

    output_txt << "Winners with 6 numbers : "
         << count(numcount.begin(), numcount.end(), 6) << '\n' << '\n';

         

}

