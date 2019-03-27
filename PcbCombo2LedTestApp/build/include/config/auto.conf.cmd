deps_config := \
	/home/albert/ESP32_101/esp-idf-v3.0/components/app_trace/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/aws_iot/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/bt/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/esp32/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/esp_adc_cal/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/ethernet/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/fatfs/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/freertos/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/heap/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/libsodium/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/log/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/lwip/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/mbedtls/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/openssl/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/pthread/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/spi_flash/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/spiffs/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/tcpip_adapter/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/wear_levelling/Kconfig \
	/home/albert/ESP32_101/esp-idf-v3.0/components/bootloader/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf-v3.0/components/esptool_py/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf-v3.0/components/partition_table/Kconfig.projbuild \
	/home/albert/ESP32_101/esp-idf-v3.0/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
