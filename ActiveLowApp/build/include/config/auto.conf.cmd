deps_config := \
	/home/albert/ESP32_101/esp-idf/components/app_trace/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/aws_iot/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/bt/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/esp32/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/ethernet/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/fatfs/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/freertos/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/heap/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/libsodium/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/log/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/lwip/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/mbedtls/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/openssl/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/pthread/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/spi_flash/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/spiffs/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/tcpip_adapter/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/wear_levelling/Kconfig \
	/home/albert/ESP32_101/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
