all:
	@echo BUILDING BOOTLOADER
	make -C Bootloader/DFU
	@echo BUILDING STRING_PARSER
	make -C Utility/string_parser
	@echo BUILDING PINDEF_PARSER
	make -C Utility/pindef_parser
	@echo BUILDING MEMORY_EDITOR
	make -C Utility/memory_editor
	@echo BUILDING APP
	make -C Code string pin all

clean:
	@echo CLEANING BOOTLOADER
	make -C Bootloader/DFU clean
	@echo CLEANING STRING_PARSER
	make -C Utility/string_parser clean
	@echo CLEANING PINDEF_PARSER
	make -C Utility/pindef_parser clean
	@echo BUILDING MEMORY_EDITOR
	make -C Utility/memory_editor clean
	@echo CLEANING APP
	make -C Code clean

clean-all:
	@echo CLEANING BOOTLOADER
	make -C Bootloader/DFU clean
	@echo CLEANING STRING_PARSER
	make -C Utility/string_parser clean-all
	@echo CLEANING PINDEF_PARSER
	make -C Utility/pindef_parser clean-all
	@echo BUILDING MEMORY_EDITOR
	make -C Utility/memory_editor clean-all
	@echo CLEANING APP
	make -C Code clean
