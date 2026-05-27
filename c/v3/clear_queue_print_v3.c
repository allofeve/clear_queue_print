#include <windows.h>
#include <stdio.h>

// ฟังก์ชันสำหรับตรวจสอบว่าโปรแกรมรันด้วยสิทธิ์ Administrator หรือไม่
BOOL IsUserAnAdmin() {
    BOOL bIsAdmin = FALSE;
    PSID AdministratorsGroup = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                 &AdministratorsGroup)) {
        CheckTokenMembership(NULL, AdministratorsGroup, &bIsAdmin);
        FreeSid(AdministratorsGroup);
    }
    return bIsAdmin;
}

int main() {
    // 1. ตรวจสอบสิทธิ์ Administrator (แทนส่วน :UACPrompt ใน Batch)
    if (!IsUserAnAdmin()) {
        char szPath[MAX_PATH];
        GetModuleFileNameA(NULL, szPath, MAX_PATH);

        // บังคับให้รันใหม่ด้วยสิทธิ์ Admin ผ่าน ShellExecute
        SHELLEXECUTEINFOA sei = { sizeof(sei) };
        sei.lpVerb = "runas";  // ขอสิทธิ์ Administrator
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExA(&sei)) {
            return 0; // ปิดโปรแกรมตัวเดิมที่ไม่มีสิทธิ์ Admin
        } else {
            MessageBoxA(NULL, "จำเป็นต้องรันโปรแกรมนี้ด้วยสิทธิ์ Administrator!", "ข้อผิดพลาด", MB_ICONERROR | MB_OK);
            return 1;
        }
    }

    // ตั้งค่า Directory ให้ตรงกับตำแหน่งของโปรแกรม (แทน cd /d "%~dp0")
    char szCurrentDir[MAX_PATH];
    GetModuleFileNameA(NULL, szCurrentDir, MAX_PATH);
    char *lastSlash = strrchr(szCurrentDir, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
        SetCurrentDirectoryA(szCurrentDir);
    }

    // 2. หยุดบริการ Print Spooler (net stop spooler)
    system("net stop spooler");

    // 3. ลบไฟล์คิวงานพิมพ์ในโฟลเดอร์ Printers (del /Q /F /S ...)
    // ใช้ %systemroot% จาก Environment Variable เพื่อความปลอดภัย
    char systemRoot[MAX_PATH];
    GetEnvironmentVariableA("systemroot", systemRoot, MAX_PATH);
    
    char deleteCommand[MAX_PATH + 50];
    snprintf(deleteCommand, sizeof(deleteCommand), "del /Q /F /S \"%s\\System32\\spool\\Printers\\*.*\"", systemRoot);
    system(deleteCommand);

    // 4. เริ่มบริการ Print Spooler ใหม่ (net start spooler)
    system("net start spooler");

    // 5. แสดงกล่องข้อความแจ้งเตือนเมื่อเสร็จสิ้น (แทนส่วนสร้างไฟล์ VBS)
    // ใช้ MessageBoxW (Unicode) เพื่อรองรับภาษาไทยอย่างสมบูรณ์แบบ ไม่ต้องกังวลเรื่องการเข้ารหัส ANSI/TIS-620
    MessageBoxW(NULL, L"ลบคิวปริ้นเตอร์ที่ค้างเรียบร้อยแล้ว", L"ระบบจัดการคิวปริ้น", MB_ICONINFORMATION | MB_OK);

    return 0;
}