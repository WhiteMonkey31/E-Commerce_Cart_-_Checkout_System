

void lock_screen(){

    bool entery=true;
    do{
        int choice;
        cout<<"SAM E-Commerce Store\n"
            <<"Welcome to our Store.\n"
            <<"**********************\n"<<endl
            <<"Please Login/Register first to use our store.\n"
            <<"Thanks.\n"<<endl
            <<"Please select (1-3):\n"
            <<"1. Register User.\n"
            <<"2. Login User.\n"
            <<"3. Admin Panel.\n"
            <<"\nPlease Enter YOur Choice: ";
        cin>>choice;
        switch(choice){
            case 1:{
                cout<<"Register user.\n";
                break;
            }
            case 2:{
                cout<<"login page.\n";
                break;
            }
            case 3:{
                cout<<"Admin Page.\n";
                break;
            }
            default:{
                cout<<"You have entered wrong choice. Please select the correct one.\n";
                break;
            }
        }
    }while(entery);
    
}

