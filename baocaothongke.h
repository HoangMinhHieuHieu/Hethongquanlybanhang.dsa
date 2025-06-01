#ifndef REPORT_H
#define REPORT_H

#include "../utils.h"
#include "../invoice/invoice.h" // Cần để truy vấn hóa đơn

// Khai báo hàm báo cáo
void revenueReport(); // Thống kê doanh thu
void productStats();  // Thống kê sản phẩm
void customerStats(); // Thống kê khách hàng
void reportMenu();    // Menu báo cáo
#endif
