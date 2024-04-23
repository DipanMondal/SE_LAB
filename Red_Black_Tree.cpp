#include<iostream>
using namespace std;

typedef struct node{
	int data;
	node *left;
	node *right;
	node *parent;
	int color;		
}node;	
class RedBlackTree{	
	private:
		node *root;
		int red,black;
		void print(node *n,int label,int side){
			if(n!=NULL){
				for(int i=0;i<label;i++)
					cout<<"  ";
				if(label!=0)
					cout<<"|__";
				if(side==1)
					cout<<"(right)";
				else if(side==-1)
					cout<<"(left)";
				cout<<n->data<<"("<<n->color<<")"<<endl;
				
				print(n->left,label+1,-1);
				print(n->right,label+1,1);
			}
		}
	public:
		RedBlackTree(){
			root = NULL;
			red = 1;
			black = 0;
		}
		
		node* search(int data){
			node *t = root;
			while(t!=NULL){
				if(t->data==data)
					return t;
				else{
					if(t->data < data)
						t = t->left;
					else
						t = t->right;
				} 
			}
		}
		
		void insert(int data){
			node *n = new node;
			n->data = data;
			n->left = n->right = n->parent = NULL;
			n->color = red;
			if(root == NULL){
				root = n;
				root->color = black;
			}
			else{
				node *x = NULL;
				node *y = root;
				while(y!=NULL){
					x = y;
					if(n->data < y->data)
						y = y->left;
					else
						y = y->right;
				}
				if(n->data < x->data){
					x->left = n;
					n->parent = x;
				}
				else{
					x->right = n;
					n->parent = x;
				}
				label :
				if(n==root)
					n->color = black;
				else if(x->color == red){
					node *gp = x->parent;
					node *u = NULL;
					int rotations;
					int uc;
					if(gp->left == x)
						u = gp->right;
					else
						u = gp->left;
					if(u==NULL)
						uc = black;
					else
						uc = u->color;
					
					//case 1
					if(uc==red){
						x->color = black;
						u->color = black;
						
						gp->color = red;
						n = gp;
						x = n->parent;
						goto label;
					}
					//case 2
					else{
						int c;
						if(gp->left == x){
							if(x->left==n)
								rotations=1; //ll
							else
								rotations=2; // lr
						}
						else{
							if(x->left==n)
								rotations=3; //rl
							else
								rotations=4; //rr
						}
						//rotations
						
						switch(rotations){
							case 1:	//ll
								if(gp==root){
									root = x;
									x->parent = NULL;
								}
								else{
									node *p = gp->parent;
									if(p->left == gp){
										p->left = x;
										x->parent = p;
									}
									else{
										p->right = x;
										x->parent = p;
									}
								}
								gp->parent = x;
								gp->left = x->right;
								if(x->right != NULL)
									x->right->parent = gp;
								x->right = gp;
								c=x->color;
								x->color = gp->color;
								gp->color = c;
								break;
								
							case 2:		//lr
								if(gp==root){
									root=n;
									n->parent = NULL;
								}
								else{
									node* p = gp->parent;
									if(p->left == gp){
										p->left = n;
										n->parent = p;
									}
									else{
										p->right = n;
										n->parent = p;
									}
								}
								gp->left = n->right;
								if(n->right!=NULL)
									n->right->parent = gp;
								n->right = gp;
								gp->parent = n;
								x->right = n->left;
								if(n->left!=NULL)
									n->left->parent = x;
								n->left = x;
								x->parent = n;
								c = n->color;
								n->color = gp->color;
								gp->color = c;
								break;
							case 3:	//rl
								if(gp==root){
									root=n;
									n->parent = NULL;
								}
								else{
									node *p = gp->parent;
									if(p->left==gp){
										p->left=n;
										n->parent = p;
									}
									else{
										p->right = n;
										n->parent = p;
									}
								}
								gp->right = n->left;
								if(n->left!=NULL)
									n->left->parent = gp;
								n->left = gp;
								gp->parent = n;
								x->left = n->right;
								if(n->right!=NULL)
									n->right->parent = x;
								n->right = x;
								x->parent = n;
								c = n->color;
								n->color = gp->color;
								gp->color = c;
								break;
							case 4: //rr
								if(gp==root){
									root = x;
									x->parent = NULL;
								}
								else{
									node *p = gp->parent;
									if(p->left == gp){
										p->left = x;
										x->parent = p;
									}
									else{
										p->right = x;
										x->parent = p;
									}
								}
								gp->parent = x;
								gp->right = x->left;
								if(x->left!=NULL)
									x->left->parent = gp;
								x->left = gp;
								c = x->color;
								x->color = gp->color;
								gp->color = c;
								break;
						}
					}
				}
			}
		}
		
		void display(){
			this->print(this->root,0,0);
			cout<<endl;
		}
};

int main(){
	RedBlackTree tree;
	tree.insert(5);
	tree.insert(4);
	tree.insert(2);
	tree.insert(6);
	tree.insert(1);
	tree.insert(-1);
	tree.insert(7);
	tree.insert(3);
	tree.insert(-3);
	tree.insert(-2 );
	tree.display();
	cout<<endl<<endl;
	tree.insert(0);
	tree.display();
	tree.insert(-5);
	tree.insert(-6);
	tree.display();
	return 0;
}
