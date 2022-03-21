################################################################################
# 2015-2025 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
# 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
# CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
# THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
# signed NDA do not have permission to distribute this documentation further.
# Unauthorized recipients must destroy any electronic and hard-copies
# and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
# immediately at gregory.mardinian@youtransactor.com.
#
# @date: 28/07/2020
#
# @author: schhim
################################################################################


################################################################################
#
# Makefile for compiling and generating the modem application
#
################################################################################


################################################################################
# Generic variable
CROSS_COMPILE?=
COMPILER?=
ARCH?=
EXAMPLE?=
VERSION?=
################################################################################


################################################################################
# Application name, binary folder & build folder
# Modify if needed
VERSION=1.0.0.9
EXEC=yt_modem_demo_application_$(VERSION)
TARGETDIR=$(ARCH)/bin
BUILDDIR=$(ARCH)/build
QEXPLORER=
HOME=/home/mortadha
################################################################################

ifndef ARCH
$(error 'Set ARCH: Call this make as: make ARCH=?? - with ?? in [PC, MODEM] -')
endif

ifeq ($(ARCH), MODEM)
################################################################################
# Binary output
# Do NOT modify !!!
LINKER_SCRIPT=application_mapping.ld
BIN=$(TARGETDIR)/$(EXEC).bin
OEM=$(TARGETDIR)/oem_app_path.ini
################################################################################


################################################################################
# System variable
# Modify if needed
PYTHON=/usr/bin/python
################################################################################


################################################################################
# Toolchains for cross compilation
# Change path to toolchains location
TOOLCHAIN_PATH_ROOT=$(HOME)/toolchains
# !!! Do NOT modify !!!
TOOLCHAIN_PATH=$(TOOLCHAIN_PATH_ROOT)/llvm/4.0.3
TOOLCHAIN_STD_LIB_INC_PATH=$(TOOLCHAIN_PATH)/armv7m-none-eabi/libc/include
TOOLCHAIN_ARPA_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/arpa
TOOLCHAIN_BITS_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/bits
TOOLCHAIN_NET_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/net
TOOLCHAIN_NETINET_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/netinet
TOOLCHAIN_NETPACKET_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/netpacket
TOOLCHAIN_SCSI_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/scsi
TOOLCHAIN_SYS_INC_PATH=$(TOOLCHAIN_STD_LIB_INC_PATH)/sys
LLVMLIB=$(TOOLCHAIN_PATH)/lib/clang/4.0.3/lib
LLVMLINK_PATH=$(TOOLCHAIN_PATH)/tools/bin
CROSS_COMPILE=$(TOOLCHAIN_PATH_ROOT)/llvm/4.0.3/bin/
COMPILER=clang
# Virtual address start from 0x43000000
RO_BASE=0x43000000
################################################################################


################################################################################
# Src & inc paths from YT
# !!! Do NOT modify !!!
SRCDIR=src
LIB_PATH=lib
DOXYFILE_GEN_PATH=tools/docs
DOXYFILE_GEN_IMAGES_PATH=$(DOXYFILE_GEN_PATH)/images
LIB_YT_PATH=$(LIB_PATH)/$(ARCH)/yt
LIB_YT_DRIVERS_PATH=$(LIB_YT_PATH)/drivers
LIB_YT_MIDDLEWARE_PATH=$(LIB_YT_PATH)/middleware
LIB_YT_PAYMENT_PATH=$(LIB_YT_PATH)/payment
LIB_YT_UCUBE_API_PATH=$(LIB_YT_PATH)/ucube_api
LIB_YT_UTILITIES_PATH=$(LIB_YT_PATH)/utilities
LIB_QUECTEL_PATH=$(LIB_PATH)/quectel
LIB_THREADX_API_INC_PATH=$(LIB_QUECTEL_PATH)/inc/threadx_api
LIB_QAPI_INC_PATH=$(LIB_QUECTEL_PATH)/inc/qapi
TMX_MODULE_OBJ=$(LIB_QUECTEL_PATH)/txm_module_preamble_llvm.o
################################################################################

else ifeq ($(ARCH), PC)
################################################################################
# Toolchains for compiling PC simulator
COMPILER=gcc
################################################################################

################################################################################
# Src & inc paths from YT
# !!! Do NOT modify !!!
SRCDIR=src
LIB_PATH=lib
LIB_YT_PATH=$(LIB_PATH)/$(ARCH)/yt
LIB_YT_DRIVERS_PATH=$(LIB_YT_PATH)/drivers
LIB_YT_MIDDLEWARE_PATH=$(LIB_YT_PATH)/middleware
LIB_YT_PAYMENT_PATH=$(LIB_YT_PATH)/payment
LIB_YT_UCUBE_API_PATH=$(LIB_YT_PATH)/ucube_api
LIB_YT_UTILITIES_PATH=$(LIB_YT_PATH)/utilities
#LIB_QUECTEL_PATH=$(LIB_PATH)/quectel
#LIB_THREADX_API_INC_PATH=$(LIB_QUECTEL_PATH)/inc/threadx_api
#LIB_QAPI_INC_PATH=$(LIB_QUECTEL_PATH)/inc/qapi
#TMX_MODULE_OBJ=$(LIB_QUECTEL_PATH)/txm_module_preamble_llvm.o
################################################################################
endif


################################################################################
# Sources of the application
# Modify if needed

# Variable for source folders
OS_SRC_PATH=$(SRCDIR)/middleware/os
PROJETS_SRC_PATH=$(SRCDIR)/projects
MIDDLEWARE_SRC_PATH=$(SRCDIR)/middleware
PAYMENT_SRC_PATH=$(SRCDIR)/payment
UCUBE_API_SRC_PATH=$(SRCDIR)/ucube_api
APPMODEM_SRC_PATH=$(PROJETS_SRC_PATH)/appmodem
DEBUG_CONFIG_SRC_PATH=$(APPMODEM_SRC_PATH)/debug_config
MAIN_SRC_PATH=$(APPMODEM_SRC_PATH)/main
CUSTOMER_APP_SRC_PATH=$(APPMODEM_SRC_PATH)/customer_app
APPMODEM_CONFIG_PATH=$(APPMODEM_SRC_PATH)/config
ifeq ($(EXAMPLE), PING)
EXAMPLE_PATH=$(SRCDIR)/example/example_ping
endif
ifeq ($(EXAMPLE), HTTP)
EXAMPLE_PATH=$(SRCDIR)/example/example_http
endif

# whitelist the sources for more control
SRC=\
	$(OS_SRC_PATH)/os_itf.c \
	$(CUSTOMER_APP_SRC_PATH)/communication_menu.c \
	$(CUSTOMER_APP_SRC_PATH)/customer_app.c \
	$(CUSTOMER_APP_SRC_PATH)/display.c \
	$(CUSTOMER_APP_SRC_PATH)/display_message.c \
	$(CUSTOMER_APP_SRC_PATH)/display_list_box.c \
	$(CUSTOMER_APP_SRC_PATH)/firmware_update_ct.c \
	$(CUSTOMER_APP_SRC_PATH)/get_info.c \
	$(CUSTOMER_APP_SRC_PATH)/menu.c \
	$(CUSTOMER_APP_SRC_PATH)/network_manager.c \
	$(CUSTOMER_APP_SRC_PATH)/payment_ct.c \
	$(CUSTOMER_APP_SRC_PATH)/power_off_timeout.c \
	$(CUSTOMER_APP_SRC_PATH)/send_data.c \
	$(DEBUG_CONFIG_SRC_PATH)/debug_config.c \
	$(MAIN_SRC_PATH)/appmodem.c \

ifeq ($(EXAMPLE), PING)
SRC+= \
	$(EXAMPLE_PATH)/example_ping.c
endif
ifeq ($(EXAMPLE), HTTP)
SRC+= \
	$(EXAMPLE_PATH)/example_http.c
endif
# In case you don't want to whitelist, you can use the below command
# and adapt it to your project.
# SRC=$(shell find $(SRCDIR) -name "*.c") # -not -path "./src/drivers/*")

# Objects
OBJ=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRC:.c=.o))

# Include
INC_PATHS= \
			-I $(LIB_YT_DRIVERS_PATH) \
			-I $(LIB_YT_MIDDLEWARE_PATH) \
			-I $(LIB_YT_PAYMENT_PATH) \
			-I $(LIB_YT_UCUBE_API_PATH) \
			-I $(OS_SRC_PATH) \
			-I $(UCUBE_API_SRC_PATH) \
			-I $(APPMODEM_CONFIG_PATH) \
			-I $(CUSTOMER_APP_SRC_PATH) \
			-I $(LIB_YT_UTILITIES_PATH) \
			-I $(LIB_UTILITIES_SRC_PATH) \
			-I $(LIB_PAYMENT_SRC_PATH) \
			-I $(LIB_UCUBE_API_SRC_PATH) \
			-I $(LIB_DRIVERS_SRC_PATH)
ifeq ($(ARCH), MODEM)
INC_PATHS+= \
			-I $(LIB_THREADX_API_INC_PATH) \
			-I $(LIB_QAPI_INC_PATH) \
			-I $(TOOLCHAIN_STD_LIB_INC_PATH) \
			-I $(TOOLCHAIN_ARPA_INC_PATH) \
			-I $(TOOLCHAIN_BITS_INC_PATH) \
			-I $(TOOLCHAIN_NET_INC_PATH) \
			-I $(TOOLCHAIN_NETINET_INC_PATH) \
			-I $(TOOLCHAIN_NETPACKET_INC_PATH) \
			-I $(TOOLCHAIN_SCSI_INC_PATH) \
			-I $(TOOLCHAIN_SYS_INC_PATH) \
			-I $(LLVMLIB)

else ifeq ($(ARCH), PC)
INC_PATHS+= \
			-I /usr/include/json-c \
			-I /usr/include/openssl \
			-I /usr/include/yt-atmel-test
endif

ifeq ($(EXAMPLE), PING)
INC_PATHS+= \
			-I $(EXAMPLE_PATH) 
endif
ifeq ($(EXAMPLE), HTTP)
INC_PATHS+= \
			-I $(EXAMPLE_PATH) 
endif	
################################################################################

################################################################################
# Update of variables
CC=$(CROSS_COMPILE)$(COMPILER)
LD=$(CROSS_COMPILE)$(COMPILER)
################################################################################

ifeq ($(ARCH), MODEM)
################################################################################
# Flags from YT
# !!! Modify with caution !!!
PLATFORM=target armv7m-none-musleabi
CPPFLAGS= \
			-DQAPI_TXM_MODULE \
			-DTXM_MODULE \
			-DTX_DAM_QC_CUSTOMIZATIONS \
			-DTX_ENABLE_PROFILING \
			-DTX_ENABLE_EVENT_TRACE \
			-DTX_DISABLE_NOTIFY_CALLBACKS \
			-DFX_FILEX_PRESENT \
			-DTX_ENABLE_IRQ_NESTING \
			-DTX3_CHANGES \
			-DOPENSSL_SMALL_FOOTPRINT \
			-DCONFIG_MODEM
ifeq ($(EXAMPLE), PING)
CPPFLAGS+= \
			-DEXAMPLE_PING
endif
ifeq ($(EXAMPLE), HTTP)
CPPFLAGS+= \
			-DEXAMPLE_HTTP
endif

CFLAGS= \
		-marm \
		-$(PLATFORM) \
		-mfloat-abi=softfp \
		-mfpu=none \
		-mcpu=cortex-a7 \
		-mno-unaligned-access \
		-fms-extensions \
		-Osize \
		-fshort-enums \
		-Wbuiltin-macro-redefined \
		-Werror \
		$(INC_PATHS)

LDFLAGS=$(TARGETDIR)/$(EXEC).elf -$(PLATFORM) -fuse-ld=qcld -lc \
		-Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt \
		-Wl,-entry=$(RO_BASE) $(TMX_MODULE_OBJ) \
		-Wl,-T./$(LINKER_SCRIPT) \
		-Wl,-Map=$(TARGETDIR)/$(EXEC).map,-gc-sections -Wl,-gc-sections $^

LDLIBS= \
		-Wl,--start-group \
		$(LIB_QUECTEL_PATH)/qapi_psm_lib.lib \
		$(LIB_QUECTEL_PATH)/diag_dam_lib.lib \
		$(LIB_QUECTEL_PATH)/timer_dam_lib.lib \
		$(LIB_QUECTEL_PATH)/txm_lib.lib \
		$(LIB_YT_PATH)/lib_yt_drivers.a \
		$(LIB_YT_PATH)/lib_yt_utilities.a \
		$(LIB_YT_PATH)/lib_yt_payment.a \
		$(LIB_YT_PATH)/lib_yt_ucube_api.a \
		$(LIB_YT_PATH)/lib_yt_middleware.a \
		-Wl,--end-group
################################################################################
else ifeq ($(ARCH), PC)
################################################################################
# Flags for PC simulator target
CPPFLAGS= -DCONFIG_PC

ifeq ($(DEBUG), 1)
CPPFLAGS+= -g
endif # DEBUG

CFLAGS= $(INC_PATHS) -Werror

LDFLAGS=
LDLIBS=	-lcurl \
		-lcrypto \
		-ljson-c \
		-lpthread \
		-lrt \
		-lssl \
		-lyt-atmel-test
LD_SLIBS=-L $(LIB_YT_PATH)  -l_yt_middleware -l_yt_payment -l_yt_ucube_api -l_yt_utilities -l_yt_drivers
################################################################################
endif

################################################################################
# Makefile rules
# !!! Modify with caution !!!
all:: $(BUILDDIR) $(EXEC) 

$(BUILDDIR):
	@mkdir -p $@
	@mkdir -p $(TARGETDIR)

# build *.c
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

# creating executable
$(EXEC): $(OBJ)
ifeq ($(ARCH), MODEM)
	$(LD) -d -o $(LDFLAGS) $(LDLIBS)
	$(PYTHON) $(LLVMLINK_PATH)/llvm-elf-to-hex.py \
		--bin $(TARGETDIR)/$(EXEC).elf \
		--output $(TARGETDIR)/$(EXEC).bin
	@echo -n $(EXEC).bin > $(OEM)
else ifeq ($(ARCH), PC)
	$(LD) $(LDFLAGS) $^ $(LD_SLIBS) $(LDLIBS) -o $(TARGETDIR)/$@
endif

#flash modem
flash:
	@echo $(shell $(QEXPLORER) -f ./MODEM/bin/$(EXEC).bin)
	@echo $(shell $(QEXPLORER) -f ./MODEM/bin/oem_app_path.ini)

clean:
	rm -rf $(ARCH)
################################################################################
