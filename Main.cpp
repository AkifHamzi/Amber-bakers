#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <limits>

using namespace std;


struct Product {
    string name;
    double price;
    int stock;
};



struct Admin{
    string userName;
    string password;
};

struct Customer {
    string userName;
    string password;
     
    
};


struct CartItem {
    string productName;
    int quantity;
    double price;
  

};

struct Cart {
    vector<CartItem> items;


    void addItem(string name, int qty, double price) {
        for(auto& item : items) {
            if(item.productName == name) {
                item.quantity += qty;   
                return;
            }
        }

        items.push_back({name, qty, price});

    }



    void printCart() {
        if (items.empty()) {
            cout << "\nYour cart is empty." << endl;
            return;
        }

        cout << "\n-----------| YOUR CART |-----------" << endl;
        cout << left << setw(25) << "Item" << setw(15) << "Quantity" << setw(10) << "Price" << endl;
        cout << "----------------------------------------------------" << endl;
        double total = 0.0;
        for (const auto& item : items) {
            cout << left << setw(25) << item.productName
                 << setw(15) << item.quantity
                 << fixed << setprecision(2) << "GBP" << item.price * item.quantity << endl;
            total += item.price * item.quantity;
        }
        cout << "----------------------------------------------------" << endl;
        cout << "TOTAL: GBP" << fixed << setprecision(2) << total << endl;
    }

};

void adminLogin();
void adminMenu();
void customerLogin();
void customerMenu(const string& customerUsername);
void aboutUs();
vector<Product> readProductsFromFile();
void writeProductsToFile(const vector<Product>& products);
void viewProducts();
void addProduct();
bool searchProduct(const string& searchName, Product& foundProduct);
void editProduct();
void deleteProduct();
void purchase(Cart& cart);
int getIntegerInput();



int main() {
    int choice;

    // loop will continue until the user chooses option 4 to exit.
    do {
        cout << "\n================| WELCOME TO AMBER BAKERS UK |================" << endl;
        cout << "\n1. Login as an Admin" << endl;
        cout << "2. Login as a Customer." << endl;
        cout << "3. About us" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        choice = getIntegerInput(); //this will be the users choice

        switch(choice) {
            case 1:
                adminLogin(); // After admin logs out, this function ends, and the loop restarts.
                break;
            case 2:
                customerLogin(); // Same thing as adminLogin()
                break;
            case 3:
                aboutUs();
                break;
            case 4:
                cout << "Thank you for visiting Amber Bakers!";
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 4); // The loop only stops when the user enters 4.

    return 0;
}

int getIntegerInput() {
    int choice;
    cin >> choice;

    // Check if the user entered something that isn't a number.
    while (cin.fail()) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear(); // Clear the error flag.
        // Discard the rest of the line to avoid an infinite loop.
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cin >> choice;
    }
    
    // Discard the rest of the line in case the user entered "1 abc"
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void aboutUs() {
    cout << "\t\tAMBER BAKERS\n" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "SERVING YOU SINCE 2001" << endl;
    cout << "\nAmber Bakers UK is your favorite neighborhood bakery in Cardiff!" <<endl;
    cout << "We specialize in fresh bread, cakes, and pastries.\n" << endl;
    cout << "Contact us: " << endl;
    cout << "Phone: +44 982-928-291" << endl;
    cout << "Email: AmberBakers@Gmail.com" << endl;
    cout << "Website: www.amberbakers.co.uk" << endl;

}

vector<Product> readProductsFromFile() {
    ifstream file("products.txt");
    vector<Product> products;
    string line;

    while (getline(file, line)) {
        // If the line is empty, skip it to prevent errors.
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        vector<string> words;
        string word;

        // 1. Read all the "words" from the line into a vector.
        // Example: "Carrot Cake Slice 3.50 15" -> {"Carrot", "Cake", "Slice", "3.50", "15"}
        while (ss >> word) {
            words.push_back(word);
        }
            if (words.size() < 3) continue; // Skip malformed lines
        Product p;
        
        // 2. The last word is the stock.
        // We use stod/stoi to convert string to double/int.
        p.stock = stoi(words.back());
        words.pop_back(); // Remove it from the vector

        // 3. The new last word is the price.
        p.price = stod(words.back());
        words.pop_back(); // Remove it from the vector

        // 4. Everything left in the vector is the product name.
        // We re-join the remaining words with spaces.
        string name;
        for (const auto& w : words) {
            name += w + " ";
        }
        // Remove the final trailing space
        if (!name.empty()) {
            name.pop_back(); 
        }
        p.name = name;
        
        products.push_back(p);
    }
    file.close();
    return products;
}

void writeProductsToFile(const vector<Product>& products) {
    ofstream file("products.txt");
    if (!file) {
        cout << "Error opening products file for writing.\n";
        return;
    }
    for (const auto& p : products) {
        file << p.name << " " << p.price << " " << p.stock << endl;
    }
    file.close();
}

void adminLogin() {
    string inputUsername, inputPassword;
    cout << "\n---| Welcome |---" << endl;
    cout << "----------------------" << endl;
    cout << "Enter username: ";
    getline(cin, inputUsername);
    cout << "Enter password: ";
    getline(cin, inputPassword);

    ifstream file("admin.txt");
    if (!file) {
        cout << "Error opening admin credentials file." << endl;
        return;
    }
 
    string fileUsername, filePassword;
    bool loggedIn = false;
    while (file >> fileUsername >> filePassword) { 
        if (inputUsername == fileUsername && inputPassword == filePassword) {
            cout << "\nLogin successful! Welcome, " << inputUsername << "." << endl;
            loggedIn = true;
            adminMenu();
            break;
        }
    }

    if (!loggedIn) {
        cout << "Invalid credentials.\n";
    }
    file.close();
}

void adminMenu() {
    int choice;

    do {
        cout << "\n\tAdmin Menu" << endl;
        cout << "-------------------" << endl;
        cout << "1. View All Products" << endl;
        cout << "2. Search for a Product" << endl;
        cout << "3. Add a Product" << endl;
        cout << "4. Edit a Product" << endl;
        cout << "5. Delete a Product" << endl;
        cout << "6. About Company" << endl;
        cout << "7. Log Out" << endl;
        cout << "\nEnter your choice: ";
        choice = getIntegerInput();

        switch (choice) {
            case 1:
                viewProducts();
                break;
            case 2: { 
                string searchName;
                cout << "Enter product name to search: ";
                getline(cin, searchName);
                Product p;
                if(searchProduct(searchName, p)){
                    cout << "\n--- Product Found ---" << endl;
                    cout << "Name: " << p.name << endl;
                    cout << "Price: " "GBP " << p.price << endl;
                    cout << "Stock: " << p.stock << endl;
                } else {
                    cout << "Product not found." << endl;
                }
                break;
            }
            case 3:
                addProduct();
                break;
            case 4:
                editProduct(); 
                break;
            case 5:
                deleteProduct();
                break;
            case 6:
                aboutUs();
                break;
            case 7:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (choice != 7); 
}
 

void viewProducts() {
    vector<Product> products = readProductsFromFile(); // <-- NEW: Use helper function

    if (products.empty()) {
        cout << "No products found." << endl;
        return;
    }
    
    //Header
    cout << "\n-------------------PRODUCT LIST-------------------\n";
    cout << left << setw(25) << "Name" << setw(15) << "Price (GBP)" << setw(10) << "Stock" << endl;
    cout << "----------------------------------------------------" << endl;

    for (const auto& p : products) {
        cout << left << setw(25) << p.name
             << fixed << setprecision(2) << setw(15) << p.price
             << setw(10) << p.stock << endl;
    }
}


void addProduct() {
    Product newProduct;

    cout << "Enter product name: ";
    
    getline(cin, newProduct.name);

    cout << "Enter product price: ";
    cin >> newProduct.price;

    cout << "Enter product stock quantity: ";
    cin >> newProduct.stock;

    // Append to the file
    ofstream file("products.txt", ios::app);
    if (!file) {
        cout << "Error opening products file.\n";
        return;
    }

    file << newProduct.name << " " << newProduct.price << " " << newProduct.stock << endl;
    cout << "\nProduct added successfully!" << endl;
    file.close();
}


    void deleteProduct() {
    string searchName;
    cout << "Enter the product name to delete: ";
    getline(cin, searchName);

    vector<Product> products = readProductsFromFile();
    vector<Product> updatedProducts;
    bool found = false;

    for (const auto& p : products) {
        if (p.name == searchName) {
            found = true;
        } else {
            updatedProducts.push_back(p);
        }
    }

    if (found) {
        writeProductsToFile(updatedProducts);
        cout << "Product deleted successfully.\n";
    } else {
        cout << "Product not found.\n";
    }
}


// Searches for a single product by name
bool searchProduct(const string& searchName, Product& foundProduct) {
    vector<Product> products = readProductsFromFile();
    for (const auto& p : products) {
        if (p.name == searchName) {
            foundProduct = p;
            return true;
        }
    }
    return false;
}

// Edit product function
void editProduct() {
    string searchName;
    cout << "Enter the product name to edit: ";
    cin.ignore();
    getline(cin, searchName);

    vector<Product> products = readProductsFromFile();
    bool found = false;
    for (auto& p : products) {
        if (p.name == searchName) {
            found = true;
            cout << "\n--- Editing Product: " << p.name << " ---" << endl;
            cout << "Current Price: " << p.price << " | Current Stock: " << p.stock << endl;

            cout << "Enter new name (or press Enter to keep '" << p.name << "'): ";
            string newName;
            getline(cin, newName);
            if (!newName.empty()) {
                p.name = newName;
            }

            cout << "Enter new price (current: " << p.price << "): ";
            cin >> p.price;

            cout << "Enter new stock (current: " << p.stock << "): ";
            cin >> p.stock;

            break;
        }
    }

    if (found) {
        writeProductsToFile(products);
        cout << "\nProduct updated successfully!" << endl;
    } else {
        cout << "Product not found." << endl;
    }
} 


void customerLogin() {
    string inputUsername, inputPassword;
    cout << "\n---| Welcome |---" << endl;
    cout << "----------------------" << endl;
    cout << "Enter username: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    ifstream file("customer.txt");
    if (!file) {
        cout << "Error opening admin credentials file." << endl;
        return;
    }

    string fileUsername, filePassword;
    bool loggedIn = false;
    while (file >> fileUsername >> filePassword) { 
        if (inputUsername == fileUsername && inputPassword == filePassword) {
            cout << "\nLogin successful! Welcome, " << inputUsername << "." << endl;
            loggedIn = true;
            customerMenu(inputUsername);
            break;
        }
    }

    if (!loggedIn) {
        cout << "Invalid credentials.\n";
    }
    file.close();
}

        
void customerMenu(const string& customerUsername) {
    int choice;
    Cart customerCart;

    do {
        cout << "\n\tCustomer Menu" << endl;
        cout << "----------------------" << endl;
        cout << "1. View All Products" << endl;
        cout << "2. Search for Product" << endl;
        cout << "3. Add Item to Cart" << endl;
        cout << "4. View Cart" << endl;
        cout << "5. Purchase" << endl;
        cout << "6. Log Out" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewProducts();
                break;
            case 2: { 
                string searchName;
                cout << "Enter product name to search: ";
                getline(cin, searchName);
                Product p;
                if(searchProduct(searchName, p)){
                    cout << "\n--- Product Found ---" << endl;
                    cout << "Name: " << p.name << endl;
                    cout << "Price: " "GBP " << p.price << endl;
                    cout << "Stock: " << p.stock << endl;
                } else {
                    cout << "Product not found." << endl;
                }
                break;
            }
            case 3: {
                string productName;
                int quantity;
                cout << "Enter the name of the product to add: ";
                cin.ignore();
                getline(cin, productName);

                Product p;
                if (searchProduct(productName, p)) {
                    cout << "Enter quantity (available: " << p.stock << "): ";
                    cin >> quantity;
                    if (quantity > 0 && quantity <= p.stock) {
                        customerCart.addItem(p.name, quantity, p.price);
                        cout << quantity << " x " << p.name << " added to cart." << endl;
                    } else {
                        cout << "Invalid quantity or not enough stock." << endl;
                    }
                } else {
                    cout << "Product not found." << endl;
                }
                break;
            }
            case 4:
            customerCart.printCart();
                break;
            case 5:
                purchase(customerCart);
               
                customerCart = Cart();
                break;
            case 6:
                cout << "Logging out. Thank you for shopping with us!" << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }

    } while (choice != 5);
}


// Finalizes the purchase and updates stock
void purchase(Cart& cart) {
    if (cart.items.empty()) {
        cout << "Your cart is empty. Nothing to purchase." << endl;
        return;
    }

    cart.printCart();
    cout << "\nConfirm purchase? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        vector<Product> products = readProductsFromFile();

        // Update stock for each item in the cart
        for (const auto& item : cart.items) {
            for (auto& product : products) {
                if (item.productName == product.name) {
                    product.stock -= item.quantity;
                    break;
                }
            }
        }

        writeProductsToFile(products); // Save the updated stock levels

        cout << "\nThank you for your purchase! Your order is confirmed." << endl;
    } else {
        cout << "Purchase cancelled." << endl;
    }
}
