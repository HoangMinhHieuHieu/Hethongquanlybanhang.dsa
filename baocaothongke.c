#include "report.h"

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
