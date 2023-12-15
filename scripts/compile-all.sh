find ../ -type f -name '*.o' -delete;
find ../ -type f -name '*.d' -delete;
echo "================= Make Memoria ================="
make main-build -C ../memoria/Debug;
echo "================= CPU ================="
make main-build -C ../cpu/Debug;
echo "================= Kernel ================="
make main-build -C ../kernel/Debug;
echo "================= File-System ================="
make main-build -C ../file-system/Debug;
echo "================= Consola ================="
make main-build -C ../consola/Debug;
echo "================= Consola2 ================="
make main-build -C ../consola2/Debug;
echo "================= Consola3 ================="
make main-build -C ../consola3/Debug;
echo "================= Consola4 ================="
make main-build -C ../consola4/Debug;