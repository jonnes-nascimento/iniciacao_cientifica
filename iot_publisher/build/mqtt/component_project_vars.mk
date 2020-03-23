# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/mqtt/include $(PROJECT_PATH)/components/mqtt/paho/MQTTClient-C/src $(PROJECT_PATH)/components/mqtt/paho/MQTTClient-C/src/FreeRTOS $(PROJECT_PATH)/components/mqtt/paho/MQTTPacket/src
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/mqtt -lmqtt
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += mqtt
COMPONENT_LDFRAGMENTS += 
component-mqtt-build: 
