/*
**********************************************************************************************************************
*
*						           the Embedded Secure Bootloader System
*
*
*						       Copyright(C), 2006-2014, Allwinnertech Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "common.h"
#include "asm/io.h"
#include "asm/armv7.h"
#include "asm/arch/cpu.h"
#include "asm/arch/ccmu.h"
#include "asm/arch/timer.h"
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
static int clk_set_divd(void)
{
	unsigned int reg_val;

	//config axi
	reg_val = readl(CCMU_CPUX_AXI_CFG_REG);
	reg_val &= ~(0x03 << 8);
	reg_val |=  (0x01 << 8);
	reg_val |=  (1 << 0);
	writel(reg_val, CCMU_CPUX_AXI_CFG_REG);
	
	//config ahb
	reg_val = readl(CCMU_AHB1_APB1_CFG_REG);;
	reg_val &= ~((0x03 << 12) | (0x03 << 8) |(0x03 << 4));
	reg_val |=  (0x02 << 12);
	reg_val |=  (2 << 6);
	reg_val |=  (1 << 8);

	writel(reg_val, CCMU_AHB1_APB1_CFG_REG);

	return 0;
}
/*******************************************************************************
*��������: set_pll
*����ԭ�ͣ�void set_pll( void )
*��������: ����CPUƵ��
*��ڲ���: void
*�� �� ֵ: void
*��    ע:
*******************************************************************************/
void set_pll( void )
{
    unsigned int reg_val;
    unsigned int i;
    //����ʱ��ΪĬ��408M

    //�л���24M
    reg_val = readl(CCMU_CPUX_AXI_CFG_REG);
    reg_val &= ~(0x01 << 16);
    reg_val |=  (0x01 << 16);
	reg_val |=  (0x01 << 0);
    writel(reg_val, CCMU_CPUX_AXI_CFG_REG);
    //��ʱ���ȴ�ʱ���ȶ�
    for(i=0; i<0x400; i++);
	//��дPLL1
    reg_val = (0x01<<12)|(0x01<<31);
    writel(reg_val, CCMU_PLL_CPUX_CTRL_REG);
    //��ʱ���ȴ�ʱ���ȶ�
#ifndef CONFIG_FPGA
	do
	{
		reg_val = readl(CCMU_PLL_CPUX_CTRL_REG);
	}
	while(!(reg_val & (0x1 << 28)));
#endif
    //�޸�AXI,AHB,APB��Ƶ
    clk_set_divd();
		//dma reset
	writel(readl(CCMU_BUS_SOFT_RST_REG0)  | (1 << 6), CCMU_BUS_SOFT_RST_REG0);
	for(i=0;i<100;i++);
	//gating clock for dma pass
	writel(readl(CCMU_BUS_CLK_GATING_REG0) | (1 << 6), CCMU_BUS_CLK_GATING_REG0);
	//��MBUS,clk src is pll6
	writel(0x80000000, CCMU_MBUS_RST_REG);       //Assert mbus domain
	writel(0x81000002, CCMU_MBUS_CLK_REG);  //dram>600M, so mbus from 300M->400M
	//ʹ��PLL6
	writel(readl(CCMU_PLL_PERIPH0_CTRL_REG) | (1U << 31), CCMU_PLL_PERIPH0_CTRL_REG);

    //�л�ʱ�ӵ�COREPLL��
    reg_val = readl(CCMU_CPUX_AXI_CFG_REG);
    reg_val &= ~(0x03 << 16);
    reg_val |=  (0x02 << 16);
    writel(reg_val, CCMU_CPUX_AXI_CFG_REG);

    return  ;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
void reset_pll( void )
{
	writel(0x00010000, CCMU_CPUX_AXI_CFG_REG);
	writel(0x00001000, CCMU_PLL_CPUX_CTRL_REG);
	writel(0x00001010, CCMU_AHB1_APB1_CFG_REG);

	return ;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
void set_gpio_gate(void)
{
	writel(readl(CCMU_BUS_CLK_GATING_REG2)   | (1 << 5), CCMU_BUS_CLK_GATING_REG2);
}
