find ../ -type f -name '*.o' -delete;
find ../ -type f -name '*.d' -delete;
echo "================= Make Memoria ================="
make -C ../memoria/Debug;
echo "================= File-System ================="
make -C ../file-system/Debug;
echo "================= CPU ================="
make -C ../cpu/Debug;
echo "================= Kernel ================="
make -C ../kernel/Debug;
echo "================= Consola ================="
make -C ../consola/Debug;
echo "================= Consola2 ================="
make -C ../consola2/Debug;
echo "================= Consola3 ================="
make -C ../consola3/Debug;
echo "================= Consola4 ================="
make -C ../consola4/Debug;