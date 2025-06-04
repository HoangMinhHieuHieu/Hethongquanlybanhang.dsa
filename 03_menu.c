#include "menu.h"
#include "../product/product.h"
#include "../customer/customer.h"
#include "../invoice/invoice.h"
#include "../report/report.h"

void freeMemory() {
    // Giải phóng bộ nhớ sản phẩm
    Product* p = productList;
    while (p != NULL) {
        Product* temp = p;
        p = p->next;
        free(temp);
    }
    
    // Giải phóng bộ nhớ khách hàng
    Customer* c = customerList;
    while (c != NULL) {
        Customer* temp = c;
        c = c->next;
        free(temp);
    }
    
    // Giải phóng bộ nhớ hóa đơn
    Invoice* inv = invoiceList;
    while (inv != NULL) {
        // Giải phóng chi tiết hóa đơn
        InvoiceDetail* det = inv->details;
        while (det != NULL) {
            InvoiceDetail* temp = det;
            det = det->next;
            free(temp);
        }
        
        Invoice* temp = inv;
        inv = inv->next;
        free(temp);
    }
}

void mainMenu() {
    // Tải dữ liệu khi khởi động
    loadProductsFromFile();
    loadCustomersFromFile();
    loadInvoicesFromFile();
    
    int choice;
    do {
        printf("\nHE THONG QUAN LY BAN HANG\n");
        printf("1. Quan ly san pham\n");
        printf("2. Quan ly khach hang\n");
        printf("3. Quan ly hoa don\n");
        printf("4. Bao cao - Thong ke\n");
        printf("0. Thoat\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: productMenu(); break;
            case 2: customerMenu(); break;
            case 3: invoiceMenu(); break;
            case 4: reportMenu(); break;
            case 0: 
                // Lưu dữ liệu và giải phóng bộ nhớ trước khi thoát
                saveProductsToFile();
                saveCustomersToFile();
                saveInvoicesToFile();
                freeMemory();
                printf("Cam on ban da su dung chuong trinh!\n"); 
                break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}