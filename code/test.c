#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b

// Wow wtf

#define AAA 0123

/*
Block Comment
*/

typedef unsigned long ulong;
typedef struct Node Node;

struct Node {
    int value;
    struct Node *next;
};

enum Color {
    RED = 0,
    GREEN,
    BLUE
};

union Data {
    int i;
    float f;
    char c;
};

static volatile int global = 42;
const double pi = 3.141592653589793;
_Thread_local int thread_value;

static int function(int a, int b)
{
    int x = 0;
    unsigned int u = 123U;
    long l = 123456L;
    unsigned long ul = 123456UL;
    long long ll = 123456789LL;
    unsigned long long ull = 123456789ULL;

    float f = 1.25f;
    double d = 2.5;
    long double ld = 3.75L;

    char c = 'A';
    char escaped = '\n';
    const char *str = "Hello, \"C\"!\n\t\\";
    const wchar_t *wide = L"Wide string";

    bool flag = true;

    x += a;
    x -= b;
    x *= 2;
    x /= 3;
    x %= 5;

    x <<= 1;
    x >>= 1;

    x &= 0xff;
    x |= 0x10;
    x ^= 0x20;

    ++x;
    --x;
    x++;
    x--;

    if (a == b) {
        x = a && b;
    } else if (a != b || !flag) {
        x = a < b ? a : b;
    }

    if ((x >= 0) && (x <= 100)) {
        x = ~x;
    }

    switch (x) {
        case 0:
            goto zero;

        case 1:
        case 2:
            break;

        default:
            return -1;
    }

    for (int i = 0; i < 10; ++i) {
        if (i == 5)
            continue;

        x += i;
    }

    int i = 0;

    while (i < 10) {
        ++i;

        if (i == 7)
            break;
    }

    do {
        x--;
    } while (x > 0);

    {
        int local = 123;
        x += local;
    }

zero:
    return x;
}

static void pointers(void)
{
    int value = 42;
    int *ptr = &value;
    int **pptr = &ptr;

    *ptr = 100;
    **pptr = 200;

    void *generic = ptr;
    ptr = (int *)generic;

    int array[5] = { 1, 2, 3, 4, 5 };

    for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
        printf("%d\n", array[i]);
    }

    int (*func)(int, int) = function;
    int result = func(10, 20);

    (void)result;
}

static void structures(void)
{
    Node a = {
        .value = 10,
        .next = NULL
    };

    Node b = {
        .value = 20,
        .next = &a
    };

    b.next->value++;

    union Data data;

    data.i = 123;
    data.f = 3.14f;
    data.c = 'X';

    enum Color color = BLUE;

    (void)color;
    (void)data;
}

#define TEST_MACRO(x) \
    do {               \
        if ((x) > 0)    \
            printf("positive\n"); \
        else            \
            printf("negative\n"); \
    } while (0)

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int x = function(10, 20);

    TEST_MACRO(x);

    printf(
        "chars: ! \" # %% & ' ( ) * + , - . / "
        ": ; < = > ? @ [ \\ ] ^ _ ` { | } ~\n"
    );

    printf("decimal: %d\n", 123);
    printf("hex: %x\n", 0xDEADBEEF);
    printf("octal: %o\n", 0777);
    printf("binary: %d\n", 0b101010);
    printf("float: %f\n", 1.23e+10);

    if (x) {
        return 0;
    }

    return 1;
}