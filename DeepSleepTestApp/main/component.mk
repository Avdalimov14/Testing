#
# Main Makefile. This is basically the same as a component makefile.
#
COMPONENT_ADD_LDFLAGS=-lstdc++ -l$(COMPONENT_NAME)
#COMPONENT_SRCDIRS := esp32 PN532/PN532_SPI PN532/PN532 PN532/NDEF
#COMPONENT_ADD_INCLUDEDIRS := esp32 PN532/PN532_SPI PN532/PN532 PN532/NDEF

#
# ULP support additions to component makefile.
#
# 1. ULP_APP_NAME must be unique (if multiple components use ULP)
#    Default value, override if necessary:
ULP_APP_NAME ?= ulp_$(COMPONENT_NAME)
#
# 2. Specify all assembly source files here.
#    Files should be placed into a separate directory (in this case, ulp/),
#    which should not be added to COMPONENT_SRCDIRS.
ULP_S_SOURCES = $(addprefix $(COMPONENT_PATH)/ulp/, \
	pulse_cnt.S \
	)
#
# 3. List all the component object files which include automatically
#    generated ULP export file, $(ULP_APP_NAME).h:
ULP_EXP_DEP_OBJECTS := ulp_example_main.o
#
# 4. Include build rules for ULP program 
include $(IDF_PATH)/components/ulp/component_ulp_common.mk
#
# End of ULP support additions to component makefile.
#
