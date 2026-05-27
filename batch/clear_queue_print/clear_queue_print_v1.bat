net stop spooler
del /Q /F /S "%systemroot%\System32\spool\Printers\*.*"
net start spooler