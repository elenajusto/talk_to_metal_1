Set-ExecutionPolicy RemoteSigned

usbipd bind --busid 2-1

usbipd attach --wsl --hardware-id '0483:374b'

usbipd list

#Start-Sleep -Seconds 1

#wsl esptool -c esp32 --baud 921600 --after hard_reset write_flash -z 0x1000 .pio/build/esp32dev/bootloader.bin 0x8000 .pio/build/esp32dev/partitions.bin 0x10000 .pio/build/esp32dev/firmware.bin

#usbipd detach --hardware-id $HWID