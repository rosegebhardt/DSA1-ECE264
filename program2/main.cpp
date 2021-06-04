// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2021

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData:l) {
    output << pData->lastName << " " 
	   << pData->firstName << " " 
	   << pData->ssn << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

// Initialize arrays for sorting algorithms
Data *arrayData[1100000];
Data *L[1100000];
string arraySSN[1100000];

// Determine test type to choose sorting algorithm: considers size of the list, whether 
// the first 100 last names are the same and whether the first 100 first names are the same
int determineCase(list<Data *> &l)
{
  list<string> FN;
  list<string> LN;

  int N = 100;
  int index = 0;

  for (list<Data *>::iterator it = l.begin(); index < N; it++)
  {
    FN.push_front((*it)->firstName);
    LN.push_front((*it)->lastName);
    index++;
  }

  if (l.size() < 150000)
  {
    return 1;
  }
  else
  {
    if (count(LN.begin(), LN.end(), LN.front()) == LN.size())
    {
      if (count(FN.begin(), FN.end(), FN.front())== FN.size())
      {
        return 4;
      }
      else
      {
        return 3;
      }
    }
    else
    {
      return 2;
    }
  }
}

// Defines comparitor for instances of Data class
bool compareData(Data* a, Data* b)
{
  if ((a->lastName) != (b->lastName))
  {
    return ((a->lastName) < (b->lastName));
  }
  else if ((a->firstName) != (b->firstName))
  {
    return ((a->firstName) < (b->firstName));
  }
  else
  {
    return ((a->ssn) < (b->ssn));
  }
}

// // Defines comparitor for SSNs 
// bool compareSSN(Data* a, Data* b)
// {
//   return ((a->ssn) < (b->ssn));
// }

// Insertion sort for nearly sorted list (case 3)
void insertionSort(list<Data *> &l)
{
  int i, j;
  int N = l.size();
  Data key;

  L[0] = *l.begin();
  auto it = next(l.begin());

  for (i = 1; i < N; i++)
  {
    auto key = (*it++);
    j = i - 1;

    while (j >= 0 && compareData(key,L[j]))
    {
        L[j + 1] = L[j];
        j = j - 1;
    }
    
    L[j + 1] = key;
  }

  it = l.begin();
  for (int i = 0; i < N; i++)
  {
    (*it++) = L[i];
  }
}

// Copies data to array, uses std::sort, copies sorted array back to list
void dataSort(list<Data *> &l)
{

  int index = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
  {
    arrayData[index] = (*it);
    index++;
  }

  sort(arrayData,arrayData+l.size(),compareData);
  
  index = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
  {
    (*it) = arrayData[index];
    index++;
  }

}

// Copies SSNs to array, uses std::sort, copies sorted SSNs back to list
void ssnSort(list<Data *> &l)
{

  int index = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
  {
    arraySSN[index] = (*it)->ssn;
    index++;
  }

  sort(arraySSN,arraySSN+l.size());
  
  index = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
  {
    (*it)->ssn = arraySSN[index];
    index++;
  }

}

// Sorts list based on which type of input is given
void sortDataList(list<Data *> &l) 
{
  int whichCase = determineCase(l);

  if (whichCase == 1)
  {
    dataSort(l);
    // l.sort ~ 0.133
    // dataSort ~ 0.116
  }
  if (whichCase == 2)
  {
    dataSort(l);
    // l.sort ~ 2.6
    // dataSort ~ 2.0
  }
  if (whichCase == 3)
  {
    insertionSort(l);
    // l.sort ~ 1.0
    // insertionSort ~ 0.1
  }
  if (whichCase == 4)
  {
    ssnSort(l);
    // compareData ~ 3.0 
    // compareSSN ~ 1.6
    // ssnSort ~ 0.56
  }
}