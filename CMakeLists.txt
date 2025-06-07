cmake_minimum_required(VERSION 3.13)
project(foot_light)

OPTION(IMX6_64 "Compile for ARM 64 CPU" OFF)

IF(IMX6_64)
	ADD_DEFINITIONS(-DIMX6_64)
	SET(IMX6_TOOLCHAIN /opt/buildroot_sdk/aarch64-buildroot-linux-gnu_sdk-buildroot)
	SET(IMX6_SYSROOT  /opt/buildroot_sdk/aarch64-buildroot-linux-gnu_sdk-buildroot/aarch64-buildroot-linux-gnu/sysroot)
	INCLUDE_DIRECTORIES(${IMX6_SYSROOT}/usr/include)
	INCLUDE_DIRECTORIES(${IMX6_SYSROOT}/usr/include/gstreamer-1.0)
	INCLUDE_DIRECTORIES(${IMX6_SYSROOT}/usr/include/glib-2.0)
	INCLUDE_DIRECTORIES(${IMX6_SYSROOT}/usr/lib/glib-2.0/include)
	INCLUDE_DIRECTORIES(${IMX6_SYSROOT}/usr/include/dbus-1.0)
	SET(CMAKE_C_COMPILER ${IMX6_TOOLCHAIN}/bin/aarch64-buildroot-linux-gnu-gcc)
	#SET(CMAKE_LINKER ${IMX6_TOOLCHAIN}/bin/arm-buildroot-linux-gnueabihf-ld)
	#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T linker_imx6.ld -m armelf_linux_eabi")  
	#SET(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
	#set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

	#	SET(CMAKE_C_LINK_EXECUTABLE ${IMX6_TOOLCHAIN}/bin/arm-buildroot-linux-gnueabihf-ld <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>)
	SET(CMAKE_STRIP ${IMX6_SYSROOT}/bin/aarch64-buildroot-linux-gnu-strip)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -lm --sysroot=${IMX6_SYSROOT}")  
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -lm -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard --sysroot=${IMX6_SYSROOT}")  
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${IMX6_SYSROOT}/usr/include/gstreamer-1.0 -I${IMX6_SYSROOT}/usr/include/glib-2.0 -I${IMX6_SYSROOT}/usr/lib/glib-2.0/include")  
    #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ludev")  
	#SET(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -L${IMX6_SYSROOT}/rootfs/lib/arm-linux-gnueabihf")  
	SET(MICROHTTPD_LIBRARY "${IMX6_SYSROOT}/usr/lib/libmicrohttpd.so")
	SET(PTHREAD_LIBRARY "${IMX6_SYSROOT}/lib/libpthread.so.0")
	#SET(LIBGSTREAMER "${IMX6_SYSROOT}/usr/lib/libgstreamer-1.0.so")
	#SET(LIBGLIB "${IMX6_SYSROOT}/usr/lib/libglib-2.0.so.0")
	SET(LIBUSB "${IMX6_SYSROOT}/usr/lib/libusb-1.0.so.0")
	SET(LIBX264 "${IMX6_SYSROOT}/usr/lib/libx264.so") # .a ?
	SET(LIBAVCODEC ${IMX6_SYSROOT}/usr/lib/libavcodec.so)
	SET(LIBAVUTIL ${IMX6_SYSROOT}/usr/lib/libavutil.so)
	SET(LIBSWRESAMPLE ${IMX6_SYSROOT}/usr/lib/libswresample.so)
	SET(ALSA ${IMX6_SYSROOT}/usr/lib/libasound.so.2.0.0) 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -T linker_aarch64.ld")
    SET(LIBCURL "${IMX6_SYSROOT}/usr/lib/libcurl.so")
	SET(LIBGPIO "${IMX6_SYSROOT}/usr/lib/libgpiod.so")
    SET(CMAKE_LINK_DEPENDS_USE_LINKER FALSE)

	# Dépendances
	SET(ADDITIONAL_LIBS
		gpiod
		pthread
	)


	LINK_DIRECTORIES(${IMX6_SYSROOT}/usr/lib)

ENDIF(IMX6_64)

set(SOURCE_FILES main.c dmx.c starville_4_bundle.c ludipocket5g.c gpio.c)

add_executable(foot_light ${SOURCE_FILES})

target_link_libraries(foot_light ${ADDITIONAL_LIBS})