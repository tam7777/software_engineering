#include <iostream>

using namespace std;

//return an integer that is an index number at the hash table
int bucketnum(int bucket,int size){
    int index;
    index=bucket%size;
    return index;
}

//an node with data key and next pointer and the index key
struct hash_node{
    int data, bucket;
    hash_node* next;
};

int main(){
    //the data set
    int arr[]={15,13,14,1,7,0,9,2,3};
    //size of the data
    int size=sizeof(arr)/sizeof(arr[0]);
    int delta=2;

    // create an linked list
    hash_node* hashtable[size]; 

    //intialize hash table(linked list)
    for(int i=0; i<size; i++){
        hashtable[i]= new hash_node;
        hashtable[i]->bucket=i;
        //negative num can't appear in this table so assigned -1 meaning no num.
        hashtable[i]->data=-1;
        hashtable[i]->next=NULL;
        //linked the nodes
        if (i!=0){
            hashtable[i-1]->next=hashtable[i];
        }
    }

    //the hash program
    for (int i=0; i<size; i++){
        //getting the index num
        int bucket=bucketnum(arr[i],size);
        //identify if the num is available in the hash. 1 means the data isn't assigned in the hash. 0 means that it is
        int flag=1;
        while(flag==1){
            //if we can't fing the data in the hash table 
            if (hashtable[bucket]->data==-1){
                //add the data into the designated area and thus flag becomes 0.
                hashtable[bucket]->data=arr[i];
                flag=0;
            }
            //if the bucket is already used then skip to the next index
            bucket+=delta;
            //if the index num exseeds the size then go back
            if (bucket>size){
                bucket-=size;
            }

        }

    }
    //print the hash table
    for(int i=0; i<size; i++){
        cout<<hashtable[i]->bucket<<" is "<<hashtable[i]->data<<endl;
    }

}
