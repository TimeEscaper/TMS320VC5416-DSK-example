/*   CSL configuration is no longer supported.    */
/*   Your CSL configuration has been saved here as part  */
/*   of the process of converting cdb to tcf.    */

/* INPUT tone.cdb */

#define CHIP_5416 1

/*  Include Header Files  */
#include <csl_mcbsp.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MCBSP_Config mcbspCfg0;
extern MCBSP_Handle C54XX_DMA_MCBSP_hMcbsp;
extern void cslCfgInit();

#ifdef __cplusplus
}
#endif /* extern "C" */
