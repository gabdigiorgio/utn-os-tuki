find ../ -type f -name '*.log' -delete;
find ../ -type f -name '*.o' -delete;
find ../ -type f -name '*.d' -delete;
rm ../memoria/Debug/memoria;
rm ../kernel/Debug/kernel;
rm ../cpu/Debug/cpu;
rm ../file-system/Debug/file-system