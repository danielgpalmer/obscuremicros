/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x8ddf5b5d */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "/home/daniel/coding/obscuremicros/hardware/m68kintglue/shiftreg.vhd";
extern char *IEEE_P_2592010699;

unsigned char ieee_p_2592010699_sub_2763492388968962707_503743352(char *, char *, unsigned int , unsigned int );


static void work_a_1834584528_3212880686_p_0(char *t0)
{
    char *t1;
    unsigned char t2;
    char *t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    int t11;
    int t12;
    unsigned int t13;
    unsigned int t14;
    unsigned int t15;
    char *t16;

LAB0:    xsi_set_current_line(46, ng0);
    t1 = (t0 + 1312U);
    t2 = ieee_p_2592010699_sub_2763492388968962707_503743352(IEEE_P_2592010699, t1, 0U, 0U);
    if (t2 != 0)
        goto LAB2;

LAB4:
LAB3:    t1 = (t0 + 3104);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(48, ng0);
    t3 = (t0 + 1192U);
    t4 = *((char **)t3);
    t5 = *((unsigned char *)t4);
    t6 = (t5 == (unsigned char)3);
    if (t6 != 0)
        goto LAB5;

LAB7:    xsi_set_current_line(52, ng0);
    t1 = (t0 + 1032U);
    t3 = *((char **)t1);
    t1 = (t0 + 1808U);
    t4 = *((char **)t1);
    t11 = *((int *)t4);
    t12 = (t11 - 7);
    t13 = (t12 * -1);
    xsi_vhdl_check_range_of_index(7, 0, -1, t11);
    t14 = (1U * t13);
    t15 = (0 + t14);
    t1 = (t3 + t15);
    t2 = *((unsigned char *)t1);
    t7 = (t0 + 3184);
    t8 = (t7 + 56U);
    t9 = *((char **)t8);
    t10 = (t9 + 56U);
    t16 = *((char **)t10);
    *((unsigned char *)t16) = t2;
    xsi_driver_first_trans_fast_port(t7);
    xsi_set_current_line(53, ng0);
    t1 = (t0 + 1808U);
    t3 = *((char **)t1);
    t11 = *((int *)t3);
    t12 = (t11 + 1);
    t1 = (t0 + 1808U);
    t4 = *((char **)t1);
    t1 = (t4 + 0);
    *((int *)t1) = t12;

LAB6:    goto LAB3;

LAB5:    xsi_set_current_line(49, ng0);
    t3 = (t0 + 3184);
    t7 = (t3 + 56U);
    t8 = *((char **)t7);
    t9 = (t8 + 56U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = (unsigned char)4;
    xsi_driver_first_trans_fast_port(t3);
    xsi_set_current_line(50, ng0);
    t1 = (t0 + 1808U);
    t3 = *((char **)t1);
    t1 = (t3 + 0);
    *((int *)t1) = 0;
    goto LAB6;

}


extern void work_a_1834584528_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1834584528_3212880686_p_0};
	xsi_register_didat("work_a_1834584528_3212880686", "isim/testbench_isim_beh.exe.sim/work/a_1834584528_3212880686.didat");
	xsi_register_executes(pe);
}
