TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPBUILDER=$(TOP)/scripts/appbuilder
APPNAME=corerun
HEAP_SIZE="768M"

ifdef STRACE
OPTS += --strace
endif

all: appdir rootfs ext2fs

appdir:
	$(APPBUILDER) -i vtikoo/aspnetcore-build:smaller

rootfs: appdir
	$(MYST) mkcpio appdir rootfs

ext2fs: rootfs
	sudo $(MYST) mkext2 appdir ext2fs
	$(MYST) fssig --roothash ext2fs > roothash

clean:
	sudo rm -fr appdir ext2fs rootfs roothash myst /tmp/myst*

##################################
#			dev targets			 #
##################################
run-ext2:
	$(MYST) exec ext2fs --roothash=roothash \
	--memory-size $(HEAP_SIZE) \
	/aspnetcore/.dotnet/dotnet test \
	/aspnetcore/src/Middleware/Session/test/Microsoft.AspNetCore.Session.Tests.csproj $(OPTS)

run-ext2-gdb:
	$(MYST_GDB) --args $(MYST) exec ext2fs --roothash=roothash \
	--memory-size $(HEAP_SIZE) \
	/aspnetcore/.dotnet/dotnet test \
	/aspnetcore/src/Middleware/Session/test/Microsoft.AspNetCore.Session.Tests.csproj $(OPTS)

ct:
	sudo rm -fr /tmp/myst*
