deps_config := \
	/home/mtacti/Testing/esp-idf/components/app_trace/Kconfig \
	/home/mtacti/Testing/esp-idf/components/aws_iot/Kconfig \
	/home/mtacti/Testing/esp-idf/components/bt/Kconfig \
	/home/mtacti/Testing/esp-idf/components/esp32/Kconfig \
	/home/mtacti/Testing/esp-idf/components/ethernet/Kconfig \
	/home/mtacti/Testing/esp-idf/components/fatfs/Kconfig \
	/home/mtacti/Testing/esp-idf/components/freertos/Kconfig \
	/home/mtacti/Testing/esp-idf/components/heap/Kconfig \
	/home/mtacti/Testing/esp-idf/components/libsodium/Kconfig \
	/home/mtacti/Testing/esp-idf/components/log/Kconfig \
	/home/mtacti/Testing/esp-idf/components/lwip/Kconfig \
	/home/mtacti/Testing/esp-idf/components/mbedtls/Kconfig \
	/home/mtacti/Testing/esp-idf/components/openssl/Kconfig \
	/home/mtacti/Testing/esp-idf/components/pthread/Kconfig \
	/home/mtacti/Testing/esp-idf/components/spi_flash/Kconfig \
	/home/mtacti/Testing/esp-idf/components/spiffs/Kconfig \
	/home/mtacti/Testing/esp-idf/components/tcpip_adapter/Kconfig \
	/home/mtacti/Testing/esp-idf/components/wear_levelling/Kconfig \
	/home/mtacti/Testing/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/mtacti/Testing/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/mtacti/Testing/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/mtacti/Testing/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
