#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

//call for outputting results to txt function
void output_file(std::vector<int> numcount, std::vector<int>& numbers);

//call for random number generator function
void num_gen(std::vector<int>& numbers);

int main(){

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
    std::string path = "C:\\Users\\samhu\\lotterychecker\\Lottery-numbers-csv\\lotto-results-2001.csv";

    std::ifstream file(path);

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



//random number generator - stored in vector
void num_gen(std::vector<int>& numbers){
    
    //lambda to generate random vector
    std::generate(numbers.begin(), numbers.end(), []() {
        return 1 + ( std::rand() % ( 50 - 1 + 1 ));
    });

}

