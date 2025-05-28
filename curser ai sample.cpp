#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <limits>

using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_USERS = 50;
const int MAX_CART_ITEMS = 50;
const int MAX_ORDER_HISTORY = 100;
const int MAX_ITEMS_PER_ORDER = 20;

// Forward declarations
class Store;
class Cart;

// Product class with enhanced OOP
class Product {
private:
    int id;
    char name[50];
    double price;
    int quantity;

public:
    Product() : id(0), price(0.0), quantity(0) {
        strcpy(name, "");
    }

    // Getters
    int getId() const { return id; }
    const char* getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters
    void setId(int i) { id = i; }
    void setName(const char* n) { strcpy(name, n); }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }

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
        cout << "ID: " << id << " | Name: " << name 
             << " | Price: " << fixed << setprecision(2) << price 
             << " | Quantity: " << quantity << endl;
    }

    friend class Store;
    friend class Cart;
};

// User class with enhanced OOP
class User {
private:
    char username[50];
    char password[50];
    bool isAdmin;

public:
    User() : isAdmin(false) {
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

    const char* getUsername() const { return username; }
    bool getIsAdmin() const { return isAdmin; }
    void setIsAdmin(bool admin) { isAdmin = admin; }

    friend class Store;
};

// Order class with enhanced OOP
class Order {
private:
    char username[50];
    Product items[MAX_ITEMS_PER_ORDER];
    int quantities[MAX_ITEMS_PER_ORDER];
    int count;
    double total;

public:
    Order() : count(0), total(0.0) {
        strcpy(username, "");
    }

    void setUsername(const char* uname) { strcpy(username, uname); }
    void addItem(const Product& p, int qty) {
        if (count < MAX_ITEMS_PER_ORDER) {
            items[count] = p;
            quantities[count] = qty;
            count++;
        }
    }

    double getTotal() const { return total; }
    void setTotal(double t) { total = t; }
    const char* getUsername() const { return username; }
    int getCount() const { return count; }
    
    void display() const {
        cout << "Order by: " << username << " | Total: $" << fixed << setprecision(2) << total << endl;
        for (int i = 0; i < count; i++) {
            cout << "  " << items[i].getName() << " x" << quantities[i] << endl;
        }
    }

    friend class Store;
};

// CartNode for linked list implementation
class CartNode {
public:
    Product product;
    int quantity;
    CartNode* next;

    CartNode(const Product& p, int q) : product(p), quantity(q), next(nullptr) {}
};

// UndoStack implementation
class UndoStack {
private:
    Product stack[MAX_CART_ITEMS];
    int top;

public:
    UndoStack() : top(-1) {}

    void push(const Product& p) {
        if (top < MAX_CART_ITEMS - 1)
            stack[++top] = p;
    }

    Product pop() {
        if (top >= 0)
            return stack[top--];
        return Product();
    }

    bool isEmpty() const { return top == -1; }
};

// Cart class with enhanced OOP
class Cart {
private:
    CartNode* head;
    UndoStack undoStack;

public:
    Cart() : head(nullptr) {}

    void addToCart(const Product& p, int qty) {
        CartNode* temp = head;
        while (temp) {
            if (temp->product.getId() == p.getId()) {
                temp->quantity += qty;
                undoStack.push(p);
                cout << qty << " more " << p.getName() << " added to cart.\n";
                return;
            }
            temp = temp->next;
        }
        CartNode* newNode = new CartNode(p, qty);
        newNode->next = head;
        head = newNode;
        undoStack.push(p);
        cout << qty << " " << p.getName() << " added to cart.\n";
    }

    void undoLastAdd() {
        if (undoStack.isEmpty()) {
            cout << "Nothing to undo!\n";
            return;
        }
        Product last = undoStack.pop();
        CartNode* temp = head;
        CartNode* prev = nullptr;
        while (temp) {
            if (temp->product.getId() == last.getId()) {
                temp->quantity--;
                if (temp->quantity <= 0) {
                    if (prev) prev->next = temp->next;
                    else head = temp->next;
                    delete temp;
                }
                cout << "Removed 1 unit of " << last.getName() << " from cart.\n";
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    void display() const {
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "\nCart Contents:\n";
        CartNode* temp = head;
        while (temp) {
            cout << temp->product.getName() << " x" << temp->quantity 
                 << " ($" << temp->product.getPrice() << " each)\n";
            temp = temp->next;
        }
    }

    Order checkout(Store& store);

    ~Cart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    friend class Store;
};

// Store class with enhanced OOP
class Store {
private:
    Product products[MAX_PRODUCTS];
    int productCount;
    User users[MAX_USERS];
    int userCount;
    Order orders[MAX_ORDER_HISTORY];
    int orderCount;

public:
    Store() : productCount(0), userCount(0), orderCount(0) {
        // Add default admin user
        strcpy(users[userCount].username, "admin");
        strcpy(users[userCount].password, "admin");
        users[userCount].isAdmin = true;
        userCount++;
    }

    void loadUsers() {
        ifstream file("users.txt");
        userCount = 1; // Keep admin user
        while (file >> users[userCount].username >> users[userCount].password) {
            users[userCount].isAdmin = false;
            userCount++;
        }
        file.close();
    }

    void saveUsers() const {
        ofstream file("users.txt");
        for (int i = 1; i < userCount; i++) { // Skip admin user
            file << users[i].username << " " << users[i].password << endl;
        }
        file.close();
    }

    void loadProducts() {
        ifstream file("products.txt");
        productCount = 0;
        while (file >> products[productCount].id >> products[productCount].name 
               >> products[productCount].price >> products[productCount].quantity) {
            productCount++;
        }
        file.close();
    }

    void saveProducts() const {
        ofstream file("products.txt");
        for (int i = 0; i < productCount; i++) {
            file << products[i].id << " " << products[i].name << " "
                 << products[i].price << " " << products[i].quantity << endl;
        }
        file.close();
    }

    void registerUser() {
        if (userCount >= MAX_USERS) {
            cout << "User limit reached.\n";
            return;
        }
        users[userCount].input();
        users[userCount].isAdmin = false;
        userCount++;
        saveUsers();
        cout << "Registration successful.\n";
    }

    User* login(const char* uname, const char* pass) {
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, uname) == 0 && 
                strcmp(users[i].password, pass) == 0) {
                return &users[i];
            }
        }
        return nullptr;
    }

    void adminPanel() {
        int choice;
        do {
            cout << "\nAdmin Panel:\n"
                 << "1. Add Product\n2. Edit Product\n3. Remove Product\n"
                 << "4. View Products\n5. View Order History\n6. Exit\n"
                 << "Enter choice: ";
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
                    int id;
                    cout << "Enter product ID to edit: ";
                    cin >> id;
                    bool found = false;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id) {
                            products[i].input();
                            found = true;
                            saveProducts();
                            break;
                        }
                    }
                    if (!found) cout << "Product not found.\n";
                    break;
                }

                case 3: {
                    int id;
                    cout << "Enter product ID to remove: ";
                    cin >> id;
                    bool found = false;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id) {
                            for (int j = i; j < productCount - 1; j++)
                                products[j] = products[j + 1];
                            productCount--;
                            found = true;
                            saveProducts();
                            break;
                        }
                    }
                    if (!found) cout << "Product not found.\n";
                    break;
                }

                case 4:
                    cout << "\nProduct List:\n";
                    for (int i = 0; i < productCount; i++)
                        products[i].display();
                    break;

                case 5:
                    cout << "\nOrder History:\n";
                    for (int i = 0; i < orderCount; i++)
                        orders[i].display();
                    break;
            }
        } while (choice != 6);
    }

    void customerPanel(const char* username) {
        Cart cart;
        int choice;
        do {
            cout << "\nCustomer Panel:\n"
                 << "1. View Products\n2. Add to Cart\n3. View Cart\n"
                 << "4. Undo Last Add\n5. Checkout\n6. Exit\n"
                 << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "\nAvailable Products:\n";
                    for (int i = 0; i < productCount; i++)
                        products[i].display();
                    break;

                case 2: {
                    int id, qty;
                    cout << "Enter product ID: ";
                    cin >> id;
                    cout << "Enter quantity: ";
                    cin >> qty;
                    bool found = false;
                    for (int i = 0; i < productCount; i++) {
                        if (products[i].id == id && products[i].quantity >= qty) {
                            cart.addToCart(products[i], qty);
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Product not found or insufficient quantity.\n";
                    break;
                }

                case 3:
                    cart.display();
                    break;

                case 4:
                    cart.undoLastAdd();
                    break;

                case 5: {
                    Order order = cart.checkout(*this);
                    if (order.getTotal() > 0) {
                        order.setUsername(username);
                        orders[orderCount++] = order;
                        saveProducts();
                    }
                    break;
                }
            }
        } while (choice != 6);
    }

    friend class Cart;
};

// Implementation of Cart::checkout now that Store is defined
Order Cart::checkout(Store& store) {
    Order order;
    if (!head) {
        cout << "Cart is empty.\n";
        return order;
    }

    double total = 0.0;
    CartNode* temp = head;
    int itemCount = 0;

    while (temp && itemCount < MAX_ITEMS_PER_ORDER) {
        bool found = false;
        for (int i = 0; i < store.productCount; i++) {
            if (store.products[i].id == temp->product.id) {
                if (store.products[i].quantity >= temp->quantity) {
                    store.products[i].quantity -= temp->quantity;
                    total += temp->quantity * temp->product.price;
                    order.addItem(temp->product, temp->quantity);
                    found = true;
                } else {
                    cout << "Not enough quantity for " << temp->product.name << endl;
                    return Order();
                }
                break;
            }
        }
        if (!found) {
            cout << "Product " << temp->product.name << " no longer available.\n";
            return Order();
        }
        temp = temp->next;
        itemCount++;
    }

    order.setTotal(total);
    cout << "Checkout successful. Total: $" << fixed << setprecision(2) << total << endl;

    // Clear cart after successful checkout
    while (head) {
        CartNode* temp = head;
        head = head->next;
        delete temp;
    }

    return order;
}

int main() {
    Store store;
    store.loadUsers();
    store.loadProducts();

    int option;
    do {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            store.registerUser();
        }
        else if (option == 2) {
            char username[50], password[50];
            cout << "Enter username: ";
            cin.getline(username, 50);
            cout << "Enter password: ";
            cin.getline(password, 50);

            User* user = store.login(username, password);
            if (user) {
                if (user->getIsAdmin()) {
                    store.adminPanel();
                } else {
                    store.customerPanel(user->getUsername());
                }
            } else {
                cout << "Invalid login.\n";
            }
        }
    } while (option != 3);

    cout << "Thank you for using our system!\n";
    return 0;
} 