#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[20];
    char name[50];
    double priceImport;
    double priceSale;
    int quantity;
} Product;

typedef struct Node {
    Product product;
    struct Node* next;
} Node;

// Hàm kiểm tra trùng ID trong danh sách liên kết
int isDuplicateID(Node* head, const char* id) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->product.id, id) == 0) {
            return 1; // trùng ID
        }
        current = current->next;
    }
    return 0; // không trùng
}

// Thêm sản phẩm mới (có kiểm tra trùng ID)
void inputProductWithCheck(Node** head) {
    Product newProduct;
    char tempID[20];

    while (1) {
        printf("Mã sản phẩm: ");
        fgets(tempID, sizeof(tempID), stdin);
        tempID[strcspn(tempID, "\n")] = '\0';

        if (isDuplicateID(*head, tempID)) {
            printf("Lỗi: Mã sản phẩm đã tồn tại. Vui lòng nhập lại.\n");
        } else {
            strcpy(newProduct.id, tempID);
            break;
        }
    }

    printf("Tên sản phẩm: ");
    fgets(newProduct.name, sizeof(newProduct.name), stdin);
    newProduct.name[strcspn(newProduct.name, "\n")] = '\0';

    printf("Giá nhập: ");
    scanf("%lf", &newProduct.priceImport);

    printf("Giá bán: ");
    scanf("%lf", &newProduct.priceSale);

    printf("Số lượng: ");
    scanf("%d", &newProduct.quantity);
    getchar();

    // Tạo node mới và thêm vào cuối danh sách
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Lỗi cấp phát bộ nhớ!\n");
        return;
    }
    newNode->product = newProduct;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Đã thêm sản phẩm mới.\n");
}

// Hiển thị danh sách sản phẩm
void printProducts(Node* head) {
    printf("\n%-10s %-20s %-12s %-12s %-8s\n", "ID", "Name", "PriceImport", "PriceSale", "Quantity");
    printf("---------------------------------------------------------------------\n");
    Node* current = head;
    while (current != NULL) {
        Product p = current->product;
        printf("%-10s %-20s %-12.2lf %-12.2lf %-8d\n", p.id, p.name, p.priceImport, p.priceSale, p.quantity);
        current = current->next;
    }
}

// Ghi toàn bộ danh sách vào file CSV
void writeToCSV(Node* head) {
    FILE* f = fopen("products.csv", "w");
    if (!f) {
        printf("Không thể mở file để ghi!\n");
        return;
    }
    fprintf(f, "ID,Name,PriceImport,PriceSale,Quantity\n");
    Node* current = head;
    while (current != NULL) {
        Product p = current->product;
        fprintf(f, "%s,%s,%.2lf,%.2lf,%d\n", p.id, p.name, p.priceImport, p.priceSale, p.quantity);
        current = current->next;
    }
    fclose(f);
    printf("Đã ghi dữ liệu ra file CSV.\n");
}

// Đọc dữ liệu từ file CSV vào linked list
void loadFromCSV(Node** head) {
    FILE* f = fopen("products.csv", "r");
    if (!f) {
        printf("Không thể mở file để đọc hoặc file chưa tồn tại.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), f); // Bỏ dòng header

    // Xóa danh sách cũ nếu có
    while (*head != NULL) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    while (fgets(line, sizeof(line), f)) {
        Product p;
        char* token = strtok(line, ",");
        if (token == NULL) continue;
        strcpy(p.id, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(p.name, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        p.priceImport = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        p.priceSale = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        p.quantity = atoi(token);

        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) {
            printf("Lỗi cấp phát bộ nhớ!\n");
            fclose(f);
            return;
        }
        newNode->product = p;
        newNode->next = NULL;

        if (*head == NULL) {
            *head = newNode;
        } else {
            Node* temp = *head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
    }
    fclose(f);
    printf("Đã đọc dữ liệu từ file CSV.\n");
}

// Sửa thông tin sản phẩm theo ID
void updateProduct(Node* head) {
    if (head == NULL) {
        printf("Danh sách rỗng!\n");
        return;
    }
    char targetID[20];
    printf("Nhập ID sản phẩm cần sửa: ");
    getchar();
    fgets(targetID, sizeof(targetID), stdin);
    targetID[strcspn(targetID, "\n")] = '\0';

    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->product.id, targetID) == 0) {
            printf("Sản phẩm tìm thấy. Nhập thông tin mới:\n");

            printf("Tên mới: ");
            fgets(current->product.name, sizeof(current->product.name), stdin);
            current->product.name[strcspn(current->product.name, "\n")] = '\0';

            printf("Giá nhập mới: ");
            scanf("%lf", &current->product.priceImport);

            printf("Giá bán mới: ");
            scanf("%lf", &current->product.priceSale);

            printf("Số lượng mới: ");
            scanf("%d", &current->product.quantity);
            getchar();

            printf("Cập nhật thành công.\n");
            return;
        }
        current = current->next;
    }
    printf("Không tìm thấy sản phẩm với ID %s\n", targetID);
}

// Xóa sản phẩm theo ID
void deleteProduct(Node** head) {
    if (*head == NULL) {
        printf("Danh sách rỗng!\n");
        return;
    }
    char targetID[20];
    printf("Nhập ID sản phẩm cần xóa: ");
    getchar();
    fgets(targetID, sizeof(targetID), stdin);
    targetID[strcspn(targetID, "\n")] = '\0';

    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->product.id, targetID) == 0) {
            if (prev == NULL) {
                // Xóa node đầu
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Đã xóa sản phẩm.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Không tìm thấy sản phẩm với ID %s\n", targetID);
}

// Giải phóng toàn bộ danh sách
void freeList(Node** head) {
    Node* current = *head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}
// Hiển thị 1 sản phẩm theo ID
void showProductByID(ProductNode *head) {
    char id[20];
    printf("Nhập ID sản phẩm cần tìm: ");
    getchar();
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    ProductNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            printf("Sản phẩm được tìm thấy:\n");
            printf("%-10s %-15s %-12s %-10s %-8s\n", "ID", "Name", "PriceImport", "PriceSale", "Qty");
            printf("%-10s %-15s %-12.2lf %-10.2lf %-8d\n",
                   current->data.id, current->data.name,
                   current->data.priceImport, current->data.priceSale,
                   current->data.quantity);
            return;
        }
        current = current->next;
    }
    printf("Không tìm thấy sản phẩm với ID: %s\n", id);
}

// Giảm số lượng sản phẩm theo ID
ProductNode* decreaseQuantity(ProductNode *head) {
    char id[20];
    int reduce;
    printf("Nhập ID sản phẩm cần giảm số lượng: ");
    getchar();
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    ProductNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            printf("Sản phẩm hiện tại có %d đơn vị.\n", current->data.quantity);
            printf("Nhập số lượng muốn giảm: ");
            scanf("%d", &reduce);

            if (reduce <= 0) {
                printf("Số lượng giảm phải > 0.\n");
                return head;
            }

            if (reduce > current->data.quantity) {
                printf("Không đủ hàng để giảm.\n");
                return head;
            }

            current->data.quantity -= reduce;

            if (current->data.quantity == 0) {
                printf("Số lượng về 0. Xóa sản phẩm khỏi danh sách.\n");
                head = deleteProduct(head, id);
            } else {
                printf("Đã giảm số lượng. Số còn lại: %d\n", current->data.quantity);
            }

            return head;
        }
        current = current->next;
    }

    printf("Không tìm thấy sản phẩm với ID: %s\n", id);
    return head;
}


void productProgram() {
    Node* head = NULL;
    int choice;

    loadFromCSV(&head);

    do {
        printf("\n===== MENU =====\n");
        printf("1. Thêm sản phẩm mới\n");
        printf("2. Hiển thị danh sách sản phẩm\n");
        printf("3. Sửa thông tin sản phẩm\n");
        printf("4. Xóa sản phẩm\n");
        printf("5. Lưu danh sách ra file CSV\n");
        printf("6. Hiển thị 1 sản phẩm theo ID\n");
        printf("0. Thoát\n");
        printf("Chọn: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                inputProductWithCheck(&head);
                break;
            case 2:
                printProducts(head);
                break;
            case 3:
                updateProduct(head);
                break;
            case 4:
                deleteProduct(&head);
                break;
            case 5:
                writeToCSV(head);
                break;
            case 6:
                showProductByID(head);
                break;
            case 0:
                printf("Thoát chương trình.\n");
                break;
            default:
                printf("Lựa chọn không hợp lệ.\n");
        }
    } while (choice != 0);

    freeList(&head);
    return 0;
}
