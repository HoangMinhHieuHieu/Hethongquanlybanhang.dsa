#include "utils.h"

// Xóa các ký tự còn sót lại trong bộ đệm 
void clearInputBuffer() {
    while (getchar() != '\n');
}

// In thông báo, chờ người dùng nhấn Enter để tiếp tục chương trình.
void pressEnterToContinue() {
    printf("\nNhan Enter de tiep tuc...");
    clearInputBuffer();
    getchar();
}

// Dùng sscanf để tách ngày, tháng, năm từ chuỗi. Nếu không đúng định dạng dd/mm/yyyy thì lỗi
int isValidDate(const char* dateStr) {
    int day, month, year;
    if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) != 3)
        return 0;
    
// Kiểm tra các giới hạn hợp lý cho ngày, tháng, năm.
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return 0;
    
    if (month == 2) {
        int isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
        if (day > (isLeap ? 29 : 28))
            return 0;
    }
    
    return 1;
}

// Kiểm tra từng ký tự trong số điện thoại
int isValidPhone(const char* phone) {
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-')
            return 0;
    }
    return strlen(phone) >= 10 && strlen(phone) <= 15;
}

// Dùng để chuyển thời gian kiểu time_t sang chuỗi định dạng "dd/mm/yyyy".
char* formatTime(time_t time) {
    static char buffer[20];
    strftime(buffer, sizeof(buffer), DATE_FORMAT, localtime(&time));
    return buffer;
}
