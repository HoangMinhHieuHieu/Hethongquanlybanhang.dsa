#include "tienich.h"
#include "manageproduct.c"
#include "baocaothongke.h"
#include "KHang.c"
int main(){
  cout << "chào mừng đến với chương trình";
  cout << "MENU"<< \n << 
    "1.Quản lí sản phẩm" << \n <<
    "2.Quản lí khách hàng" << \n <<
    "3.Quản lí hóa đơn" << \n << 
    "4.Báo cáo thống kê " << \n <<
    "0.Thoát<<;
  int choice;
  cin >> choice;
  switch( choice !=0){
    case 1:
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
            case 0:
                printf("Thoát chương trình.\n");
                break;
            default:
                printf("Lựa chọn không hợp lệ.\n");
        }
    } while (choice != 0);

    freeList(&head);
    return 0;
    case 2:
    case 3:
    case 4:
    case 0:
    default:
    cout << "Lỗi. Vui lòng nhập lại";

  }
}
