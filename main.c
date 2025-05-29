#include "hoadon.h"

int main() {
    LinkedList list;
    initList(&list);
    int choice;
    
    while (1) {
        mainMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("\nChuc nang quan ly khach hang chua duoc trien khai!\n");
                break;
            case 2:
                printf("\nChuc nang quan ly san pham chua duoc trien khai!\n");
                break;
            case 3:
                invoiceMenu(&list);
                break;
            case 4:
                printf("Tam biet!\n");
                // Giải phóng bộ nhớ trước khi thoát
                while (!isEmpty(list)) {
                    PNode temp = list;
                    list = list->next;
                    free(temp);
                }
                return 0;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}