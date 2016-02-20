#! /bin/sh

# show usage
show_usage="args: [-g <yes/no>, -d <yes/no>]	\
                  [--gui <yes/no>, --debug <yes/no>]"
opt_gui="no"
opt_debug="no"

GETOPT_ARGS=`getopt -o g:d: -al gui:,debug: -- "$@"`
eval set -- "$GETOPT_ARGS"
# get parameter
while [ -n "$1" ]
do
    case "$1" in
        -g|--gui) opt_gui=$2; shift 2;;
        -d|--debug) opt_debug=$2; shift 2;;
        --) break ;;
        *) echo $1,$2,$show_usage; break ;;
    esac
done
echo "#! /bin/sh" > tmp_qemu_run.sh
echo "cd out" >> tmp_qemu_run.sh
echo "qemu-system-arm -kernel rootfs/zImage -dtb rootfs/vexpress-v2p-ca9.dtb -M vexpress-a9 \\" >> tmp_qemu_run.sh
if [ $opt_gui = "no" ]; then
	echo "  -nographic -append \"init=/linuxrc root=/dev/mmcblk0 rw console=ttyAMA0\"\\" >> tmp_qemu_run.sh
	echo "  -sd sdcard.ext3 -smp 4 -m 512M" >> tmp_qemu_run.sh
else
	echo "  -append \"init=/linuxrc root=/dev/mmcblk0 rw console=ttyAMA0 console=tty0\" \\" >> tmp_qemu_run.sh
	echo "  -sd sdcard.ext3 -serial stdio -net nic -net user -smp 4 -m 512M" >> tmp_qemu_run.sh
fi

chmod +x tmp_qemu_run.sh
./tmp_qemu_run.sh
exit
