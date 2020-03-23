deps_config := \
	/home/jonnes/esp/iot_publisher/components/DHT22/Kconfig \
	/home/jonnes/esp/esp-idf/components/app_trace/Kconfig \
	/home/jonnes/esp/esp-idf/components/aws_iot/Kconfig \
	/home/jonnes/esp/esp-idf/components/bt/Kconfig \
	/home/jonnes/esp/esp-idf/components/driver/Kconfig \
	/home/jonnes/esp/esp-idf/components/esp32/Kconfig \
	/home/jonnes/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/jonnes/esp/esp-idf/components/esp_event/Kconfig \
	/home/jonnes/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/jonnes/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/jonnes/esp/esp-idf/components/ethernet/Kconfig \
	/home/jonnes/esp/esp-idf/components/fatfs/Kconfig \
	/home/jonnes/esp/esp-idf/components/freemodbus/Kconfig \
	/home/jonnes/esp/esp-idf/components/freertos/Kconfig \
	/home/jonnes/esp/esp-idf/components/heap/Kconfig \
	/home/jonnes/esp/esp-idf/components/libsodium/Kconfig \
	/home/jonnes/esp/esp-idf/components/log/Kconfig \
	/home/jonnes/esp/esp-idf/components/lwip/Kconfig \
	/home/jonnes/esp/esp-idf/components/mbedtls/Kconfig \
	/home/jonnes/esp/esp-idf/components/mdns/Kconfig \
	/home/jonnes/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/jonnes/esp/esp-idf/components/openssl/Kconfig \
	/home/jonnes/esp/esp-idf/components/pthread/Kconfig \
	/home/jonnes/esp/esp-idf/components/spi_flash/Kconfig \
	/home/jonnes/esp/esp-idf/components/spiffs/Kconfig \
	/home/jonnes/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/jonnes/esp/esp-idf/components/unity/Kconfig \
	/home/jonnes/esp/esp-idf/components/vfs/Kconfig \
	/home/jonnes/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/jonnes/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/jonnes/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/jonnes/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/jonnes/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
