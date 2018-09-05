deps_config := \
	/home/afridi/esp/esp-idf/components/app_trace/Kconfig \
	/home/afridi/esp/esp-idf/components/aws_iot/Kconfig \
	/home/afridi/esp/esp-idf/components/bt/Kconfig \
	/home/afridi/esp/esp-idf/components/driver/Kconfig \
	/home/afridi/esp/esp-idf/components/esp32/Kconfig \
	/home/afridi/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/afridi/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/afridi/esp/esp-idf/components/ethernet/Kconfig \
	/home/afridi/esp/esp-idf/components/fatfs/Kconfig \
	/home/afridi/esp/esp-idf/components/freertos/Kconfig \
	/home/afridi/esp/esp-idf/components/heap/Kconfig \
	/home/afridi/esp/esp-idf/components/libsodium/Kconfig \
	/home/afridi/esp/esp-idf/components/log/Kconfig \
	/home/afridi/esp/esp-idf/components/lwip/Kconfig \
	/home/afridi/esp/esp-idf/components/mbedtls/Kconfig \
	/home/afridi/esp/esp-idf/components/openssl/Kconfig \
	/home/afridi/esp/esp-idf/components/pthread/Kconfig \
	/home/afridi/esp/esp-idf/components/spi_flash/Kconfig \
	/home/afridi/esp/esp-idf/components/spiffs/Kconfig \
	/home/afridi/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/afridi/esp/esp-idf/components/vfs/Kconfig \
	/home/afridi/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/afridi/esp/esp-idf/Kconfig.compiler \
	/home/afridi/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/afridi/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/afridi/esp/esp-idf/examples/protocols/https_mbedtls/main/Kconfig.projbuild \
	/home/afridi/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/afridi/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
