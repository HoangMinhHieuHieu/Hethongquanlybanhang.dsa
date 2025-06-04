#include "invoice.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Invoice* invoiceList = NULL;

Invoice* createInvoice(const char* id, const char* customerId, time_t date) {
    Invoice* newInvoice = (Invoice*)malloc(sizeof(Invoice));
    strncpy(newInvoice->id, id, MAX_INVOICE_ID_LEN);
    strncpy(newInvoice->customerId, customerId, MAX_ID_LEN);
    newInvoice->date = date;
    newInvoice->total = 0;
    newInvoice->details = NULL;
    newInvoice->next = NULL;
    return newInvoice;
}

InvoiceDetail* createInvoiceDetail(const char* productId, int quantity, float price) {
    InvoiceDetail* newDetail = (InvoiceDetail*)malloc(sizeof(InvoiceDetail));
    strncpy(newDetail->productId, productId, MAX_ID_LEN);
    newDetail->quantity = quantity;
    newDetail->price = price;
    newDetail->next = NULL;
    return newDetail;
}

Invoice* findInvoiceById(const char* id) {
    Invoice* current = invoiceList;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchInvoice() {
    char keyword[MAX_INVOICE_ID_LEN];
    printf("\nTIM KIEM HOA DON\n");
    printf("Nhap ma hoa don hoac ma khach hang: ");
    scanf("%s", keyword);
    clearInputBuffer();

    printf("\nKET QUA TIM KIEM\n");
    printf("%-15s %-10s %-20s %-15s\n", "Ma HD", "Ma KH", "Ngay", "Tong tien");
    printf("------------------------------------------------------------\n");

    Invoice* current = invoiceList;
    int found = 0;
    while (current != NULL) {
        if (strstr(current->id, keyword) != NULL || strstr(current->customerId, keyword) != NULL) {
            printf("%-15s %-10s %-20s %15.2f\n", 
                   current->id, current->customerId, formatTime(current->date), current->total);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Khong tim thay hoa don phu hop!\n");
    }

    pressEnterToContinue();
}

void addInvoice() {
    char invoiceId[MAX_INVOICE_ID_LEN], customerId[MAX_ID_LEN];
    time_t now = time(NULL);
    
    printf("\nTAO HOA DON MOI\n");
    printf("Ma hoa don (VD: HD001): "); scanf("%s", invoiceId);
    clearInputBuffer();
    
    if (findInvoiceById(invoiceId) != NULL) {
        printf("Ma hoa don da ton tai!\n");
        pressEnterToContinue();
        return;
    }
    
    printf("Ma khach hang: "); scanf("%s", customerId);
    Customer* customer = findCustomerById(customerId);
    if (customer == NULL) {
        printf("Khong tim thay khach hang voi ma %s\n", customerId);
        pressEnterToContinue();
        return;
    }
    
    Invoice* invoice = createInvoice(invoiceId, customerId, now);
    
    char choice;
    do {
        char productId[MAX_ID_LEN];
        int quantity;
        
        printf("\nThem san pham vao hoa don\n");
        printf("Ma san pham: "); scanf("%s", productId);
        Product* product = findProductById(productId);
        if (product == NULL) {
            printf("Khong tim thay san pham voi ma %s\n", productId);
            continue;
        }
        
        printf("So luong: "); scanf("%d", &quantity);
        if (quantity <= 0) {
            printf("So luong phai lon hon 0\n");
            continue;
        }
        
        if (quantity > product->quantity) {
            printf("Khong du so luong ton kho. Chi con %d\n", product->quantity);
            continue;
        }
        
        InvoiceDetail* detail = createInvoiceDetail(productId, quantity, product->price);
        if (invoice->details == NULL) {
            invoice->details = detail;
        } else {
            InvoiceDetail* current = invoice->details;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = detail;
        }
        
        invoice->total += quantity * product->price;
        product->quantity -= quantity;
        
        printf("Da them %s - %s x%d vao hoa don\n", product->id, product->name, quantity);
        
        printf("Tiep tuc them san pham? (y/n): ");
        clearInputBuffer();
        choice = getchar();
    } while (choice == 'y' || choice == 'Y');
    
    if (invoiceList == NULL) {
        invoiceList = invoice;
    } else {
        Invoice* current = invoiceList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = invoice;
    }
    
    saveProductsToFile();
    saveInvoicesToFile();
    
    printf("\nTao hoa don thanh cong!\n");
    printf("Tong tien: %.2f\n", invoice->total);
    pressEnterToContinue();
}

void displayInvoices() {
    printf("\nDANH SACH HOA DON\n");
    printf("%-15s %-10s %-20s %-15s\n", "Ma HD", "Ma KH", "Ngay", "Tong tien");
    printf("------------------------------------------------------------\n");
    
    Invoice* current = invoiceList;
    while (current != NULL) {
        printf("%-15s %-10s %-20s %15.2f\n", 
               current->id, current->customerId, formatTime(current->date), current->total);
        current = current->next;
    }
    
    pressEnterToContinue();
}

void displayInvoiceDetails() {
    char invoiceId[MAX_INVOICE_ID_LEN];
    printf("\nXEM CHI TIET HOA DON\n");
    printf("Nhap ma hoa don: "); scanf("%s", invoiceId);
    
    Invoice* invoice = findInvoiceById(invoiceId);
    if (invoice == NULL) {
        printf("Khong tim thay hoa don voi ma %s\n", invoiceId);
        pressEnterToContinue();
        return;
    }
    
    Customer* customer = findCustomerById(invoice->customerId);
    
    printf("\nCHI TIET HOA DON %s\n", invoice->id);
    printf("Khach hang: %s - %s\n", customer->id, customer->name);
    printf("Ngay: %s\n", formatTime(invoice->date));
    printf("%-10s %-20s %-10s %-10s %-15s\n", 
           "Ma SP", "Ten SP", "Don gia", "SL", "Thanh tien");
    printf("------------------------------------------------------------\n");
    
    float total = 0;
    InvoiceDetail* detail = invoice->details;
    while (detail != NULL) {
        Product* product = findProductById(detail->productId);
        float amount = detail->quantity * detail->price;
        printf("%-10s %-20s %10.2f %10d %15.2f\n", 
               product->id, product->name, detail->price, detail->quantity, amount);
        total += amount;
        detail = detail->next;
    }
    
    printf("\nTONG CONG: %45.2f\n", total);
    pressEnterToContinue();
}

void saveInvoicesToFile() {
    FILE* file = fopen(INVOICE_FILE, "wb");
    if (file == NULL) {
        printf("Khong the mo file de ghi hoa don\n");
        return;
    }
    
    Invoice* current = invoiceList;
    while (current != NULL) {
        // Lưu thông tin hóa đơn (không lưu con trỏ next và details)
        Invoice temp = *current;
        temp.next = NULL;
        temp.details = NULL;
        fwrite(&temp, sizeof(Invoice), 1, file);
        
        // Lưu chi tiết hóa đơn
        InvoiceDetail* detail = current->details;
        while (detail != NULL) {
            fwrite(detail, sizeof(InvoiceDetail), 1, file);
            detail = detail->next;
        }
        
        // Đánh dấu kết thúc chi tiết
        InvoiceDetail endMarker = {0};
        fwrite(&endMarker, sizeof(InvoiceDetail), 1, file);
        
        current = current->next;
    }
    
    fclose(file);
}

void loadInvoicesFromFile() {
    FILE* file = fopen(INVOICE_FILE, "rb");
    if (file == NULL) return;
    
    Invoice tempInvoice;
    while (fread(&tempInvoice, sizeof(Invoice), 1, file) == 1) {
        Invoice* newInvoice = createInvoice(tempInvoice.id, tempInvoice.customerId, tempInvoice.date);
        newInvoice->total = tempInvoice.total;
        
        // Đọc chi tiết hóa đơn
        InvoiceDetail tempDetail;
        while (fread(&tempDetail, sizeof(InvoiceDetail), 1, file) == 1) {
            if (strlen(tempDetail.productId) == 0) break; // Kết thúc chi tiết
            
            InvoiceDetail* newDetail = createInvoiceDetail(
                tempDetail.productId, tempDetail.quantity, tempDetail.price);
            
            if (newInvoice->details == NULL) {
                newInvoice->details = newDetail;
            } else {
                InvoiceDetail* current = newInvoice->details;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newDetail;
            }
        }
        
        // Thêm hóa đơn vào danh sách
        if (invoiceList == NULL) {
            invoiceList = newInvoice;
        } else {
            Invoice* current = invoiceList;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newInvoice;
        }
    }
    
    fclose(file);
}

void invoiceMenu() {
    int choice;
    do {
        printf("\nQUAN LY HOA DON\n");
        printf("1. Tao hoa don moi\n");
        printf("2. Hien thi danh sach hoa don\n");
        printf("3. Xem chi tiet hoa don\n");
        printf("4. Tim kiem hoa don\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: addInvoice(); break;
            case 2: displayInvoices(); break;
            case 3: displayInvoiceDetails(); break;
            case 4: searchInvoice(); break;
            case 0: break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}
