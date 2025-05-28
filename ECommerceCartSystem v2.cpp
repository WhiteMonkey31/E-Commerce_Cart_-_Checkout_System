
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_USERS = 50;
const int MAX_CART_ITEMS = 50;
const int MAX_ORDER_HISTORY = 100;

// Product class
class Product {
public:
    int id;
    char name[50];
    double price;
    int quantity;

    Product() : id(0), price(0.0), quantity(0) {
        strcpy(name, "");
    }

    void input() {
        cout << "Enter product ID: ";
        cin >> id;
        cout << "Enter product name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter price: ";
        cin >> price;
        cout << "Enter quantity: ";
        cin >> quantity;
    }

    void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Price: " << price << " | Quantity: " << quantity << endl;
    }
};

// User class
class User {
public:
    char username[50];
    char password[50];

    User() {
        strcpy(username, "");
        strcpy(password, "");
    }

    void input() {
        cout << "Enter username: ";
        cin.ignore();
        cin.getline(username, 50);
        cout << "Enter password: ";
        cin.getline(password, 50);
    }
};

// Node for cart linked list
class CartNode {
public:
    Product product;
    int quantity;
    CartNode* next;

    CartNode(Product p, int q) : product(p), quantity(q), next(nullptr) {}
};

// Stack for undo functionality
class UndoStack {
public:
    Product stack[MAX_CART_ITEMS];
    int top;

    UndoStack() : top(-1) {}

    void push(Product p) {
        if (top < MAX_CART_ITEMS - 1)
            stack[++top] = p;
    }

    Product pop() {
        if (top >= 0)
            return stack[top--];
        Product p;
        return p;
    }

    bool isEmpty() const {
        return top == -1;
    }
};

// Order struct
struct Order {
    char username[50];
    Product items[MAX_CART_ITEMS];
    int quantities[MAX_CART_ITEMS];
    int count;
    double total;
};

// Store class
class Store {
    Product products[MAX_PRODUCTS];
    int productCount;
    User users[MAX_USERS];
    int userCount;
    Order orders[MAX_ORDER_HISTORY];
    int orderCount;

public:
    Store() : productCount(0), userCount(0), orderCount(0) {}

    void loadUsers() {
        ifstream file("users.txt");
        userCount = 0;
        while (file >> users[userCount].username >> users[userCount].password) {
            userCount++;
        }
        file.close();
    }

    void loadProducts() {
        ifstream file("products.txt");
        productCount = 0;
        while (file >> products[productCount].id >> products[productCount].name >> products[productCount].price >> products[productCount].quantity) {
            productCount++;
        }
        file.close();
    }

    void saveProducts() {
        ofstream file("products.txt");
        for (int i = 0; i < productCount; i++) {
            file << products[i].id << " " << products[i].name << " " << products[i].price << " " << products[i].quantity << endl;
        }
        file.close();
    }

    void registerUser() {
        if (userCount >= MAX_USERS) {
            cout << "User limit reached.";
            return;
        }
        users[userCount].input();
        ofstream file("users.txt", ios::app);
        file << users[userCount].username << " " << users[userCount].password << endl;
        userCount++;
    }

    bool login(char uname[], char pass[], bool& isAdmin) {
        if (strcmp(uname, "admin") == 0 && strcmp(pass, "admin") == 0) {
            isAdmin = true;
            return true;
        }
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, uname) == 0 && strcmp(users[i].password, pass) == 0) {
                isAdmin = false;
                return true;
            }
        }
        return false;
    }

    void adminPanel() {
        int choice;
        do {
            cout << "\nAdmin Panel:\n1. Add Product\n2. Edit Product\n3. Remove Product\n4. View Products\n5. View Order History\n6. Exit\nEnter choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    if (productCount < MAX_PRODUCTS) {
                        products[productCount].input();
                        productCount++;
                        saveProducts();
                    } else {
                        cout << "Product limit reached.\n";
                    }
                    break;
                case 2: {
                    int id, found = 0;
                    cout << "Enter product ID to edit: ";
                    cin >> id;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id) {
                            products[i].input();
                            found = 1;
                            break;
                        }
                    }
                    if (!found) cout << "Product not found.\n";
                    else saveProducts();
                    break;
                }
                case 3: {
                    int id, found = 0;
                    cout << "Enter product ID to remove: ";
                    cin >> id;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id) {
                            for (int j = i; j < productCount - 1; j++)
                                products[j] = products[j + 1];
                            productCount--;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) cout << "Product not found.\n";
                    else saveProducts();
                    break;
                }
                case 4:
                    for (int i = 0; i < productCount; i++)
                        products[i].display();
                    break;
                case 5:
                    for (int i = 0; i < orderCount; i++) {
                        cout << "Order by: " << orders[i].username << " | Total: " << orders[i].total << endl;
                        for (int j = 0; j < orders[i].count; j++) {
                            cout << "  " << orders[i].items[j].name << " x" << orders[i].quantities[j] << endl;
                        }
                    }
                    break;
            }
        } while (choice != 6);
    }

    void customerPanel(const char* username) {
        CartNode* cart = nullptr;
        UndoStack undoStack;
        int choice;
        do {
            cout << "\nCustomer Panel:\n1. View Products\n2. Add to Cart\n3. View Cart\n4. Undo Last Add\n5. Checkout\n6. Exit\nEnter choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    for (int i = 0; i < productCount; i++)
                        products[i].display();
                    break;
                case 2: {
                    int id, qty;
                    cout << "Enter product ID: ";
                    cin >> id;
                    cout << "Enter quantity: ";
                    cin >> qty;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id && products[i].quantity >= qty) {
                            CartNode* newNode = new CartNode(products[i], qty);
                            newNode->next = cart;
                            cart = newNode;
                            undoStack.push(products[i]);
                            break;
                        }
                    }
                    break;
                }
                case 3: {
                    CartNode* temp = cart;
                    while (temp) {
                        cout << temp->product.name << " x" << temp->quantity << endl;
                        temp = temp->next;
                    }
                    break;
                }
                case 4:
                    if (!undoStack.isEmpty()) {
                        Product last = undoStack.pop();
                        CartNode* temp = cart, *prev = nullptr;
                        while (temp && temp->product.id != last.id) {
                            prev = temp;
                            temp = temp->next;
                        }
                        if (temp) {
                            if (prev) prev->next = temp->next;
                            else cart = temp->next;
                            delete temp;
                        }
                    }
                    break;
                case 5: {
                    double total = 0.0;
                    Order order;
                    strcpy(order.username, username);
                    order.count = 0;
                    while (cart) {
                        total += cart->product.price * cart->quantity;
                        order.items[order.count] = cart->product;
                        order.quantities[order.count] = cart->quantity;
                        order.count++;
                        for (int i = 0; i < productCount; i++) {
                            if (products[i].id == cart->product.id)
                                products[i].quantity -= cart->quantity;
                        }
                        CartNode* temp = cart;
                        cart = cart->next;
                        delete temp;
                    }
                    order.total = total;
                    orders[orderCount++] = order;
                    saveProducts();
                    cout << "Checkout successful. Total: " << total << endl;
                    break;
                }
            }
        } while (choice != 6);
    }
};

int main() {
    Store store;
    store.loadUsers();
    store.loadProducts();
    int option;
    do {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> option;
        if (option == 1) store.registerUser();
        else if (option == 2) {
            char uname[50], pass[50];
            bool isAdmin = false;
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pass;
            if (store.login(uname, pass, isAdmin)) {
                if (isAdmin) store.adminPanel();
                else store.customerPanel(uname);
            } else {
                cout << "Invalid login.\n";
            }
        }
    } while (option != 3);

    return 0;
}
