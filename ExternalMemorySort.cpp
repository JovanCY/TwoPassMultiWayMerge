/* This is skeleton code for the Two-Pass Multi-Way Merge Sort algorithm, you can make modifications as long as you meet 
   all question requirements*/  
// Currently, this program will just load the records into buffers (main memory) and print the records once the buffers 
// are filled. This process will continue until all of Emp.csv is read.

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

//defines how many buffers are available in the Main Memory 
#define buffer_size 22
int fileNum = 0;

struct EmpRecord {
    int eid;
    string ename;
    int age;
    double salary;
};
EmpRecord buffers[buffer_size]; // this structure contains 22 buffers; available as your main memory.

// Grab a single block from the Emp.csv file. In theory, if a block was larger than
// one tuple, this function would return an array of EmpRecords (one block worth) 
// and the entire if-then-else statement would be wrapped in a loop that would read
// tuples in until the block size was hit.
EmpRecord Grab_Emp_Record(fstream &empin) {
    string line, word;
    EmpRecord emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Print whatever is stored in the buffers of Main Memory 
// Can come in handy to see if you've sorted the records in your main memory properly.
void Print_Buffers(int cur_size) {
    for (int i = 0; i < cur_size; i++)
    {
        cout << i << " " << buffers[i].eid << " " << buffers[i].ename << " " << buffers[i].age << " " << buffers[i].salary << endl;
    }
}

// Sort the buffers in main memory based on 'eid' and then store those sorted records in
// a temporary file on disk (create a run and store it as a file on disk).
// You can change the return type and arguments as you see fit. 
bool cmp_str(EmpRecord e1, EmpRecord e2) {
    return e2.eid>e1.eid; 
}


void Sort_in_Main_Memory(){
    sort(buffers,buffers+buffer_size,cmp_str);
    ofstream myFile;
    myFile.open("f" + to_string(fileNum)+ ".csv");
    for(int i = 0; i < buffer_size; i++) {
        myFile << buffers[i].eid << "," << buffers[i].ename << "," << buffers[i].age << "," << buffers[i].salary << endl;
    }
    fileNum++;
    return;
}

// Merges your M-1 runs (from disk) using the buffers in main memory and stores them in 
// a sorted file called 'EmpSorted.csv'(The Final Output File).
// You can change the return type and arguments as you see fit.

//returns index of entry in buffer with lowest eid
int lowestEid(EmpRecord buffer[], int size){
    int result = -1;
    int lowestNum = 9999999;
    for (int i = 0; i<size; i++){
        if (buffer[i].eid<lowestNum){
            lowestNum = buffer[i].eid;
            result = i;
        }
    }
    return result;
}

void Merge_Runs_in_Main_Memory(){
    int mergeNum = fileNum % buffer_size;
    fstream outputFile;
    fstream fileStreams[mergeNum];
    string s;
    int prev = -1;
    EmpRecord tempBuffers[buffer_size];

    outputFile.open("EmpSorted.csv", ios::app);

    for (int i = 0; i< mergeNum; i++){
        string fileName = "f" + to_string(i)+ ".csv";
        fileStreams[i].open(fileName);
        tempBuffers[i] = Grab_Emp_Record(fileStreams[i]);
    }

    bool flag = true;
    while (flag){
        int lowest = lowestEid(tempBuffers, mergeNum);
        outputFile << tempBuffers[lowest].eid << "," << tempBuffers[lowest].ename << "," << tempBuffers[lowest].age << "," << tempBuffers[lowest].salary << endl;
       
        EmpRecord temp = Grab_Emp_Record(fileStreams[lowest]);
        if (temp.eid != -1){
            tempBuffers[lowest] = temp;
        }else{
            tempBuffers[lowest].eid = 99999;
        }  
    
    }
    //add it to array if it isn't eof, if eof then close stream (?)

    //tidy up the code
    for (int i = 0; i< mergeNum; i++){
        string strFName = "f" + to_string(i)+ ".csv";
        const char* char_array = strFName.c_str();
        fileStreams[i].close();
        int status = remove(char_array);
    }

}

int main() {
  // open file streams to read and write
  fstream input_file;
  input_file.open("Emp.csv", ios::in);
 
  // flags to check when relations are done being read
  bool flag = true;
  int cur_size = 0;
  
  /*First Pass: The following loop will read each block, put it into main_memory,
    sort them, and then will put write them to a temporary file (as a run) for 2nd pass */
  while (flag) {
      // FOR BLOCK IN RELATION EMP

      // grabs a block
      EmpRecord  single_EmpRecord  = Grab_Emp_Record(input_file);
      // checks if filestream is empty
      if (single_EmpRecord.eid == -1) {
          flag = false;
          Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover tuples that need to be sorted.
      }
      if(cur_size + 1 <= buffer_size){
          // Memory is not full. Store the current record into a buffer.
          buffers[cur_size] = single_EmpRecord ;
          cur_size += 1;
      }
      else{
          // Memory is now full. Sort the tuples in Main Memory and store them in a temporary file (runs)
          cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
          cout << "here" << buffers[0].age << endl;
         

          Sort_in_Main_Memory();
          Print_Buffers(buffer_size);
          
          // After sorting, start again. Clear memory and put the current tuple into main memory.
          cur_size = 0;
          buffers[cur_size] = single_EmpRecord;
          cur_size += 1;
      }
      
  }
  input_file.close();
  
  /* Implement 2nd Pass: Read the temporary sorted files (runs) and sort them as discussed in class. 
	 Write the final output (fully sorted Emp relation) to EmpSorted.csv*/

  // Uncomment when you are ready to store the sorted relation
  // fstream sorted_file;  
  // sorted_file.open("EmpSorted.csv", ios::out | ios::app);

  // Pseudocode
  bool flag_sorting_done = false;



  while(!flag_sorting_done){
      Merge_Runs_in_Main_Memory();

      break;
  }
  
  // You can delete the temporary sorted files (runs) after you're done in order to keep things clean and tidy.


  return 0;
}
