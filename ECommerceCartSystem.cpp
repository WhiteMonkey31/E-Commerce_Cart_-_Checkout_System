
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_USERS = 100;
const int MAX_CART_ITEMS = 100;
const int MAX_STACK = 100;

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

struct User {
    char username[50];
    char password[50];
    bool isAdmin;
};

struct CartItem {
    Product product;
    int quantity;
};

struct Stack {
    CartItem data[MAX_STACK];
    int top = -1;
};

Product products[MAX_PRODUCTS];
int productCount = 0;

User users[MAX_USERS];
int userCount = 0;

CartItem cart[MAX_CART_ITEMS];
int cartCount = 0;

Stack undoStack;

void push(Stack &s, CartItem item) {
    if (s.top < MAX_STACK - 1) {
        s.data[++s.top] = item;
    }
}

CartItem pop(Stack &s) {
    if (s.top >= 0) {
        return s.data[s.top--];
    }
    CartItem empty = {};
    return empty;
}

void saveProducts() {
    ofstream file("products.txt");
    for (int i = 0; i < productCount; i++) {
        file << products[i].id << "," << products[i].name << "," << products[i].price << "," << products[i].quantity << endl;
    }
    file.close();
}

void loadProducts() {
    ifstream file("products.txt");
    productCount = 0;
    while (file >> products[productCount].id) {
        file.ignore();
        file.getline(products[productCount].name, 50, ',');
        file >> products[productCount].price;
        file.ignore();
        file >> products[productCount].quantity;
        file.ignore();
        productCount++;
    }
    file.close();
}

void saveUsers() {
    ofstream file("users.txt");
    for (int i = 0; i < userCount; i++) {
        file << users[i].username << "," << users[i].password << "," << users[i].isAdmin << endl;
    }
    file.close();
}

void loadUsers() {
    ifstream file("users.txt");
    userCount = 0;
    while (file.getline(users[userCount].username, 50, ',')) {
        file.getline(users[userCount].password, 50, ',');
        file >> users[userCount].isAdmin;
        file.ignore();
        userCount++;
    }
    file.close();
}

void saveOrder(const char username[], float total) {
    ofstream file("orders.txt", ios::app);
    file << username << ",";
    for (int i = 0; i < cartCount; i++) {
        file << cart[i].product.name << "(" << cart[i].quantity << "),";
    }
    file << "Total: " << total << endl;
    file.close();
}

void adminMenu();
void customerMenu(const char username[]);

void addProduct() {
    Product p;
    p.id = productCount + 1;
    cout << "Enter product name: ";
    cin.ignore();
    cin.getline(p.name, 50);
    cout << "Enter price: ";
    cin >> p.price;
    cout << "Enter quantity: ";
    cin >> p.quantity;
    products[productCount++] = p;
    saveProducts();
}

void editProduct() {
    int id;
    cout << "Enter product ID to edit: ";
    cin >> id;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            cout << "Enter new name: ";
            cin.ignore();
            cin.getline(products[i].name, 50);
            cout << "Enter new price: ";
            cin >> products[i].price;
            cout << "Enter new quantity: ";
            cin >> products[i].quantity;
            saveProducts();
            return;
        }
    }
    cout << "Product not found!" << endl;
}

void deleteProduct() {
    int id;
    cout << "Enter product ID to delete: ";
    cin >> id;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            for (int j = i; j < productCount - 1; j++) {
                products[j] = products[j + 1];
            }
            productCount--;
            saveProducts();
            return;
        }
    }
    cout << "Product not found!" << endl;
}

void viewProducts() {
    cout << "Available Products:
";
    for (int i = 0; i < productCount; i++) {
        cout << products[i].id << ". " << products[i].name << " - $" << products[i].price << " (" << products[i].quantity << " in stock)" << endl;
    }
}

void addToCart() {
    int id, qty;
    viewProducts();
    cout << "Enter product ID to add to cart: ";
    cin >> id;
    cout << "Enter quantity: ";
    cin >> qty;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id && products[i].quantity >= qty) {
            CartItem item = {products[i], qty};
            cart[cartCount++] = item;
            push(undoStack, item);
            cout << "Added to cart." << endl;
            return;
        }
    }
    cout << "Invalid product ID or insufficient quantity!" << endl;
}

void undoLastAdd() {
    if (undoStack.top >= 0) {
        CartItem last = pop(undoStack);
        for (int i = 0; i < cartCount; i++) {
            if (strcmp(cart[i].product.name, last.product.name) == 0 && cart[i].quantity == last.quantity) {
                for (int j = i; j < cartCount - 1; j++) {
                    cart[j] = cart[j + 1];
                }
                cartCount--;
                cout << "Last item removed from cart." << endl;
                return;
            }
        }
    } else {
        cout << "Nothing to undo!" << endl;
    }
}

void checkout(const char username[]) {
    float total = 0;
    cout << "
Receipt:
";
    for (int i = 0; i < cartCount; i++) {
        cout << cart[i].product.name << " x" << cart[i].quantity << " = $" << cart[i].product.price * cart[i].quantity << endl;
        total += cart[i].product.price * cart[i].quantity;
        for (int j = 0; j < productCount; j++) {
            if (products[j].id == cart[i].product.id) {
                products[j].quantity -= cart[i].quantity;
            }
        }
    }
    cout << "Total: $" << total << endl;
    saveOrder(username, total);
    saveProducts();
    cartCount = 0;
}

void viewOrders() {
    ifstream file("orders.txt");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void adminMenu() {
    int choice;
    do {
        cout << "
1. Add Product
2. Edit Product
3. Delete Product
4. View Products
5. View All Orders
0. Logout
Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addProduct(); break;
            case 2: editProduct(); break;
            case 3: deleteProduct(); break;
            case 4: viewProducts(); break;
            case 5: viewOrders(); break;
        }
    } while (choice != 0);
}

void customerMenu(const char username[]) {
    int choice;
    do {
        cout << "
1. View Products
2. Add to Cart
3. Undo Last Add
4. Checkout
5. View My Orders
0. Logout
Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: viewProducts(); break;
            case 2: addToCart(); break;
            case 3: undoLastAdd(); break;
            case 4: checkout(username); break;
            case 5: viewOrders(); break;
        }
    } while (choice != 0);
}

void login() {
    char uname[50], pass[50];
    cout << "Enter username: ";
    cin >> uname;
    cout << "Enter password: ";
    cin >> pass;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, uname) == 0 && strcmp(users[i].password, pass) == 0) {
            if (users[i].isAdmin)
                adminMenu();
            else
                customerMenu(uname);
            return;
        }
    }
    cout << "Invalid credentials!" << endl;
}

void registerUser() {
    User u;
    cout << "Enter username: ";
    cin >> u.username;
    cout << "Enter password: ";
    cin >> u.password;
    u.isAdmin = false;
    users[userCount++] = u;
    saveUsers();
    cout << "Registration successful!" << endl;
}

int main() {
    loadUsers();
    loadProducts();
    int choice;
    do {
        cout << "
1. Register
2. Login
0. Exit
Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: registerUser(); break;
            case 2: login(); break;
        }
    } while (choice != 0);
    return 0;
}
