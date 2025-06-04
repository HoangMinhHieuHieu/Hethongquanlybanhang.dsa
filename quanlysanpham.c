#include "product.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Product* productList = NULL;

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
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Gia", "SL Ton");
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
    printf("So luong ton kho: "); scanf("%d", &quantity);
    
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
    
    saveProductsToFile(); 
    printf("Them san pham thanh cong!\n");
    pressEnterToContinue();
}

void displayProducts() {
    printf("\nDANH SACH SAN PHAM\n");
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Gia", "SL Ton");
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

void loadProductsFromFile() {
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

void productMenu() {
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