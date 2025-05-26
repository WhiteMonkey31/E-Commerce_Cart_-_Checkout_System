#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

#include "shared_definations.h"

void write_user_file(user_data ud[],int u_count){
    ofstream user_outfile("users.txt",ios::out);
    
    if(!user_outfile){
        cerr<<"Error! opening file \'users.txt\' for writing user's data.\n";
        return;
    }
    
    for(int i=0;i<u_count;i++){
        user_outfile<<ud[i].id<<","<<ud[i].username<<","<<ud[i].password<<endl;
    }
    
    user_outfile.close();
    cout<<"User data written to file successfully.\n";

}

void read_user_file(user_data ud[], int &u_count){
    u_count=0;
    ifstream user_infile("users.txt");

    if(!user_infile){
        cerr << "Error! opening file 'users.txt' for reading user's data.\n";
        return;
    }

    const int MAX=240;
    char buffer[MAX];
    while(user_infile){
        user_infile.getline(buffer, MAX);
        if(strlen(buffer)>0){
            const char* delim=",";
            char* next_token;

            char* c_id=strtok_s(buffer,delim,&next_token);
            char* c_username=strtok_s(NULL,delim,&next_token);
            char* c_password=strtok_s(NULL,delim,&next_token);

            if(c_id == NULL || c_username == NULL || c_password == NULL) {
                cerr << "Error! Invalid data format in 'users.txt'.\n";
                continue; // Skip this line if data is invalid
            }

            if(c_id && c_username && c_password){
                ud[u_count].id=atoi(c_id);
                ud[u_count].username=c_username;
                ud[u_count].password=c_password;
                u_count++;
            }
            

            if(u_count >= u_max) {
                break; 
            }
        }
    }
    user_infile.close();
    cout << "User data read from file successfully.\n";
}

