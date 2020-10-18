//Desktop\padhai ka saman\IR\prog ass\1
#include <bits/stdc++.h>
using namespace std;

typedef struct node
{
	string s;			//stores string word
	int f;				//stores frequency
	int ht;				//stores the level at which the node is stored
	struct  node *left,*right;
}node;

//helper function to print vector
void printV(vector <pair<string,int>> const &a) {
   cout << "The vector elements are : "<<endl;

   for(int i=0; i < a.size(); i++)
   cout << a[i].first<<"-"<<a[i].second << endl;
}
//helper funtion to print optimal BST
void printOBST(node *start){
	//cout<<"hello p"<<endl;
	if(start==NULL){
		cout<<"Error at 34"<<endl;
		return;
	}
	cout<<"The tree is:"<<endl;
	queue<pair<node*,string>>q;
	node*temp=NULL;
	int ht=start->ht;
	q.push(pair<node*,string>(start,"Root"));
	while(!q.empty()){
		temp=q.front().first;
		if(temp!=NULL){
			q.push(pair<node*,string>(temp->left,"L of "+temp->s));
			q.push(pair<node*,string>(temp->right,"R of "+temp->s));
			if(temp->ht==ht){
				cout<<temp->s<<" - at ht "<<temp->ht<<" => "<<q.front().second<<"|\t";
			}
			else{
				cout<<endl;
				ht=temp->ht;
				cout<<temp->s<<" - at ht "<<temp->ht<<" => "<<q.front().second<<"|\t";
			}
		}
		
		q.pop();
	}
	//cout<<"hello p end"<<endl;
	cout<<endl;
}

//helper function to check if the word exist in vector and populate the vector
void populate(vector<pair<string,int>> &v,string s){
	bool t=false;
	vector<pair<string, int>>::iterator it;
	for(it = v.begin(); it!=v.end() && t==false; it++ ){ 
		if(it->first == s ){ 
			it->second++; 
			t=true;
		}
	}
	if(t==false){
		v.push_back(pair<string,int>(s,1));
	}
}
//helper function to sum the freq
int sumF(vector <pair<string,int>> const &a, int i, int j)  
{  
    int s = 0;  
    for (int k = i; k <= j; k++)  
    s += a[k].second;  
    return s;  
}
//this function populates the cost matrix named dp and root matrix containing key to be put at nodes
void makeMat(vector<pair<string,int>> &v,int ** dp, int ** root/* vector<vector<int>>& dp, vector<vector<int>>& root*/){
	int n=v.size();
	//int dp[n][n],root[n][n];
	// For a single key, cost is equal to frequency of the key
	for (int i = 0; i < n; i++){
		dp[i][i] = v[i].second;
		//as these make up for the leaf nodes;
		root[i][i]=i;
	} 
	 // Now we need to consider chains of length 2, 3, ... .  
    // L is chain length.  
    for (int L = 2; L <= n; L++)  
    {  
        // i is row number in cost[][]  
        for (int i = 0; i < n-L+1; i++)  
        {  
            // Get column number j from row number i and  
            // chain length L  
            int j = i+L-1;  
            dp[i][j] = INT_MAX;  
  
            // Try making all keys in interval keys[i..j] as root  
            for (int r = i; r <= j; r++)  
            {  
	            // c = cost when keys[r] becomes root of this subtree 
	            //if r is made a root/node there then cost[r]=cose[left subtree]+cost[right subtree]+frequencies
	            int c = ((r > i)? dp[i][r-1]:0) +  
	                    ((r < j)? dp[r+1][j]:0) +  
                    	sumF(v, i, j);  
	            if (c < dp[i][j]) {
	            	dp[i][j] = c; 
	                root[i][j]=r;
	            }
                 
            }  
        }  
    } 
    // cout<<"the root matrix is: "<<endl;
    // for(int i=0;i<n;i++){
    // 	for(int j=0;j<n;j++){
    // 		cout<<root[i][j]<<"\t";
    // 	}
    // 	cout<<endl;
    // }
    // cout<<endl;
    cout<<"The cost of optimal BST is: "<<dp[0][n-1]<<endl;
    
}
//this helper function make the optimal bst and returns ptr to the root of the same
node* makeOptBST(vector<pair<string,int>> &v,int ** dp, int ** root,int i,int j,int height){
	//cout<<"hello1"<<endl;
	node* ret=NULL;
	//cout<<"hello2"<<endl;
	if(i<=j){
		ret=new node;//(node*)malloc(sizeof(node));
		if(ret==NULL){
			cout<<"Error at line 131"<<endl;
		}
		else{
			if(i==j){
				//cout<<"hello3"<<endl;
				//leaf node
				ret->s=v[i].first;
				ret->f=v[i].second;
				ret->left=ret->right=NULL;
				ret->ht=height;
			}
			else{
				//cout<<"hello4"<<endl;
				int r=root[i][j];
				ret->s=v[r].first;
				ret->f=v[r].second;
				ret->ht=height;
				//cout<<"hello5"<<endl;
				ret->left=makeOptBST(v,dp,root,i,r-1,height+1);
				//cout<<"hello6"<<endl;
				ret->right=makeOptBST(v,dp,root,r+1,j,height+1);
				//cout<<"hello7"<<endl;
			}
		}
	}
	return ret;
}
//function to run search on optimal BST
void searchOBST(node* start,string sW){
	node* temp=start;
	if(temp!=NULL){
		if(temp->s==sW){
			cout<<sW<<" found at depth "<<temp->ht<<" having frequency "<<temp->f<<endl;
		}
		else if(temp->s>sW){
			searchOBST(temp->left,sW);
		}
		else if(temp->s<sW){
			searchOBST(temp->right,sW);
		}
		else{
			cout<<"Error at 172"<<endl;
		}
	}
	else{
		cout<<"Error 404: word not found :("<<endl;
		cout<<sW<<" not found in the given document"<<endl;
	}
}


int main(){
	//stores frequency
	//string stores the word, int stores the frequency, index will be used to calculate root.
	vector<pair<string,int>>freq;
    vector<pair<string, int>>::iterator it;

	//file operations
	fstream file;
	string filename = "doc.txt",word;
	file.open(filename.c_str());
	while (file >> word) 
    { 
        //adding frequency
        transform(word.begin(), word.end(), word.begin(), ::tolower);  
        populate(freq,word);
    } 
    file.close();
    sort(freq.begin(), freq.end());
    //prints frequency vector
	printV(freq);
	cout<<"----------------------------------------------------------------"<<endl;
    //vector<vector<int>> dp(freq.size(),vector<int> (freq.size(),0)), root(freq.size(),vector<int> (freq.size(),0));
    int ** dp=new int*[freq.size()];
    int ** root=new int*[freq.size()];
    for(int i=0;i<freq.size();i++){
    	dp[i]=new int[freq.size()];
    	root[i]=new int[freq.size()];
    }
    //makes dp matrix and root matrix
    makeMat(freq,dp,root);
    cout<<"----------------------------------------------------------------"<<endl;
    //makes and returns tree
    node * start=makeOptBST(freq,dp,root,0,freq.size()-1,1);
    // cout<<"hello m"<<endl;
    //prints optimal BST
    printOBST(start);
    cout<<"----------------------------------------------------------------"<<endl;
    int choice=1;
    while(choice){
    	cout<<"Enter Word to search: ";
    	string sWord;
    	cin>>sWord;
    	transform(sWord.begin(), sWord.end(), sWord.begin(), ::tolower);
    	//search word
    	searchOBST(start,sWord);
    	cout<<"----------------------------------------------------------------"<<endl;
    	cout<<"Press 1 to continue searching for other words, or 0 to end program: ";
    	cin>>choice;
    }
    cout<<"Thank you!! :)"<<endl;
	return 0;
}