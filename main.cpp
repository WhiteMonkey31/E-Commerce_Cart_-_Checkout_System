
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<math.h>

using namespace std;

const int u_max=10;

const int p_MAX=100;

int u_count=0;

struct user_data{
    string username,
        password,
        role;
    int id;
};

struct product{
    int p_id,
        p_quantity;
    string p_name;
    float p_price;
};

void store_intro(){
    cout<<"\n-----------------------------------\n"
        <<"Welcome to SAM E-Commerce Store.\n"
        <<"-----------------------------------\n"<<endl;
}

struct CartNode {
    int product_id;
    string product_name;
    float product_price;
    int quantity;

    CartNode* next;
    CartNode(int id, string name, float price, int qty)
        : product_id(id), product_name(name), product_price(price), quantity(qty), next(nullptr) {}
};

class Cart {
private:
    CartNode* head;

public:
    Cart() {
        head = nullptr;
    }

    void add_to_cart(product p, int qty) {
        CartNode* newNode = new CartNode(p.p_id, p.p_name, p.p_price, qty);
        newNode->next = head;
        head = newNode;
        cout << "Product added to cart.\n";
    }

    void view_cart() {
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }
        CartNode* temp = head;
        cout << "\n--- Your Cart ---\n";
        cout << "ID\tName\tPrice\tQty\n";
        while (temp) {
            cout << temp->product_id << "\t" << temp->product_name << "\t"
                 << temp->product_price << "\t" << temp->quantity << endl;
            temp = temp->next;
        }
    }

    bool remove_last_item() {
        if (!head) return false;
        CartNode* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    void clear_cart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~Cart() {
        clear_cart();
    }
};


struct UndoNode {
    CartNode* cart_snapshot;
    UndoNode* next;
    UndoNode(CartNode* node) : cart_snapshot(node), next(nullptr) {}
};

class UndoStack {
private:
    UndoNode* top;

public:
    UndoStack() : top(nullptr) {}

    void push(CartNode* node) {
        UndoNode* newNode = new UndoNode(node);
        newNode->next = top;
        top = newNode;
    }

    CartNode* pop() {
        if (!top) return nullptr;
        UndoNode* temp = top;
        top = top->next;
        CartNode* node = temp->cart_snapshot;
        delete temp;
        return node;
    }

    bool is_empty() {
        return top == nullptr;
    }

    ~UndoStack() {
        while (top) {
            UndoNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
};


class product_catalog{
    public:
        product p_list[p_MAX];
        int p_count;
        product_catalog(){
            p_count=0;
        }
        void add_product();
        void edit_product();
        void delete_product();
        void display_all_product();
        void search_product();
        void sort_product();

};

// cout << "5. Undo Last Added Product\n";
// ...
// case 5: {
//     cout << "Undoing last product...\n";
//     CartNode* last = undo_stack.pop();
//     if (last) {
//         c_cart.remove_last_item();
//         cout << "Last product removed from cart.\n";
//         delete last; // Prevent memory leak
//     } else {
//         cout << "Nothing to undo.\n";
//     }
//     break;
// }



class user_interface{

    public:
        void main_menu(product_catalog &pc, user_data ud[], int user_i, Cart &c_cart);

};

void user_interface::main_menu(product_catalog &pc, user_data ud[], int user_i, Cart &c_cart){

    store_intro();
    int m_choice;
    if(ud[user_i].role=="customer"){
        cout<<"You are Logged in as Customer.\n";

        do{
            cout<<"\nPlease Select (1-4): \n"
                <<"1. Display all Products.\n"
                <<"2. View Cart.\n"
                <<"3. Search Product.\n"
                <<"4. Sort Product.\n"
                <<"0. Logout.\n"
                <<"\nPlease Enter Your Choice: ";
                cin>>m_choice;
            switch(m_choice){
                case 1:{
                    cout<<"Display All Product Page.\n";
                    pc.display_all_product();
                    break;
                }
                case 2:{
                    cout<<"View Cart Page.\n";
                    c_cart.view_cart();
                    break;
                }
                case 3:{
                    cout<<"Search Page Product.\n";
                    pc.search_product();
                    break;
                }
                case 4:{
                    cout<<"Sort Product Page.\n";
                    pc.sort_product();
                    break;
                }
                case 0:{
                    cout<<"Going Back to login page.\n";
                    return;
                }
                default:{
                    cout<<"Invalid Choice! Please try again.\n";
                    cin.ignore();
                    break;
                }
            }
        }while(m_choice!=0);

    }else if(ud[user_i].role=="admin"){
        cout<<"You are Logged in as Admin.\n"
            <<"----------------\n"
            <<"Admin Panel.\n"
            <<"----------------\n";
        
        do{
            cout<<"\nPlease Select (1-4): \n"
                <<"1. Display all Products.\n"
                <<"2. Edit Product.\n"
                <<"3. Search Product.\n"
                <<"4. Sort Product.\n"
                <<"5. Delete Product.\n"
                <<"0. Logout.\n"
                <<"\nPlease Enter Your Choice: ";
                
                cin>>m_choice;
            switch(m_choice){
                case 1:{
                    cout<<"Display All Product Page.\n";
                    pc.display_all_product();
                    break;
                }
                case 2:{
                    cout<<"Edit Product Page.\n";
                    pc.edit_product();
                    break;
                }
                case 3:{
                    cout<<"Search Page Product.\n";
                    pc.search_product();
                    break;
                }
                case 4:{
                    cout<<"Sort Product Page.\n";
                    pc.sort_product();
                    break;
                }
                case 5:{
                    cout<<"Delete Product Page.\n";
                    pc.delete_product();
                    break;
                }
                case 0:{
                    cout<<"Going Back to login page.\n";
                    return;
                }
                default:{
                    cout<<"Invalid Choice! Please try again.\n";
                    cin.ignore();
                    break;
                }
            }
        }while(m_choice!=0);

    }
}

void add_to_cart(product_catalog &pc, Cart &c_cart, UndoStack &undo_stack){
    int prod_id, qty;
    cout << "Enter Product ID to add to cart: ";
    cin >> prod_id;
    if (prod_id < 0 || prod_id >= pc.p_count) {
        cout << "Invalid Product ID.\n";
        // break;
    }
    cout << "Enter Quantity: ";
    cin >> qty;
    c_cart.add_to_cart(pc.p_list[prod_id], qty);

    // Push to undo stack
    CartNode* snapshot = new CartNode(pc.p_list[prod_id].p_id, pc.p_list[prod_id].p_name, pc.p_list[prod_id].p_price, qty);
    undo_stack.push(snapshot);

}


void product_catalog::add_product(){

    if(p_count>=p_MAX){
        cout<<"Product Catalog is Full. Cannot add more products.\n";
        return;
    }

    product &p=p_list[p_count];
    p.p_id = p_count;
    
    cout<<"Enter Product Details: \n"
        <<"Name:     ";
    cin.ignore();
    getline(cin,p.p_name);
    cout<<"Price:    ";
    cin>>p.p_price;
    cout<<"Quantity: ";
    cin>>p.p_quantity;
    cout<<"\nProduct Added Successfully.\n"
        <<"Product ID: "<<p.p_id<<endl;
    p_count++;
    
}

void product_catalog::edit_product(){
    int c_id;
    cout<<"Enter Product ID to Edit: ";
    cin>>c_id;
    if(c_id<0 || c_id>=p_count){
        cout<<"Invalid Product ID.\n";
        return;
    }
    for(int i=0;i<p_count;i++){
        if(i==c_id){
            cout<<"Editing Product ID: "<<i<<endl
                <<"Current Name: "<<p_list[i].p_name<<endl
                <<"New Name: ";
            cin.ignore();
            getline(cin,p_list[i].p_name);
            cout<<"Current Price: "<<p_list[i].p_price<<endl
                <<"New Price: ";
            cin>>p_list[i].p_price;
            cout<<"Current Quantity: "<<p_list[i].p_quantity<<endl
                <<"New Quantity: ";
            cin>>p_list[i].p_quantity;
            cout<<"\nProduct Edited Successfully.\n";

        }
    }
}

void product_catalog::delete_product(){
    int d_id;
    cout<<"Enter Product ID to Delete: ";
    cin>>d_id;
    if(d_id<0 || d_id>=p_count){
        cout<<"Invalid Product ID Entered.\n";
        return;
    }

    for(int i=0;i<p_count;i++){
        if(i==d_id){
            cout<<"Deleting Product ID: "<<i<<endl
                <<"Product Name: "<<p_list[i].p_name<<endl
                <<"Product Quantity: "<<p_list[i].p_quantity<<endl
                <<"Product Price: "<<p_list[i].p_price<<endl
                <<"\nAre you sure you want to delete this product? (1/0): \n"
                <<"1. Yes\n"
                <<"0. No\n"
                <<"Enter Choice: ";
            int d_choice;
            cin>>d_choice;
            if(d_choice==1){
                for(int j=i;j<p_count-1;j++){
                    p_list[i]=p_list[j+1];
                }
                p_count--;
                cout<<"Product Delected Successfully.\n";
                return;
                
            }else{
                cout<<"Product Deletion Cancelled.\n";
                return;
            }
        }
    }
}

void product_catalog::display_all_product(){
    if(p_count==0){
        cout<<"No Product Available.\n";
        return;
    }
    cout<<"Product Catalog:\n"<<endl;
    cout<<"---------------------------------------\n"
        <<"ID\tName\tPrice\tQuantity\n"
        <<"---------------------------------------\n";
    for(int i=0;i<p_count;i++){
        cout<<p_list[i].p_id<<"\t"<<p_list[i].p_name<<"\t"<<p_list[i].p_price<<"\t"<<p_list[i].p_quantity
            <<endl;
    }
    cout<<"-----------------------------------------\n"
        <<"Total Products: "<<p_count<<endl;

}

void product_catalog::search_product(){
    string s_name;
    cout<<"Enter Product Name to Search: ";
    cin.ignore();
    getline(cin,s_name);
    bool s_found=false;
    for(int i=0;i<p_count;i++){
        if(s_name==p_list[i].p_name){
            cout<<"\nProduct Found: \n"
                <<"ID:       "<<p_list[i].p_id<<endl
                <<"Name:     "<<p_list[i].p_name<<endl
                <<"Quantity: "<<p_list[i].p_quantity<<endl
                <<"Price:    "<<p_list[i].p_price<<endl;
                s_found=true;               
               break;
        }
    }
    if(!s_found){
        cout<<"Product Not Found.\n";
    }
}

void product_catalog::sort_product(){

    if(p_count==0){
        cout<<"No Product Available to sort.\n";
        return;
    }
    for(int i=0;i<p_count-1;i++){
        for(int j=i+1;j<p_count;j++){
            if(p_list[i].p_price>p_list[j].p_price){
                swap(p_list[i].p_name,p_list[j].p_name);
                swap(p_list[i].p_quantity,p_list[j].p_quantity);
                swap(p_list[i].p_price,p_list[j].p_price);
            }
        }
    }
}

void write_product_file(product_catalog &pc){
    ofstream product_outfile("products.txt",ios::out);
    if(!product_outfile){
        cerr<<"Error! opening file \'products.txt\' for writting products data.\n";
        return;
    }
    for(int i=0;i<pc.p_count;i++){
        product_outfile<<pc.p_list[i].p_id<<","<<pc.p_list[i].p_name<<","<<pc.p_list[i].p_price<<","
            <<pc.p_list[i].p_quantity<<endl;
    }
    product_outfile.close();
    cout<<"Product Data written to file successfully.\n";
}

void read_product_file(product_catalog &pc){
    ifstream product_infile("products.txt");
    if(!product_infile){
        cerr<<"Error! can't able to open \'products.txt\' file for reading data.\n";
        return;
    }
    pc.p_count=0;
    const int MAX=240;
    char buffer[MAX];
    while(product_infile){
        product_infile.getline(buffer, MAX);
        if(strlen(buffer)>0){
            const char* delim=",";
            char* next_token;

            char* c_id=strtok_s(buffer,delim,&next_token);
            char* c_pname=strtok_s(NULL,delim,&next_token);
            char* c_pprice=strtok_s(NULL,delim,&next_token);
            char* c_pquantity=strtok_s(NULL,delim,&next_token);

            if(c_id == NULL || c_pname == NULL || c_pprice == NULL || c_pquantity == NULL) {
                cerr << "Error! Invalid data format in 'products.txt'.\n";
                continue; 
            }

            if(c_id && c_pname && c_pprice && c_pprice){
                pc.p_list[pc.p_count].p_id=atoi(c_id);
                pc.p_list[pc.p_count].p_name=c_pname;
                pc.p_list[pc.p_count].p_price=atof(c_pprice);
                pc.p_list[pc.p_count].p_quantity=atoi(c_pquantity);

                pc.p_count++;
            }
            if(pc.p_count >= p_MAX) {
                break; 
            }
        }
    }
    product_infile.close();
    cout << "Product data read from file successfully.\n";
    
}

void write_user_file(user_data ud[],int u_count){
    ofstream user_outfile("users.txt",ios::out);
    if(!user_outfile){
        cerr<<"Error! opening file \'users.txt\' for writing user's data.\n";
        return;
    }
    for(int i=0;i<u_count;i++){
        user_outfile<<ud[i].id<<","<<ud[i].username<<","<<ud[i].password<<","<<ud[i].role<<endl;
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
            char* c_role=strtok_s(NULL,delim,&next_token);

            if(c_id == NULL || c_username == NULL || c_password == NULL || c_role == NULL) {
                cerr << "Error! Invalid data format in 'users.txt'.\n";
                continue; 
            }

            if(c_id && c_username && c_password){
                ud[u_count].id=atoi(c_id);
                ud[u_count].username=c_username;
                ud[u_count].password=c_password;
                ud[u_count].role=c_role;
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

void register_user(user_data ud[]);
void login_user(user_data ud[],user_interface ui, product_catalog &pc, Cart c_cart);

void lock_screen(user_data ud[], user_interface ui, product_catalog &pc, Cart c_cart){

    bool b_entery=true;
    do{
        
        int choice;
        store_intro();
        cout<<"Please Login/Register first to use our store.\n"
            <<"Thanks.\n"<<endl
            <<"\nPlease select (1-3):\n"
            <<"1. Login User.\n"
            <<"2. Register User.\n"
            // <<"3. Admin Panel.\n"
            <<"0. Exit.\n"
            <<"\nPlease Enter Your Choice: ";
        cin>>choice;
        switch(choice){
            case 1:{
                cout<<"login user page.\n";
                login_user(ud,ui,pc,c_cart);
                break;
            }
            case 2:{
                cout<<"resgister user page.\n";
                register_user(ud);
                break;
            }
            // case 3:{
            //     cout<<"Admin Page.\n";
            //     break;
            // }
            case 0:{
                b_entery=false;
                cout<<"\nExisting the Program.\n"
                    <<"Thanks for using our store.\n"
                    <<"Have a nice day.\n"<<endl;
                break;
            }
            default:{
                cout<<"You have entered wrong choice. Please select the correct one.\n";
                cin.ignore();
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
    cout<<"Please Enter Your Credentails:\n"
        <<"Username: ";
    string c_username;
    

    while(true){
        cin>>c_username;
        bool is_user_duplicate=false;
        for(int i=0;i<u_count;i++){
            if(ud[i].username==c_username){
                cout<<"Username already exists. Please choose a different username.\n"
                    <<"Username: ";
                    is_user_duplicate=true;
                break;
            }
            
        }
        if(!is_user_duplicate){
            break;
        }
        
    }
    ud[u_count].username=c_username;
    
    cout<<"Password: ";
    cin>>ud[u_count].password;
    ud[u_count].id=u_count;
    ud[u_count].role="customer";

    cout<<"\nRegistration Successfull.\n"
        <<"Your User ID is: "<<ud[u_count].id<<".\n"
        <<"Username: "<<ud[u_count].username<<".\n"
        <<"Password: "<<ud[u_count].password<<".\n"
        <<"Role:     "<<ud[u_count].role<<".\n"
        <<"Now you can login with your credentails.\n";

    u_count++;
}

void login_user(user_data ud[],user_interface ui, product_catalog &pc, Cart c_cart){
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
            cout<<"\nLogin Successfull.\n"
                <<"Welcome "<<ud[i].username<<".\n";
                ui.main_menu(pc,ud,i,c_cart);
                b_found=true;
                break;
            
        }
    }
    if(!b_found){
        cout<<"\nLogin Failed.\n"
            <<"Please check your credentails and try again.\n"
            <<"If you are not registered yet, please register first.\n";

    }
}

int main(){
    user_data ud[u_max];
    user_interface ui;
    product_catalog pc;
    Cart c_cart;
    UndoStack undo_stack;

    read_user_file(ud,u_count);
    read_product_file(pc);

    lock_screen(ud,ui,pc,c_cart);

    write_product_file(pc);
    write_user_file(ud, u_count);

    return 0;
}