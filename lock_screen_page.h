
int u_count=0;
const int u_max=10;
struct user_data{
    string username,
        password;
    int id;
};


void register_user(user_data ud[]);
void login_user(user_data ud[]);

void lock_screen(){

    user_data ud[u_max];
    bool b_entery=true;
    do{
        
        int choice;
        cout<<"\nSAM E-Commerce Store\n"
            <<"Welcome to our Store.\n"
            <<"**********************\n"<<endl
            <<"Please Login/Register first to use our store.\n"
            <<"Thanks.\n"<<endl
            <<"Please select (1-3):\n"
            <<"1. Register User.\n"
            <<"2. Login User.\n"
            <<"3. Admin Panel.\n"
            <<"0. Exit.\n"
            <<"\nPlease Enter YOur Choice: ";
        cin>>choice;
        switch(choice){
            case 1:{
                cout<<"Register user.\n";
                register_user(ud);
                // b_entery=true;
                break;
            }
            case 2:{
                cout<<"login page.\n";
                
                login_user(ud);
                // b_entery=true;
                break;
            }
            case 3:{
                cout<<"Admin Page.\n";
                break;
            }
            case 0:{
                b_entery=false;
                cout<<"\nExisting the Program.\n"
                    <<"Thanks for using our store.\n"
                    <<"Have a nice day.\n";
                break;
            }
            default:{
                // b_entery=true;
                cout<<"You have entered wrong choice. Please select the correct one.\n";
                break;
            }
        }
    }while(b_entery);
    
}

void register_user(user_data ud[]){
    
    cin.ignore();
    if(u_count>=u_max){
        return;
    }

    u_count++;

    cout<<"Please Enter Your Credentails:\n"
        <<"Username: ";
    cin>>ud[u_count].username;
    cout<<"Password: ";
    cin>>ud[u_count].password;
    ud[u_count].id=u_count;

    cout<<"\nRegistration Successfull.\n"
        <<"your User ID is: "<<ud[u_count].id<<".\n"
        <<"Now you can login with your credentails.\n";


}

void login_user(user_data ud[]){
    cin.ignore();
    bool b_found=false;
    if(u_count==0){
        cout<<"No user registered yet.\n"
            <<"Please register first to login.\n";
        return;
    }
    string c_username, c_password;
    cout<<"Please Enter Your Credentails:\n"
        <<"UserName: ";
    cin>>c_username;
    cout<<"Password: ";
    cin>>c_password;

    for(int i=0;i<u_count;i++){
        if(ud[i].username==c_username && ud[i].password==c_password){
            cout<<"Login Successfull.\n"
                <<"Welcome "<<ud[i].username<<".\n";
                b_found=true;
                break;
            
        }
    }
    if(!b_found){
        cout<<"Login Failed.\n"
            <<"Please check your credentails and try again.\n"
            <<"If you are not registered yet, please register first.\n";

    }
}