#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Customer {
    char id[10];
    char name[50];
    char phone[13];
};

struct CustomerNode {
    struct Customer data;
    struct CustomerNode* next;
};

typedef struct CustomerNode CustomerNode;
typedef CustomerNode* PNode;
typedef CustomerNode* LinkedList;

// ===== KHỞI TẠO DANH SÁCH =====
void initList(LinkedList* list) {
    *list = NULL;
}

int isEmpty(LinkedList list) {
    return list == NULL;
}

// ===== THÊM KHÁCH HÀNG =====
void insertCustomer(LinkedList* list) {
    struct Customer c;
    printf("\nNhap ma khach hang: ");
    scanf("%s", c.id);
    printf("Nhap ten: ");
    scanf(" %[^\n]", c.name);
    printf("Nhap so dien thoai: ");
    scanf("%s", c.phone);

    PNode newNode = (PNode)malloc(sizeof(CustomerNode));
    newNode->data = c;
    newNode->next = NULL;

    if (*list == NULL) {
        *list = newNode;
    } else {
        PNode ptr = *list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newNode;
    }

    printf("Them khach hang thanh cong!\n");
}

// ===== SỬa KHÁCH HÀNG =====
void editCustomer(LinkedList* list) {
    char id[10];
    printf("\nNhap ma khach hang can sua: ");
    scanf("%s", id);

    PNode ptr = *list;
    while (ptr != NULL) {
        if (strcmp(ptr->data.id, id) == 0) {
            printf("Nhap ten moi: ");
            scanf(" %[^\n]", ptr->data.name);
            printf("Nhap so dien thoai moi: ");
            scanf("%s", ptr->data.phone);
            printf("Sua thong tin thanh cong!\n");
            return;
        }
        ptr = ptr->next;
    }
    printf("Khong tim thay khach hang co ma %s\n", id);
}

// ===== XOÁ KHÁCH HÀNG =====
void deleteCustomer(LinkedList* list) {
    char id[10];
    printf("\nNhap ma khach hang can xoa: ");
    scanf("%s", id);

    if (*list == NULL) {
        printf("Danh sach rong!\n");
        return;
    }

    if (strcmp((*list)->data.id, id) == 0) {
        PNode temp = *list;
        *list = (*list)->next;
        free(temp);
        printf("Xoa khach hang thanh cong!\n");
        return;
    }

    PNode ptr = *list;
    while (ptr->next != NULL) {
        if (strcmp(ptr->next->data.id, id) == 0) {
            PNode temp = ptr->next;
            ptr->next = temp->next;
            free(temp);
            printf("Xoa khach hang thanh cong!\n");
            return;
        }
        ptr = ptr->next;
    }

    printf("Khong tim thay khach hang co ma %s\n", id);
}

// ===== TÌM KIẾM KHÁCH HÀNG =====
void searchCustomer(LinkedList list) {
    char id[10];
    printf("\nNhap ma khach hang can tim: ");
    scanf("%s", id);

    PNode ptr = list;
    while (ptr != NULL) {
        if (strcmp(ptr->data.id, id) == 0) {
            printf("\n-- Thong tin khach hang --\n");
            printf("Ma KH: %s\n", ptr->data.id);
            printf("Ten: %s\n", ptr->data.name);
            printf("So dien thoai: %s\n", ptr->data.phone);
            return;
        }
        ptr = ptr->next;
    }

    printf("Khong tim thay khach hang co ma %s\n", id);
}

// ===== HIỂN THỊ DANH SÁCH =====
void displayCustomers(LinkedList list) {
    if (isEmpty(list)) {
        printf("\nDanh sach khach hang rong!\n");
        return;
    }

    printf("\n=== DANH SACH KHACH HANG ===\n");
    PNode ptr = list;
    while (ptr != NULL) {
        printf("Ma KH: %s | Ten: %s | SDT: %s\n", ptr->data.id, ptr->data.name, ptr->data.phone);
        ptr = ptr->next;
    }
}

// ===== MENU KHÁCH HÀNG =====
void customerMenu(LinkedList* list) {
    int choice;
    while (1) {
        printf("\n=== QUAN LY KHACH HANG ===\n");
        printf("1. Them khach hang\n");
        printf("2. Sua khach hang\n");
        printf("3. Xoa khach hang\n");
        printf("4. Tim kiem khach hang\n");
        printf("5. Hien thi danh sach\n");
        printf("6. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insertCustomer(list);
                break;
            case 2:
                editCustomer(list);
                break;
            case 3:
                deleteCustomer(list);
                break;
            case 4:
                searchCustomer(*list);
                break;
            case 5:
                displayCustomers(*list);
                break;
            case 6:
                return;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}

int main() {
    LinkedList customerList;
    initList(&customerList);

    customerMenu(&customerList);

    return 0;
}

