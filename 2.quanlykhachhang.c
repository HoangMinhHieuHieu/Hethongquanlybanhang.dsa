#include "customer.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Customer* customerList = NULL;

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
    
    saveCustomersToFile();
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

void loadCustomersFromFile() {
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

void customerMenu() {
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
