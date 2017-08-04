/*
 *  arch/arm/mach-sunxi/arisc/include/arisc_dbgs.h
 *
 * Copyright (c) 2012 Allwinner.
 * 2012-10-01 Written by superm (superm@allwinnertech.com).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ARISC_DBGS_H
#define __ARISC_DBGS_H

/*
 * debug level define,
 * level 0 : dump debug information--none;
 * level 1 : dump debug information--error;
 * level 2 : dump debug information--error+warning;
 * level 3 : dump debug information--error+warning+information;
 * extern void printk(const char *, ...);
 */
#ifdef ARISC_DEBUG_ON
/* debug levels */
#define DEBUG_LEVEL_INF    ((uint32_t)1 << 0)
#define DEBUG_LEVEL_LOG    ((uint32_t)1 << 1)
#define DEBUG_LEVEL_WRN    ((uint32_t)1 << 2)
#define DEBUG_LEVEL_ERR    ((uint32_t)1 << 3)

#define ARISC_INF(...)                          \
	if(DEBUG_LEVEL_INF & (0xf0 >> (arisc_debug_level +1)))  \
		tf_printf("[SCP] :" __VA_ARGS__);

#define ARISC_LOG(...)                                      \
	if(DEBUG_LEVEL_LOG & (0xf0 >> (arisc_debug_level +1)))	\
		tf_printf("[SCP] :" __VA_ARGS__);

#define ARISC_WRN(...)                          \
	if(DEBUG_LEVEL_WRN & (0xf0 >> (arisc_debug_level +1)))  \
		tf_printf("[SCP WARING] :" __VA_ARGS__);

#define ARISC_ERR(...)                          \
	if(DEBUG_LEVEL_ERR & (0xf0 >> (arisc_debug_level +1)))  \
		tf_printf("[SCP ERROR] :" __VA_ARGS__);

#else /* ARISC_DEBUG_ON */
#define ARISC_INF(...)
#define ARISC_WRN(...)
#define ARISC_ERR(...)
#define ARISC_LOG(...)
#endif /* ARISC_DEBUG_ON */

/* report error information id */
#define ERR_NMI_INT_TIMEOUT    (0x1)

#endif /* __ARISC_DBGS_H */
