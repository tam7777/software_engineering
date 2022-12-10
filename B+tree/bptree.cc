#include "bptree.h"
#include <vector>
#include <sys/time.h>
using namespace std;

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

void
print_tree_core(NODE *n)
{
	printf("["); 
	for (int i = 0; i < n->nkey; i++) {
		if (!n->isLeaf) print_tree_core(n->chi[i]); 
		printf("%d", n->key[i]); 
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
find_leaf(NODE *node, int key)
{//find the node which has key in it
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{	
	int i;
	if (key < leaf->key[0]) {//if the input data is smallest 
		for (i = leaf->nkey; i > 0; i--) {
			leaf->chi[i] = leaf->chi[i-1] ;
			leaf->key[i] = leaf->key[i-1] ; 
		} //nkey is bigger than the position of the key[45,NULL,NULL] nkey=1
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;

	}
	else {
		for (i = 0; i < leaf->nkey; i++) {
			if (key < leaf->key[i]) break;
		}//the position of the input should be bigger than location i
		for (int j = leaf->nkey; j > i; j--) {//j is just bigger than the input		
			leaf->chi[j] = leaf->chi[j-1] ;
			leaf->key[j] = leaf->key[j-1] ;
		} 
		leaf->key[i]=key;
		leaf->chi[i]=(NODE *)data;

	}
	leaf->nkey++;

	return leaf;
}

TEMP *
insert_in_t(TEMP *leaf, int key, DATA *data)
{	
	int i;
	if (key < leaf->key[0]) {//if the input data is smallest 
		for (i = leaf->nkey; i > 0; i--) {
			leaf->chi[i] = leaf->chi[i-1] ;
			leaf->key[i] = leaf->key[i-1] ; 
		} //nkey is bigger than the position of the key[45,NULL,NULL] nkey=1
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else {
		for (i = 0; i < leaf->nkey; i++) {
			if (key < leaf->key[i]) break;
		}//the position of the input should be bigger than location i
		for (int j = leaf->nkey; j > i; j--) {//j is just bigger than the input		
			leaf->chi[j] = leaf->chi[j-1] ;
			leaf->key[j] = leaf->key[j-1] ;
		} 
		leaf->key[i]=key;
		leaf->chi[i]=(NODE *)data;

	}
	leaf->nkey++;

	return leaf;
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

TEMP *
alloc_temp()
{
	TEMP *temp;
	if (!(temp = (TEMP *)calloc(1, sizeof(TEMP)))) ERR;
	temp->nkey = 0;

	return temp;
}

NODE *
alloc_root(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = false;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
insert_in_parent(NODE *leaf,int key, NODE *fleaf)
{   
	int i;
	if(leaf==Root){//if leaf is the root	
		NODE *root;//create new root candidate
		root=alloc_root(NULL);
		root->key[0]=key;//root 1st key should be the first of fleaf
		root->chi[0]=(NODE *) leaf;//connect pointers to the 2 lists
		root->chi[1]=(NODE *) fleaf;
		leaf->parent=(NODE *) root; //set as a parent
		fleaf->parent=(NODE *) root;
		root->nkey++;
		Root =(NODE *)root;//make root the Root and return 
		return root;
	}

	//if leaf is not the root aka not the first parent node

	NODE *fp;
	fp=(NODE *)leaf->parent;//set p as the parent of the leaf
	//couldn't fp->nkey++

	NODE *p;
	p=alloc_root(NULL);//create a new parent candidate copying fp
	for (i=0; i<(fp->nkey);i++){//careful to add +1 coz num of chi is 1more than key
		p->key[i]=fp->key[i];
		p->chi[i]=fp->chi[i];
	}

	p->chi[i]=fp->chi[i];
	p->nkey=fp->nkey;
	Root=(NODE *)p;

	if(p->nkey<(N-1)){//if p has less then n pointers, if p has the space to put key&ptr

		// insert (K', N') to p. this part could be cleaner
		if (key > p->key[p->nkey-1]) {//if the input data is biggest

			for(i=0; i<p->nkey; i++){// insert (K', N') to p
				if(leaf->key[0]==p->key[i]) break;
			}

			p->chi[i+2]=(NODE *) fleaf;
			p->key[i+1]=key;

		}

		else{//if the input data is not the biggest
			for(i=0; i<p->nkey; i++){
				if(key<p->key[i]) break;
			}

			int j;
			for (j = p->nkey; j > i; j--) {//j is just bigger than the input		
				p->chi[j+1] = p->chi[j] ;
				p->key[j] = p->key[j-1] ;
			}		
			p->chi[i+1]=(NODE *) fleaf;
			p->key[i]=key;
		}
		p->nkey++;
		leaf->parent=p;
		fleaf->parent=p;//the parent of leaf and fleaf is still fp so change the parent
		//however the parent of the first ptr is still fp

	}
	
	else{//if parent node is full
		TEMP *mem;
		mem=alloc_temp();
		for (i=0; i<p->nkey; i++){
			mem->key[i]=p->key[i];
			mem->chi[i]=p->chi[i];
			mem->nkey++;
		}//copy p to a block of memory capable of P and (K&N)
		mem->chi[i]=p->chi[i];

		for(i=0; i<mem->nkey; i++){//insert key and fleaf into mem
			if(mem->chi[i]==leaf) break;
		}
		mem->chi[i+1]=fleaf;
		mem->key[i]=key;
		mem->nkey++;

		for(i=0; i<p->nkey; i++){//erase p the key and ptr
			p->key[i]=0;
			p->chi[i]=NULL;
		}
		p->nkey=0;

		NODE *pd;
		pd=alloc_root(NULL);
		for(i=0; i<(mem->nkey+1)/2; i++){//copt TP[n+1/2]to T
			p->key[i]=mem->key[i];
			p->chi[i]=mem->chi[i];
			p->nkey++;
		}
		p->chi[i]=mem->chi[i];

		int ffkey=mem->key[(mem->nkey+1)/2];//Let K = T.K(n+1)/2

		int k=0;
		for(i=((mem->nkey+1)/2)+1; i<mem->nkey; i++){//copt TP[n+1/2]to T
			pd->key[k]=mem->key[i];
			pd->chi[k]=mem->chi[i];
			pd->nkey++;
			k++;
		}
		pd->chi[k]=mem->chi[i];

		insert_in_parent(p,ffkey,pd);
		
	}
	
	return 0;
}

void 
insert(int key, DATA *data)
{
	NODE *leaf;//define a node called leaf
	if (Root == NULL) {//if this is the first node
		leaf = alloc_leaf(NULL);//parent node is NULL but is leaf and nkey=0
		Root = leaf;//this leaf is the first node
	}
	else {
		leaf = find_leaf(Root, key);//find the appropriate leaf node
	}//find the node which is close to the key
	//leaf is the node which the key(input data) should be in

	if (leaf->nkey < (N-1)) {//if the node is not full 
		insert_in_leaf(leaf, key, data);//insert
	}
	else {//if the node is full 

		NODE *fleaf;//create node L'
		fleaf=alloc_leaf(leaf->parent);
		
		TEMP* temp;//create a temporary memory that can hold n pairs
		temp=alloc_temp();
		
		for(int i=0; i<leaf->nkey; i++){
			temp->key[i]=leaf->key[i];
			temp->chi[i]=leaf->chi[i];
			temp->nkey++;
		}	
		
		insert_in_t(temp,key,data);//insert new ket & ptr to temp

		fleaf->chi[N]=leaf->chi[N];//end of fleaf is the same as leaf rn
		leaf->chi[N]=(NODE *) fleaf;//connect the end of leaf to fleaf

		for(int i=0; i<leaf->nkey; i++){//delete key and pointers and nkey of leaf
			leaf->key[i]=0;
			leaf->chi[i]=NULL;
		}
		leaf->nkey=0;

		for(int i=0; i<(temp->nkey)/2; i++){//copy half temp to leaf
			leaf->key[i]=temp->key[i];
			leaf->chi[i]=temp->chi[i];
			leaf->nkey++;
		}

		int j=0;

		for(int i=(temp->nkey)/2; i<temp->nkey; i++){//copy last half to fleaf
			fleaf->key[j]=temp->key[i];
			fleaf->chi[j]=temp->chi[i];
			j++;
			fleaf->nkey++;
		}
		int fkey=fleaf->key[0];

		insert_in_parent(leaf,fkey,fleaf);
		
	}
}

void
init_root(void)
{
	Root = NULL;
}

int 
interactive()
{
  int key;

  std::cout << "Key: ";
  std::cin >> key;

  return key;
}

int
main(int argc, char *argv[])
{
  struct timeval begin, end;

	init_root();

	printf("-----Insert-----\n");
	begin = cur_time();
    /*
	while (true) {
			insert(interactive(), NULL);
		print_tree(Root);
	}*/

    for (int i=1; i<15; i++) {
			insert(i, NULL);
            //Place NULL to data, key and data
		print_tree(Root);
	}

	end = cur_time();

	return 0;
}
