#include <windows.h>
#include <stdio.h>
#include <winsvc.h>

// ฟังก์ชันตรวจสอบสิทธิ์ Admin
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

// ฟังก์ชันควบคุม Service (หยุด/เริ่ม Print Spooler) แทนการใช้ system("net ...")
BOOL ControlPrintSpooler(DWORD dwControl) {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM) return FALSE;

    SC_HANDLE hService = OpenService(hSCM, "Spooler", SERVICE_STOP | SERVICE_START | SERVICE_QUERY_STATUS);
    if (!hService) {
        CloseServiceHandle(hSCM);
        return FALSE;
    }

    SERVICE_STATUS status;
    BOOL bResult = FALSE;

    if (dwControl == SERVICE_CONTROL_STOP) {
        bResult = ControlService(hService, SERVICE_CONTROL_STOP, &status);
        // รอให้ Service หยุดสนิท
        while (QueryServiceStatus(hService, &status) && status.dwCurrentState != SERVICE_STOPPED) {
            Sleep(100);
        }
    } else if (dwControl == SERVICE_CONTROL_CONTINUE) { // ใช้แทนการสั่ง Start
        bResult = StartService(hService, 0, NULL);
        // รอให้ Service เริ่มทำงานสนิท
        while (QueryServiceStatus(hService, &status) && status.dwCurrentState != SERVICE_RUNNING) {
            Sleep(100);
        }
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
    return bResult;
}

// ฟังก์ชันลบไฟล์ทั้งหมดในโฟลเดอร์คิวปริ้น แทนการใช้ system("del ...")
void ClearPrinterFolder() {
    char systemRoot[MAX_PATH];
    GetEnvironmentVariableA("systemroot", systemRoot, MAX_PATH);

    char szSearchPath[MAX_PATH];
    snprintf(szSearchPath, sizeof(szSearchPath), "%s\\System32\\spool\\Printers\\*.*", systemRoot);

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(szSearchPath, &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // ไม่ลบโฟลเดอร์ย้อนกลับ (.) และ (..)
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                char szFilePath[MAX_PATH];
                snprintf(szFilePath, sizeof(szFilePath), "%s\\System32\\spool\\Printers\\%s", systemRoot, findData.cFileName);
                
                // ลบคุณสมบัติ Read-only ออกก่อนลบไฟล์
                SetFileAttributesA(szFilePath, FILE_ATTRIBUTE_NORMAL);
                DeleteFileA(szFilePath);
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
}

int main() {
    if (!IsUserAnAdmin()) {
        char szPath[MAX_PATH];
        GetModuleFileNameA(NULL, szPath, MAX_PATH);

        SHELLEXECUTEINFOA sei = { sizeof(sei) };
        sei.lpVerb = "runas";
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExA(&sei)) return 0;
        else return 1;
    }

    // 1. หยุด Print Spooler
    ControlPrintSpooler(SERVICE_CONTROL_STOP);

    // 2. ลบไฟล์คิวงานพิมพ์
    ClearPrinterFolder();

    // 3. เริ่ม Print Spooler ใหม่
    ControlPrintSpooler(SERVICE_CONTROL_CONTINUE);

    // 4. แจ้งเตือน
    MessageBoxW(NULL, L"ลบคิวปริ้นเตอร์ที่ค้างเรียบร้อยแล้ว", L"ระบบจัดการคิวปริ้น", MB_ICONINFORMATION | MB_OK);

    return 0;
}