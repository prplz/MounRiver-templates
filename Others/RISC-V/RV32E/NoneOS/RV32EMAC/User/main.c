/*
 ============================================================================
 Name        : main.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello RISC-V World in C
 ============================================================================
 */

#include <stdio.h>

/*
 * Demonstrate how to print a greeting message on standard output
 * and exit.
 *
 * WARNING: This is a build-only project. Do not try to run it on a
 * physical board, since it lacks the device specific startup.
 */

int
main(void)
{
  printf("Hello RISC-V World!" "\n");
  return 0;
}
