#include <iostream>
#include <cassert>

int binaryS(int arr[], int tar, int num ){
    int total, p;
    total=num;
    int low=0;
    while(true){
        p=total/2;
        if (tar<arr[p]){
            total=p;
        } else if (tar>arr[p]){
            total=total+p;
        } else if (tar==arr[p]){
            return p;
        }
    }
}

int main(int argc, char** argv) {
    //needs to have 2 input
    assert(3);
    //first input is the num of array
    int num= atoi(argv[1]);
    //second input is the number your looking for
    int target=atoi(argv[2]);
    int arr[num];
    //the input number has to be more than 1
    if (num<=2){
        exit(0);
    }
    //create an array 
    for (int i=0;i<num; i++){
        arr[i]=i;
    }
    //excute binary search

    std::cout<<"the number you're looking for is "<<binaryS(arr,target,num)<<"th number"<<std::endl;
    exit(0);
}
