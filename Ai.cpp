#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
struct TreeNode {
	string val;         	// The data in this node.
	TreeNode *left; 	  // Pointer to the left subtree.
	TreeNode *right; 	 // Pointer to the right subtree.
	TreeNode *up; 		 // Pointer to the up subtree.
};

int findiff(string v[],int size);
int findimp(string v[],int size);
int findsymbolfront(string v[],int beg,int n);//(beg,n)
int findsymbolback(string v[],int n,int end);//(n,end)
int findfrontpar(string v[],int n,int end);
int findbackpar(string v[],int n,int end);
TreeNode* buildtree(string v[],int size);
TreeNode* insertnode(string v[],TreeNode* node,int n,int count,int flag);//(n,count) flag=0->left,flag1->right
TreeNode* addtree(TreeNode* up,string question[],int start,int symbol,int count);//(start,count]
void Preorder(TreeNode* node);
void Inorder(TreeNode* node);
void Postorder(TreeNode* node);

int main (int argc , char* argv[])
{
	char comment[100] ;
	vector<string> v;
	vector<string>::iterator it_i;
	TreeNode* treeroot;		
	if (argc < 2)
	{
		cout << "no file\n";
	}
	else
	{
		FILE *file = fopen( argv[1], "r" );
		int len = 0;
		while  (fgets(comment ,100 , file) != NULL )
		{
			//char t =" ";
			char *test = strtok(comment, " ");
			while (test != NULL) 
			{
				v.push_back(test);
				test = strtok(NULL, " ");
			}
			//v.push_back(comment);

			//cout << x;
			//len++;
			//cout << i;
		}
		int count=0;
		for ( it_i = v.begin() ; it_i != v.end() ; it_i++ )
		{	
			count++;
		}
		//change vector to string
		string question[count];
		int i;
		for ( i=0,it_i = v.begin(); it_i != v.end(),i<count ; it_i++,i++ )
		{	
			question[i]=*it_i;
		}

		//build tree
		int iff=findiff(question,count);
		int imp=findimp(question,count);
		if(iff!=-1)
		{
			treeroot=buildtree(question, iff);
			addtree(treeroot,question,0,iff,count-1);
		}
		else if(imp!=-1)
		{
			treeroot=buildtree(question, imp);
			addtree(treeroot,question,0,imp,count-1);
		}
		else
		{
			int front=findsymbolfront(question,0,count-1);
			treeroot=buildtree(question,front);
			addtree(treeroot,question,0,front,count-1);
		}
		Preorder(treeroot); cout<<endl;
		Inorder(treeroot);	cout<<endl;
		Postorder(treeroot); cout<<endl;
		fclose( file );

		//for (int i = 0 ; i < len ; i++ )
		//	cout << comment[i];
	}
}

TreeNode* addtree(TreeNode* up,string question[],int start,int sym,int count)//(start,count)
{
	int front,back;
	TreeNode* tmp;
	int parf,parb;
	parf=findfrontpar(question,start,sym-1);
//	cout<<"start="<<start<<", sym="<<sym<<", count"<<count<<endl;
	if(parf==-1)// no ( before sym
	{
		front=findsymbolfront(question,start,sym-1);
//		cout<<"parf==-1 ,front="<<front<<endl;
		if(front==-1)//no symbol before sym
		{
			if((question[sym-2]!="neg")&&(question[sym-2]!="not"))
				insertnode(question,up,sym-1,count,0);	
			else
			{
				tmp=insertnode(question,up,sym-2,count,0);	
				insertnode(question,tmp,sym-1,count,1);	
			}
		}
		else
		{
			tmp=insertnode(question,up,front,count,0);	
			addtree(tmp,question,start,front,sym-1);	
		}
	}
	else
	{
		TreeNode* temp2;
//		cout<<"parf "<<question[parf]<<endl;
		parb=findbackpar(question,parf+1,count-1);
		if(parb!=-1)//eg. (A sym2....) sym ...
		{
//			cout<<"parf 1"<<endl;
			back=findsymbolback(question,parf+1,parb-1);
			tmp=insertnode(question,up,back,count,0);	
			addtree(tmp,question,parf,back,parb);	
		}
		else//eg. 1.(A sym ...     2.( neg A sym....
		{
//			cout<<"parf 2"<<endl;
			if((question[sym-2]!="neg")&&(question[sym-2]!="not"))
				insertnode(question,up,sym-1,count,0);	
			else
			{
				tmp=insertnode(question,up,sym-2,count,0);	
				insertnode(question,tmp,sym-1,count,1);	
			}
		}
	}
	back=findsymbolback(question,sym+1,count-1);
//		cout<<"back="<<back<<endl;
	if(back==-1)//no symbol after sym
	{
		if((question[sym+1]!="neg")&&(question[sym+1]!="not"))
			insertnode(question,up,sym+1,count,1);	
		else
		{
			tmp=insertnode(question,up,sym+1,count,1);	
			insertnode(question,tmp,sym+2,count,1);	
		}
	}
	else//sym+1~count has a symbol
	{
		tmp=insertnode(question,up,back,count,1);	
		addtree(tmp,question,sym+1,back,count);	
	}

}

int findiff(string v[],int size)
{
	int ans=-1;
	for(int i=0;i<size;i++)
	{
		if(v[i].compare("iff")==0)
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}


int findimp(string v[],int size)
{
	int ans=-1;
	for(int i=0;i<size;i++)
	{
		if(v[i].compare("imp")==0)
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}


int findsymbolfront(string v[],int beg,int n)//(beg,n)
{
	int ans=-1;
	for(int i=n;i>=beg;i--)
	{
		if(v[i].compare("and")==0 || v[i].compare("or")==0)
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}	

int findsymbolback(string v[],int n,int end)//(n,end)
{
	int ans=-1;
	for(int i=n;i<=end;i++)
	{
		if(v[i].compare("and")==0 || v[i].compare("or")==0)
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}

int findfrontpar(string v[],int n,int end)
{
	int ans=-1;
	for(int i=n;i<=end;i++)
	{
		if(v[i][0]=='(')
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}

int findbackpar(string v[],int n,int end)
{
	int ans=-1;
	for(int i=n;i<=end;i++)
	{
		if(v[i][1]==')')
		{
			ans=i;
			return ans;
		}
	}
	return ans;
}

TreeNode* buildtree(string v[],int size)
{
	TreeNode *tmp;
	tmp=(TreeNode *)malloc(sizeof(TreeNode));
	tmp->left=tmp->right=tmp->up=NULL;
	tmp->val=v[size];
	return tmp;
}


TreeNode* insertnode(string v[],TreeNode* node,int n,int count,int flag)//(n,count) flag=0->left,flag1->right
{
	if(n>count)
		return NULL;
	TreeNode *tmp;
	tmp=(TreeNode *)malloc(sizeof(TreeNode));
	tmp->left=tmp->right=NULL;
	tmp->up=node;
	if(v[n][0]=='(')
		tmp->val=v[n][1];
	else if(v[n][1]==')')
		tmp->val=v[n][0];
	else
	{
		tmp->val=v[n];
	}
	if(flag==0){
		node->left=tmp;
	}
	else{
		node->right=tmp;
	}
//		cout<<"11"<<node->val<<"\n";
	return tmp;
}
void Preorder(TreeNode* node)
{
	if ( node )
	{
		cout << node->val << " ";
		Preorder(node->left);
		Preorder(node->right);
	}
}
void Inorder(TreeNode* node)
{
	if ( node )
	{
//		cout<<"(";
		Inorder(node->left);
		cout << node->val << " ";
		Inorder(node->right);
//		cout<<")";
	}
}
void Postorder(TreeNode* node)
{
	if ( node )
	{
		Postorder(node->left);
		Postorder(node->right);
		cout << node->val << " ";
	}
}
