#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//declared a list type
struct list {
struct list *next;
int data;
};

int main(){
    //create the first node
    struct list* h=new list;
    struct list* s=h;

//create a list 100 times
for (int i=0; i<=100; i++){
  //add number to list
    s->data=i;
  
  //add next node
    s->next=new list;
  
  //go to the next node. if the end of list next node is NULL
    if ((s->data)==100){
        s->next= NULL;
    }else {
        s=s->next;
    }
}

//intialise s
s=h;
  
// print the list
while ((s=s->next)!=NULL){
    cout<<s->data<<endl;
}


return(0);
}
