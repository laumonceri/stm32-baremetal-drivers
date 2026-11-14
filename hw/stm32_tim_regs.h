#ifndef STM32_TIM_REGS_H
#define STM32_TIM_REGS_H

#include "stm32_mmio.h"

#define TIM_CR1_OFFSET         0x00U /* control register 1 */
#define TIM_CR2_OFFSET         0x04U /* control register 2 */
#define TIM_SMCR_OFFSET        0x08U /* slave mode control register */
#define TIM_DIER_OFFSET        0x0CU /* DMA/interrupt enable register */
#define TIM_SR_OFFSET          0x10U /* status register */
#define TIM_EGR_OFFSET         0x14U /* event generation register */
#define TIM_CCER_OFFSET        0x20U /* capture/compare enable register */
#define TIM_CNT_OFFSET         0x24U /* counter */
#define TIM_PSC_OFFSET         0x28U /* prescaler */
#define TIM_ARR_OFFSET         0x2CU /* auto-reload register */
#define TIM_RCR_OFFSET         0x30U /* repetition counter register */
#define TIM_BDTR_OFFSET        0x44U /* break and dead-time register */
#define TIM_DCR_OFFSET         0x48U /* DMA control register */
#define TIM_DMAR_OFFSET        0x4CU /* DMA address for full transfer */

#define TIM_CCMR1_OFFSET       0x18U /* capture/compare mode register 1 */
#define TIM_CCMR2_OFFSET       0x1CU /* capture/compare mode register 2 */
#define TIM_CCMR3_OFFSET       0x54U /* capture/compare mode register 3 */

#define TIM_CCR1_OFFSET        0x34U /* capture/compare register 1 */
#define TIM_CCR2_OFFSET        0x38U /* capture/compare register 2 */
#define TIM_CCR3_OFFSET        0x3CU /* capture/compare register 3 */
#define TIM_CCR4_OFFSET        0x40U /* capture/compare register 4 */
#define TIM_CCR5_OFFSET        0x58U /* capture/compare register 5 */
#define TIM_CCR6_OFFSET        0x5CU /* capture/compare register 6 */

/* Option registers - offsets shared, bit fields differ per timer (fields below are TIM1's) */
#define TIM_OR1_OFFSET         0x50U /* option register 1 */
#define TIM_OR2_OFFSET         0x60U /* option register 2 */
#define TIM_OR3_OFFSET         0x64U /* option register 3 */

#define TIM_CR1(base)          REG32((base) + TIM_CR1_OFFSET)
#define TIM_CR2(base)          REG32((base) + TIM_CR2_OFFSET)
#define TIM_SMCR(base)         REG32((base) + TIM_SMCR_OFFSET)
#define TIM_DIER(base)         REG32((base) + TIM_DIER_OFFSET)
#define TIM_SR(base)           REG32((base) + TIM_SR_OFFSET)
#define TIM_EGR(base)          REG32((base) + TIM_EGR_OFFSET)
#define TIM_CCER(base)         REG32((base) + TIM_CCER_OFFSET)
#define TIM_CNT(base)          REG32((base) + TIM_CNT_OFFSET)
#define TIM_PSC(base)          REG32((base) + TIM_PSC_OFFSET)
#define TIM_ARR(base)          REG32((base) + TIM_ARR_OFFSET)
#define TIM_RCR(base)          REG32((base) + TIM_RCR_OFFSET)
#define TIM_BDTR(base)         REG32((base) + TIM_BDTR_OFFSET)
#define TIM_DCR(base)          REG32((base) + TIM_DCR_OFFSET)
#define TIM_DMAR(base)         REG32((base) + TIM_DMAR_OFFSET)

#define TIM_CCMR1(base)        REG32((base) + TIM_CCMR1_OFFSET)
#define TIM_CCMR2(base)        REG32((base) + TIM_CCMR2_OFFSET)
#define TIM_CCMR3(base)        REG32((base) + TIM_CCMR3_OFFSET)

#define TIM_CCR1(base)         REG32((base) + TIM_CCR1_OFFSET)
#define TIM_CCR2(base)         REG32((base) + TIM_CCR2_OFFSET)
#define TIM_CCR3(base)         REG32((base) + TIM_CCR3_OFFSET)
#define TIM_CCR4(base)         REG32((base) + TIM_CCR4_OFFSET)
#define TIM_CCR5(base)         REG32((base) + TIM_CCR5_OFFSET)
#define TIM_CCR6(base)         REG32((base) + TIM_CCR6_OFFSET)

#define TIM_OR1(base)          REG32((base) + TIM_OR1_OFFSET)
#define TIM_OR2(base)          REG32((base) + TIM_OR2_OFFSET)
#define TIM_OR3(base)          REG32((base) + TIM_OR3_OFFSET)

/* TIM_CR1 */
#define TIM_CR1_CEN            BIT(0) /* counter enable */
#define TIM_CR1_UDIS           BIT(1) /* update disable */
#define TIM_CR1_URS            BIT(2) /* update request source */
#define TIM_CR1_OPM            BIT(3) /* one pulse mode */
#define TIM_CR1_DIR            BIT(4) /* direction */
/* CMS[6:5] */
#define TIM_CR1_CMS_Pos        5U
#define TIM_CR1_CMS_Msk        (0x3UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_CMS(n)         (((n) << TIM_CR1_CMS_Pos) & TIM_CR1_CMS_Msk)
#define TIM_CR1_ARPE           BIT(7) /* auto-reload preload enable */
/* CKD[9:8] */
#define TIM_CR1_CKD_Pos        8U
#define TIM_CR1_CKD_Msk        (0x3UL << TIM_CR1_CKD_Pos)
#define TIM_CR1_CKD(n)         (((n) << TIM_CR1_CKD_Pos) & TIM_CR1_CKD_Msk)
#define TIM_CR1_UIFREMAP       BIT(11)

/* TIM_CR2 */
#define TIM_CR2_CCPC           BIT(0)  /* capture/compare preloaded control */
#define TIM_CR2_CCUS           BIT(2)  /* capture/compare control update selection */
#define TIM_CR2_CCDS           BIT(3)  /* capture/compare DMA selection */
/* MMS[6:4] */
#define TIM_CR2_MMS_Pos        4U
#define TIM_CR2_MMS_Msk        (0x7UL << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS(n)         (((n) << TIM_CR2_MMS_Pos) & TIM_CR2_MMS_Msk)
#define TIM_CR2_TI1S           BIT(7)  /* TI1 selection */
#define TIM_CR2_OIS1           BIT(8)  /* output idle state 1 (OC1) */
#define TIM_CR2_OIS1N          BIT(9)  /* output idle state 1 (OC1N) */
#define TIM_CR2_OIS2           BIT(10) /* output idle state 2 (OC2) */
#define TIM_CR2_OIS2N          BIT(11) /* output idle state 2 (OC2N) */
#define TIM_CR2_OIS3           BIT(12) /* output idle state 3 (OC3) */
#define TIM_CR2_OIS3N          BIT(13) /* output idle state 3 (OC3N) */
#define TIM_CR2_OIS4           BIT(14) /* output idle state 4 (OC4) */
#define TIM_CR2_OIS5           BIT(16) /* output idle state 5 (OC5) */
#define TIM_CR2_OIS6           BIT(18) /* output idle state 6 (OC6) */
/* MMS2[23:20] */
#define TIM_CR2_MMS2_Pos       20U
#define TIM_CR2_MMS2_Msk       (0xFUL << TIM_CR2_MMS2_Pos)
#define TIM_CR2_MMS2(n)        (((n) << TIM_CR2_MMS2_Pos) & TIM_CR2_MMS2_Msk)

/* TIM_SMCR */
/* SMS[3:0] is split: SMS[2:0] -> bits 2:0, SMS[3] -> bit 16 */
#define TIM_SMCR_SMS_Pos       0U
#define TIM_SMCR_SMS3_Pos      16U
#define TIM_SMCR_SMS_Msk       ((0x7UL << TIM_SMCR_SMS_Pos) | BIT(TIM_SMCR_SMS3_Pos))
#define TIM_SMCR_SMS(n)        (((((n) & 0x7UL) << TIM_SMCR_SMS_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_SMCR_SMS3_Pos)) & TIM_SMCR_SMS_Msk)
#define TIM_SMCR_OCCS          BIT(3)  /* OCREF clear selection */
/* TS[6:4] */
#define TIM_SMCR_TS_Pos        4U
#define TIM_SMCR_TS_Msk        (0x7UL << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS(n)         (((n) << TIM_SMCR_TS_Pos) & TIM_SMCR_TS_Msk)
#define TIM_SMCR_MSM           BIT(7)  /* master/slave mode */
/* ETF[11:8] */
#define TIM_SMCR_ETF_Pos       8U
#define TIM_SMCR_ETF_Msk       (0xFUL << TIM_SMCR_ETF_Pos)
#define TIM_SMCR_ETF(n)        (((n) << TIM_SMCR_ETF_Pos) & TIM_SMCR_ETF_Msk)
/* ETPS[13:12] */
#define TIM_SMCR_ETPS_Pos      12U
#define TIM_SMCR_ETPS_Msk      (0x3UL << TIM_SMCR_ETPS_Pos)
#define TIM_SMCR_ETPS(n)       (((n) << TIM_SMCR_ETPS_Pos) & TIM_SMCR_ETPS_Msk)
#define TIM_SMCR_ECE           BIT(14) /* external clock enable (mode 2) */
#define TIM_SMCR_ETP           BIT(15) /* external trigger polarity */

/* TIM_DIER (rw: interrupt/DMA request enable bits) */
#define TIM_DIER_UIE           BIT(0)  /* update interrupt enable */
#define TIM_DIER_CC1IE         BIT(1)  /* capture/compare 1 interrupt enable */
#define TIM_DIER_CC2IE         BIT(2)  /* capture/compare 2 interrupt enable */
#define TIM_DIER_CC3IE         BIT(3)  /* capture/compare 3 interrupt enable */
#define TIM_DIER_CC4IE         BIT(4)  /* capture/compare 4 interrupt enable */
#define TIM_DIER_COMIE         BIT(5)  /* COM interrupt enable */
#define TIM_DIER_TIE           BIT(6)  /* trigger interrupt enable */
#define TIM_DIER_BIE           BIT(7)  /* break interrupt enable */
#define TIM_DIER_UDE           BIT(8)  /* update DMA request enable */
#define TIM_DIER_CC1DE         BIT(9)  /* capture/compare 1 DMA request enable */
#define TIM_DIER_CC2DE         BIT(10) /* capture/compare 2 DMA request enable */
#define TIM_DIER_CC3DE         BIT(11) /* capture/compare 3 DMA request enable */
#define TIM_DIER_CC4DE         BIT(12) /* capture/compare 4 DMA request enable */
#define TIM_DIER_COMDE         BIT(13) /* COM DMA request enable */
#define TIM_DIER_TDE           BIT(14) /* trigger DMA request enable */

/* TIM_SR (rc_w0: write 0 to a flag to clear it; writing 1 has no effect) */
#define TIM_SR_UIF             BIT(0)  /* update interrupt flag */
#define TIM_SR_CC1IF           BIT(1)  /* capture/compare 1 interrupt flag */
#define TIM_SR_CC2IF           BIT(2)  /* capture/compare 2 interrupt flag */
#define TIM_SR_CC3IF           BIT(3)  /* capture/compare 3 interrupt flag */
#define TIM_SR_CC4IF           BIT(4)  /* capture/compare 4 interrupt flag */
#define TIM_SR_COMIF           BIT(5)  /* COM interrupt flag */
#define TIM_SR_TIF             BIT(6)  /* trigger interrupt flag */
#define TIM_SR_BIF             BIT(7)  /* break interrupt flag */
#define TIM_SR_B2IF            BIT(8)  /* break 2 interrupt flag */
#define TIM_SR_CC1OF           BIT(9)  /* capture/compare 1 overcapture flag */
#define TIM_SR_CC2OF           BIT(10) /* capture/compare 2 overcapture flag */
#define TIM_SR_CC3OF           BIT(11) /* capture/compare 3 overcapture flag */
#define TIM_SR_CC4OF           BIT(12) /* capture/compare 4 overcapture flag */
#define TIM_SR_SBIF            BIT(13) /* system break interrupt flag */
#define TIM_SR_CC5IF           BIT(16) /* compare 5 interrupt flag */
#define TIM_SR_CC6IF           BIT(17) /* compare 6 interrupt flag */

/* TIM_EGR (write-only: set a bit to generate the event, hardware clears it) */
#define TIM_EGR_UG             BIT(0)  /* update generation */
#define TIM_EGR_CC1G           BIT(1)  /* capture/compare 1 generation */
#define TIM_EGR_CC2G           BIT(2)  /* capture/compare 2 generation */
#define TIM_EGR_CC3G           BIT(3)  /* capture/compare 3 generation */
#define TIM_EGR_CC4G           BIT(4)  /* capture/compare 4 generation */
#define TIM_EGR_COMG           BIT(5)  /* capture/compare control update generation */
#define TIM_EGR_TG             BIT(6)  /* trigger generation */
#define TIM_EGR_BG             BIT(7)  /* break generation */
#define TIM_EGR_B2G            BIT(8)  /* break 2 generation */

/* TIM_CCMR1 (rw: input capture mode - channels 1 and 2) */
/* CC1S[1:0] -> CC1 selection (direction) - writable only when CC1E = 0 */
#define TIM_CCMR1_CC1S_Pos     0U
#define TIM_CCMR1_CC1S_Msk     (0x3UL << TIM_CCMR1_CC1S_Pos)
#define TIM_CCMR1_CC1S(n)      (((n) << TIM_CCMR1_CC1S_Pos) & TIM_CCMR1_CC1S_Msk)
/* IC1PSC[3:2] -> input capture 1 prescaler */
#define TIM_CCMR1_IC1PSC_Pos   2U
#define TIM_CCMR1_IC1PSC_Msk   (0x3UL << TIM_CCMR1_IC1PSC_Pos)
#define TIM_CCMR1_IC1PSC(n)    (((n) << TIM_CCMR1_IC1PSC_Pos) & TIM_CCMR1_IC1PSC_Msk)
/* IC1F[7:4] -> input capture 1 filter */
#define TIM_CCMR1_IC1F_Pos     4U
#define TIM_CCMR1_IC1F_Msk     (0xFUL << TIM_CCMR1_IC1F_Pos)
#define TIM_CCMR1_IC1F(n)      (((n) << TIM_CCMR1_IC1F_Pos) & TIM_CCMR1_IC1F_Msk)
/* CC2S[9:8] -> CC2 selection (direction) - writable only when CC2E = 0 */
#define TIM_CCMR1_CC2S_Pos     8U
#define TIM_CCMR1_CC2S_Msk     (0x3UL << TIM_CCMR1_CC2S_Pos)
#define TIM_CCMR1_CC2S(n)      (((n) << TIM_CCMR1_CC2S_Pos) & TIM_CCMR1_CC2S_Msk)
/* IC2PSC[11:10] -> input capture 2 prescaler */
#define TIM_CCMR1_IC2PSC_Pos   10U
#define TIM_CCMR1_IC2PSC_Msk   (0x3UL << TIM_CCMR1_IC2PSC_Pos)
#define TIM_CCMR1_IC2PSC(n)    (((n) << TIM_CCMR1_IC2PSC_Pos) & TIM_CCMR1_IC2PSC_Msk)
/* IC2F[15:12] -> input capture 2 filter */
#define TIM_CCMR1_IC2F_Pos     12U
#define TIM_CCMR1_IC2F_Msk     (0xFUL << TIM_CCMR1_IC2F_Pos)
#define TIM_CCMR1_IC2F(n)      (((n) << TIM_CCMR1_IC2F_Pos) & TIM_CCMR1_IC2F_Msk)

/* TIM_CCMR1 (rw: output compare mode - channels 1 and 2)
 * CC1S/CC2S are shared with input capture mode (see above); set them to 00 for output. */
#define TIM_CCMR1_OC1FE        BIT(2)  /* output compare 1 fast enable */
#define TIM_CCMR1_OC1PE        BIT(3)  /* output compare 1 preload enable */
/* OC1M[3:0] is split: OC1M[2:0] -> bits 6:4, OC1M[3] -> bit 16 */
#define TIM_CCMR1_OC1M_Pos     4U
#define TIM_CCMR1_OC1M3_Pos    16U
#define TIM_CCMR1_OC1M_Msk     ((0x7UL << TIM_CCMR1_OC1M_Pos) | BIT(TIM_CCMR1_OC1M3_Pos))
#define TIM_CCMR1_OC1M(n)      (((((n) & 0x7UL) << TIM_CCMR1_OC1M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR1_OC1M3_Pos)) & TIM_CCMR1_OC1M_Msk)
#define TIM_CCMR1_OC1CE        BIT(7)  /* output compare 1 clear enable */
#define TIM_CCMR1_OC2FE        BIT(10) /* output compare 2 fast enable */
#define TIM_CCMR1_OC2PE        BIT(11) /* output compare 2 preload enable */

/* OC2M[3:0] is split: OC2M[2:0] -> bits 14:12, OC2M[3] -> bit 24 */
#define TIM_CCMR1_OC2M_Pos     12U
#define TIM_CCMR1_OC2M3_Pos    24U
#define TIM_CCMR1_OC2M_Msk     ((0x7UL << TIM_CCMR1_OC2M_Pos) | BIT(TIM_CCMR1_OC2M3_Pos))
#define TIM_CCMR1_OC2M(n)      (((((n) & 0x7UL) << TIM_CCMR1_OC2M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR1_OC2M3_Pos)) & TIM_CCMR1_OC2M_Msk)
#define TIM_CCMR1_OC2CE        BIT(15) /* output compare 2 clear enable */

/* TIM_CCMR2 (rw: input capture mode - channels 3 and 4) */
/* CC3S[1:0] -> CC3 selection (direction) - writable only when CC3E = 0 */
#define TIM_CCMR2_CC3S_Pos     0U
#define TIM_CCMR2_CC3S_Msk     (0x3UL << TIM_CCMR2_CC3S_Pos)
#define TIM_CCMR2_CC3S(n)      (((n) << TIM_CCMR2_CC3S_Pos) & TIM_CCMR2_CC3S_Msk)
/* IC3PSC[3:2] -> input capture 3 prescaler */
#define TIM_CCMR2_IC3PSC_Pos   2U
#define TIM_CCMR2_IC3PSC_Msk   (0x3UL << TIM_CCMR2_IC3PSC_Pos)
#define TIM_CCMR2_IC3PSC(n)    (((n) << TIM_CCMR2_IC3PSC_Pos) & TIM_CCMR2_IC3PSC_Msk)
/* IC3F[7:4] -> input capture 3 filter */
#define TIM_CCMR2_IC3F_Pos     4U
#define TIM_CCMR2_IC3F_Msk     (0xFUL << TIM_CCMR2_IC3F_Pos)
#define TIM_CCMR2_IC3F(n)      (((n) << TIM_CCMR2_IC3F_Pos) & TIM_CCMR2_IC3F_Msk)
/* CC4S[9:8] -> CC4 selection (direction) - writable only when CC4E = 0 */
#define TIM_CCMR2_CC4S_Pos     8U
#define TIM_CCMR2_CC4S_Msk     (0x3UL << TIM_CCMR2_CC4S_Pos)
#define TIM_CCMR2_CC4S(n)      (((n) << TIM_CCMR2_CC4S_Pos) & TIM_CCMR2_CC4S_Msk)
/* IC4PSC[11:10] -> input capture 4 prescaler */
#define TIM_CCMR2_IC4PSC_Pos   10U
#define TIM_CCMR2_IC4PSC_Msk   (0x3UL << TIM_CCMR2_IC4PSC_Pos)
#define TIM_CCMR2_IC4PSC(n)    (((n) << TIM_CCMR2_IC4PSC_Pos) & TIM_CCMR2_IC4PSC_Msk)
/* IC4F[15:12] -> input capture 4 filter */
#define TIM_CCMR2_IC4F_Pos     12U
#define TIM_CCMR2_IC4F_Msk     (0xFUL << TIM_CCMR2_IC4F_Pos)
#define TIM_CCMR2_IC4F(n)      (((n) << TIM_CCMR2_IC4F_Pos) & TIM_CCMR2_IC4F_Msk)

/* TIM_CCMR2 (rw: output compare mode - channels 3 and 4)
 * CC3S/CC4S are shared with input capture mode (see above); set them to 00 for output. */
#define TIM_CCMR2_OC3FE        BIT(2)  /* output compare 3 fast enable */
#define TIM_CCMR2_OC3PE        BIT(3)  /* output compare 3 preload enable */
/* OC3M[3:0] is split: OC3M[2:0] -> bits 6:4, OC3M[3] -> bit 16 */
#define TIM_CCMR2_OC3M_Pos     4U
#define TIM_CCMR2_OC3M3_Pos    16U
#define TIM_CCMR2_OC3M_Msk     ((0x7UL << TIM_CCMR2_OC3M_Pos) | BIT(TIM_CCMR2_OC3M3_Pos))
#define TIM_CCMR2_OC3M(n)      (((((n) & 0x7UL) << TIM_CCMR2_OC3M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR2_OC3M3_Pos)) & TIM_CCMR2_OC3M_Msk)
#define TIM_CCMR2_OC3CE        BIT(7)  /* output compare 3 clear enable */
#define TIM_CCMR2_OC4FE        BIT(10) /* output compare 4 fast enable */
#define TIM_CCMR2_OC4PE        BIT(11) /* output compare 4 preload enable */
/* OC4M[3:0] is split: OC4M[2:0] -> bits 14:12, OC4M[3] -> bit 24 */
#define TIM_CCMR2_OC4M_Pos     12U
#define TIM_CCMR2_OC4M3_Pos    24U
#define TIM_CCMR2_OC4M_Msk     ((0x7UL << TIM_CCMR2_OC4M_Pos) | BIT(TIM_CCMR2_OC4M3_Pos))
#define TIM_CCMR2_OC4M(n)      (((((n) & 0x7UL) << TIM_CCMR2_OC4M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR2_OC4M3_Pos)) & TIM_CCMR2_OC4M_Msk)
#define TIM_CCMR2_OC4CE        BIT(15) /* output compare 4 clear enable */

/* TIM_CCMR3 (rw: output compare mode only - channels 5 and 6, no CCxS bits) */
#define TIM_CCMR3_OC5FE        BIT(2)  /* output compare 5 fast enable */
#define TIM_CCMR3_OC5PE        BIT(3)  /* output compare 5 preload enable */
/* OC5M[3:0] is split: OC5M[2:0] -> bits 6:4, OC5M[3] -> bit 16 */
#define TIM_CCMR3_OC5M_Pos     4U
#define TIM_CCMR3_OC5M3_Pos    16U
#define TIM_CCMR3_OC5M_Msk     ((0x7UL << TIM_CCMR3_OC5M_Pos) | BIT(TIM_CCMR3_OC5M3_Pos))
#define TIM_CCMR3_OC5M(n)      (((((n) & 0x7UL) << TIM_CCMR3_OC5M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR3_OC5M3_Pos)) & TIM_CCMR3_OC5M_Msk)
#define TIM_CCMR3_OC5CE        BIT(7)  /* output compare 5 clear enable */
#define TIM_CCMR3_OC6FE        BIT(10) /* output compare 6 fast enable */
#define TIM_CCMR3_OC6PE        BIT(11) /* output compare 6 preload enable */
/* OC6M[3:0] is split: OC6M[2:0] -> bits 14:12, OC6M[3] -> bit 24 */
#define TIM_CCMR3_OC6M_Pos     12U
#define TIM_CCMR3_OC6M3_Pos    24U
#define TIM_CCMR3_OC6M_Msk     ((0x7UL << TIM_CCMR3_OC6M_Pos) | BIT(TIM_CCMR3_OC6M3_Pos))
#define TIM_CCMR3_OC6M(n)      (((((n) & 0x7UL) << TIM_CCMR3_OC6M_Pos) | \
                                 ((((n) >> 3) & 0x1UL) << TIM_CCMR3_OC6M3_Pos)) & TIM_CCMR3_OC6M_Msk)
#define TIM_CCMR3_OC6CE        BIT(15) /* output compare 6 clear enable */

/* TIM_CCER (rw: capture/compare output enable and polarity) */
#define TIM_CCER_CC1E          BIT(0)  /* capture/compare 1 output enable */
#define TIM_CCER_CC1P          BIT(1)  /* capture/compare 1 output polarity */
#define TIM_CCER_CC1NE         BIT(2)  /* capture/compare 1 complementary output enable */
#define TIM_CCER_CC1NP         BIT(3)  /* capture/compare 1 complementary output polarity */
#define TIM_CCER_CC2E          BIT(4)  /* capture/compare 2 output enable */
#define TIM_CCER_CC2P          BIT(5)  /* capture/compare 2 output polarity */
#define TIM_CCER_CC2NE         BIT(6)  /* capture/compare 2 complementary output enable */
#define TIM_CCER_CC2NP         BIT(7)  /* capture/compare 2 complementary output polarity */
#define TIM_CCER_CC3E          BIT(8)  /* capture/compare 3 output enable */
#define TIM_CCER_CC3P          BIT(9)  /* capture/compare 3 output polarity */
#define TIM_CCER_CC3NE         BIT(10) /* capture/compare 3 complementary output enable */
#define TIM_CCER_CC3NP         BIT(11) /* capture/compare 3 complementary output polarity */
#define TIM_CCER_CC4E          BIT(12) /* capture/compare 4 output enable */
#define TIM_CCER_CC4P          BIT(13) /* capture/compare 4 output polarity */
#define TIM_CCER_CC4NP         BIT(15) /* capture/compare 4 complementary output polarity */
#define TIM_CCER_CC5E          BIT(16) /* capture/compare 5 output enable */
#define TIM_CCER_CC5P          BIT(17) /* capture/compare 5 output polarity */
#define TIM_CCER_CC6E          BIT(20) /* capture/compare 6 output enable */
#define TIM_CCER_CC6P          BIT(21) /* capture/compare 6 output polarity */

/* 16-bit vs 32-bit: TIM1/TIM15/TIM16 have 16-bit CNT, ARR and CCRx; TIM2 has 32-bit ones.
 * The _Msk values below are the 16-bit ones - don't apply them to TIM2. PSC is 16-bit on all. */

/* TIM_CNT (rw: counter value) */
/* CNT[15:0] -> counter value (CNT[31:0] on TIM2) */
#define TIM_CNT_CNT_Pos        0U
#define TIM_CNT_CNT_Msk        (0xFFFFUL << TIM_CNT_CNT_Pos)
#define TIM_CNT_UIFCPY         BIT(31) /* read-only UIF copy when CR1.UIFREMAP = 1 (on TIM2 it replaces CNT[31]) */

/* TIM_PSC (rw: prescaler - CK_CNT = fCK_PSC / (PSC + 1), loaded at each update event) */
/* PSC[15:0] -> prescaler value */
#define TIM_PSC_PSC_Pos        0U
#define TIM_PSC_PSC_Msk        (0xFFFFUL << TIM_PSC_PSC_Pos)

/* TIM_ARR (rw: auto-reload - counter period, reset value 0xFFFF; counter is blocked while ARR = 0) */
/* ARR[15:0] -> auto-reload value */
#define TIM_ARR_ARR_Pos        0U
#define TIM_ARR_ARR_Msk        (0xFFFFUL << TIM_ARR_ARR_Pos)

/* TIM_RCR (rw: repetition counter - update event every REP + 1 periods,
 * or half-periods in center-aligned mode; a new value applies after the next repetition update) */
/* REP[15:0] -> repetition counter value */
#define TIM_RCR_REP_Pos        0U
#define TIM_RCR_REP_Msk        (0xFFFFUL << TIM_RCR_REP_Pos)

/* TIM_CCR1..TIM_CCR4 (rw: capture/compare value)
 * Output mode: compare value vs CNT; with OCxPE = 1 it is loaded at the next update event.
 * Input mode: read-only, holds CNT from the last capture. */
/* CCR1[15:0] */
#define TIM_CCR1_CCR1_Pos      0U
#define TIM_CCR1_CCR1_Msk      (0xFFFFUL << TIM_CCR1_CCR1_Pos)
/* CCR2[15:0] */
#define TIM_CCR2_CCR2_Pos      0U
#define TIM_CCR2_CCR2_Msk      (0xFFFFUL << TIM_CCR2_CCR2_Pos)
/* CCR3[15:0] */
#define TIM_CCR3_CCR3_Pos      0U
#define TIM_CCR3_CCR3_Msk      (0xFFFFUL << TIM_CCR3_CCR3_Pos)
/* CCR4[15:0] */
#define TIM_CCR4_CCR4_Pos      0U
#define TIM_CCR4_CCR4_Msk      (0xFFFFUL << TIM_CCR4_CCR4_Pos)

/* TIM_CCR5 (rw: compare value 5 + group channel bits; output only, OC5PE in CCMR3) */
/* CCR5[15:0] */
#define TIM_CCR5_CCR5_Pos      0U
#define TIM_CCR5_CCR5_Msk      (0xFFFFUL << TIM_CCR5_CCR5_Pos)
#define TIM_CCR5_GC5C1         BIT(29) /* OC1REFC = OC1REFC AND OC5REF */
#define TIM_CCR5_GC5C2         BIT(30) /* OC2REFC = OC2REFC AND OC5REF */
#define TIM_CCR5_GC5C3         BIT(31) /* OC3REFC = OC3REFC AND OC5REF */

/* TIM_CCR6 (rw: compare value 6; output only, OC6PE in CCMR3) */
/* CCR6[15:0] */
#define TIM_CCR6_CCR6_Pos      0U
#define TIM_CCR6_CCR6_Msk      (0xFFFFUL << TIM_CCR6_CCR6_Pos)

/* TIM_BDTR (rw: break and dead-time)
 * LOCK can be written only once after reset, and LOCK levels freeze other bits,
 * so configure every field in the first write to this register. */
/* DTG[7:0] -> dead-time generator setup */
#define TIM_BDTR_DTG_Pos       0U
#define TIM_BDTR_DTG_Msk       (0xFFUL << TIM_BDTR_DTG_Pos)
#define TIM_BDTR_DTG(n)        (((n) << TIM_BDTR_DTG_Pos) & TIM_BDTR_DTG_Msk)
/* LOCK[9:8] -> lock configuration (write-once after reset) */
#define TIM_BDTR_LOCK_Pos      8U
#define TIM_BDTR_LOCK_Msk      (0x3UL << TIM_BDTR_LOCK_Pos)
#define TIM_BDTR_LOCK(n)       (((n) << TIM_BDTR_LOCK_Pos) & TIM_BDTR_LOCK_Msk)
#define TIM_BDTR_OSSI          BIT(10) /* off-state selection for idle mode */
#define TIM_BDTR_OSSR          BIT(11) /* off-state selection for run mode */
#define TIM_BDTR_BKE           BIT(12) /* break enable */
#define TIM_BDTR_BKP           BIT(13) /* break polarity */
#define TIM_BDTR_AOE           BIT(14) /* automatic output enable */
#define TIM_BDTR_MOE           BIT(15) /* main output enable - required for any OC/OCN output */
/* BKF[19:16] -> break filter */
#define TIM_BDTR_BKF_Pos       16U
#define TIM_BDTR_BKF_Msk       (0xFUL << TIM_BDTR_BKF_Pos)
#define TIM_BDTR_BKF(n)        (((n) << TIM_BDTR_BKF_Pos) & TIM_BDTR_BKF_Msk)
/* BK2F[23:20] -> break 2 filter */
#define TIM_BDTR_BK2F_Pos      20U
#define TIM_BDTR_BK2F_Msk      (0xFUL << TIM_BDTR_BK2F_Pos)
#define TIM_BDTR_BK2F(n)       (((n) << TIM_BDTR_BK2F_Pos) & TIM_BDTR_BK2F_Msk)
#define TIM_BDTR_BK2E          BIT(24) /* break 2 enable */
#define TIM_BDTR_BK2P          BIT(25) /* break 2 polarity */

/* TIM_DCR (rw: DMA burst control - bursts are triggered by accesses to TIM_DMAR) */
/* DBA[4:0] -> DMA base address, in 32-bit words from TIM_CR1 (0 = CR1, 1 = CR2, 2 = SMCR, ...) */
#define TIM_DCR_DBA_Pos        0U
#define TIM_DCR_DBA_Msk        (0x1FUL << TIM_DCR_DBA_Pos)
#define TIM_DCR_DBA(n)         (((n) << TIM_DCR_DBA_Pos) & TIM_DCR_DBA_Msk)
/* DBL[12:8] -> DMA burst length, number of transfers - 1 (0 = 1 transfer, max 17 = 18 transfers) */
#define TIM_DCR_DBL_Pos        8U
#define TIM_DCR_DBL_Msk        (0x1FUL << TIM_DCR_DBL_Pos)
#define TIM_DCR_DBL(n)         (((n) << TIM_DCR_DBL_Pos) & TIM_DCR_DBL_Msk)

/* TIM_DMAR (rw: DMA burst window)
 * Each access goes to (TIM_CR1 address) + (DBA + DMA index) * 4, index = 0..DBL */
/* DMAB[31:0] -> DMA register for burst accesses */
#define TIM_DMAR_DMAB_Pos      0U
#define TIM_DMAR_DMAB_Msk      (0xFFFFFFFFUL << TIM_DMAR_DMAB_Pos)

/* TIM_OR1 (rw: option register 1 - input remapping) */
/* ETR_ADC1_RMP[1:0] -> ETR remap: 0 = ETR from I/O, 1..3 = ADC1 AWD1..AWD3 */
#define TIM_OR1_ETR_ADC1_RMP_Pos    0U
#define TIM_OR1_ETR_ADC1_RMP_Msk    (0x3UL << TIM_OR1_ETR_ADC1_RMP_Pos)
#define TIM_OR1_ETR_ADC1_RMP(n)     (((n) << TIM_OR1_ETR_ADC1_RMP_Pos) & TIM_OR1_ETR_ADC1_RMP_Msk)
#define TIM_OR1_TI1_RMP             BIT(4) /* IC1 remap: 0 = I/O, 1 = COMP1 */

/* TIM_OR2 (rw: BRK input sources and ETR selection - reset value 0x1, BKINE on)
 * BRK sources are ORed together; BKxP bits are relative to BDTR.BKP. Frozen by LOCK level 1. */
#define TIM_OR2_BKINE               BIT(0)  /* BRK BKIN pin input enable */
#define TIM_OR2_BKCMP1E             BIT(1)  /* BRK COMP1 enable */
#define TIM_OR2_BKCMP2E             BIT(2)  /* BRK COMP2 enable */
#define TIM_OR2_BKINP               BIT(9)  /* BRK BKIN input polarity (1 = inverted) */
#define TIM_OR2_BKCMP1P             BIT(10) /* BRK COMP1 input polarity (1 = inverted) */
#define TIM_OR2_BKCMP2P             BIT(11) /* BRK COMP2 input polarity (1 = inverted) */
/* ETRSEL[16:14] -> ETR source: 0 = from OR1 remap, 1 = COMP1, 2 = COMP2, others reserved */
#define TIM_OR2_ETRSEL_Pos          14U
#define TIM_OR2_ETRSEL_Msk          (0x7UL << TIM_OR2_ETRSEL_Pos)
#define TIM_OR2_ETRSEL(n)           (((n) << TIM_OR2_ETRSEL_Pos) & TIM_OR2_ETRSEL_Msk)

/* TIM_OR3 (rw: BRK2 input sources - reset value 0x1, BK2INE on)
 * BRK2 sources are ORed together; BK2xP bits are relative to BDTR.BK2P. Frozen by LOCK level 1. */
#define TIM_OR3_BK2INE              BIT(0)  /* BRK2 BKIN2 pin input enable */
#define TIM_OR3_BK2CMP1E            BIT(1)  /* BRK2 COMP1 enable */
#define TIM_OR3_BK2CMP2E            BIT(2)  /* BRK2 COMP2 enable */
#define TIM_OR3_BK2INP              BIT(9)  /* BRK2 BKIN2 input polarity (1 = inverted) */
#define TIM_OR3_BK2CMP1P            BIT(10) /* BRK2 COMP1 input polarity (1 = inverted) */
#define TIM_OR3_BK2CMP2P            BIT(11) /* BRK2 COMP2 input polarity (1 = inverted) */

/* OCxM[3:0] values - shared by every channel (OC1M..OC6M) */
#define TIM_OCM_FROZEN             0x0U /* compare has no effect on the output */
#define TIM_OCM_ACTIVE_ON_MATCH    0x1U /* OCxREF forced high on CNT == CCRx */
#define TIM_OCM_INACTIVE_ON_MATCH  0x2U /* OCxREF forced low on CNT == CCRx */
#define TIM_OCM_TOGGLE             0x3U /* OCxREF toggles on CNT == CCRx */
#define TIM_OCM_FORCE_INACTIVE     0x4U /* OCxREF forced low */
#define TIM_OCM_FORCE_ACTIVE       0x5U /* OCxREF forced high */
#define TIM_OCM_PWM1               0x6U /* upcounting: active while CNT < CCRx */
#define TIM_OCM_PWM2               0x7U /* upcounting: inactive while CNT < CCRx */
#define TIM_OCM_RETRIG_OPM1        0x8U /* retriggerable one-pulse mode 1 */
#define TIM_OCM_RETRIG_OPM2        0x9U /* retriggerable one-pulse mode 2 */
#define TIM_OCM_COMBINED_PWM1      0xCU /* OCxREFC = OC1REF OR OC2REF */
#define TIM_OCM_COMBINED_PWM2      0xDU /* OCxREFC = OC1REF AND OC2REF */
#define TIM_OCM_ASYM_PWM1          0xEU /* asymmetric PWM mode 1 */
#define TIM_OCM_ASYM_PWM2          0xFU /* asymmetric PWM mode 2 */

/* TODO: TIM2 option registers (OR1/OR2) - see the general-purpose timers chapter in RM0394 */


#endif /* STM32_TIM_REGS_H */