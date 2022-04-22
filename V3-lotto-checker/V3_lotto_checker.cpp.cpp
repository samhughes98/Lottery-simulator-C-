#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <filesystem>
#include <mutex>
#include <windows.h>

//call for outputting results to txt function
void output_file(std::vector<int> numcount, std::vector<int> & numbers);

//call for random number generator function
void num_gen(std::vector<int> & numbers);

//call for file iteration function
void file_iterator(std::vector<std::string> & filenames);

//matcher function
void matcher(std::vector<int> & numbers, std::vector<std::string> & filenames, std::vector<int> & numcount);

//print vectors function
void print(std::vector<int> & ans);

//setter for numcount vector
std::vector<int> * numcount;

//mutex to use for synchronisation in threading
std::mutex mut;

int main() {

  //clock to start timing program
  clock_t start, end, start1, end1;
  start = clock();

  //storage and run of file iterator function
  std::vector<std::string> filenames;
  std::thread ff(file_iterator, std::ref(filenames));

  //random num gen variables
  int size = 6;

  //fetch and print generated numbers
  std::vector<int> numbers(size);
  std::thread numt(num_gen, std::ref(numbers));

  //vector to store matching number count later
  std::vector<int> numcount;

  ff.join();
  numt.join();

  //vectors to split all filenames equally for multithreading purposes
  std::vector<std::string> path1;
  std::vector<std::string> path2;
  std::vector<std::string> path3;

  //inserting into above vectors 7 filenames/7 filenames/6 filenames - to make all 20 files
  for (int i = 0; i < 7; i++) {
    path1.push_back(filenames[i]);
  }

  for (int i = 0; i < 7; i++) {
    path2.push_back(filenames[i]);
  }

  for (int i = 0; i < 6; i++) {
    path3.push_back(filenames[i]);
  }

  start1 = clock();

  //initialise threads to matcher function with above vectors as arguments
  std::thread t1(matcher, std::ref(numbers), std::ref(path1), std::ref(numcount));
  std::thread t2(matcher, std::ref(numbers), std::ref(path2), std::ref(numcount));
  std::thread t3(matcher, std::ref(numbers), std::ref(path3), std::ref(numcount));

  t1.join();
  t2.join();
  t3.join();

  end1 = clock();

  double time_taken1 = double(end1 - start1) / double(CLOCKS_PER_SEC);
  std::cout << "Time during multithreading : " << std::fixed 
      << time_taken1 << std::setprecision(5);
  std::cout << " sec " << std::endl;

  std::ofstream outfile1("time_threaded1.txt", std::ios_base::app | std::ios_base::in);
  outfile1 << time_taken1 << " ";


  //send to function to output data to txt file
  output_file(numcount, numbers);

  //end timer
  end = clock();

  //function to print time taken for 1 launch
  double runtime = double(end - start) / double(CLOCKS_PER_SEC);
  std::cout << "runtime : " << runtime << std::setprecision(5) << '\n';

  //open final txt file with results
  system("start Winners!.txt");

  end = clock();

  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  std::cout << "Time taken by program is : " << std::fixed 
      << time_taken << std::setprecision(5);
  std::cout << " sec " << std::endl;

}

//random number generator - stored in vector
void num_gen(std::vector<int> & numbers) {

  srand(time(NULL));

  //lambda to generate random vector
  std::generate(numbers.begin(), numbers.end(), []() {
    return 1 + (std::rand() % (50 - 1 + 1));
  });

  //checking if random nums are unique
  auto it = std::unique(numbers.begin(), numbers.end());
  bool duplicate = (it == numbers.end());

  //if not unique, run generator again
  if (duplicate == 0) {
    num_gen(numbers);
  }

  //formatting for terminal output of random nums
  std::cout << std::endl;
  std::cout << "Numbers generated: ";
  print(numbers);
  std::cout << std::endl;
  std::cout << std::endl;
}

void file_iterator(std::vector<std::string> & filenames) {

  //open csv file for reading
  std::string path = "C:\\Users\\samhu\\lotterychecker\\Lottery-numbers-csv";
  for (const auto & f: std::filesystem::directory_iterator(path)) {

    filenames.push_back(f.path().string());

  }
}

//function to match parse csv files and match with random gen numbers
void matcher(std::vector<int> & numbers, std::vector<std::string> & y, std::vector<int> & numcount) {

  //initialising variables
  std::string line;
  int size = 6;

  //launch mutex for reading csv files
  std::scoped_lock lock(mut);

  //read path1/path2/path3 vectors from threads
  for (int e = 0; e < y.size(); e++) {
    std::ifstream file(y.at(e));

    //if csv is successfully open
    if (file.is_open()) {
      //initialising counters
      int found = 0;
      int line_count = 0;

      //run through entire file
      while (getline(file, line, ',')) {
        //create stream to turn numbers to ints
        std::stringstream linestream(line);
        int item;

        //if stream is in int format
        while (linestream >> item) {
          //counter for lines in file
          ++line_count;
          std::vector < int > split;
          split.push_back(item);

          //output all numbers from file
          std::cout << item << '\n';

          //split into chunks of 7
          if (line_count % 7 == 0) {
            std::cout << std::endl;
          }

          //check if file contains any of the 6 numbers in 'numbers' vector
          for (int i = 0; i < size; i++) {
            if (item == numbers.at(i)) {
              //counter for number of nums found in file
              ++found;

              //output number found and how many nums have been found in the 7 lines so far
              std::cout << "Number found: " << numbers.at(i) << " - count: " << found << '\n';
            }

            //if linecount is a multiple of 7 (end of section) - send numbers to vector and reset found count to 0
            if (line_count == 7) {
              numcount.push_back(found);
              found = 0;
              line_count = 0;
            }
          }

        }
      }
    }
  }
}

//function to output results to txt file
void output_file(std::vector<int> numcount, std::vector<int> & numbers) {

  //sort results in order
  std::sort(numcount.begin(), numcount.end());

  //erase all 0's
  numcount.erase(std::remove(numcount.begin(), numcount.end(), 0), numcount.end());

  //outputting result vector and formatted for easier reading

  std::ofstream output_txt("Winners!.txt");
  output_txt << "         Results of lottery draw" << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Original numbers were " << '\n' << '\n';

  for (auto i: numbers) {
    output_txt << " " << i;
  }

  output_txt << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 1 number : " <<
    count(numcount.begin(), numcount.end(), 1) << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 2 numbers : " <<
    count(numcount.begin(), numcount.end(), 2) << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 4 numbers : " <<
    count(numcount.begin(), numcount.end(), 3) << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 4 numbers : " <<
    count(numcount.begin(), numcount.end(), 4) << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 5 numbers : " <<
    count(numcount.begin(), numcount.end(), 5) << '\n' << '\n';

  output_txt << "-------------------------------------------" << '\n' << '\n';

  output_txt << "Winners with 6 numbers : " <<
    count(numcount.begin(), numcount.end(), 6) << '\n' << '\n';

}

// Function to print vectors
void print(std::vector<int> & v) {

  for (auto & it: v) {
    std::cout << it << ' ';
  }
}
