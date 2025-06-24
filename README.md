# 🛒 SAM E-Commerce Store (C++ Terminal Project)

A terminal-based E-Commerce system built in **C++** as a part of the **Data Structures & Algorithms (DS&A)** semester project at **International Islamic University Islamabad (IIUI)**.  
This project simulates a simplified E-Commerce store with Admin and Customer roles using core Data Structures like Arrays, Linked Lists, and Stacks.

---

## 📌 Features

### 👨‍💼 Admin Panel
- Display all products
- Add/edit/delete products
- Search products by name
- Sort products by price (ascending)
- Persistent product data via file handling

### 👥 Customer Panel
- View and search products
- Add products to cart (Linked List-based)
- View cart and remove items
- Checkout with shipping/payment details
- Undo last cart action (Stack-based)
- Receipt generation and store quantity update

---

## 💾 File Handling

The application uses text files for persistent storage.

| File          | Description                          |
|---------------|--------------------------------------|
| `products.txt`| Stores all product data              |
| `users.txt`   | Stores user credentials and roles    |

- Files are automatically read at program start.
- Data is saved automatically on exit.

---

## 🧱 Data Structures Used

| Feature                 | Data Structure       |
|------------------------|----------------------|
| Cart System            | Singly Linked List   |
| Undo Cart Operations   | Stack (Custom Class) |
| Product Management     | Static Array         |
| File Parsing           | C-style Tokenization |

---

## ▶️ Compilation & Execution

### 🧰 Requirements
- C++ compiler (e.g. `g++`)
- Terminal/Command Line

### 🔧 Compile
g++ -o ecommerce main.cpp

## ▶️ Run
./ecommerce

📁 Make sure products.txt and users.txt are in the same folder.
If not found, the system will create them automatically.

## 🔐 Admin Access

- To access the admin panel, manually add an admin user to users.txt:
   0,admin,admin123,admin
- Then login as:
   Username: admin
   Password: admin123

## 📚 Functional Overview

### 🔄 Cart System
- Add multiple products with quantity
- View total price, individual subtotals
- Undo last cart modification (stack restores previous state)
- Remove specific items from cart

### 📤 Checkout Process
- Prompt for full name, address, payment method
- Generate summary with updated product quantities
- Clear cart after purchase

## 🧑‍💻 Developers
This project was developed by students of BS Software Engineering - 4th Semester as a DS&A Course Project under the supervision of Mr. Shakeel Ahmed at IIUI:

Saif-ur-Rehman Awan
4778-FOC/BSSE/F23

Abdul Moiz
4819-FOC/BSSE/F23

Ahmed Khurseed
4796-FOC/BSSE/F23

## 📬 Contact
For queries, suggestions, or bug reports:

📧 Email: saif.bsse4778@student.iiu.edu.pk

## 📝 License
This project is intended for academic and educational purposes only.
If you wish to use this system commercially or expand upon it, please contact the developers.

## 🙏 Acknowledgment
Special thanks to Mr. Shakeel Ahmed for his guidance, support, and valuable feedback throughout the development of this project.
