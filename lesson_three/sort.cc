#include <iostream>
#include <bits/stdc++.h>
#include "common.h"
#include <chrono>
#include <vector>
#define RANGE (1000*1000*10)
using namespace std;

std::vector<int> Input;         
std::vector<int> OriginalInput; 

Measure measure;

void init(int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: ./program datasize" << std::endl;
    exit(0);
  }
  int size = atoi(argv[1]);

  for(int i = 0; i < size; i++) {
    OriginalInput.push_back(i);
  }
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(OriginalInput.begin(), OriginalInput.end(), engine);
}

void resetInput()
{
  Input.clear();
  for(int i = 0; i < (int)OriginalInput.size(); i++) {
    int value = OriginalInput[i]; 
    Input.push_back(value);
  }
}

void check_vec(std::vector<int>& array)
{
  int prev = array[0];
  for (int i = 1; i < (int)array.size(); i++) {
    int cur = array[i];
    if (prev > cur) {
      cout << "Sort failure: " << prev << ", " << cur << endl;
      exit(0);
    }
  }

  //display_vec(array);
}

void iCannotBelieve()
{
  std::vector<int>& array = Input;
  // begin
  resetInput();

  measure.start();
  for (int i = 0; i < (int)array.size(); i++) {
    printf("[%d  ] ",i);
    for (int i = 0; i < (int)array.size(); i++) {
      printf("%d ", array[i]);
    } printf("\n");
    for (int j = 0; j < (int)array.size(); j++) {    
      if (array[i] < array[j]) {
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        
        printf("[%d:%d] ",i,j);
        for (int k = 0; k < (int)array.size(); k++) {
          printf("%d ", array[k]);
        } printf("\n");

      }
    }
  }
  
  measure.end();
  check_vec(array);
  measure.print_elapsed_time("ICan'tBelieveItCanSort: ");
}

void insertSort()
{
  vector<int>& arr = Input;

  resetInput();

  int n=arr.size();

  measure.start();

  for (int j=1; j<n; j++){
    int key=arr[j];
    int i=j-1;
    while(i>=0 && arr[i]>key){
      arr[i+1]=arr[i];
      i=i-1;
    }
    arr[i+1]=key;
  }
  
  // end
  measure.end();
  check_vec(arr);
  measure.print_elapsed_time("Insert: ");
}

// A utility function to swap two elements
void swap(int* a, int* b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (std::vector<int>& arr, int low, int high)
{
  int x=arr[high];
  int i=low-1;
  for(int j=low; j<high; j++){
    if (arr[j]<=x){
      i=i+1;
      swap(arr[i],arr[j]);
    }
  }
  swap(arr[i+1], arr[high]);
  return i+1;
}

/* The main function that implements QuickSort
arr[] --> Input to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(std::vector<int>& arr, int low, int high)
{
  if (high>low){
    int q=partition(arr, low,high);
    quickSort(arr,low, q-1);
    quickSort(arr,q+1,high);
  }
}

void quickSortStart() {
  vector<int>& arr = Input;
  resetInput();
  measure.start();

  quickSort(arr, 0, arr.size() - 1);

  measure.end();
  check_vec(arr);
  measure.print_elapsed_time("Quick: ");
}

// The main function that sort
void countSort(int k)
{
  vector<int>& arr = Input;
  resetInput();
  int n=arr.size();
  int* output = new int[n];
  // Create a count array to store count of individual
  // number and initialize count array as 0
  //num of count is insanely huge defined RANGE
  int* count = new int[k];
  //set all num in count 0. 
  //memset(pointer(if you put +2 then 2 byte from the start), num, byte(how long write the num))
  //memset(count, 0, sizeof(int) * (RANGE+1));
  // largest num  
  
  measure.start();

  for(int i=0; i<k;i++){
    count[i]=0;
  }

  //input num of element i in count array
  for(int i=0;i<n;i++){
    count[arr[i]]+=1;
  }


  //add the previous count array number to complete array
  for(int i=1;i<=k;i++){
    count[i]=count[i]+count[i-1];
  }

  for(int i=0;i<n;i++){
    output[count[arr[i]]-1]=arr[i];//place the count num into the output
    count[arr[i]]=count[arr[i]]-1;//decrease the count num for i
  }

  for(int i=0;arr[i];i++){
    arr[i]=output[i];
  }

  measure.end();

  free(count);
  free(output);
  check_vec(arr);
  measure.print_elapsed_time("Count: ");
}

// A utility function to get maximum value in arr[]
int getMax(std::vector<int>& arr, int n)
{
  int mx = arr[0];
  for (int i = 1; i < n; i++)
  {
    if (arr[i] > mx)
      mx = arr[i];
  }
  return mx;
}


void defaultSort()
{
  vector<int>& arr = Input;
  resetInput();
  measure.start();
  std::sort(arr.begin(), arr.end());
  measure.end();
  check_vec(arr);
  measure.print_elapsed_time("std::sort: ");
}

// A utility function to print an array
void print(int arr[], int n)
{
  for (int i = 0; i < n; i++)
    cout << arr[i] << " ";
}

int main(int argc, char *argv[])
{
  init(argc, argv);

  insertSort();
  quickSortStart();  
  defaultSort();
  countSort(atoi(argv[1]));

  return 0;
}
