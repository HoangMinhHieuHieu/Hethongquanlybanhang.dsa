#include "InvoiceManagement.h"

// Khởi tạo danh sách
void initList(LinkedList* list) {
    *list = NULL;
}

// Kiểm tra danh sách rỗng
int isEmpty(LinkedList list) {
    return (list == NULL);
}

// Thêm hóa đơn vào cuối danh sách
void insertInvoice(LinkedList* list) {
    struct Invoice inv;
    float unitPrice;
    
    printf("\nNhap ma hoa don: ");
    scanf("%s", inv.invoiceID);
    printf("Nhap ma san pham: ");
    scanf("%s", inv.productID);
    printf("Nhap ma khach hang: ");
    scanf("%s", inv.customerID);
    printf("Nhap so luong: ");
    scanf("%d", &inv.quantity);
    printf("Nhap gia don vi san pham: ");
    scanf("%f", &unitPrice);
    printf("Nhap ngay mua (gio ngay thang nam): ");
    scanf("%d %d %d %d", &inv.date.hour, &inv.date.day, &inv.date.month, &inv.date.year);
    inv.totalPrice = inv.quantity * unitPrice;
    
    PNode q = (PNode)malloc(sizeof(struct Node));
    q->data = inv;
    q->next = NULL;
    
    if (isEmpty(*list)) {
        *list = q;
    } else {
        PNode ptr = *list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = q;
    }
    printf("Them hoa don thanh cong!\n");
}

// Sửa hóa đơn
void editInvoice(LinkedList* list) {
    char id[10];
    printf("\nNhap ma hoa don can sua: ");
    scanf("%s", id);
    
    PNode ptr = *list;
    while (ptr != NULL) {
        if (strcmp(ptr->data.invoiceID, id) == 0) {
            printf("Nhap thong tin moi:\n");
            printf("Ma san pham: ");
            scanf("%s", ptr->data.productID);
            printf("Ma khach hang: ");
            scanf("%s", ptr->data.customerID);
            printf("So luong: ");
            scanf("%d", &ptr->data.quantity);
            printf("Nhap ngay mua (gio ngay thang nam): ");
            scanf("%d %d %d %d", &ptr->data.date.hour, &ptr->data.date.day, 
                  &ptr->data.date.month, &ptr->data.date.year);
            float unitPrice;
            printf("Nhap gia don vi san pham: ");
            scanf("%f", &unitPrice);
            ptr->data.totalPrice = ptr->data.quantity * unitPrice;
            printf("Sua hoa don thanh cong!\n");
            return;
        }
        ptr = ptr->next;
    }
    printf("Khong tim thay hoa don voi ma %s\n", id);
}

// Xóa hóa đơn
void deleteInvoice(LinkedList* list) {
    char id[10];
    printf("\nNhap ma hoa don can xoa: ");
    scanf("%s", id);
    
    if (isEmpty(*list)) {
        printf("Danh sach rong!\n");
        return;
    }
    
    if (strcmp((*list)->data.invoiceID, id) == 0) {
        PNode temp = *list;
        *list = (*list)->next;
        free(temp);
        printf("Xoa hoa don thanh cong!\n");
        return;
    }
    
    PNode ptr = *list;
    while (ptr->next != NULL) {
        if (strcmp(ptr->next->data.invoiceID, id) == 0) {
            PNode temp = ptr->next;
            ptr->next = temp->next;
            free(temp);
            printf("Xoa hoa don thanh cong!\n");
            return;
        }
        ptr = ptr->next;
    }
    printf("Khong tim thay hoa don voi ma %s\n", id);
}

// Tìm kiếm theo mã hóa đơn
void searchByInvoiceID(LinkedList list) {
    char id[10];
    printf("\nNhap ma hoa don can tim: ");
    scanf("%s", id);
    
    PNode ptr = list;
    while (ptr != NULL) {
        if (strcmp(ptr->data.invoiceID, id) == 0) {
            printf("\nThong tin hoa don:\n");
            printf("Ma hoa don: %s\n", ptr->data.invoiceID);
            printf("Ma san pham: %s\n", ptr->data.productID);
            printf("Ma khach hang: %s\n", ptr->data.customerID);
            printf("So luong: %d\n", ptr->data.quantity);
            printf("Ngay mua: %02d:%02d:%02d:%04d\n", ptr->data.date.hour, 
                   ptr->data.date.day, ptr->data.date.month, ptr->data.date.year);
            printf("Tong gia: %.2f\n", ptr->data.totalPrice);
            return;
        }
        ptr = ptr->next;
    }
    printf("Khong tim thay hoa don voi ma %s\n", id);
}

// Tìm kiếm theo mã khách hàng
void searchByCustomerID(LinkedList list) {
    char id[10];
    printf("\nNhap ma khach hang can tim: ");
    scanf("%s", id);
    
    PNode ptr = list;
    int found = 0;
    while (ptr != NULL) {
        if (strcmp(ptr->data.customerID, id) == 0) {
            printf("\nThong tin hoa don:\n");
            printf("Ma hoa don: %s\n", ptr->data.invoiceID);
            printf("Ma san pham: %s\n", ptr->data.productID);
            printf("Ma khach hang: %s\n", ptr->data.customerID);
            printf("So luong: %d\n", ptr->data.quantity);
            printf("Ngay mua: %02d:%02d:%02d:%04d\n", ptr->data.date.hour, 
                   ptr->data.date.day, ptr->data.date.month, ptr->data.date.year);
            printf("Tong gia: %.2f\n", ptr->data.totalPrice);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (!found) {
        printf("Khong tim thay hoa don voi ma khach hang %s\n", id);
    }
}

// Tìm kiếm theo mã sản phẩm
void searchByProductID(LinkedList list) {
    char id[10];
    printf("\nNhap ma san pham can tim: ");
    scanf("%s", id);
    
    PNode ptr = list;
    int found = 0;
    while (ptr != NULL) {
        if (strcmp(ptr->data.productID, id) == 0) {
            printf("\nThong tin hoa don:\n");
            printf("Ma hoa don: %s\n", ptr->data.invoiceID);
            printf("Ma san pham: %s\n", ptr->data.productID);
            printf("Ma khach hang: %s\n", ptr->data.customerID);
            printf("So luong: %d\n", ptr->data.quantity);
            printf("Ngay mua: %02d:%02d:%02d:%04d\n", ptr->data.date.hour, 
                   ptr->data.date.day, ptr->data.date.month, ptr->data.date.year);
            printf("Tong gia: %.2f\n", ptr->data.totalPrice);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (!found) {
        printf("Khong tim thay hoa don voi ma san pham %s\n", id);
    }
}

// Tìm kiếm theo ngày
void searchByDate(LinkedList list) {
    struct DateTime date;
    printf("\nNhap ngay can tim (gio ngay thang nam): ");
    scanf("%d %d %d %d", &date.hour, &date.day, &date.month, &date.year);
    
    PNode ptr = list;
    int found = 0;
    while (ptr != NULL) {
        if (ptr->data.date.hour == date.hour && 
            ptr->data.date.day == date.day && 
            ptr->data.date.month == date.month && 
            ptr->data.date.year == date.year) {
            printf("\nThong tin hoa don:\n");
            printf("Ma hoa don: %s\n", ptr->data.invoiceID);
            printf("Ma san pham: %s\n", ptr->data.productID);
            printf("Ma khach hang: %s\n", ptr->data.customerID);
            printf("So luong: %d\n", ptr->data.quantity);
            printf("Ngay mua: %02d:%02d:%02d:%04d\n", ptr->data.date.hour, 
                   ptr->data.date.day, ptr->data.date.month, ptr->data.date.year);
            printf("Tong gia: %.2f\n", ptr->data.totalPrice);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (!found) {
        printf("Khong tim thay hoa don voi ngay %02d:%02d:%02d:%04d\n", 
               date.hour, date.day, date.month, date.year);
    }
}

// Hàm tìm kiếm hóa đơn (menu con)
void searchInvoice(LinkedList list) {
    int choice;
    while (1) {
        printf("\n=== TIM KIEM HOA DON ===\n");
        printf("1. Theo ma hoa don\n");
        printf("2. Theo ma khach hang\n");
        printf("3. Theo ma san pham\n");
        printf("4. Theo ngay mua\n");
        printf("5. Quay lai\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                searchByInvoiceID(list);
                break;
            case 2:
                searchByCustomerID(list);
                break;
            case 3:
                searchByProductID(list);
                break;
            case 4:
                searchByDate(list);
                break;
            case 5:
                return; // Quay lại menu hóa đơn
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}

// Hiển thị danh sách hóa đơn
void displayInvoice(LinkedList list) {
    if (isEmpty(list)) {
        printf("\nDanh sach hoa don rong!\n");
        return;
    }
    printf("\nDanh sach hoa don:\n");
    PNode ptr = list;
    while (ptr != NULL) {
        printf("Ma hoa don: %s | Ma san pham: %s | Ma khach hang: %s | "
               "So luong: %d | Ngay mua: %02d:%02d:%02d:%04d | Tong gia: %.2f\n",
               ptr->data.invoiceID, ptr->data.productID, ptr->data.customerID,
               ptr->data.quantity, ptr->data.date.hour, ptr->data.date.day,
               ptr->data.date.month, ptr->data.date.year, ptr->data.totalPrice);
        ptr = ptr->next;
    }
}

// Thống kê theo ngày
void statisticByDate(LinkedList list) {
    struct DateTime date;
    printf("\nNhap ngay can thong ke (ngay thang nam): ");
    scanf("%d %d %d", &date.day, &date.month, &date.year);
    
    PNode ptr = list;
    int count = 0;
    float totalRevenue = 0.0;
    
    while (ptr != NULL) {
        if (ptr->data.date.day == date.day && 
            ptr->data.date.month == date.month && 
            ptr->data.date.year == date.year) {
            count++;
            totalRevenue += ptr->data.totalPrice;
        }
        ptr = ptr->next;
    }
    
    printf("\nThong ke ngay %02d:%02d:%04d:\n", date.day, date.month, date.year);
    printf("Tong so hoa don: %d\n", count);
    printf("Tong doanh thu: %.2f\n", totalRevenue);
}

// Menu quản lý hóa đơn
void invoiceMenu(LinkedList* list) {
    int choice;
    while (1) {
        printf("\n=== QUAN LY HOA DON ===\n");
        printf("1. Them hoa don\n");
        printf("2. Sua hoa don\n");
        printf("3. Xoa hoa don\n");
        printf("4. Tim kiem hoa don\n");
        printf("5. Hien thi danh sach hoa don\n");
        printf("6. Thong ke\n");
        printf("7. Quay lai\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                insertInvoice(list);
                break;
            case 2:
                editInvoice(list);
                break;
            case 3:
                deleteInvoice(list);
                break;
            case 4:
                searchInvoice(*list);
                break;
            case 5:
                displayInvoice(*list);
                break;
            case 6:
                statisticByDate(*list);
                break;
            case 7:
                return; // Quay lại menu chính
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}

// Menu chính
void mainMenu() {
    printf("\n=== HE THONG QUAN LY BAN HANG ===\n");
    printf("1. Quan ly khach hang\n");
    printf("2. Quan ly san pham\n");
    printf("3. Quan ly hoa don\n");
    printf("4. Thoat\n");
    printf("Chon chuc nang: ");
}

