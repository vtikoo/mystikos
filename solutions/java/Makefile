TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPBUILDER=$(TOP)/scripts/appbuilder
APPNAME=java

ifdef STRACE
OPTS += --strace
endif

all: appdir private.pem myst

run:
	myst/bin/$(APPNAME) $(OPTS)
	
gdb: appdir private.pem
	$(MYST_GDB) --args myst/bin/$(APPNAME) $(OPTS)

appdir:
	$(APPBUILDER) Dockerfile

myst:
	$(MYST) package appdir private.pem config.json

private.pem:
	openssl genrsa -out private.pem -3 3072

clean:
	sudo rm -fr appdir rootfs private.pem myst /tmp/myst*
