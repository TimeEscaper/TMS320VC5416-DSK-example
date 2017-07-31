/*   CSL configuration is no longer supported.    */
/*   Your CSL configuration has been saved here as part  */
/*   of the process of converting cdb to tcf.    */

/* INPUT tone.cdb */

/*  Include Header File  */
#include "tonecfg_csl.h"


/*
 * The following steps are required to use this file:
 *
 * 1.  Modify any source files that use these structures to
 *     `#include <tonecfg_csl.h>`.  These structure definitions were
 *     previously contained in <tonecfg.h>.
 * 2.  Call `CSL_init()` and `cslCfgInit()` from your application's main()
 *     function.  This is necessary to initialize CSL and the assorted
 *     CSL handles.  These functions were previously called by BIOS internally.
 * 3.  Remove the #error line above.
 * 4.  Update your linker options to include the CSL library for the device
 *     that you are using. This library name was listed in the original
 *     generated tonecfg.cmd file (e.g., "-lcsl6416.lib").
 * 5.  If you are using the 54x or 55x, you will need to create an additional
 *     linker .cmd file that includes the generated tonecfg.cmd file
 *     and adds section placement for the '.csldata' section.
 *
 * See the DSP/BIOS Setup Guide for more information.
 */

#pragma CODE_SECTION(cslCfgInit,".text:cslCfgInit")
/*  Config Structures */

/* MCBSP_Config mcbspCfg0
 * comment                                                 <add comments here>
 * Configure DX, FSX, and CLKX as Serial Port Pins         0x1
 * Clock Mode (CLKXM)                                      External
 * SPI Clock Mode (CLKXM)                                  Slave
 * Clock Polarity (CLKXP)                                  Falling Edge
 * Frame-Sync Polarity (FSXP)                              Active High
 * DX Pin Delay (DXENA)                                    Disable
 * Transmit Delay (XDATDLY)                                0-bit
 * Phase (XPHASE)                                          Single-phase
 * Word Length Phase1 (XWDLEN1)                            32-bits
 * Word Length Phase2 (XWDLEN2)                            8-bits
 * Words/Frame Phase1 (XFRLEN1)                            0x2
 * Words/Frame Phase2 (XFRLEN2)                            0x1
 * Detect Sync Error (XSYNCERR)                            Disable
 * Interrupt Mode (XINTM)                                  XRDY
 * Early Frame Sync Response (XFIG)                        Restart Transfer
 * Companding (XCOMPAND)                                   No Companding-MSB First
 * Transmit Frame-Sync Source                              External
 * Configure DR, FSR, CLKR, and CLKS as Serial Port Pins   0x1
 * Clock Mode [DLB = 0] (CLKRM)                            External
 * Clock Mode [DLB = 1] (CLKRM)                            CLKX Driven
 * Clock Polarity (CLKRP)                                  Rising Edge
 * Frame-Sync Polarity (FSRP)                              Active High
 * Receive Delay (RDATDLY)                                 0-bit
 * Phases (RPHASE)                                         Single-phase
 * Word Length Phase1 (RWDLEN1)                            32-bits
 * Word Length Phase2 (RWDLEN2)                            8-bits
 * Words/Frame Phase1 (RFRLEN1)                            0x2
 * Words/Frame Phase2 (RFRLEN2)                            0x1
 * Detect Sync Error (RSYNCERR)                            Disable
 * Interrupt Mode (RINTM)                                  RRDY
 * Frame-Sync Mode (FSRM)                                  External
 * Early Frame Sync Response (RFIG)                        Restart Transfer
 * Sign-Ext & Justification (RJUST)                        Right-justify/zero-fill
 * Companding (RCOMPAND)                                   No Companding-MSB First
 * Breakpoint Emulation                                    Stop Immediately
 * SPI Mode (CLKSTP)                                       Disable
 * Digital Loop Back (DLB)                                 Disable
 * A-Bis (ABIS)                                            Disable
 * Generator Clock Source (CLKSM/SCLKME)                   CPU Clock
 * Clock Synchronization With CLKS Pin (GSYNC)             Disable
 * CLKS Polarity Clock Edge (CLKSP)                        Rising Edge of CLKS
 * Frame Width (1-256) (FWID)                              0x20
 * Clock Divider (1-256) (CLKGDV)                          0x1
 * Frame Period (1-4096) (FPER)                            0x1
 * Configure DX,FSX,CLKX, as GPIO                          0x0
 * Configure DR,FSR,CLKR, as GPIO                          0x0
 * Select CLKX Pin as                                      Input
 * Select FSX Pin as                                       Input
 * Select CLKR Pin as                                      Input
 * Select FSR Pin as                                       Input
 * Select DX Pin as                                        Output
 * Select DR Pin as                                        Input
 * Select CLKS Pin as                                      Input
 * Serial Port Control Register 1                          0x0000
 * Serial Port Control Register 2                          0x0000
 * Receive Control Register 1                              0x01a0
 * Receive Control Register 2                              0x0000
 * Transmit Control Register 1                             0x01a0
 * Transmit Control Register 2                             0x0000
 * Sample Rate Generator Register 1                        0x1f00
 * Sample Rate Generator Register 2                        0x2000
 * Multichannel Control Register 1                         0x0000
 * Multichannel Control Register 2                         0x0000
 * Pin Control Register                                    0x0003
 * RX Channel Enable                                       All 128 Channels
 * Receive Partition A (RPABLK)                            Block 0. Channel 0-15
 * Receive Partition B (RPBBLK)                            Block 1. Channel 16-31
 * Receive Channel Enable Register Partition A             0x0000
 * Receive Channel Enable Register Partition B             0x0000
 * Receive Channel Enable Register Partition C             0x0000
 * Receive Channel Enable Register Partition D             0x0000
 * Receive Channel Enable Register Partition E             0x0000
 * Receive Channel Enable Register Partition F             0x0000
 * Receive Channel Enable Register Partition G             0x0000
 * Receive Channel Enable Register Partition H             0x0000
 * TX Channel Enable                                       All 128 Channels
 * Transmit Partition A (XPABLK)                           Block 0. Channel 0-15
 * Transmit Partition B (XPBBLK)                           Block 1. Channel 16-31
 * Transmit Channel Enable Register Partition A            0x0000
 * Transmit Channel Enable Register Partition B            0x0000
 * Transmit Channel Enable Register Partition C            0x0000
 * Transmit Channel Enable Register Partition D            0x0000
 * Transmit Channel Enable Register Partition E            0x0000
 * Transmit Channel Enable Register Partition F            0x0000
 * Transmit Channel Enable Register Partition G            0x0000
 * Transmit Channel Enable Register Partition H            0x0000
 * ABIS RX. Bit Enable Register Partition A                0x0000
 * ABIS RX. Bit Enable Register Partition B                0x0000
 * ABIS TX. Bit Enable Register Partition A                0x0000
 * ABIS TX. Bit Enable Register Partition B                0x0000
 * Receive Channel Enable Register Partition A             0x0000
 * Receive Channel Enable Register Partition B             0x0000
 * Receive Channel Enable Register Partition C             0x0000
 * Receive Channel Enable Register Partition D             0x0000
 * Receive Channel Enable Register Partition E             0x0000
 * Receive Channel Enable Register Partition F             0x0000
 * Receive Channel Enable Register Partition G             0x0000
 * Receive Channel Enable Register Partition H             0x0000
 * Transmit Channel Enable Register Partition A            0x0000
 * Transmit Channel Enable Register Partition B            0x0000
 * Transmit Channel Enable Register Partition C            0x0000
 * Transmit Channel Enable Register Partition D            0x0000
 * Transmit Channel Enable Register Partition E            0x0000
 * Transmit Channel Enable Register Partition F            0x0000
 * Transmit Channel Enable Register Partition G            0x0000
 * Transmit Channel Enable Register Partition H            0x0000
 */
MCBSP_Config mcbspCfg0 = {
    0x0000,        /*  Serial Port Control Register 1   */
    0x0000,        /*  Serial Port Control Register 2   */
    0x01a0,        /*  Receive Control Register 1   */
    0x0000,        /*  Receive Control Register 2   */
    0x01a0,        /*  Transmit Control Register 1   */
    0x0000,        /*  Transmit Control Register 2   */
    0x1f00,        /*  Sample Rate Generator Register 1   */
    0x2000,        /*  Sample Rate Generator Register 2   */
    0x0000,        /*  Multichannel Control Register 1   */
    0x0000,        /*  Multichannel Control Register 2   */
    0x0003,        /*  Pin Control Register   */
    0x0000,        /*  Receive Channel Enable Register Partition A   */
    0x0000,        /*  Receive Channel Enable Register Partition B   */
    0x0000,        /*  Receive Channel Enable Register Partition C   */
    0x0000,        /*  Receive Channel Enable Register Partition D   */
    0x0000,        /*  Receive Channel Enable Register Partition E   */
    0x0000,        /*  Receive Channel Enable Register Partition F   */
    0x0000,        /*  Receive Channel Enable Register Partition G   */
    0x0000,        /*  Receive Channel Enable Register Partition H   */
    0x0000,        /*  Transmit Channel Enable Register Partition A   */
    0x0000,        /*  Transmit Channel Enable Register Partition B   */
    0x0000,        /*  Transmit Channel Enable Register Partition C   */
    0x0000,        /*  Transmit Channel Enable Register Partition D   */
    0x0000,        /*  Transmit Channel Enable Register Partition E   */
    0x0000,        /*  Transmit Channel Enable Register Partition F   */
    0x0000,        /*  Transmit Channel Enable Register Partition G   */
    0x0000         /*  Transmit Channel Enable Register Partition H   */
};

/*  Handles  */
MCBSP_Handle C54XX_DMA_MCBSP_hMcbsp;

/*
 *  ======== cslCfgInit() ========  
 */
void cslCfgInit()
{
    C54XX_DMA_MCBSP_hMcbsp = MCBSP_open(MCBSP_PORT2, MCBSP_OPEN_RESET);
    MCBSP_config(C54XX_DMA_MCBSP_hMcbsp, &mcbspCfg0);




}
