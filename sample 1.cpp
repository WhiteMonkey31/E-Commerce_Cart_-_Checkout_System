#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// --- Product Struct ---
struct Product {
    int id;
    string name;
    double price;
    int quantity;
    Product* next;
};

// --- Cart Node (Linked List) ---
struct CartNode {
    Product product;
    int quantity; // quantity added to cart
    CartNode* next;
};

// --- Stack Node for Undo ---
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
};

// --- Cart Class ---
class Cart {
    CartNode* head;
    Stack undoStack; // To store last added product ids for undo
public:
    Cart() : head(nullptr) {}

    void addToCart(Product p, int qty) {
        // Add product to cart linked list or update quantity if exists
        CartNode* temp = head;
        while (temp) {
            if (temp->product.id == p.id) {
                temp->quantity += qty;
                undoStack.push(p.id);
                cout << qty << " more added to existing product in cart.\n";
                return;
            }
            temp = temp->next;
        }
        // If product not found in cart, add new node
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
        CartNode *temp = head, *prev = nullptr;
        while (temp) {
            if (temp->product.id == pid) {
                temp->quantity--;
                cout << "Removed 1 unit of product ID " << pid << " from cart.\n";
                if (temp->quantity <= 0) {
                    // Remove node from linked list
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
            cout << temp->product.id << "\t" << temp->product.name << "\t" 
                 << temp->quantity << "\t" << temp->product.price << "\n";
            temp = temp->next;
        }
    }

    double checkout(Product* productList) {
        if (!head) {
            cout << "Cart empty. Nothing to checkout.\n";
            return 0;
        }
        double total = 0;
        CartNode* temp = head;
        while (temp) {
            total += temp->quantity * temp->product.price;
            // Deduct quantity from productList (store)
            Product* p = productList;
            while (p && p->id != temp->product.id) p = p->next;
            if (p) p->quantity -= temp->quantity;
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
        // Clear undo stack
        while (!undoStack.empty()) undoStack.pop();
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

    ~Cart() {
        clearCart();
    }
};

// --- Function to load products from file (simplified) ---
Product* loadProducts() {
    ifstream file("products.txt");
    if (!file) {
        cout << "products.txt not found. Starting with empty store.\n";
        return nullptr;
    }
    Product* head = nullptr;
    Product* tail = nullptr;
    int id; string name; double price; int qty;
    while (file >> id >> ws && getline(file, name, '\t') && file >> price >> qty) {
        Product* p = new Product{id, name, price, qty, nullptr};
        if (!head) head = p;
        else tail->next = p;
        tail = p;
    }
    file.close();
    return head;
}

// --- Sample main to demonstrate ---
int main() {
    Product* store = nullptr;

    // TODO: Load products from file (implement fully)
    // store = loadProducts();

    // Example products for testing
    Product p1 = {1, "Phone", 500.0, 10, nullptr};
    Product p2 = {2, "Laptop", 1000.0, 5, nullptr};

    Cart cart;

    // Add products to cart
    cart.addToCart(p1, 2);
    cart.addToCart(p2, 1);

    cart.displayCart();

    cart.undoLastAdd();
    cart.displayCart();

    double total = cart.checkout(store);

    return 0;
}
