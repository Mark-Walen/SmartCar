//
// Created by Mark-Walen on 2025/3/6.
//
#include "system/init.h"
#include "app_main.h"

static void do_initcall() {
    __do_initcall(initcall);
}

static void do_early_initcall()
{
    __do_initcall(early_initcall);
}

static void do_late_initcall()
{
    __do_initcall(late_initcall);
}

static void do_platform_initcall()
{
    __do_initcall(platform_initcall);
}

static void do_module_initcall()
{
    __do_initcall(module_initcall);
}

void __attribute__((weak)) board_init()
{

}

void __attribute__((weak)) board_early_init()
{

}

int init_function(void) {
    return 0;
}

int module_function() {
    return 0;
}

int platform_function(void) {
    return 0;
}

// Example usage
int early_init_function(void) {
    // Early initialization code
    return 0;
}

int late_init_function(void) {
    // Late initialization code
    return 0;
}

static void app_init() {
    do_early_initcall();
    do_platform_initcall();

    board_init();

    do_initcall();

    do_module_initcall();
    do_late_initcall();
}

__initcall(init_function);
early_initcall(early_init_function);
late_initcall(late_init_function);
platform_initcall(platform_function);
module_initcall(module_function);

int main() {
    board_early_init();
    board_init();

    app_init();
    app_main();

    while(1) {
    }
}
