#include "report.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Hàm thống kê doanh thu theo khoảng thời gian
void revenueReport() {
    char startStr[20], endStr[20];
    printf("\nTHONG KE DOANH THU\n");
    
    // Nhập ngày bắt đầu
    do {
        printf("Nhap ngay bat dau (dd/mm/yyyy): ");
        scanf("%s", startStr);
        if (!isValidDate(startStr)) {
            printf("Ngay khong hop le! Vui long nhap lai.\n");
        }
    } while (!isValidDate(startStr));
    
    // Nhập ngày kết thúc
    do {
        printf("Nhap ngay ket thuc (dd/mm/yyyy): ");
        scanf("%s", endStr);
        if (!isValidDate(endStr)) {
            printf("Ngay khong hop le! Vui long nhap lai.\n");
        }
    } while (!isValidDate(endStr));

    // Chuyển đổi ngày thành kiểu time_t
    struct tm tmStart = {0}, tmEnd = {0};
    sscanf(startStr, "%d/%d/%d", &tmStart.tm_mday, &tmStart.tm_mon, &tmStart.tm_year);
    tmStart.tm_mon -= 1; // tm_mon từ 0-11
    tmStart.tm_year -= 1900; // Năm tính từ 1900
    time_t startDate = mktime(&tmStart);

    sscanf(endStr, "%d/%d/%d", &tmEnd.tm_mday, &tmEnd.tm_mon, &tmEnd.tm_year);
    tmEnd.tm_mon -= 1;
    tmEnd.tm_year -= 1900;
    tmEnd.tm_hour = 23; // Đặt giờ cuối ngày
    tmEnd.tm_min = 59;
    tmEnd.tm_sec = 59;
    time_t endDate = mktime(&tmEnd);

    float totalRevenue = 0;
    int invoiceCount = 0;
    
    printf("\nHOA DON TU NGAY %s DEN %s\n", startStr, endStr);
    printf("%-15s %-20s %-15s\n", "Ma HD", "Ngay", "Tong tien");
    printf("--------------------------------------------\n");
    
    // Duyệt qua tất cả hóa đơn
    Invoice* current = invoiceList;
    while (current != NULL) {
        // Kiểm tra hóa đơn trong khoảng ngày
        if (current->date >= startDate && current->date <= endDate) {
            printf("%-15s %-20s %15.2f\n", 
                  current->id, formatTime(current->date), current->total);
            totalRevenue += current->total;
            invoiceCount++;
        }
        current = current->next;
    }
    
    // In tổng kết
    printf("\nTONG CONG:\n");
    printf("So luong hoa don: %d\n", invoiceCount);
    printf("Tong doanh thu: %.2f\n", totalRevenue);
    pressEnterToContinue();
}

// Hàm thống kê sản phẩm đã bán
void productStats() {
    printf("\nTHONG KE SAN PHAM\n"); // in tiêu đề bảng thống kê
    printf("%-10s %-20s %-10s %-10s\n", "Ma SP", "Ten SP", "Da ban", "Doanh thu");
    printf("--------------------------------------------------\n");
    
    Product* p = productList;
    while (p != NULL) {
        int sold = 0;
        float revenue = 0;
        
        Invoice* inv = invoiceList;
        while (inv != NULL) {
            InvoiceDetail* det = inv->details;
            while (det != NULL) {
                if (strcmp(det->productId, p->id) == 0) {
                    sold += det->quantity;
                    revenue += det->quantity * det->price;
                }
                det = det->next;
            }
            inv = inv->next;
        }
        
        printf("%-10s %-20s %10d %10.2f\n", 
               p->id, p->name, sold, revenue);
        p = p->next;
    }
    
    pressEnterToContinue();
}

// Hàm thống kê khách hàng
void customerStats() {
    printf("\nTHONG KE KHACH HANG\n"); // in tiêu đề thống kê khách hàng
    printf("%-10s %-20s %-10s %-15s\n", "Ma KH", "Ten KH", "So HD", "Tong chi tieu");
    printf("--------------------------------------------------\n");
    
    Customer* c = customerList;
    while (c != NULL) {
        int invoiceCount = 0;
        float totalSpent = 0;
        
        Invoice* inv = invoiceList;
        while (inv != NULL) {
            if (strcmp(inv->customerId, c->id) == 0) {
                invoiceCount++;
                totalSpent += inv->total;
            }
            inv = inv->next;
        }
        
        printf("%-10s %-20s %10d %15.2f\n", 
               c->id, c->name, invoiceCount, totalSpent);
        c = c->next;
    }
    
    pressEnterToContinue();
}

// Hàm hiển thị menu báo cáo
void reportMenu() {
    int choice;
    do {
        printf("\nBAO CAO - THONG KE\n");
        printf("1. Thong ke doanh thu\n");
        printf("2. Thong ke san pham\n");
        printf("3. Thong ke khach hang\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: "); scanf("%d", &choice);
        
        switch (choice) {
            case 1: revenueReport(); break;
            case 2: productStats(); break;
            case 3: customerStats(); break;
            case 0: break;
            default: printf("Lua chon khong hop le!\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}
