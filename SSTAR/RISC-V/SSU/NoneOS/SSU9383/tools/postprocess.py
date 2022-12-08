#!/usr/bin/python

import os, sys, struct


def main():
    if len(sys.argv) != 4:
        print('ERROR! Wrong argument number')
        exit()

    objcopy_cmd = sys.argv[1] + " -O binary -R .bss -R .bss_sram " + sys.argv[2] + " " + sys.argv[3]
    os.system(objcopy_cmd)

    if not os.path.exists(sys.argv[3]):
        print('ERROR! NO target file')
        exit()

    size = os.path.getsize(sys.argv[3])
    pad_size = 16-size%16
    f = open(sys.argv[3], 'a+')
    for x in range(pad_size):
        f.write('%c' % 0x00)
    f.close()

    f = open(sys.argv[3], 'rb+')
    f.seek(32, 0)
    if f.read(3).decode("utf-8") == 'NOS':
        f.seek(36, 0)
        f.write(struct.pack('I', size))
        print('Add NonOS size header=0x%X(%d)' % (size, size))
    else:
        print('ERROR! NO NonOS HEADER!')
        f.close()
        exit()

    check_sum = 0
    count = 0
    size_without_version = size - 48
    if (size_without_version % 4) != 0:
        print('ERROR! file size no 4-bytes alignment!')
        f.close()
        exit()
    f.seek(48, 0)
    while (count != size_without_version):
        bytes = f.read(4)
        if bytes == "":
            break;
        data = struct.unpack("I", bytes)
        # print('0x%08X' % (data))
        check_sum += data[0]
        count += 4
    # print('0x%08X' % (count))

    # Update CRC result to header
    print('check_sum=0x%08X' % (check_sum & 0xffffffff))
    f.seek(40, 0)
    f.write(struct.pack('I', (check_sum & 0xffffffff)))

    # Update load address to header
    ld_addr = 0x10000000
    f.seek(44, 0)
    f.write(struct.pack('I', ld_addr))

    f.close()


if __name__ == '__main__':
    main()
