#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

// ------------------- Data Structures -------------------

struct Product {
    int id;
    string name;
    double price;
    int quantity;
};

struct User {
    string username;
    string password;
    string role; // "admin" or "customer"
};

struct Order {
    string username;
    vector<Product> items;
    double totalAmount;
};

// ------------------- Utility Functions -------------------

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pause() {
    cout << "Press Enter to continue...";
    cin.get();
}

// ------------------- File Handling -------------------

vector<User> loadUsers(const string& filename) {
    vector<User> users;
    ifstream file(filename);
    if (!file) return users;

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        User u;
        getline(ss, u.username, ',');
        getline(ss, u.password, ',');
        getline(ss, u.role);
        users.push_back(u);
    }
    return users;
}

void saveUsers(const string& filename, const vector<User>& users) {
    ofstream file(filename);
    for (auto& u : users) {
        file << u.username << "," << u.password << "," << u.role << "\n";
    }
}

vector<Product> loadProducts(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    if (!file) return products;

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Product p;
        string priceStr, qtyStr;
        getline(ss, line, ','); p.id = stoi(line);
        getline(ss, p.name, ',');
        getline(ss, priceStr, ','); p.price = stod(priceStr);
        getline(ss, qtyStr); p.quantity = stoi(qtyStr);
        products.push_back(p);
    }
    return products;
}

void saveProducts(const string& filename, const vector<Product>& products) {
    ofstream file(filename);
    for (auto& p : products) {
        file << p.id << "," << p.name << "," << p.price << "," << p.quantity << "\n";
    }
}

vector<Order> loadOrders(const string& filename) {
    vector<Order> orders;
    ifstream file(filename);
    if (!file) return orders;

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Order o;
        getline(ss, o.username, '|');
        string itemsStr;
        getline(ss, itemsStr, '|');
        istringstream itemsStream(itemsStr);
        string itemPart;
        while (getline(itemsStream, itemPart, ';')) {
            istringstream itemSS(itemPart);
            Product p;
            string priceStr, qtyStr;
            getline(itemSS, line, ','); p.id = stoi(line);
            getline(itemSS, p.name, ',');
            getline(itemSS, priceStr, ','); p.price = stod(priceStr);
            getline(itemSS, qtyStr); p.quantity = stoi(qtyStr);
            o.items.push_back(p);
        }
        getline(ss, line);
        o.totalAmount = stod(line);
        orders.push_back(o);
    }
    return orders;
}

void saveOrders(const string& filename, const vector<Order>& orders) {
    ofstream file(filename);
    for (auto& o : orders) {
        file << o.username << "|";
        for (size_t i = 0; i < o.items.size(); i++) {
            file << o.items[i].id << "," << o.items[i].name << "," << o.items[i].price << "," << o.items[i].quantity;
            if (i != o.items.size() - 1) file << ";";
        }
        file << "|" << o.totalAmount << "\n";
    }
}

// ------------------- Sorting -------------------

void bubbleSortProductsByPrice(vector<Product>& products) {
    bool swapped;
    int n = products.size();
    for (int i = 0; i < n-1; i++) {
        swapped = false;
        for (int j = 0; j < n-i-1; j++) {
            if (products[j].price > products[j+1].price) {
                swap(products[j], products[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ------------------- Cart Implementation -------------------

struct CartNode {
    Product product;
    int quantity;
    CartNode* next;
};

struct StackNode {
    int productId;
    StackNode* next;
};

class Stack {
    StackNode* top;
public:
    Stack() : top(nullptr) {}
    void push(int pid) {
        StackNode* temp = new StackNode{pid, top};
        top = temp;
    }
    int pop() {
        if (!top) return -1;
        int pid = top->productId;
        StackNode* temp = top;
        top = top->next;
        delete temp;
        return pid;
    }
    bool empty() { return top == nullptr; }
    void clear() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    ~Stack() { clear(); }
};

class Cart {
    CartNode* head;
    Stack undoStack;

public:
    Cart() : head(nullptr) {}

    void addToCart(const Product& p, int qty) {
        CartNode* temp = head;
        while (temp) {
            if (temp->product.id == p.id) {
                temp->quantity += qty;
                undoStack.push(p.id);
                cout << qty << " more " << p.name << " added to cart.\n";
                return;
            }
            temp = temp->next;
        }
        CartNode* newNode = new CartNode{p, qty, head};
        head = newNode;
        undoStack.push(p.id);
        cout << qty << " " << p.name << " added to cart.\n";
    }

    void undoLastAdd() {
        if (undoStack.empty()) {
            cout << "Nothing to undo!\n";
            return;
        }
        int pid = undoStack.pop();
        CartNode* temp = head;
        CartNode* prev = nullptr;
        while (temp) {
            if (temp->product.id == pid) {
                temp->quantity--;
                cout << "Removed 1 unit of product ID " << pid << " from cart.\n";
                if (temp->quantity <= 0) {
                    if (prev) prev->next = temp->next;
                    else head = temp->next;
                    delete temp;
                }
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    void displayCart() {
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "Cart Items:\nID\tName\tQty\tPrice\n";
        CartNode* temp = head;
        while (temp) {
            cout << temp->product.id << "\t" << temp->product.name << "\t" << temp->quantity << "\t" << temp->product.price << "\n";
            temp = temp->next;
        }
    }

    double checkout(vector<Product>& products) {
        if (!head) {
            cout << "Cart empty. Nothing to checkout.\n";
            return 0;
        }
        double total = 0;
        CartNode* temp = head;
        // Check if all quantities are available
        while (temp) {
            bool available = false;
            for (auto& p : products) {
                if (p.id == temp->product.id) {
                    if (p.quantity < temp->quantity) {
                        cout << "Not enough quantity for product " << p.name << ". Checkout aborted.\n";
                        return -1;
                    }
                    available = true;
                    break;
                }
            }
            if (!available) {
                cout << "Product with ID " << temp->product.id << " not found in store. Checkout aborted.\n";
                return -1;
            }
            temp = temp->next;
        }
        // Deduct quantities
        temp = head;
        while (temp) {
            for (auto& p : products) {
                if (p.id == temp->product.id) {
                    p.quantity -= temp->quantity;
                    break;
                }
            }
            total += temp->quantity * temp->product.price;
            temp = temp->next;
        }
        cout << fixed << setprecision(2);
        cout << "Total Amount: $" << total << "\n";

        generateReceipt(total);
        clearCart();
        return total;
    }

    void clearCart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
        undoStack.clear();
    }

    void generateReceipt(double total) {
        ofstream receiptFile("receipt.txt");
        receiptFile << "----- RECEIPT -----\n";
        CartNode* temp = head;
        receiptFile << "Items:\nID\tName\tQty\tPrice\n";
        while (temp) {
            receiptFile << temp->product.id << "\t" << temp->product.name << "\t"
                        << temp->quantity << "\t" << temp->product.price << "\n";
            temp = temp->next;
        }
        receiptFile << "Total Amount Paid: $" << total << "\n";
        receiptFile << "Thank you for shopping!\n";
        receiptFile.close();
        cout << "Receipt generated in receipt.txt\n";
    }

    vector<Product> getCartProducts() {
        vector<Product> result;
        CartNode* temp = head;
        while (temp) {
            Product p = temp->product;
            p.quantity = temp->quantity; // Quantity in cart
            result.push_back(p);
            temp = temp->next;
        }
        return result;
    }

    ~Cart() {
        clearCart();
    }
};

// ------------------- Admin Functions -------------------

void adminAddProduct(vector<Product>& products) {
    Product p;
    cout << "Enter product id: ";
    cin >> p.id;
    clearInput();

    for (auto& prod : products) {
        if (prod.id == p.id) {
            cout << "Product with this ID already exists.\n";
            return;
        }
    }

    cout << "Enter product name: ";
    getline(cin, p.name);
    cout << "Enter price: ";
    cin >> p.price;
    cout << "Enter quantity: ";
    cin >> p.quantity;
    clearInput();

    products.push_back(p);
    cout << "Product added successfully.\n";
}

void adminEditProduct(vector<Product>& products) {
    int id;
    cout << "Enter product ID to edit: ";
    cin >> id;
    clearInput();

    for (auto& p : products) {
        if (p.id == id) {
            cout << "Editing product: " << p.name << "\n";
            cout << "Enter new name (or press Enter to keep current): ";
            string newName;
            getline(cin, newName);
            if (!newName.empty()) p.name = newName;

            cout <<"Enter new price (or -1 to keep current): ";
            double newPrice;
            cin >> newPrice;
            clearInput();
            if (newPrice >= 0) p.price = newPrice;
                    cout << "Enter new quantity (or -1 to keep current): ";
        int newQty;
        cin >> newQty;
        clearInput();
        if (newQty >= 0) p.quantity = newQty;

        cout << "Product updated.\n";
        return;
    }
}
cout << "Product not found.\n";
}

void adminRemoveProduct(vector<Product>& products) {
int id;
cout << "Enter product ID to remove: ";
cin >> id;
clearInput();
for (auto it = products.begin(); it != products.end(); ++it) {
    if (it->id == id) {
        products.erase(it);
        cout << "Product removed.\n";
        return;
    }
}
cout << "Product not found.\n";
}

void adminViewUsers(const vector<User>& users) {
cout << "Registered Users:\nUsername\tRole\n";
for (auto& u : users) {
cout << u.username << "\t" << u.role << "\n";
}
}

void adminViewOrders(const vector<Order>& orders) {
cout << "Order History:\n";
for (auto& o : orders) {
cout << "User: " << o.username << "\nItems:\n";
for (auto& p : o.items) {
cout << " - " << p.name << " x" << p.quantity << " @ $" << p.price << "\n";
}
cout << "Total: $" << o.totalAmount << "\n\n";
}
}

void adminSalesReport(const vector<Order>& orders) {
double totalSales = 0;
for (auto& o : orders) {
totalSales += o.totalAmount;
}
cout << fixed << setprecision(2);
cout << "Total sales amount: $" << totalSales << "\n";
ofstream reportFile("sales_report.txt");
reportFile << "Total Sales Report\n";
reportFile << "------------------\n";
reportFile << "Total Sales: $" << totalSales << "\n";
reportFile.close();

cout << "Sales report saved to sales_report.txt\n";
}

// ------------------- User Login -------------------

User* loginUser(const vector<User>& users) {
string username, password;
cout << "Username: ";
getline(cin, username);
cout << "Password: ";
getline(cin, password);
for (auto& u : users) {
    if (u.username == username && u.password == password) {
        cout << "Login successful. Role: " << u.role << "\n";
        return new User(u);
    }
}
cout << "Invalid credentials.\n";
return nullptr;
}

bool registerUser(vector<User>& users) {
string username, password, role = "customer";
cout << "Enter username: ";
getline(cin, username);
for (auto& u : users) {
    if (u.username == username) {
        cout << "Username already exists.\n";
        return false;
    }
}
cout << "Enter password: ";
getline(cin, password);

users.push_back(User{username, password, role});
cout << "Registration successful. You can now log in.\n";
return true;
}

// ------------------- Customer Functions -------------------

void displayProducts(const vector<Product>& products) {
cout << "Available Products:\nID\tName\tPrice\tQuantity\n";
for (auto& p : products) {
cout << p.id << "\t" << p.name << "\t" << p.price << "\t" << p.quantity << "\n";
}
}

void customerViewOrderHistory(const string& username, const vector<Order>& orders) {
cout << "Your Order History:\n";
for (auto& o : orders) {
if (o.username == username) {
cout << "Order:\n";
for (auto& p : o.items) {
cout << " - " << p.name << " x" << p.quantity << " @ $" << p.price << "\n";
}
cout << "Total: $" << o.totalAmount << "\n\n";
}
}
}

// ------------------- Main Program -------------------

int main() {
vector<User> users = loadUsers("users.txt");
vector<Product> products = loadProducts("products.txt");
vector<Order> orders = loadOrders("orders.txt");
cout << "Welcome to the E-Commerce Cart and Checkout System\n";

User* currentUser = nullptr;

while (!currentUser) {
    cout << "\n1. Login\n2. Register\n3. Exit\nChoose option: ";
    int choice;
    cin >> choice;
    clearInput();

    if (choice == 1) {
        currentUser = loginUser(users);
    } else if (choice == 2) {
        if (registerUser(users)) {
            saveUsers("users.txt", users);
        }
    } else if (choice == 3) {
        cout << "Exiting...\n";
        return 0;
    } else {
        cout << "Invalid choice.\n";
    }
}

if (currentUser->role == "admin") {
    int option;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Product\n2. Edit Product\n3. Remove Product\n4. View Products\n5. View Users\n6. View Orders\n7. Sales Report\n8. Logout\nChoose option: ";
        cin >> option;
        clearInput();

        switch(option) {
            case 1: adminAddProduct(products); saveProducts("products.txt", products); break;
            case 2: adminEditProduct(products); saveProducts("products.txt", products); break;
            case 3: adminRemoveProduct(products); saveProducts("products.txt", products); break;
            case 4: displayProducts(products); break;
            case 5: adminViewUsers(users); break;
            case 6: adminViewOrders(orders); break;
            case 7: adminSalesReport(orders); break;
            case 8: cout << "Logging out...\n"; break;
            default: cout << "Invalid option.\n"; break;
        }
        if (option != 8) pause();
    } while(option != 8);
} else if (currentUser->role == "customer") {
    Cart cart;
    int option;
    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View Products\n2. Sort Products by Price\n3. Add Product to Cart\n4. Undo Last Add\n5. View Cart\n6. Checkout\n7. View Order History\n8. Logout\nChoose option: ";
        cin >> option;
        clearInput();

        switch(option) {
            case 1: displayProducts(products); break;
            case 2: 
                bubbleSortProductsByPrice(products);
                cout << "Products sorted by price.\n";
                displayProducts(products);
                break;
            case 3: {
                int pid, qty;
                cout << "Enter Product ID to add: ";
                cin >> pid;
                cout << "Enter quantity: ";
                cin >> qty;
                clearInput();
                bool found = false;
                for (auto& p : products) {
                    if (p.id == pid) {
                        if (qty <= 0) {
                            cout << "Quantity must be positive.\n";
                            found = true;
                            break;
                        }
                        if (p.quantity < qty) {
                            cout << "Not enough stock.\n";
                            found = true;
                            break;
                        }
                        cart.addToCart(p, qty);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Product not found.\n";
                break;
            }
            case 4: cart.undoLastAdd(); break;
            case 5: cart.displayCart(); break;
            case 6: {
                double total = cart.checkout(products);
                if (total >= 0) {
                    Order newOrder;
                    newOrder.username = currentUser->username;
                    newOrder.items = cart.getCartProducts();
                    newOrder.totalAmount = total;
                    orders.push_back(newOrder);
                    saveOrders("orders.txt", orders);
                    saveProducts("products.txt", products);
                }
                break;
            }
            case 7: customerViewOrderHistory(currentUser->username, orders); break;
            case 8: cout << "Logging out...\n"; break;
            default: cout << "Invalid option.\n"; break;
        }
        if (option != 8) pause();
    } while(option != 8);
}

delete currentUser;
cout << "Goodbye!\n";
return 0;
}
