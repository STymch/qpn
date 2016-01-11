/**
* @file
* @brief QF-nano port to Cortex-M, cooperative QV kernel, TI-ARM CCS toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.6.1
* Date of the Last Update:  2016-01-10
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qfn_port_h
#define qfn_port_h

/* is the target M3/M4/M7? see NOTE2 */
#if (defined __TI_TMS470_V7M3__) \
    || (defined __TI_TMS470_V7M4__) \
    || (defined __TI_TMS470_V7M7__)

    /* BASEPRI limit for QF-aware ISR priorities, see NOTE3 */
    #define QF_BASEPRI          (0xFFU >> 2)

    /* QF-aware ISR priority for CMSIS function NVIC_SetPriority(), NOTE4 */
    #define QF_AWARE_ISR_CMSIS_PRI  (QF_BASEPRI >> (8 - __NVIC_PRIO_BITS))

    /* QF interrupt disable/enable */
    #define QF_INT_DISABLE()    QF_set_BASEPRI(QF_BASEPRI)
    #define QF_INT_ENABLE()     QF_set_BASEPRI(0U)

    /* the intrinsic function _norm() generates the CLZ instruction */
    #define QF_LOG2(n_) ((uint8_t)(32U - _norm(n_)))

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        __asm(" CPSID I"); \
        QF_INT_ENABLE(); \
        __asm(" WFI"); \
        __asm(" CPSIE I"); \
    } while (0)

    /* assembly function for setting the BASEPRI register */
    void QF_set_BASEPRI(unsigned basePri);

/* not M3/M4/M7, assuming no BASEPRI register or CLZ instruction */
#else

    /* QF interrupt disable/enable */
    #define QF_INT_DISABLE()    __asm(" CPSID I")
    #define QF_INT_ENABLE()     __asm(" CPSIE I")

    /* QF-aware ISR priority for CMSIS function NVIC_SetPriority(), NOTE5 */
    #define QF_AWARE_ISR_CMSIS_PRI  0

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        __asm(" WFI"); \
        QF_INT_ENABLE(); \
    } while (0)

#endif /* not M3/M4/M7 */

/* interrupt nesting policy for ISR level (ISRs can nest) */
#define QF_ISR_NEST

#include <stdint.h>     /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h>    /* Boolean type.      WG14/N843 C99 Standard */

#include "qepn.h"       /* QEP-nano platform-independent public interface */
#include "qfn.h"        /* QF-nano platform-independent public interface */
#include "qvn.h"        /* QV-nano platform-independent public interface */

/*****************************************************************************
* NOTE2:
* On Cortex-M3/M4/M7, the interrupt disable/enable policy uses the BASEPRI
* register (which is not implemented in Cortex-M0/M0+/M1) to disable
* interrupts only with priority lower than the level specified by the
* QF_BASEPRI macro. The interrupts with priorities above QF_BASEPRI (i.e.,
* with numerical priority values lower than QF_BASEPRI) are not disabled in
* this method. These free-running interrupts are not allowed to call any QF
* services, because QF is not aware of these interrupts. Coversely, only
* "QF-aware" interrupts, with numerical values of priorities eqal to or
* higher than QF_BASEPRI, can call QF services.
*
* NOTE3:
* For Cortex-M3/M4/M7, the macro QF_BASEPRI leaves the top 2 priority bits
* empty for QF-aware interrupts. This is the highest-possible priority
* (lowest possible numerical value) for the guaranteed 3 priority bits
* implemented in the NVIC.
*
* NOTE4:
* The QF_AWARE_ISR_CMSIS_PRI macro is useful as an offset for enumerating
* the QF-aware interrupt priority levels in the applications, whereas the
* numerical values of the QF-aware interrupts must be greater or equal to
* QF_AWARE_ISR_CMSIS_PRI. The enumerated values based on
* QF_AWARE_ISR_CMSIS_PRI can be passed directly to the CMSIS function
* NVIC_SetPriority(), which shifts them by (8 - __NVIC_PRIO_BITS) into the
* correct bit position, while __NVIC_PRIO_BITS is the CMSIS macro defining
* the number of implemented priority bits in the NVIC. Please note that
* the macro QF_AWARE_ISR_CMSIS_PRI is intended only for applications and
* is not used inside the QF port, which remains generic and not dependent
* on the number of implemented priority bits in the NVIC.
*
* NOTE5:
* On Cortex-M0/M0+/M1 (architecture v6-M, v6S-M), the interrupt disabling
* policy uses the PRIMASK register to disable interrupts globally. The
* QF_AWARE_ISR_CMSIS_PRI level is zero, meaning that all interrupts are
* "kernel-aware".
*/

#endif /* qfn_port_h */
