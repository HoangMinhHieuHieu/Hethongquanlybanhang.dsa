#ifndef PRODUCT_H     // Bảo vệ include nhiều lần
#define PRODUCT_H

#include "../utils.h" // Include file tiện ích chung

// Định nghĩa cấu trúc sản phẩm
typedef struct Product {
    char id[MAX_ID_LEN];      // Mã sản phẩm (tối đa 10 ký tự)
    char name[MAX_NAME_LEN];  // Tên sản phẩm (tối đa 50 ký tự)
    float price;              // Giá bán
    int quantity;             // Số lượng tồn kho
    struct Product* next;     // Con trỏ next cho danh sách liên kết
} Product;

// Khai báo biến toàn cục (sẽ được định nghĩa trong quanlysanpham.c)
extern Product* productList;  // Danh sách sản phẩm

// Khai báo các hàm API
Product* createProduct(const char* id, const char* name, float price, int quantity);
Product* findProductById(const char* id);
void addProduct();            // Thêm sản phẩm mới
void displayProducts();       // Hiển thị danh sách
void editProduct();           // Sửa thông tin
void deleteProduct();         // Xóa sản phẩm
void searchProduct();         // Tìm kiếm
void saveProductsToFile();    // Lưu vào file
void loadProductsFromFile();  // Đọc từ file
void productMenu();           // Menu quản lý sản phẩm

#endif // Kết thúc PRODUCT_H