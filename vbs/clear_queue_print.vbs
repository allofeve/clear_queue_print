Set objShell = WScript.CreateObject("WScript.Shell")

' 1. หยุดบริการ Print Spooler
objShell.Run "cmd /c net stop spooler", 0, True

' 2. ลบไฟล์งานพิมพ์ที่ค้างอยู่ในโฟลเดอร์ Printers
' ใช้ cmd /c เพื่อให้สามารถใช้ wildcards (*.*) ได้
objShell.Run "cmd /c del /Q /F /S ""%systemroot%\System32\spool\Printers\*.*""", 0, True

' 3. เริ่มบริการ Print Spooler อีกครั้ง
objShell.Run "cmd /c net start spooler", 0, True

MsgBox "ลบคิวงานพิมพ์ที่ค้างอยู่เรียบร้อยแล้ว", vbInformation, "เสร็จสิ้น"