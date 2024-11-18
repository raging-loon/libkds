#include "kds_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __linux__
#   include <dlfcn.h>
#endif

#ifndef S_IFREG
#   define S_IFREG  0100000
#endif

#define MUTANT_NAME_BUFFER_SZ   256
#define printerr(x,...) fprintf(stderr, x, __VA_ARGS__)

static void print_help();

/// 
/// @brief
///     Check if a module exists
///
static bool module_exists(const char* name);

///
/// @brief 
///     Checks for existing files by mutating the base
/// @details
///     Checks for:
///         - "kds_<base>"      - "kds_<base>.so"
///         - "kds_<base>.dll"  - "<base>.so"
///         - "base.dll"
///
static bool module_exists_mutant(const char* base, char* out);

///
/// @brief
///     Load the KESTREL_DATA_SOURCE_MOD data structure
///     from the file
/// @details
///     Uses the dlfcn API in linux and LoadLibrary/GetProcAddress on Windows
/// 
/// @returns NULL on failure
///
static kds_module_t* load_module_from_file(const char* name);

///
/// @brief
///     Print what functions are available for a given module
///
static void info_dump_module(const kds_module_t* mod);

int main(int argc, char** argv)
{

    if(argc != 2) 
    {
        print_help();
        return 1;
    }


    char filenameBuffer[MUTANT_NAME_BUFFER_SZ];
    strncpy(filenameBuffer, argv[1], MUTANT_NAME_BUFFER_SZ);


    if(!module_exists(argv[1]) && !module_exists_mutant(argv[1], filenameBuffer))
    {
        fprintf(stderr, "Could not find module \"%s\".\n", argv[1]);
        return 1;    
    }

    fprintf(stderr,"Loading %s....\n", filenameBuffer);
    
    kds_module_t* mod = load_module_from_file(filenameBuffer);
    if(!mod)
        return 1;
    
    printf("Module Name: %s\n",mod->name);

    void* handle = NULL;

    if(mod->instantiate(&handle) == false)
    {
        fprintf(stderr,"Failed to instantiate module!\n");
        return 1;
    }

    if(!mod->load(handle))
        fprintf(stderr, "Failed to load module!\n");

    info_dump_module(mod);

    printf("Unloading %s-module...\n", mod->name);
    mod->unload(handle);

    return 0;
}

void print_help()
{
    fprintf(stderr, "Usage: kdscli <data source>\n"

    );
}

bool module_exists(const char* name)
{
    struct stat s;
    if((stat(name, &s)) != 0 || !(s.st_mode & S_IFREG))
        return false;

    return true;
}

bool module_exists_mutant(const char* base, char* out)
{
    ///
    /// Essentially, we are chopping base in half
    /// 'offset' will be the index of the right-most '/' or '\' character
    /// 'out' will constist of the left of that character, and we will 
    /// continually overwrite the right of it until we find a match
    ///

    memset(out, 0, sizeof(MUTANT_NAME_BUFFER_SZ));

    int offset = strlen(base);

    for( ; offset > 0; offset--)
    {
#if defined(WIN32) || defined(WIN64)
        if(base[offset-1] == '\\' || base[offset-1] == '/')
#else
        if(base[offset-1] == '/')
#endif
            break;
    }
    char* file = base;
    if(offset != 0)
    {
        strncat(out, base, offset);
        file = base + offset;
    }

#define CHECK_MUTANT(x) {                                    \
    snprintf(out + offset, MUTANT_NAME_BUFFER_SZ, x, file);  \
    printf("Checking: %s\n", out);                           \
    if(module_exists(out))                                   \
        return true;                                         \
}

#if defined(unix) || defined(__linux__)
    CHECK_MUTANT("%s.so");
    CHECK_MUTANT("kds_%s.so");
#elif defined(WIN32) || defined(WIN64)
    CHECK_MUTANT("%s.dll");
    CHECK_MUTANT("kds_%s.dll");
#endif
    CHECK_MUTANT("kds_%s");

    return false;
}


#ifdef __linux__

kds_module_t* load_module_from_file(const char* name)
{
    void* so_handle = NULL;
    kds_module_t* mod = NULL;

    if((so_handle = dlopen(name, RTLD_NOW)) == NULL) 
    {
        fprintf(stderr, "Could not load module: %s\n", dlerror());
        fprintf(stderr, "If you are certain this module exists, try using the full path or preceding it's name with './'.\n");
        return NULL;
    }

    mod = (kds_module_t*)dlsym(so_handle, KDS_MOD_NAME_STR);
    if(mod == NULL) 
    {
        printerr("Could not extract module data from file: %s\n", dlerror());
        return NULL;
    }

    return mod;    
}

#endif

#define _COLOR_GREEN(text) "\x1b[32m" text "\x1b[0m"
#define _COLOR_RED(text)   "\x1b[31m" text "\x1b[0m"

#define MOD_CHECK_FN(name) {\
    printf("%-20s", #name);\
    if(mod-> name)\
        printf("[" _COLOR_GREEN("Yes") "]\n");\
    else\
        printf("[" _COLOR_RED("No") "]\n");\
}

void info_dump_module(const kds_module_t* mod)
{
    printf("\nFunctions available for module '%s':\n",mod->name);

    MOD_CHECK_FN(instantiate);
    MOD_CHECK_FN(load);
    MOD_CHECK_FN(unload);
    MOD_CHECK_FN(recv);
    MOD_CHECK_FN(send);

    printf("\n");

}