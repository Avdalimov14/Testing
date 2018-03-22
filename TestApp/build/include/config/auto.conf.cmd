deps_config := \
	/home/albert/Testing/esp-idf/components/app_trace/Kconfig \
	/home/albert/Testing/esp-idf/components/aws_iot/Kconfig \
	/home/albert/Testing/esp-idf/components/bt/Kconfig \
	/home/albert/Testing/esp-idf/components/esp32/Kconfig \
	/home/albert/Testing/esp-idf/components/ethernet/Kconfig \
	/home/albert/Testing/esp-idf/components/fatfs/Kconfig \
	/home/albert/Testing/esp-idf/components/freertos/Kconfig \
	/home/albert/Testing/esp-idf/components/heap/Kconfig \
	/home/albert/Testing/esp-idf/components/libsodium/Kconfig \
	/home/albert/Testing/esp-idf/components/log/Kconfig \
	/home/albert/Testing/esp-idf/components/lwip/Kconfig \
	/home/albert/Testing/esp-idf/components/mbedtls/Kconfig \
	/home/albert/Testing/esp-idf/components/openssl/Kconfig \
	/home/albert/Testing/esp-idf/components/pthread/Kconfig \
	/home/albert/Testing/esp-idf/components/spi_flash/Kconfig \
	/home/albert/Testing/esp-idf/components/spiffs/Kconfig \
	/home/albert/Testing/esp-idf/components/tcpip_adapter/Kconfig \
	/home/albert/Testing/esp-idf/components/wear_levelling/Kconfig \
	/home/albert/Testing/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/albert/Testing/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/albert/Testing/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/albert/Testing/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
