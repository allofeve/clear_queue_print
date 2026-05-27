#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

int main() {
    // ตั้งค่าให้หน้าต่าง Console แสดงผลแบบ Unicode (UTF-16)
    // วิธีนี้จะช่วยให้ภาษาไทยแสดงผลได้แม่นยำที่สุดบน Windows
    _setmode(_fileno(stdout), _O_U16TEXT);

    wprintf(L"==========================================\n");
    wprintf(L"   โปรแกรมล้างคิวงานพิมพ์ (Print Spooler Cleaner)\n");
    wprintf(L"==========================================\n\n");

    // 1. หยุดบริการ Print Spooler
    wprintf(L"[1/3] กำลังหยุดบริการ Spooler...\n");
    int status = system("net stop spooler");

    if (status != 0) {
        wprintf(L"\n------------------------------------------\n");
        wprintf(L"เกิดข้อผิดพลาด: ไม่สามารถหยุดบริการได้!\n");
        wprintf(L"สาเหตุ: คุณไม่ได้รันโปรแกรมด้วยสิทธิ์ Administrator\n");
        wprintf(L"วิธีแก้: คลิกขวาที่ไฟล์ .exe แล้วเลือก 'Run as administrator'\n");
        wprintf(L"------------------------------------------\n");
        system("pause");
        return 1;
    }

    // 2. ลบคิวงาน [cite: 1]
    wprintf(L"\n[2/3] กำลังลบคิวงานที่ค้างอยู่...\n");
    system("del /Q /F /S \"%systemroot%\\System32\\spool\\Printers\\*.*\""); // [cite: 1]

    // 3. เริ่มบริการใหม่ [cite: 1]
    wprintf(L"\n[3/3] กำลังเริ่มบริการ Spooler ใหม่อีกครั้ง...\n");
    system("net start spooler"); // [cite: 1]

    wprintf(L"\n==========================================\n");
    wprintf(L"ดำเนินการเสร็จสิ้น! คิวงานพิมพ์ถูกล้างแล้ว\n");
    wprintf(L"==========================================\n");

    system("pause");
    return 0;
}