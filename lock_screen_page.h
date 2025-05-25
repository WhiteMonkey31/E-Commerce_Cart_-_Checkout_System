
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

    bool b_entery=false;
    do{
        user_data ud[u_max];
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
            <<"\nPlease Enter YOur Choice: ";
        cin>>choice;
        switch(choice){
            case 1:{
                cout<<"Register user.\n";
                register_user(ud);

                break;
            }
            case 2:{
                cout<<"login page.\n";
                login_user(ud);
                break;
            }
            case 3:{
                cout<<"Admin Page.\n";
                break;
            }
            default:{
                b_entery=true;
                cout<<"You have entered wrong choice. Please select the correct one.\n";
                break;
            }
        }
    }while(b_entery);
    
}

void register_user(user_data ud[]){
    
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

    string c_username, c_password;
    cout<<"Please Enter Your Credentails:\n"
        <<"UserName: ";
    cin>>c_username;
    cout<<"Password: ";
    cin>>c_password;

    for(int i=0;i<u_count;i++){
        if(u_count>0 && ud[i].username==c_username && ud[i].password==c_password){
            cout<<"Login Successfull.\n"
                <<"Welcome "<<ud[i].username<<".\n";
            
        }else{
            cout<<"Login Failed.\n"
                <<"Please check your credentails and try again.\n"
                <<"If you are not registered yet, please register first.\n";
            
        }
    }
}