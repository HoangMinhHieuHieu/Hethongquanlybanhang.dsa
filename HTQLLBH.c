#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Định nghĩa các hằng số
#define MAX_ID_LEN 10
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_INVOICE_ID_LEN 15
#define PRODUCT_FILE "products.dat"
#define CUSTOMER_FILE "customers.dat"
#define INVOICE_FILE "invoices.dat"
#define DATE_FORMAT "%d/%m/%Y"

// Cấu trúc dữ liệu
typedef struct Product {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    float price;
    int quantity;
    struct Product* next;
} Product;

typedef struct Customer { // Định nghĩa cấu trúc sản phẩm
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    struct Customer* next;
} Customer;

typedef struct InvoiceDetail { // Định nghĩa cấu trúc khách hàng
    char productId[MAX_ID_LEN];
    int quantity;
    float price;
    struct InvoiceDetail* next;
} InvoiceDetail;

typedef struct Invoice { // Định nghĩa hóa đơn
    char id[MAX_INVOICE_ID_LEN];
    char customerId[MAX_ID_LEN];
    time_t date;
    float total;
    InvoiceDetail* details;
    struct Invoice* next;
} Invoice;

// Danh sách liên kết toàn cục
Product* productList = NULL;
Customer* customerList = NULL;
Invoice* invoiceList = NULL;

// Nguyên mẫu hàm
void saveProductsToFile();
void loadProductsFromFile();
void saveCustomersToFile();
void loadCustomersFromFile();
void saveInvoicesToFile();
void loadInvoicesFromFile();
void freeMemory();

// ==================== TIỆN ÍCH ====================
void clearInputBuffer() { 
    while (getchar() != '\n');
}

void pressEnterToContinue() {
    printf("\nNhan Enter de tiep tuc...");
    clearInputBuffer();
    getchar();
}

int isValidDate(const char* dateStr) {
    int day, month, year;
    if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) != 3)
        return 0;
    
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return 0;
    
    if (month == 2) {
        int isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
        if (day > (isLeap ? 29 : 28))
            return 0;
    }
    
    return 1;
}

int isValidPhone(const char* phone) {
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-')
            return 0;
    }
    return strlen(phone) >= 10 && strlen(phone) <= 15;
}

char* formatTime(time_t time) {
    static char buffer[20];
    strftime(buffer, sizeof(buffer), DATE_FORMAT, localtime(&time));
    return buffer;
}

// ==================== QUẢN LÝ SẢN PHẨM ====================
Product* createProduct(const char* id, const char* name, float price, int quantity) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    strncpy(newProduct->id, id, MAX_ID_LEN);
    strncpy(newProduct->name, name, MAX_NAME_LEN);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->next = NULL;
    return newProduct;
}

Product* findProductById(const char* id) {
    Product* current = productList;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchProduct() {
    char keyword[MAX_NAME_LEN];
    printf("\nTIM KIEM SAN PHAM\n");
    printf("Nhap ma hoac ten san pham: ");
    scanf("%s", keyword);
    clearInputBuffer();

    printf("\nKET QUA TIM KIEM\n");
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Gia", "SL Tong");
    printf("--------------------------------------------------\n");

    Product* current = productList;
    int found = 0;
    while (current != NULL) {
        if (strstr(current->id, keyword) != NULL || strstr(current->name, keyword) != NULL) {
            printf("%-10s %-20s %10.2f %10d\n", 
                   current->id, current->name, current->price, current->quantity);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Khong tim thay san pham phu hop!\n");
    }

    pressEnterToContinue();
}

void addProduct() {
    char id[MAX_ID_LEN], name[MAX_NAME_LEN];
    float price;
    int quantity;
    
    printf("\nTHEM SAN PHAM MOI\n");
    printf("Ma san pham: "); scanf("%s", id);
    clearInputBuffer();
    
    if (findProductById(id) != NULL) {
        printf("Ma san pham da ton tai!\n");
        pressEnterToContinue();
        return;
    }
    
    printf("Ten san pham: "); fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    
    printf("Gia ban: "); scanf("%f", &price);
    printf("So luong : "); scanf("%d", &quantity);
    
    Product* newProduct = createProduct(id, name, price, quantity);
    
    if (productList == NULL) {
        productList = newProduct;
    } else {
        Product* current = productList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newProduct;
    }
    
    saveProductsToFile(); // Lưu vào file
    printf("Them san pham thanh cong!\n");
    pressEnterToContinue();
}

void displayProducts() {
    printf("\nDANH SACH SAN PHAM\n");
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Gia", "SL Tong");
    printf("--------------------------------------------------\n");
    
    Product* current = productList;
    while (current != NULL) {
        printf("%-10s %-20s %10.2f %10d\n", 
               current->id, current->name, current->price, current->quantity);
        current = current->next;
    }
    
    pressEnterToContinue();
}

void editProduct() {
    char id[MAX_ID_LEN];
    printf("\nSUA THONG TIN SAN PHAM\n");
    printf("Nhap ma san pham can sua: "); scanf("%s", id);
    
    Product* product = findProductById(id);
    if (product == NULL) {
        printf("Khong tim thay san pham voi ma %s\n", id);
        pressEnterToContinue();
        return;
    }
    
    printf("\nThong tin hien tai:\n");
    printf("Ten: %s\n", product->name);
    printf("Gia: %.2f\n", product->price);
    printf("So luong: %d\n", product->quantity);
    
    printf("\nNhap thong tin moi (de trong neu khong thay doi):\n");
    char name[MAX_NAME_LEN], input[50];
    float newPrice;
    int newQuantity;
    
    clearInputBuffer();
    printf("Ten moi: "); fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    if (strlen(name) > 0) strcpy(product->name, name);
    
    printf("Gia moi: "); fgets(input, sizeof(input), stdin);
    if (strlen(input) > 0 && sscanf(input, "%f", &newPrice) == 1) {
        product->price = newPrice;
    }
    
    printf("So luong moi: "); fgets(input, sizeof(input), stdin);
    if (strlen(input) > 0 && sscanf(input, "%d", &newQuantity) == 1) {
        product->quantity = newQuantity;
    }
    
    saveProductsToFile();
    printf("Cap nhat thong tin thanh cong!\n");
    pressEnterToContinue();
}

void deleteProduct() {
    char id[MAX_ID_LEN];
    printf("\nXOA SAN PHAM\n");
    printf("Nhap ma san pham can xoa: "); scanf("%s", id);
    
    Product* current = productList;
    Product* prev = NULL;
    
    while (current != NULL && strcmp(current->id, id) != 0) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Khong tim thay san pham voi ma %s\n", id);
        pressEnterToContinue();
        return;
    }
    
    if (prev == NULL) {
        productList = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    saveProductsToFile();
    printf("Da xoa san pham voi ma %s\n", id);
    pressEnterToContinue();
}

// ==================== QUẢN LÝ KHÁCH HÀNG ====================
Customer* createCustomer(const char* id, const char* name, const char* phone) {
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));
    strncpy(newCustomer->id, id, MAX_ID_LEN);
    strncpy(newCustomer->name, name, MAX_NAME_LEN);
    strncpy(newCustomer->phone, phone, MAX_PHONE_LEN);
    newCustomer->next = NULL;
    return newCustomer;
}

Customer* findCustomerById(const char* id) {
    Customer* current = customerList;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchCustomer() {
    char keyword[MAX_NAME_LEN];
    printf("\nTIM KIEM KHACH HANG\n");
    printf("Nhap ma hoac ten khach hang: ");
    scanf("%s", keyword);
    clearInputBuffer();

    printf("\nKET QUA TIM KIEM\n");
    printf("%-10s %-20s %-15s\n", "Ma KH", "Ten KH", "So DT");
    printf("------------------------------------------\n");

    Customer* current = customerList;
    int found = 0;
    while (current != NULL) {
        if (strstr(current->id, keyword) != NULL || strstr(current->name, keyword) != NULL) {
            printf("%-10s %-20s %-15s\n", 
                   current->id, current->name, current->phone);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Khong tim thay khach hang phu hop!\n");
    }

    pressEnterToContinue();
}

void addCustomer() {
    char id[MAX_ID_LEN], name[MAX_NAME_LEN], phone[MAX_PHONE_LEN];
    
    printf("\nTHEM KHACH HANG MOI\n");
    printf("Ma khach hang: "); scanf("%s", id);
    clearInputBuffer();
    
    if (findCustomerById(id) != NULL) {
        printf("Ma khach hang da ton tai!\n");
        pressEnterToContinue();
        return;
    }
    
    printf("Ten khach hang: "); fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    
    do {
        printf("So dien thoai: "); fgets(phone, MAX_PHONE_LEN, stdin);
        phone[strcspn(phone, "\n")] = '\0';
        if (!isValidPhone(phone)) {
            printf("So dien thoai khong hop le! Vui long nhap lai.\n");
        }
    } while (!isValidPhone(phone));
    
    Customer* newCustomer = createCustomer(id, name, phone);
    
    if (customerList == NULL) {
        customerList = newCustomer;
    } else {
        Customer* current = customerList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCustomer;
    }
    
    saveCustomersToFile(); // Lưu vào file
    printf("Them khach hang thanh cong!\n");
    pressEnterToContinue();
}

void displayCustomers() {
    printf("\nDANH SACH KHACH HANG\n");
    printf("%-10s %-20s %-15s\n", "Ma KH", "Ten KH", "So DT");
    printf("------------------------------------------\n");
    
    Customer* current = customerList;
    while (current != NULL) {
        printf("%-10s %-20s %-15s\n", 
               current->id, current->name, current->phone);
        current = current->next;
    }
    
    pressEnterToContinue();
}

void editCustomer() {
    char id[MAX_ID_LEN];
    printf("\nSUA THONG TIN KHACH HANG\n");
    printf("Nhap ma khach hang can sua: "); scanf("%s", id);
    
    Customer* customer = findCustomerById(id);
    if (customer == NULL) {
        printf("Khong tim thay khach hang voi ma %s\n", id);
        pressEnterToContinue();
        return;
    }
    
    printf("\nThong tin hien tai:\n");
    printf("Ten: %s\n", customer->name);
    printf("So dien thoai: %s\n", customer->phone);
    
    printf("\nNhap thong tin moi (de trong neu khong thay doi):\n");
    char name[MAX_NAME_LEN], phone[MAX_PHONE_LEN];
    
    clearInputBuffer();
    printf("Ten moi: "); fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    if (strlen(name) > 0) strcpy(customer->name, name);
    
    do {
        printf("So dien thoai moi: "); fgets(phone, MAX_PHONE_LEN, stdin);
        phone[strcspn(phone, "\n")] = '\0';
        if (strlen(phone) > 0 && !isValidPhone(phone)) {
            printf("So dien thoai khong hop le! Vui long nhap lai.\n");
        } else if (strlen(phone) > 0) {
            strcpy(customer->phone, phone);
            break;
        } else {
            break;
        }
    } while (1);
    
    saveCustomersToFile();
    printf("Cap nhat thong tin thanh cong!\n");
    pressEnterToContinue();
}

void deleteCustomer() {
    char id[MAX_ID_LEN];
    printf("\nXOA KHACH HANG\n");
    printf("Nhap ma khach hang can xoa: "); scanf("%s", id);
    
    Customer* current = customerList;
    Customer* prev = NULL;
    
    while (current != NULL && strcmp(current->id, id) != 0) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Khong tim thay khach hang voi ma %s\n", id);
        pressEnterToContinue();
        return;
    }
    
    if (prev == NULL) {
        customerList = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    saveCustomersToFile();
    printf("Da xoa khach hang voi ma %s\n", id);
    pressEnterToContinue();
}

// ==================== QUẢN LÝ HÓA ĐƠN ====================
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
    
    saveProductsToFile(); // Lưu vào file
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

// ==================== BÁO CÁO THỐNG KÊ ====================
void revenueReport() {
    char startStr[20], endStr[20];
    printf("\nTHONG KE DOANH THU\n");
    
    do {
        printf("Nhap ngay bat dau (dd/mm/yyyy): ");
        scanf("%s", startStr);
        if (!isValidDate(startStr)) {
            printf("Ngay khong hop le! Vui long nhap lai.\n");
        }
    } while (!isValidDate(startStr));
    
    do {
        printf("Nhap ngay ket thuc (dd/mm/yyyy): ");
        scanf("%s", endStr);
        if (!isValidDate(endStr)) {
            printf("Ngay khong hop le! Vui long nhap lai.\n");
        }
    } while (!isValidDate(endStr));

    struct tm tmStart = {0}, tmEnd = {0};
    sscanf(startStr, "%d/%d/%d", &tmStart.tm_mday, &tmStart.tm_mon, &tmStart.tm_year);
    tmStart.tm_mon -= 1;
    tmStart.tm_year -= 1900;
    time_t startDate = mktime(&tmStart);

    sscanf(endStr, "%d/%d/%d", &tmEnd.tm_mday, &tmEnd.tm_mon, &tmEnd.tm_year);
    tmEnd.tm_mon -= 1;
    tmEnd.tm_year -= 1900;
    tmEnd.tm_hour = 23;
    tmEnd.tm_min = 59;
    tmEnd.tm_sec = 59;
    time_t endDate = mktime(&tmEnd);

    float totalRevenue = 0;
    int invoiceCount = 0;
    
    printf("\nHOA DON TU NGAY %s DEN %s\n", startStr, endStr);
    printf("%-15s %-20s %-15s\n", "Ma HD", "Ngay", "Tong tien");
    printf("--------------------------------------------\n");
    
    Invoice* current = invoiceList;
    while (current != NULL) {
        if (current->date >= startDate && current->date <= endDate) {
            printf("%-15s %-20s %15.2f\n", 
                  current->id, formatTime(current->date), current->total);
            totalRevenue += current->total;
            invoiceCount++;
        }
        current = current->next;
    }
    
    printf("\nTONG CONG:\n");
    printf("So luong hoa don: %d\n", invoiceCount);
    printf("Tong doanh thu: %.2f\n", totalRevenue);
    pressEnterToContinue();
}

void productStats() {
    printf("\nTHONG KE SAN PHAM\n");
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Da ban", "Doanh thu");
    printf("--------------------------------------------------\n");
    
    Product* p = productList;
    while (p != NULL) {
        int sold = 0;
        float revenue = 0;
        
        Invoice* inv = invoiceList;
        while (inv != NULL) {
            InvoiceDetail* det = inv->details;
            while (det != NULL) {
                if (strcmp(det->productId, p->id) == 0) {
                    sold += det->quantity;
                    revenue += det->quantity * det->price;
                }
                det = det->next;
            }
            inv = inv->next;
        }
        
        printf("%-10s %-20s %10d %10.2f\n", 
               p->id, p->name, sold, revenue);
        p = p->next;
    }
    
    pressEnterToContinue();
}

void customerStats() {
    printf("\nTHONG KE KHACH HANG\n");
    printf("%-10s %-20s %-10s %-15s\n", "Ma KH", "Ten KH", "So HD", "Tong chi tieu");
    printf("--------------------------------------------------\n");
    
    Customer* c = customerList;
    while (c != NULL) {
        int invoiceCount = 0;
        float totalSpent = 0;
        
        Invoice* inv = invoiceList;
        while (inv != NULL) {
            if (strcmp(inv->customerId, c->id) == 0) {
                invoiceCount++;
                totalSpent += inv->total;
            }
            inv = inv->next;
        }
        
        printf("%-10s %-20s %10d %15.2f\n", 
               c->id, c->name, invoiceCount, totalSpent);
        c = c->next;
    }
    
    pressEnterToContinue();
}

// ==================== LƯU VÀ ĐỌC FILE ====================
void saveProductsToFile() {
    FILE* file = fopen(PRODUCT_FILE, "wb");
    if (file == NULL) {
        printf("Khong the mo file de ghi san pham\n");
        return;
    }
    
    Product* current = productList;
    while (current != NULL) {
        fwrite(current, sizeof(Product), 1, file);
        current = current->next;
    }
    
    fclose(file);
}

void loadProductsFromFile() { // Đọc từ file
    FILE* file = fopen(PRODUCT_FILE, "rb");
    if (file == NULL) return;
    
    Product temp;
    while (fread(&temp, sizeof(Product), 1, file) == 1) {
        Product* newProduct = createProduct(temp.id, temp.name, temp.price, temp.quantity);
        
        if (productList == NULL) {
            productList = newProduct;
        } else {
            Product* current = productList;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newProduct;
        }
    }
    
    fclose(file);
}

void saveCustomersToFile() {
    FILE* file = fopen(CUSTOMER_FILE, "wb");
    if (file == NULL) {
        printf("Khong the mo file de ghi khach hang\n");
        return;
    }
    
    Customer* current = customerList;
    while (current != NULL) {
        fwrite(current, sizeof(Customer), 1, file);
        current = current->next;
    }
    
    fclose(file);
}

void loadCustomersFromFile() { // Đọc từ file
    FILE* file = fopen(CUSTOMER_FILE, "rb");
    if (file == NULL) return;
    
    Customer temp;
    while (fread(&temp, sizeof(Customer), 1, file) == 1) {
        Customer* newCustomer = createCustomer(temp.id, temp.name, temp.phone);
        
        if (customerList == NULL) {
            customerList = newCustomer;
        } else {
            Customer* current = customerList;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newCustomer;
        }
    }
    
    fclose(file);
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

void loadInvoicesFromFile() { // Đọc từ file
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

// ==================== MENU CHƯƠNG TRÌNH ====================
void productMenu() { // Menu quản lý sản phẩm
    int choice;
    do {
        printf("\nQUAN LY SAN PHAM\n");
        printf("1. Them san pham\n");
        printf("2. Hien thi danh sach san pham\n");
        printf("3. Sua thong tin san pham\n");
        printf("4. Xoa san pham\n");
        printf("5. Tim kiem san pham\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: editProduct(); break;
            case 4: deleteProduct(); break;
            case 5: searchProduct(); break;
            case 0: break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void customerMenu() { // Menu quản lý khách hàng
    int choice;
    do {
        printf("\nQUAN LY KHACH HANG\n");
        printf("1. Them khach hang\n");
        printf("2. Hien thi danh sach khach hang\n");
        printf("3. Sua thong tin khach hang\n");
        printf("4. Xoa khach hang\n");
        printf("5. Tim kiem khach hang\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: addCustomer(); break;
            case 2: displayCustomers(); break;
            case 3: editCustomer(); break;
            case 4: deleteCustomer(); break;
            case 5: searchCustomer(); break;
            case 0: break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void invoiceMenu() {  // Menu quản lý hóa đơn
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

void reportMenu() { // Menu báo cáo thống kê
    int choice;
    do {
        printf("\nBAO CAO - THONG KE\n");
        printf("1. Thong ke doanh thu\n");
        printf("2. Thong ke san pham\n");
        printf("3. Thong ke khach hang\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: revenueReport(); break;
            case 2: productStats(); break;
            case 3: customerStats(); break;
            case 0: break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

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

int main() {
    mainMenu();
    return 0;
}