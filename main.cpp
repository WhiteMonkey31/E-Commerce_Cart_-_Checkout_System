//Time to fuck this up

#include<iostream>
#include<fstream>
#include<string>

#include "shared_definations.h"


// using namespace std;

#include "file_handling.cpp"
#include "lock_screen_page.cpp"
#include "user_interface_page.cpp"


// int u_count=0;
// const int u_max=10;
// struct user_data{
//     string username,
//         password;
//     int id;
// };


int main(){
    user_data ud[u_max];

    user_interface ui;
   
    read_user_file(ud,u_count);
    lock_screen(ud,ui);
    write_user_file(ud, u_count);

    

    

    return 0;
}