#ifndef INVOICEMANAGEMENT_H
#define INVOICEMANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cấu trúc lưu ngày giờ
struct DateTime {
    int hour, day, month, year;
};

// Cấu trúc lưu thông tin hóa đơn
struct Invoice {
    char invoiceID[10]; // Mã hóa đơn
    char productID[10]; // Mã sản phẩm
    char customerID[10]; // Mã khách hàng
    int quantity; // Số lượng
    struct DateTime date; // Ngày mua
    float totalPrice; // Tổng giá (tự động tính)
};

// Cấu trúc node cho danh sách liên kết
struct Node {
    struct Invoice data;
    struct Node* next;
};

typedef struct Node* PNode;
typedef struct Node* LinkedList;

// Khai báo các hàm
void initList(LinkedList* list);
int isEmpty(LinkedList list);
void insertInvoice(LinkedList* list);
void editInvoice(LinkedList* list);
void deleteInvoice(LinkedList* list);
void searchByInvoiceID(LinkedList list);
void searchByCustomerID(LinkedList list);
void searchByProductID(LinkedList list);
void searchByDate(LinkedList list);
void searchInvoice(LinkedList list);
void displayInvoice(LinkedList list);
void statisticByDate(LinkedList list);
void invoiceMenu(LinkedList* list);
void mainMenu();

#endif 