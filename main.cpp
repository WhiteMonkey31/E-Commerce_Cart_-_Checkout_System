
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<math.h>

using namespace std;

const int u_max = 10;
const int p_MAX = 100;

int u_count = 0;

struct user_data {
    string username,
        password,
        role;
    int id;
};

struct product {
    int p_id,
        p_quantity;
    string p_name;
    float p_price;
};

void store_intro() {
    cout << "\n-----------------------------------\n"
        << "Welcome to SAM E-Commerce Store.\n"
        << "-----------------------------------\n" << endl;
}

void credit();

struct CartItem {
    int product_id;
    string product_name;
    float product_price;
    int quantity;
    CartItem* next;
    
    CartItem(int id, string name, float price, int qty) 
        : product_id(id), product_name(name), product_price(price), quantity(qty), next(nullptr) {}
};


class Cart {
private:
    CartItem* head;
    int itemCount;

public:
    Cart() : head(nullptr), itemCount(0) {}

    void add_to_cart(product p, int qty) {
      
        CartItem* current = head;
        while (current != nullptr) {
            if (current->product_id == p.p_id) {
                current->quantity += qty;
                cout << "Product quantity updated in cart.\n";
                return;
            }
            current = current->next;
        }
        
        
        CartItem* newItem = new CartItem(p.p_id, p.p_name, p.p_price, qty);
        newItem->next = head;
        head = newItem;
        itemCount++;
        cout << "Product added to cart.\n";
    }

    void view_cart() {
        if (head == nullptr) {
            cout << "Cart is empty.\n";
            return;
        }
        
        cout << "\n--- Your Cart ---\n";
        cout << "ID\tName\tPrice\tQty\tSubtotal\n";
        cout << "---------------------------------------\n";
        
        float total = 0;
        CartItem* current = head;
        while (current != nullptr) {
            float subtotal = current->product_price * current->quantity;
            cout << current->product_id << "\t" << current->product_name << "\t"
                 << current->product_price << "\t" << current->quantity << "\t$" << subtotal << endl;
            total += subtotal;
            current = current->next;
        }
        
        cout << "---------------------------------------\n";
        cout << "Total: $" << total << endl;
    }

    bool remove_item(int product_id) {
        CartItem* current = head;
        CartItem* prev = nullptr;
        
        while (current != nullptr) {
            if (current->product_id == product_id) {
                if (prev == nullptr) {
                    
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                itemCount--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void clear_cart() {
        while (head != nullptr) {
            CartItem* temp = head;
            head = head->next;
            delete temp;
        }
        itemCount = 0;
    }

    float calculate_total() {
        float total = 0;
        CartItem* current = head;
        while (current != nullptr) {
            total += current->product_price * current->quantity;
            current = current->next;
        }
        return total;
    }

    CartItem* get_items() const {
        return head;
    }

    bool is_empty() const {
        return head == nullptr;
    }

    int count() const {
        return itemCount;
    }

    ~Cart() {
        clear_cart();
    }
};


class UndoStack {
private:
    struct UndoNode {
        CartItem* cart_snapshot;
        UndoNode* next;
        
        UndoNode(CartItem* items) : cart_snapshot(items), next(nullptr) {
            
            if (items != nullptr) {
                CartItem* current = items;
                CartItem* tail = nullptr;
                while (current != nullptr) {
                    CartItem* newItem = new CartItem(current->product_id, current->product_name, 
                                                    current->product_price, current->quantity);
                    if (cart_snapshot == nullptr) {
                        cart_snapshot = newItem;
                        tail = newItem;
                    } else {
                        tail->next = newItem;
                        tail = tail->next;
                    }
                    current = current->next;
                }
            }
        }
        
        ~UndoNode() {
            while (cart_snapshot != nullptr) {
                CartItem* temp = cart_snapshot;
                cart_snapshot = cart_snapshot->next;
                delete temp;
            }
        }
    };
    
    UndoNode* top;

public:
    UndoStack() : top(nullptr) {}

    void push(CartItem* current_cart) {
        UndoNode* newNode = new UndoNode(current_cart);
        newNode->next = top;
        top = newNode;
    }

    CartItem* pop() {
        if (top == nullptr) return nullptr;
        
        UndoNode* temp = top;
        top = top->next;
        
        CartItem* result = temp->cart_snapshot;
        temp->cart_snapshot = nullptr; 
        delete temp;
        
        return result;
    }

    bool is_empty() const {
        return top == nullptr;
    }

    ~UndoStack() {
        while (top != nullptr) {
            UndoNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

class product_catalog {
public:
    product p_list[p_MAX];
    int p_count;
    product_catalog() {
        p_count = 0;
    }
    void add_product();
    void edit_product();
    void delete_product();
    void display_all_product();
    void search_product();
    void sort_product();
};

class user_interface {
public:
    void main_menu(product_catalog& pc, user_data ud[], int user_i, Cart& c_cart, UndoStack& undo_stack);
    void checkout(product_catalog& pc, Cart& c_cart);
};

void user_interface::checkout(product_catalog& pc, Cart& c_cart) {
    if (c_cart.is_empty()) {
        cout << "Your cart is empty. Nothing to checkout.\n";
        return;
    }

    cout << "\n--- Checkout ---\n";
    c_cart.view_cart();

    cout << "\nProceed with checkout? (1=Yes, 0=No): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        
        float total = c_cart.calculate_total();
        
        string name, address, payment;
        cin.ignore();
        
        cout << "Enter your full name: ";
        getline(cin, name);
        
        cout << "Enter shipping address: ";
        getline(cin, address);
        
        cout << "Select payment method (credit/debit/paypal/cod): ";
        getline(cin, payment);
        
        cout << "\n--- Order Summary ---\n";
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Payment: " << payment << endl;
        cout << "Items:\n";
        
        CartItem* current = c_cart.get_items();
        while (current != nullptr) {
            cout << current->product_name << " x" << current->quantity << " @ $" << current->product_price << endl;
            
            
            for (int i = 0; i < pc.p_count; i++) {
                if (pc.p_list[i].p_id == current->product_id) {
                    pc.p_list[i].p_quantity -= current->quantity;
                    if (pc.p_list[i].p_quantity < 0) {
                        pc.p_list[i].p_quantity = 0;
                    }
                }
            }
            
            current = current->next;
        }
        
        cout << "Total: $" << total << endl;
        cout << "\nOrder placed successfully!\n";
        cout << "Thank you for shopping with us.\n";
        
       
        c_cart.clear_cart();
    }
    else {
        cout << "Checkout cancelled.\n";
    }
}

void save_cart_snapshot(Cart& cart, UndoStack& undo_stack) {
    
    CartItem* current = cart.get_items();
    CartItem* snapshot = nullptr;
    CartItem* tail = nullptr;
    
    while (current != nullptr) {
        CartItem* newItem = new CartItem(current->product_id, current->product_name, 
                                       current->product_price, current->quantity);
        if (snapshot == nullptr) {
            snapshot = newItem;
            tail = newItem;
        } else {
            tail->next = newItem;
            tail = tail->next;
        }
        current = current->next;
    }
    
    undo_stack.push(snapshot);
}

void user_interface::main_menu(product_catalog& pc, user_data ud[], int user_i, Cart& c_cart, UndoStack& undo_stack) {
    store_intro();
    int m_choice;
    if (ud[user_i].role == "customer") {
        cout << "You are Logged in as Customer.\n";

        do {
            cout << "\nPlease Select (1-6): \n"
                << "1. Display all Products.\n"
                << "2. View Cart.\n"
                << "3. Search Product.\n"
                << "4. Sort Product.\n"
                << "5. Undo Last Cart Action\n"
                << "6. Checkout\n"
                << "0. Logout.\n"
                << "\nPlease Enter Your Choice: ";
            cin >> m_choice;
            switch (m_choice) {
            case 1: {
                cout << "Display All Product Page.\n";
                pc.display_all_product();
                
                
                cout << "\nWould you like to add a product to cart? (1=Yes, 0=No): ";
                int addChoice;
                cin >> addChoice;
                if (addChoice == 1) {
                    int prod_id, qty;
                    cout << "Enter Product ID to add to cart: ";
                    cin >> prod_id;
                    if (prod_id < 0 || prod_id >= pc.p_count) {
                        cout << "Invalid Product ID.\n";
                        break;
                    }
                    cout << "Enter Quantity: ";
                    cin >> qty;
                    if (qty <= 0) {
                        cout << "Invalid quantity.\n";
                        break;
                    }
                    
                    
                    save_cart_snapshot(c_cart, undo_stack);
                    c_cart.add_to_cart(pc.p_list[prod_id], qty);
                }
                break;
            }
            case 2: {
                cout << "View Cart Page.\n";
                c_cart.view_cart();
                
                if (!c_cart.is_empty()) {
                    cout << "\nWould you like to (1=Remove item, 2=Checkout, 0=Back): ";
                    int cartChoice;
                    cin >> cartChoice;
                    if (cartChoice == 1) {
                        int prod_id;
                        cout << "Enter Product ID to remove: ";
                        cin >> prod_id;
                        save_cart_snapshot(c_cart, undo_stack); 
                        if (c_cart.remove_item(prod_id)) {
                            cout << "Product removed from cart.\n";
                        } else {
                            cout << "Product not found in cart.\n";
                        }
                    }
                    else if (cartChoice == 2) {
                        checkout(pc, c_cart);
                    }
                }
                break;
            }
            case 3: {
                cout << "Search Page Product.\n";
                pc.search_product();
                break;
            }
            case 4: {
                cout << "Sort Product Page.\n";
                pc.sort_product();
                break;
            }
            case 5: {
                if (!undo_stack.is_empty()) {
                    
                    c_cart.clear_cart();
                    
                    
                    CartItem* restored = undo_stack.pop();
                    while (restored != nullptr) {
                        
                        c_cart.add_to_cart(pc.p_list[restored->product_id], restored->quantity);
                        
                        
                        CartItem* temp = restored;
                        restored = restored->next;
                        delete temp;
                    }
                    cout << "Last cart action undone.\n";
                } else {
                    cout << "Nothing to undo.\n";
                }
                break;
            }
            case 6: {
                checkout(pc, c_cart);
                break;
            }
            case 0: {
                cout << "Going Back to login page.\n";
                return;
            }
            default: {
                cout << "Invalid Choice! Please try again.\n";
                cin.ignore();
                break;
            }
            }
        } while (m_choice != 0);

    }
    else if (ud[user_i].role == "admin") {
        cout << "You are Logged in as Admin.\n"
            << "----------------\n"
            << "Admin Panel.\n"
            << "----------------\n";

        do {
            cout << "\nPlease Select (1-5): \n"
                << "1. Display all Products.\n"
                << "2. Edit Product.\n"
                << "3. Search Product.\n"
                << "4. Sort Product.\n"
                << "5. Delete Product.\n"
                << "0. Logout.\n"
                << "\nPlease Enter Your Choice: ";

            cin >> m_choice;
            switch (m_choice) {
            case 1: {
                cout << "Display All Product Page.\n";
                pc.display_all_product();
                break;
            }
            case 2: {
                cout << "Edit Product Page.\n";
                pc.edit_product();
                break;
            }
            case 3: {
                cout << "Search Page Product.\n";
                pc.search_product();
                break;
            }
            case 4: {
                cout << "Sort Product Page.\n";
                pc.sort_product();
                break;
            }
            case 5: {
                cout << "Delete Product Page.\n";
                pc.delete_product();
                break;
            }
            case 0: {
                cout << "Going Back to login page.\n";
                return;
            }
            default: {
                cout << "Invalid Choice! Please try again.\n";
                cin.ignore();
                break;
            }
            }
        } while (m_choice != 0);
    }
}

void product_catalog::add_product() {
    if (p_count >= p_MAX) {
        cout << "Product Catalog is Full. Cannot add more products.\n";
        return;
    }

    product& p = p_list[p_count];
    p.p_id = p_count;

    cout << "Enter Product Details: \n"
        << "Name:     ";
    cin.ignore();
    getline(cin, p.p_name);
    cout << "Price:    ";
    cin >> p.p_price;
    cout << "Quantity: ";
    cin >> p.p_quantity;
    cout << "\nProduct Added Successfully.\n"
        << "Product ID: " << p.p_id << endl;
    p_count++;
}

void product_catalog::edit_product() {
    int c_id;
    cout << "Enter Product ID to Edit: ";
    cin >> c_id;
    if (c_id < 0 || c_id >= p_count) {
        cout << "Invalid Product ID.\n";
        return;
    }
    for (int i = 0; i < p_count; i++) {
        if (i == c_id) {
            cout << "Editing Product ID: " << i << endl
                << "Current Name: " << p_list[i].p_name << endl
                << "New Name: ";
            cin.ignore();
            getline(cin, p_list[i].p_name);
            cout << "Current Price: " << p_list[i].p_price << endl
                << "New Price: ";
            cin >> p_list[i].p_price;
            cout << "Current Quantity: " << p_list[i].p_quantity << endl
                << "New Quantity: ";
            cin >> p_list[i].p_quantity;
            cout << "\nProduct Edited Successfully.\n";
            return;
        }
    }
}

void product_catalog::delete_product() {
    int d_id;
    cout << "Enter Product ID to Delete: ";
    cin >> d_id;
    if (d_id < 0 || d_id >= p_count) {
        cout << "Invalid Product ID Entered.\n";
        return;
    }

    for (int i = 0; i < p_count; i++) {
        if (i == d_id) {
            cout << "Deleting Product ID: " << i << endl
                << "Product Name: " << p_list[i].p_name << endl
                << "Product Quantity: " << p_list[i].p_quantity << endl
                << "Product Price: " << p_list[i].p_price << endl
                << "\nAre you sure you want to delete this product? (1/0): \n"
                << "1. Yes\n"
                << "0. No\n"
                << "Enter Choice: ";
            int d_choice;
            cin >> d_choice;
            if (d_choice == 1) {
                for (int j = i; j < p_count - 1; j++) {
                    p_list[j] = p_list[j + 1];
                    p_list[j].p_id = j; 
                }
                p_count--;
                cout << "Product Deleted Successfully.\n";
                return;
            }
            else {
                cout << "Product Deletion Cancelled.\n";
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
void login_user(user_data ud[],user_interface ui, product_catalog &pc, Cart c_cart,UndoStack& undo_stack);

void lock_screen(user_data ud[], user_interface ui, product_catalog &pc, Cart c_cart,UndoStack& undo_stack){

    bool b_entery=true;
    do{
        
        int choice;
        store_intro();
        cout<<"Please Login/Register first to use our store.\n"
            <<"Thanks.\n"<<endl
            <<"\nPlease select (1-3):\n"
            <<"1. Login User.\n"
            <<"2. Register User.\n"
            <<"3. View Credits.\n"
            <<"0. Exit.\n"
            <<"\nPlease Enter Your Choice: ";
        cin>>choice;
        switch(choice){
            case 1:{
                cout<<"login user page.\n";
                login_user(ud,ui,pc,c_cart,undo_stack);
                break;
            }
            case 2:{
                cout<<"resgister user page.\n";
                register_user(ud);
                break;
            }
            case 3:{
                cout<<"Credits Page.\n";
                credit();
                break;
            }
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

void login_user(user_data ud[],user_interface ui, product_catalog &pc, Cart c_cart, UndoStack& undo_stack){
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
                ui.main_menu(pc,ud,i,c_cart,undo_stack);
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

    lock_screen(ud,ui,pc,c_cart,undo_stack);

    write_product_file(pc);
    write_user_file(ud, u_count);

    return 0;
}

void credit(){
    store_intro();
    cout<<"\n-----------------------------------\n"
        <<"Credits:\n"
        <<"-------------------------------------\n"
        <<"This product is developed by: \n"
        <<"    1. Saif-ur-Rehman Awan.   4778-FOC/BSSE/F23\n"
        <<"    2. Abdul Moiz.            4819-FOC/BSSE/F23\n"
        <<"    3. Ahmed Khurseed.        4796-FOC/BSSE/F23\n"
        <<endl
        <<"This project was part of our DS&A Semester Project in 4th semester.\n"
        <<"Under the Supervision of: \"Mr. Shakeel Ahmed\" in IIUI.\n"
        <<"We are thankful to him for his guidance and support.\n"
        <<"-----------------------------------\n"
        <<"In case of any query or suggestion, please contact us at:\n"
        <<"   saif.bsse4778@iiu.edu.pk\n"
        <<"--------------------------------------\n"
        <<"Thank you for using our E-Commerce Store.\n"<<endl;
    system("pause");
}