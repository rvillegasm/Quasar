#pragma once

// TODO: Deprecate in favor of build system platform detection

#ifdef _WIN32
    #ifdef _WIN64
        /* Windows x64 */
        #define QS_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #define QS_PLATFORM_WINDOWS
    #endif
    #error "Windows is not supported yet!"

#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
		#define QS_PLATFORM_IOS
		#error "IOS is not supported yet!"
	#elif TARGET_OS_MAC == 1
		#define QS_PLATFORM_MACOS
		#error "MacOS is not supported yet!"
    #else
        #error "Unknown Apple platform!"
    #endif

/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
    #define QS_PLATFORM_ANDROID
    #error "Android is not supported yet!"

#elif defined(__linux__)
    #define QS_PLATFORM_LINUX

#else
    #error "Could not detect platform!"

#endif