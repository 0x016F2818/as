# on C disk do the following command:

$(inc-dir)/utility:
	@mkdir -p $@
$(inc-dir):
	@mkdir -p $@
$(src-dir):
	@mkdir -p $@

$(ASCORE)/SgDesign/SgRes/SgRes.h:
	@(cd SgDesign;$(SG) $(sgapp)/Sg.xml)	

SG:
	@(cd SgDesign;$(SG) $(sgapp)/Sg.xml)

OS:
	@(cd $(src-dir)/config.infrastructure.system;$(XCC) $(src-dir) false)	
ifeq ($(rtos),freeosek)
# as the bug of FreeOSEK generator
	@(mv -v srcme/parai/workspace/as/release/ascore/src/* $(src-dir))
	@(rm -fvr srcme/parai/workspace/as/release/ascore/src)	
endif

BSW:
	@(cd $(src-dir)/config.infrastructure.system;$(XCC) $(src-dir) true)

studio:
	@(cd $(src-dir)/config.infrastructure.system;$(STUDIO) $(src-dir))	

dep-as-virtual:
	@($(LNFS) $(VIRTUAL) virtual)

$(download):
	@mkdir -p $@

$(download)/lwip:
ifeq ($(tcpip),lwip)
	@(cd $(download);git clone git://git.savannah.nongnu.org/lwip.git;cd lwip;git checkout STABLE-1_4_1)
endif

$(download)/lwip-contrib:
ifeq ($(tcpip),lwip)
	@(cd $(download);git clone git://git.savannah.nongnu.org/lwip/lwip-contrib.git;cd lwip-contrib;git checkout STABLE-1_4_1)
endif

aslwip: $(download)/lwip $(download)/lwip-contrib

$(download)/rt-thread:
	@(cd $(download); git clone https://github.com/RT-Thread/rt-thread.git)

$(download)/qemu:
	@(cd $(download); git clone https://github.com/qemu/qemu.git; \
		cd qemu; git submodule update --init dtc ; \
		cd hw/char; $(LNFS) $(prj-dir)/com/as.tool/qemu/hw/char TRUE; \
		cat Makefile >> Makefile.objs)

asqemu:$(download)/qemu
	@(cd $(download)/qemu; ./configure; make)

$(src-dir)/pci.download.done:
ifeq ($(usepci),yes)
	@(cd $(src-dir);wget https://raw.githubusercontent.com/torvalds/linux/v4.8/include/uapi/linux/pci.h -O pci.h)
	@(cd $(src-dir);wget https://raw.githubusercontent.com/torvalds/linux/v4.8/include/uapi/linux/pci_regs.h -O pci_regs.h)
	@(cd $(src-dir);wget https://raw.githubusercontent.com/torvalds/linux/v4.8/include/linux/pci_ids.h -O pci_ids.h)
	@(cd $(src-dir);sed -i "20c #include \"pci_regs.h\"" pci.h)
endif
	@touch $@