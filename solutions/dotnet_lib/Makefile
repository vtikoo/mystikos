TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPBUILDER=$(TOP)/scripts/appbuilder

ifdef STRACE
OPTS += --strace
endif

build: rootfs

# release ubuntu prebuilt image -
# hullcritical/dotnet-library-test:release-ubuntu
appdir:
	$(APPBUILDER) -v Dockerfile

rootfs: appdir
	$(MYST) mkext2 appdir rootfs

run:
	$(MYST_EXEC) $(OPTS) rootfs /bin/echo --app-config-path config.json \
	/dotnet-lib-debug/testhost/net5.0-Linux-Debug-x64/dotnet \
	test \
	/dotnet-lib-debug/System.ValueTuple.Tests/net5.0-Debug/System.ValueTuple.Tests.dll

lldb:
	$(MYST_LLDB) -- $(MYST_EXEC) $(OPTS) rootfs $(OPTS) \
	--app-config-path config.json --report-native-tids \
	/dotnet-lib-debug/testhost/net5.0-Linux-Debug-x64/dotnet \
	test \
	/dotnet-lib-debug/System.ValueTuple.Tests/net5.0-Debug/System.ValueTuple.Tests.dll

clean:
	rm -rf appdir rootfs