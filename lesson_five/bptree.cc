#include "bptree.h"
#include <vector>
#include <sys/time.h>

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


NODE *
insert_in_parent(NODE *leaf,int key, NODE *fleaf)
{   
	int i;
	if(leaf==Root){//if leaf is the root, leaf split
		NODE *root;//create new root candidate
		root->key[0]=key;//root shold contain N,K,N
		root->chi[0]=leaf;
		root->chi[1]=fleaf;
		root->nkey++;
		Root =root;//make root the Root and return 
		return root;
	}
	NODE *p;
	p=leaf->parent;//let p=parent(N)
	if(p->chi[N-1]==NULL){//if p has less then n pointers, if p has the space to put key&ptr
		for(int i=0; i<p->nkey; i++){// insert (K', N') to p
			if(p->chi[i]==leaf) break;
		}
		p->chi[i+1]=fleaf;
		p->key[i]=key;
		p->nkey++;
	}else{
		TEMP *mem;
		for (i=0; i<p->nkey; i++){
			mem->key[i]=p->key[i];
			mem->chi[i]=p->chi[i];
			mem->nkey++;
		}//copy p to a block of memory capable of P and (K&N)

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

		NODE *fp;
		for(i=0; i<(mem->nkey+1)/2; i++){//copt TP[n+1/2]to T
			p->key[i]=mem->key[i];
			p->chi[i]=mem->chi[i];
			p->nkey++;
		}

		int ffkey=mem->key[(mem->nkey+1)/2];//Let K = T.K(n+1)/2

		for(i=((mem->nkey+1)/2)+1; i<mem->nkey+1; i++){//copt TP[n+1/2]to T
			fp->key[i]=mem->key[i];
			fp->chi[i]=mem->chi[i];
			fp->nkey++;
		}	
		insert_in_parent(p,ffkey,fp);
	}
	return 0;
}

void 
insert(int key, DATA *data)
{
	NODE *leaf;//create a node called leaf

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
		fleaf=alloc_leaf(NULL);

		TEMP* t;//create a temporary memory that can hold n pairs
		for(int i=0; i<leaf->nkey; i++){
			t->key[i]=leaf->key[i];
			t->chi[i]=leaf->chi[i];
		}
		t->nkey=leaf->nkey+1;//copy leaf to t

		insert_in_t(t,key,data);//insert new ket & ptr to t
		fleaf->chi[N-1]=leaf->chi[N-1];//set L'Pn=LPn
		leaf->chi[N-1]=fleaf;//set LPn=L'

		for(int i=0; i<leaf->nkey; i++){//erase L the key and ptr
			leaf->key[i]=0;
			leaf->chi[i]=NULL;
		}
		leaf->nkey=0;

		for(int i=0; i<(t->nkey)/2; i++){//copy TP1 to TK[n/2] to L
			leaf->key[i]=t->key[i];
			leaf->chi[i]=t->chi[i];
			leaf->nkey++;
		}

		for(int i=(t->nkey)/2; i<t->nkey; i++){//copy TP[n/2] to TKn to L'
			fleaf->key[i]=t->key[i];
			fleaf->chi[i]=t->chi[i];
			leaf->nkey++;
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
	while (true) {
			insert(interactive(), NULL);
		print_tree(Root);
	}
	end = cur_time();

	return 0;
}

//when inserting a num to t, using insert_in_leaf, can not do it coz
//cannot 'TEMP'to 'NODE' so created a insert function just for TEMP
