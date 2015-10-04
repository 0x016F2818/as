#common compilers
ifeq ($(host),Linux)
AS  = arm-linux-gnueabi-gcc
CC  = arm-linux-gnueabi-gcc
LD  = arm-linux-gnueabi-ld
AR  = arm-linux-gnueabi-ar

else
AS  = $(COMPILER_DIR)/bin/arm-none-eabi-gcc
CC  = $(COMPILER_DIR)/bin/arm-none-eabi-gcc
LD  = $(COMPILER_DIR)/bin/arm-none-eabi-ld
AR  = $(COMPILER_DIR)/bin/arm-none-eabi-ar
endif

RM  = rm

#common flags
asflags-y += -mcpu=cortex-m3  -mthumb
cflags-y  += -mcpu=cortex-m3  -mthumb -std=gnu99
cflags-y  += -mstructure-size-boundary=8 -ffreestanding
cflags-y  += -pedantic -W -Wall
ifeq ($(DEBUG),TRUE)
cflags-y += -g -O0
asflags-y += -g -O0
else
cflags-y += -O2
asflags-y += -O2
endif

ldflags-y += -static -T $(link-script)
dir-y += $(src-dir)

VPATH += $(dir-y)
inc-y += $(foreach x,$(dir-y),$(addprefix -I,$(x)))	
	
obj-y += $(patsubst %.c,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.c)))))		
obj-y += $(patsubst %.S,$(obj-dir)/%.o,$(foreach x,$(dir-y),$(notdir $(wildcard $(addprefix $(x)/*,.S)))))		

#common rules	

$(obj-dir)/%.o:%.S
	@echo
	@echo "  >> AS $(notdir $<)"
	@$(AS) $(asflags-y) $(def-y) -o $@ -c $<
	
$(obj-dir)/%.o:%.c
	@echo
	@echo "  >> CC $(notdir $<)"
	@gcc -c $(inc-y) $(def-y) -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $<
	@$(CC) $(cflags-y) $(inc-y) $(def-y) -o $@ -c $<	
	
include $(wildcard $(obj-dir)/*.d)
	
.PHONY:all clean

$(obj-dir):
	@mkdir -p $(obj-dir)
	
$(exe-dir):
	@mkdir -p $(exe-dir)	

include $(wildcard $(obj-dir)/*.d)

exe:$(obj-dir) $(exe-dir) $(obj-y)
	@echo "  >> LD $(target-y).OUT"
	@$(LD) $(obj-y) $(ldflags-y) -o $(exe-dir)/$(target-y).out 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(exe-dir)/$(target-y)  DONE   <<<<<<<<<<<<<<<<<<<<<<"	
	
dll:$(obj-dir) $(exe-dir) $(obj-y)
	@echo "  >> LD $(target-y).DLL"
	@$(CC) -shared $(obj-y) $(ldflags-y) -o $(exe-dir)/$(target-y).dll 
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(exe-dir)/$(target-y)  DONE   <<<<<<<<<<<<<<<<<<<<<<"

lib:$(obj-dir) $(exe-dir) $(obj-y)
	@echo "  >> LD $(target-y).LIB"
	@$(AR) -r $(exe-dir)/lib$(target-y).a $(obj-y)  
	@echo ">>>>>>>>>>>>>>>>>  BUILD $(exe-dir)/$(target-y)  DONE   <<<<<<<<<<<<<<<<<<<<<<"		

clean-obj:
	@rm -fv $(obj-dir)/*
	@rm -fv $(exe-dir)/*
	
clean-obj-src:clean-obj
	@rm -fv $(src-dir)/*

