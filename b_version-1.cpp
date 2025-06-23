// I'll fix the bugs in your C++ code and implement the checkout system and undo functionality as you requested. Here's the enhanced version:

// ```cpp
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<math.h>
#include<vector>
#include<stack>

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

struct CartItem {
    int product_id;
    string product_name;
    float product_price;
    int quantity;
};

class Cart {
private:
    vector<CartItem> items;
    stack<vector<CartItem>> undoStack;

public:
    void add_to_cart(product p, int qty) {
        // Save current state for undo
        undoStack.push(items);
        
        // Check if product already in cart
        for (auto& item : items) {
            if (item.product_id == p.p_id) {
                item.quantity += qty;
                cout << "Product quantity updated in cart.\n";
                return;
            }
        }
        
        // Add new item
        CartItem newItem;
        newItem.product_id = p.p_id;
        newItem.product_name = p.p_name;
        newItem.product_price = p.p_price;
        newItem.quantity = qty;
        items.push_back(newItem);
        cout << "Product added to cart.\n";
    }

    void view_cart() {
        if (items.empty()) {
            cout << "Cart is empty.\n";
            return;
        }
        
        cout << "\n--- Your Cart ---\n";
        cout << "ID\tName\tPrice\tQty\tSubtotal\n";
        cout << "---------------------------------------\n";
        
        float total = 0;
        for (const auto& item : items) {
            float subtotal = item.product_price * item.quantity;
            cout << item.product_id << "\t" << item.product_name << "\t"
                 << item.product_price << "\t" << item.quantity << "\t$" << subtotal << endl;
            total += subtotal;
        }
        
        cout << "---------------------------------------\n";
        cout << "Total: $" << total << endl;
    }

    bool remove_last_item() {
        if (items.empty()) return false;
        items.pop_back();
        return true;
    }

    bool undo_last_action() {
        if (undoStack.empty()) {
            return false;
        }
        items = undoStack.top();
        undoStack.pop();
        return true;
    }

    void clear_cart() {
        items.clear();
        while (!undoStack.empty()) {
            undoStack.pop();
        }
    }

    float calculate_total() {
        float total = 0;
        for (const auto& item : items) {
            total += item.product_price * item.quantity;
        }
        return total;
    }

    const vector<CartItem>& get_items() const {
        return items;
    }

    bool is_empty() const {
        return items.empty();
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
    void main_menu(product_catalog& pc, user_data ud[], int user_i, Cart& c_cart);
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
        // Process checkout
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
        c_cart.view_cart();
        
        // Update product quantities
        for (const auto& item : c_cart.get_items()) {
            for (int i = 0; i < pc.p_count; i++) {
                if (pc.p_list[i].p_id == item.product_id) {
                    pc.p_list[i].p_quantity -= item.quantity;
                    if (pc.p_list[i].p_quantity < 0) {
                        pc.p_list[i].p_quantity = 0;
                    }
                }
            }
        }
        
        cout << "\nOrder placed successfully!\n";
        cout << "Thank you for shopping with us.\n";
        
        // Clear cart after successful checkout
        c_cart.clear_cart();
    }
    else {
        cout << "Checkout cancelled.\n";
    }
}

void user_interface::main_menu(product_catalog& pc, user_data ud[], int user_i, Cart& c_cart) {
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
                
                // Add to cart functionality
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
                    c_cart.add_to_cart(pc.p_list[prod_id], qty);
                }
                break;
            }
            case 2: {
                cout << "View Cart Page.\n";
                c_cart.view_cart();
                
                if (!c_cart.is_empty()) {
                    cout << "\nWould you like to (1=Undo last action, 2=Checkout, 0=Back): ";
                    int cartChoice;
                    cin >> cartChoice;
                    if (cartChoice == 1) {
                        if (c_cart.undo_last_action()) {
                            cout << "Last action undone.\n";
                        }
                        else {
                            cout << "Nothing to undo.\n";
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
                if (c_cart.undo_last_action()) {
                    cout << "Last cart action undone.\n";
                }
                else {
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
                    p_list[j].p_id = j; // Update the ID to match new position
                }
                p_count--;
                cout << "Product Deleted Successfully.\n";
                return;
            }
            else {
                cout <<