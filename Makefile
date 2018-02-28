all:
	@echo BUILDING STRING_PARSER
	make -C Utility/string_parser
	@echo BUILDING PINDEF_PARSER
	make -C Utility/pindef_parser
	@echo BUILDING APP
	make -C Code string pin all

clean:
	@echo CLEANING STRING_PARSER
	make -C Utility/string_parser clean
	@echo CLEANING PINDEF_PARSER
	make -C Utility/pindef_parser clean
	@echo CLEANING APP
	make -C Code clean