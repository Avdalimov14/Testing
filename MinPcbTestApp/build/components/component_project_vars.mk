# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/esp321 $(PROJECT_PATH)/components/PN532_SPI $(PROJECT_PATH)/components/PN532 $(PROJECT_PATH)/components/NDEF
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/components -lstdc++ -lcomponents
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += components
component-components-build: 
