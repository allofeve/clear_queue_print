windres resources.rc -O coff -o resources.res
gcc clear_queue_print_v4.c resources.res -o clear_queue_print_v4.exe -lshlwapi
pause