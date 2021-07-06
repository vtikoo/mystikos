TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPBUILDER=$(TOP)/scripts/appbuilder
APPNAME=corerun
TEST_FILE=pr0-tests-all
TEST_CASE=/coreclr-tests-all/JIT/Generics/Arrays/TypeParameters/Jagged/class01/class01.dll

ifdef STRACE
OPTS += --strace
endif
OPTS += --app-config-path=config.json

ifdef LEAK_CHECK
OPTS += --memcheck
endif

all: ext2fs

# docker build time ~1hr. Using prebuilt image.
# Other docker images:
# pr0(~2.5k tests) tests in release
#$(APPBUILDER) -i vtikoo/coreclr-tests:pr0-release
# pr1(~10k tests) tests in release
#$(APPBUILDER) vtikoo/coreclr-tests:release
# runtime with debug symbols -
#$(APPBUILDER) vtikoo/coreclr-tests:debug3
appdir:
	$(APPBUILDER) Dockerfile

ext2fs: appdir
	$(MYST) mkext2 appdir ext2fs
	$(MYST) fssig --roothash ext2fs > roothash

# run:
# 	./test-runner.sh $(MYST_EXEC) ext2 $(TEST_FILE)

run:
	COMPlus_EnableDiagnostics=0 COM_EnableAlternateStackCheck=1 \
	$(MYST_EXEC) ext2fs \
	--roothash=roothash $(OPTS) \
	/coreclr-tests-all/Tests/Core_Root/run-tests \
	/coreclr-tests-all/Tests/Core_Root/pr0-PASSED

clean:
	sudo rm -fr appdir ext2fs rootfs roothash myst /tmp/myst* PASSED FAILED-*

tests:
	@ $(MAKE) -s clean
	@ $(MAKE) -s all
	@ $(MAKE) -s run

##################################
#			dev targets			 #
##################################
run-ext2-single:
	$(MYST_EXEC) ext2fs \
	--roothash=roothash $(OPTS) \
	/coreclr-tests-all/Tests/Core_Root/corerun \
	$(TEST_CASE)

run-ext2-gdb:
	$(MYST_GDB) --args $(MYST_EXEC) ext2fs --roothash=roothash $(OPTS) \
	/coreclr-tests-all/Tests/Core_Root/corerun \
	$(TEST_CASE)

ct:
	sudo rm -fr /tmp/myst* PASSED FAILED-* /tmp/dotnet-diagnostic*
