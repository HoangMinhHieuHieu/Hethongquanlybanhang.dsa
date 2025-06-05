#ifndef UTILS_H //Tránh bị include nhiều lần gây lỗi khai báo trùng.
#define UTILS_H

#include <stdio.h>   // Hàm nhập/xuất: printf, scanf, puts,...
#include <stdlib.h>  // Hàm cấp phát bộ nhớ, thoát chương trình
#include <string.h>  // Hàm xử lý chuỗi: strlen, strcpy, strcmp,...
#include <time.h>  // Làm việc với thời gian: time_t, struct tm,..
#include <ctype.h>  // Kiểm tra ký tự: isdigit, isalpha,...

// Xác định độ dài tối đa cho chuỗi:
#define MAX_ID_LEN 10
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_INVOICE_ID_LEN 15

// Tên file dữ liệu dùng để lưu thông tin sản phẩm, khách hàng, hóa đơn.
#define PRODUCT_FILE "products.dat"
#define CUSTOMER_FILE "customers.dat"
#define INVOICE_FILE "invoices.dat"

// Định dạng ngày tháng khi in hoặc đọc ngày: dd/mm/yyyy
#define DATE_FORMAT "%d/%m/%Y"

// Hàm tiện ích dùng chung
void clearInputBuffer(); //Xóa ký tự thừa trong bộ đệm
void pressEnterToContinue(); // In thông báo "Nhấn Enter để tiếp tục", dùng để tạm dừng màn hình
int isValidDate(const char* dateStr); //Kiểm tra chuỗi ngày nhập vào có đúng định dạng và hợp lệ không
int isValidPhone(const char* phone); //Kiểm tra số điện thoại hợp lệ (chỉ chứa số, độ dài hợp lý)
char* formatTime(time_t time); //Trả về chuỗi định dạng thời gian từ time_t theo định dạng

#endif
