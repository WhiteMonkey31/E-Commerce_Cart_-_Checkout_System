//Time to fuck this up

#include<iostream>
#include<fstream>
#include<string>


using namespace std;

#include "lock_screen_page.cpp"
#include "file_handling.cpp"


// int u_count=0;
// const int u_max=10;
// struct user_data{
//     string username,
//         password;
//     int id;
// };


int main(){
    user_data ud[u_max];
   

    lock_screen(ud);
    write_user_file(ud, u_count);
    return 0;
}