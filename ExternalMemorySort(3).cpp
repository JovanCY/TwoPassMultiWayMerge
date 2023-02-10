/* This is skeleton code for the Two-Pass Multi-Way Merge Sort algorithm, you can make modifications as long as you meet 
   all question requirements*/  
// Currently, this program will just load the records into buffers (main memory) and print the records once the buffers 
// are filled. This process will continue until all of Emp.csv is read.

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <stdio.h>
using namespace std;

//defines how many buffers are available in the Main Memory 
#define buffer_size 22
int fileNum = 0;

fstream streams[buffer_size];

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
void Sort_in_Main_Memory(int num_elements){
    sort(buffers,buffers+num_elements,cmp_str);
    ofstream myFile;
    myFile.open("f" + to_string(fileNum));
    for(int i = 0; i < num_elements; i++) {
        myFile << buffers[i].eid << "," << buffers[i].ename << "," << buffers[i].age << "," << buffers[i].salary << endl;
    }
    myFile.close();
    
    fileNum++;
    return;
}

// Merges your M-1 runs (from disk) using the buffers in main memory and stores them in 
// a sorted file called 'EmpSorted.csv'(The Final Output File).
// You can change the return type and arguments as you see fit.


void Merge_Runs_in_Main_Memory(){
    int n;
    fstream myFile;  
    myFile.open("EmpSorted.csv", ios::out | ios::app);

    EmpRecord e;
    
    if(buffer_size < fileNum) {
        n = buffer_size;
    }
    else {
        n = fileNum;

    }
    int min = INT_MAX;
    int index = -1;
    bool openFiles = true;
    for(int i = 0; i < n; i++) {
        streams[i].open("f" + to_string(i));
        buffers[i] = Grab_Emp_Record(streams[i]);
    }
    
    while(openFiles) {
        int min = INT_MAX;
        openFiles = false;
        for(int i = 0; i < n; i++) {
            if(buffers[i].eid < min && buffers[i].eid != -1) {
                openFiles = true;
                min = buffers[i].eid;
                index = i;
            }
        }
        if(openFiles) {
            myFile << buffers[index].eid << "," << buffers[index].ename <<"," << buffers[index].age << "," << buffers[index].salary << endl;
            buffers[index] =  Grab_Emp_Record(streams[index]);
        } 
    }
    for(int i = 0; i < n; i++) {
        string s = "f"+to_string(i);
        remove(s.c_str());
    }
    fileNum -= n;

    
/*
    string s;
    for(int i = 0; i < n; i++) {
        streams[i] >> s;
        cout << s << endl;
    }
*/
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
          // The main_memory is not filled up but there are some leftover tuples that need to be sorted.
          cout << "memory not full but sorting" << endl;
          Sort_in_Main_Memory(cur_size);
          Print_Buffers(cur_size);
         
          
      }
      if(cur_size + 1 <= buffer_size){
          // Memory is not full. Store the current record into a buffer.
          
          buffers[cur_size] = single_EmpRecord ;
          cur_size += 1;
      }
      else{
          // Memory is now full. Sort the tuples in Main Memory and store them in a temporary file (runs)
          cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
    
          Sort_in_Main_Memory(buffer_size);
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


    cout << "here" << endl;
  while(!flag_sorting_done){

      Merge_Runs_in_Main_Memory();

      break;
  }
  
  // You can delete the temporary sorted files (runs) after you're done in order to keep things clean and tidy.

  return 0;
}
