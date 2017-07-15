CPPFLAGS=-D__DO_MAIN__

########################################################################
### Several tests of NOSS algorithm
########################################################################
tests: noss
	./noss 1 2 3 4 5
	./noss 89 271 272 88
	./noss 272 88 89 271
	@#
	./noss 1 4 2 359
	./noss 4 2 359 1
	./noss 2 359 1 4
	./noss 359 1 4 2
	@#
	./noss 1 2 359 358
	./noss 2 359 358 1
	./noss 359 358 1 2
	./noss 358 1 2 359
	@#
	./noss 1 358 357 359
	./noss 358 357 359 1
	./noss 357 359 1 358
	./noss 359 1 358 357
	@#
	./noss 91 269 272 92
	./noss 269 272 92 91
	./noss 272 92 91 269
	./noss 92 91 269 272
	@#
	./noss 50 100 260 350
	./noss 100 260 350 50
	./noss 260 350 50 100
	./noss 350 50 100 260
	@#
	./noss 350 260 100 50
	./noss 260 100 50 350
	./noss 100 50 350 260
	./noss 50 350 260 100

########################################################################
### Confirm that noss() and noss2() routines do the same thing
########################################################################
compare: noss
	@make -f $(firstword $(MAKEFILE_LIST)) > noss.log
	@USE_NOSS2= make -f $(firstword $(MAKEFILE_LIST)) > noss2.log
	@diff -y --suppress-common-lines noss.log noss2.log | uniq
	@echo "*** N.B. Success if result was one line of 'noss   | noss2'"

clean:
	$(RM) noss noss.log noss2.log
