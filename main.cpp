/* @Author
Student Name: Ayse Betul Cetin
Student ID: 150180730
Date: 22.12.2019 */

#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <fstream>

#define MAX 52

using namespace std;

struct mobileNode{
	int id;
	int parentid;
	mobileNode *next;
};
struct baseNode{
	int id;
	int parentid;
	baseNode *next;
	mobileNode *below;
};
struct network{ 
	void create();
	void close();
	void add(char ,int ,int );  // takes : type of node (B or M) , id , parent id
	int recursive_print(int ID, baseNode *traverse ); 
	int recursive_path (int P_ID ,baseNode *traverse);
	baseNode *CentralController;  // head of a network 
};

void network::create(){	// this a function to create a network 
	CentralController = new baseNode; // I have to create a new space for CentralController!
	CentralController -> id = 0;
	CentralController -> parentid = -1; // it is a unique base
	
	CentralController -> next = NULL;
	CentralController -> below = NULL;
}

void network::close(){
	baseNode *ptr1 ;
	mobileNode *ptr2 ;
	mobileNode *traverse;
	while (CentralController){
		ptr2 = CentralController ->below;
		while(ptr2 ){
			traverse = ptr2;
			ptr2 = ptr2 -> next;
			delete traverse;
		}
		
		ptr1 = CentralController;
		CentralController = CentralController ->next;
		delete ptr1;
	}
}


void network::add(char type, int new_id , int parent_id ){  //function to add base stations and mobile hosts
	if(type == 'B'){  // adding a base station node
		baseNode *newbase = new baseNode;
		newbase -> next = NULL;
		newbase -> below = NULL;
		newbase -> id = new_id;
		newbase -> parentid = parent_id;
		
		if(CentralController -> next == NULL && parent_id == 0){  // if it is the first base station
			CentralController -> next = newbase;
		}else{
			if(parent_id == 0){			// if it is not connected a base station , connected to CC
				baseNode *traverse = CentralController;
				baseNode *tail = CentralController;
				while(traverse){ // adding the last base station
					tail = traverse;
					traverse = traverse->next;
				}
				tail -> next = newbase;
			}else{
				baseNode *traverse = CentralController -> next;
				while(traverse -> id  !=  parent_id){
					traverse = traverse -> next;
				}
				while ((traverse -> next)-> parentid == parent_id){
					traverse = traverse ->next;
				}
				newbase->next = traverse->next;
				traverse->next = newbase;
			}
		}
		
	}else{   // adding a mobile host node under a base (if type == M )
		mobileNode *newmobile = new mobileNode;
		newmobile -> next = NULL;
		newmobile -> id = new_id;
		newmobile -> parentid = parent_id;
		
		baseNode *traverse = CentralController -> next;
		
		while (traverse->id != parent_id ){  // finding the base station
			traverse = traverse -> next;
		}
		if(traverse -> below == NULL){  // if it is the first mobile
			traverse -> below = newmobile;
		}else{
			mobileNode *m_traverse = traverse -> below ;
			mobileNode *tail = m_traverse;
			while (m_traverse){
				tail = m_traverse;
				m_traverse = m_traverse -> next;
			}
			tail -> next = newmobile;
		}
	}
}

int network::recursive_print( int ID, baseNode *traverse ){  //in main function : baseNode *traverse = CentralController;
	int m = 0;
	if(traverse){
		cout << traverse-> id ;
		cout << " " ;
		if(traverse ->below != NULL){
			mobileNode *temp = traverse->below;
			while (temp){
				if(temp->id == ID){   // if the mobile host that we search, found
					m = temp -> parentid;
					return m;
				}
				temp = temp -> next;
			}
	    }
		return ( m + recursive_print( ID, traverse->next ));
	}
	return 0;
}
int network::recursive_path( int P_ID , baseNode *traverse){
	baseNode *temp = traverse;
	while (traverse -> id != P_ID){
		traverse = traverse->next;
	}
	if(traverse -> parentid == 0){
		cout << traverse ->id  << " ";
		return 0;
	}else{
		recursive_path(traverse->parentid ,temp );
		cout << traverse->id << " ";
	}
	return 0;
}

int main(int argc, char *argv[]){
	char *filenetwork = argv[1];
	char *filemessage = argv[2];
	network Network1;
	Network1.create();
	
	char str[MAX];
	char message[MAX-2];
	char str_id[MAX];
	
	char Type[2];
	int Id;
	int Parentid;
	
	FILE *messagefileptr = fopen (filemessage, "r"); 
	fseek (messagefileptr ,0, SEEK_SET);
	
	FILE *networkfileptr = fopen (filenetwork, "r");	
	fseek (networkfileptr ,0, SEEK_SET);

	while(true){   //creating the network
		fscanf (networkfileptr , "%s %d %d", Type , &Id, &Parentid);
		if(feof(networkfileptr)){
			break;
		}
		Network1.add(Type[0],Id,Parentid);
	}
	fclose(networkfileptr);
	
	
	while(fgets(str , MAX , messagefileptr)){  // processing messages
		if(feof(messagefileptr)){
			break;
		}
		
		int i = 0;
		while(str[i] != '>'){
			message[i] = str[i];
			i++;
		}
		message[i]= '\0';
		int j=0;
		while (str[i+1] != '\0'){
			str_id[j]= str[i+1];
			i++;
			j++;
		}
		int I_id = atoi(str_id);   // char array to integer
	
		cout << "Traversing:" ;
		int m = Network1.recursive_print(I_id, Network1.CentralController);
		if ( m == 0){
			cout << endl;
			cout << "Can not be reached the mobile host mh_";
			cout << I_id;
			cout <<" at the moment"<< endl;
		}else{
			cout << endl;
			cout << "Message:";
			i = 0;
			while (message[i] != '\0'){
				cout << message[i];
				i++;
			}
			cout << " To:0 ";
			Network1.recursive_path(m ,(Network1.CentralController) -> next );
			cout << "mh_";
			cout << I_id << endl;
		}
		
	}

	Network1.close();
	
	fclose(messagefileptr);
	return 0;
}




