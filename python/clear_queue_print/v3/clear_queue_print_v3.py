import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import ctypes
import sys

def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False

def cleanup_spooler():
    try:
        # 1. บังคับปิดโปรแกรมจัดการคิวงานพิมพ์และโปรเซสที่เกี่ยวข้อง
        # ปิด printui.exe (ตัวจัดการหน้าจอคิวงาน)
        subprocess.run(["taskkill", "/f", "/im", "printui.exe", "/t"], capture_output=True)
        # ปิดตัวกรองและจัดการงานพิมพ์ของระบบ
        subprocess.run(["taskkill", "/f", "/im", "printfiltersvc.exe", "/t"], capture_output=True)
        subprocess.run(["taskkill", "/f", "/im", "printisolationhost.exe", "/t"], capture_output=True)
        # กรณีเป็นโปรแกรมเฉพาะของ Canon อาจลองปิดตัวที่เกี่ยวข้องกับ Spl
        subprocess.run(["taskkill", "/f", "/im", "spoolsv.exe", "/t"], capture_output=True)

        # 2. หยุดบริการ Spooler (ทำซ้ำเพื่อให้มั่นใจว่าหยุดจริง)
        subprocess.run(["net", "stop", "spooler"], check=True, capture_output=True)
        
        # 3. ลบไฟล์ในโฟลเดอร์ Printers
        spool_path = os.path.join(os.environ['SystemRoot'], "System32", "spool", "Printers")
        if os.path.exists(spool_path):
            for filename in os.listdir(spool_path):
                file_path = os.path.join(spool_path, filename)
                try:
                    if os.path.isfile(file_path):
                        os.unlink(file_path)
                except Exception as e:
                    print(f"ไม่สามารถลบ {filename}: {e}")

        # 4. เริ่มบริการ Spooler ใหม่
        subprocess.run(["net", "start", "spooler"], check=True, capture_output=True)
        
        messagebox.showinfo("Success", "ดำเนินการเสร็จสิ้น!\nหน้าต่างคิวงานพิมพ์ควรจะถูกปิดและล้างข้อมูลแล้ว")
    except subprocess.CalledProcessError:
        messagebox.showerror("Error", "เกิดข้อผิดพลาดในการจัดการ Service!\nโปรดตรวจสอบสิทธิ์ Admin")
    except Exception as e:
        messagebox.showerror("Error", f"เกิดข้อผิดพลาด: {str(e)}")

def run_gui():
    if not is_admin():
        ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
        sys.exit()

    root = tk.Tk()
    root.title("Print Spooler Cleaner")
    root.geometry("300x180")

    label = tk.Label(root, text="เครื่องมือล้างคิวงานพิมพ์\n(Force Close All Windows)", font=("Helvetica", 12))
    label.pack(pady=20)

    btn_cleanup = tk.Button(root, text="เริ่มการทำงาน", command=cleanup_spooler, 
                           bg="#4CAF50", fg="white", padx=20, pady=10)
    btn_cleanup.pack()

    root.mainloop()

if __name__ == "__main__":
    run_gui()