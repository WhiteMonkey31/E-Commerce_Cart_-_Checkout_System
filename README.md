# SAM E-Commerce Cart & Checkout System

# Store Management System

A C++ console application for managing a store with admin and customer functionalities.

## Features

### Admin Features
- Add new products
- Edit existing products
- Remove products
- View all products
- View order history

### Customer Features
- View available products
- Add products to cart
- View cart
- Undo last add to cart
- Checkout

## Default Admin Credentials
- Username: admin
- Password: admin

## How to Compile and Run

1. Make sure you have a C++ compiler installed (e.g., g++)
2. Compile the program:
   ```bash
   g++ store.cpp -o store
   ```
3. Run the program:
   ```bash
   ./store
   ```

## File Structure
- `store.cpp` - Main source code
- `users.txt` - Stores user credentials
- `products.txt` - Stores product information

## Usage

1. When you first run the program, you'll see three options:
   - Register (for new customers)
   - Login (for existing users and admin)
   - Exit

2. For admin access:
   - Login with username "admin" and password "admin"
   - You can then manage products and view orders

3. For customers:
   - Register first if you're a new user
   - Login with your credentials
   - Browse products and manage your cart

## Data Persistence
- User accounts are stored in `users.txt`
- Product information is stored in `products.txt`
- Both files are created automatically when needed 
