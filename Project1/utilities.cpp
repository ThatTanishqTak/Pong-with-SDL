// Define data type aliases for different integer types
typedef char s8;                  // Signed 8-bit integer
typedef unsigned char u8;         // Unsigned 8-bit integer
typedef short s16;                // Signed 16-bit integer
typedef unsigned short u16;       // Unsigned 16-bit integer
typedef int s32;                  // Signed 32-bit integer
typedef unsigned int u32;         // Unsigned 32-bit integer
typedef long long s64;            // Signed 64-bit integer
typedef unsigned long long u64;   // Unsigned 64-bit integer

// Define macros for declaring static variables and functions with internal linkage
#define global_variable static   // Used to declare global variables with internal linkage
#define internal static          // Used to declare static functions with internal linkage

// Define an inline function 'clamp' to restrict a value within a specified range
inline int clamp(int min, int val, int max)
{
    // If 'Val' is less than 'min', return 'min'; otherwise, if 'Val' is greater than 'max', return 'max'
    // Otherwise, return 'Val' as it falls within the specified range
    if (val < min) { return min; }
    if (val > max) { return max; }
    return val;
}