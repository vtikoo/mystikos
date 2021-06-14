TOP=$(abspath ../..)
include $(TOP)/defs.mak

APPBUILDER=$(TOP)/scripts/appbuilder
APPNAME=HelloWorld

#ifdef STRACE
#OPTS += --strace
#endif

JRE_LIB_PATH="/usr/lib/jvm/java-1.8-openjdk/jre/lib/amd64/server:/usr/lib/jvm/java-1.8-openjdk/jre/lib/amd64:/usr/lib/jvm/java-1.8-openjdk/jre/../lib/amd64:/usr/lib/jvm/java-1.8-openjdk/lib/amd64/jli"
DEFAULT_JRE_ARGS="\
  -Xms2000k \
  -XX:InitialCodeCacheSize=2000k \
  -XX:ReservedCodeCacheSize=4000K \
  -XX:CompressedClassSpaceSize=4000K \
  -XX:+UseCompressedClassPointers \
  -XX:+PerfDisableSharedMem \
  -XX:+UseMembar \
  -Dsun.zip.disableMemoryMapping=true \
  -cp /app"

all: rootfs

run:
	LD_LIBRARY_PATH=$(JRE_LIB_PATH) $(MYST_EXEC) $(OPTS) --memory-size=1024m rootfs /usr/bin/java /app/HelloWorld
	
gdb: 
	LD_LIBRARY_PATH=$(JRE_LIB_PATH) $(MYST_GDB) --args $(MYST_EXEC) $(OPTS) --memory-size=1024m rootfs /usr/bin/java /app/HelloWorld

appdir:
	$(APPBUILDER) Dockerfile

rootfs: appdir
	$(MYST) mkcpio appdir rootfs

clean:
	sudo rm -fr appdir rootfs
