#include<iostream>

#include "shared_definations.h"
#include "user_interface_page_header.h"

class user_interface{

    public:
        void main_menu();

};

void user_interface::main_menu(){
    cout<<"SAM E-Commerce Store\n"
        <<"Welcome to our Store.\n"
        <<"************************\n"
        <<"\nPlease Select (1-4): \n"
        <<"1. Product Catalog.\n"
        <<"2. User Account.\n"
        <<"3. Search Product.\n"
        <<"4. Sort Product.\n"
        <<"0. Exit.\n"
        <<"\nPlease Enter Your CHoice: ";
        int m_choice;
        cin>>m_choice;
    switch(m_choice){
        case 1:{
            cout<<"Product Catalog Page.\n";
            break;
        }
        case 2:{
            cout<<"User Account Page.\n";
            break;
        }
        case 3:{
            cout<<"Search Page Product.\n";
            break;
        }
        case 4:{
            cout<<"Sort Product Page.\n";
            break;
        }
        case 0:{
            cout<<"Going Back to login page.\n";
            return;
        }
        default:{
            cout<<"Invalid Choice! Please try again.\n";
            // main_menu();
            break;
        }
    }
}