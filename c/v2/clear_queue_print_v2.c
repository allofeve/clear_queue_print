#include <windows.h>
#include <stdio.h>

int main() {
    // 1. ตรวจสอบสิทธิ์ Administrator เบื้องต้น และหยุดบริการ
    int status = system("net stop spooler");

    if (status != 0) {
        MessageBoxW(NULL, 
            L"เกิดข้อผิดพลาด: ไม่สามารถหยุดบริการได้!\nกรุณาคลิกขวาแล้วเลือก 'Run as administrator'", 
            L"Error", 
            MB_ICONERROR | MB_OK);
        return 1;
    }

    // 2. ลบคิวงานที่ค้างอยู่
    system("del /Q /F /S \"%systemroot%\\System32\\spool\\Printers\\*.*\"");

    // 3. เริ่มบริการใหม่
    system("net start spooler");

    // 4. แจ้งเตือนเมื่อเสร็จสิ้น
    MessageBoxW(NULL, 
        L"ดำเนินการเสร็จสิ้น!\nคิวงานพิมพ์ถูกล้างและเริ่มระบบใหม่แล้ว", 
        L"Success", 
        MB_ICONINFORMATION | MB_OK);

    return 0;
}