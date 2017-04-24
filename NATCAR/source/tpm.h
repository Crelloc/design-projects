/*
 * tpm.h
 *
 *  Created on: Jul 16, 2013
 *      Author: B34443
 */

#ifndef TPM_H_
#define TPM_H_

#define TPM_PLLFLL 		1
#define TPM_OSCERCLK 	2
#define TPM_MCGIRCLK	3

#define	TPM_CNT_DIS	0
#define	TPM_CLK		1
#define	TPM_EXT_CLK	2

#define PS_1	0
#define PS_2	1
#define PS_4	2
#define PS_8	3
#define PS_16	4
#define PS_32	5
#define PS_64	6
#define PS_128	7

#define TPM_OC_TOGGLE	TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK
#define TPM_OC_CLR		TPM_CnSC_MSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_SET		TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_OUTL		TPM_CnSC_MSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_OUTH		TPM_CnSC_MSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK

#define TPM_PWM_H 		TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK
#define TPM_PWM_L		TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK

#define EDGE_PWM		0
#define CENTER_PWM		1

extern void TPM_init_PWM(TPM_MemMapPtr TPMx, int  clock_source, int module, int clock_mode, int ps, int counting_mode);
extern void TPM_CH_init(TPM_MemMapPtr TPMx, int channel, int mode);
extern void set_TPM_CnV(TPM_MemMapPtr TPMx, int channel, int value);

#endif /* TPM_H_ */
