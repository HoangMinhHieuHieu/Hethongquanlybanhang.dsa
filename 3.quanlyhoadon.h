#ifndef INVOICE_H
#define INVOICE_H

#include "../utils.h"
#include "../product/product.h" // Cần để tham chiếu sản phẩm

// Định nghĩa chi tiết hóa đơn
typedef struct InvoiceDetail {
    char productId[MAX_ID_LEN]; // Mã sản phẩm
    int quantity;              // Số lượng
    float price;               // Đơn giá
    struct InvoiceDetail* next; // Next
} InvoiceDetail;

// Định nghĩa hóa đơn
typedef struct Invoice {
    char id[MAX_INVOICE_ID_LEN]; // Mã hóa đơn
    char customerId[MAX_ID_LEN]; // Mã khách hàng
    time_t date;                // Ngày lập (kiểu time_t)
    float total;                // Tổng tiền
    InvoiceDetail* details;     // Danh sách chi tiết
    struct Invoice* next;       // Next
} Invoice;

extern Invoice* invoiceList;    // Danh sách hóa đơn

// Khai báo hàm
Invoice* createInvoice(const char* id, const char* customerId, time_t date);
InvoiceDetail* createInvoiceDetail(const char* productId, int quantity, float price);
Invoice* findInvoiceById(const char* id);
void addInvoice();              // Tạo hóa đơn mới
void displayInvoices();         // Hiển thị danh sách
void displayInvoiceDetails();    // Xem chi tiết
void searchInvoice();           // Tìm kiếm
void saveInvoicesToFile();      // Lưu vào file
void loadInvoicesFromFile();    // Đọc từ file
void invoiceMenu();             // Menu quản lý

#endif // Kết thúc INVOICE_H
