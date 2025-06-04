#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "../utils.h" // Sử dụng các hằng số từ utils

// Định nghĩa cấu trúc khách hàng
typedef struct Customer {
    char id[MAX_ID_LEN];      // Mã khách hàng
    char name[MAX_NAME_LEN];   // Tên khách hàng
    char phone[MAX_PHONE_LEN]; // Số điện thoại
    struct Customer* next;     // Con trỏ next
} Customer;

extern Customer* customerList; // Danh sách khách hàng (khai báo extern)

// Khai báo hàm
Customer* createCustomer(const char* id, const char* name, const char* phone);
Customer* findCustomerById(const char* id);
void addCustomer();           // Thêm khách hàng mới
void displayCustomers();      // Hiển thị danh sách
void editCustomer();          // Sửa thông tin
void deleteCustomer();        // Xóa khách hàng
void searchCustomer();        // Tìm kiếm
void saveCustomersToFile();   // Lưu vào file
void loadCustomersFromFile(); // Đọc từ file
void customerMenu();          // Menu quản lý

#endif // Kết thúc CUSTOMER_H