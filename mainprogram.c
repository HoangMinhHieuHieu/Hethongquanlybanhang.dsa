#include "tienich.h"
#include "manageproduct.c"
#include "baocaothongke.h"
#include "KHang.c"
#include "InvoiceManagement.h"
#include"quanlyhoadon.h"

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
    productProgram();
    break;
    case 2:
    LinkedList customerList;
    initList(&customerList);
    customerMenu(&customerList);
    break;
    case 3:
    breaak;
    case 4:
    break;
    case 0:
    cout << "kết thúc chương trình";
    break;
    default:
    cout << "Lỗi. Vui lòng nhập lại";
    break;
  }
}
