// Copyright (c) 2013, Rob Gray (www.robgray.com)
//  
// Permission to use, copy, modify, and/or distribute this software for  
// any purpose with or without fee is hereby granted, provided that the  
// above copyright notice and this permission notice appear in all copies.  
// 
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
// SOFTWARE.  
//
// History:
//	17mar13:RG:Initial GIT upload
//	
//////////////////////////////// pingroup.h ////////////////////////////
//
//
//
//
//

#ifndef PINGROUP_H_
#define PINGROUP_H_

typedef struct {
	uint8	object_id;

	uint8	pin_lo;			// lowest pin in group
	uint8	pin_hi;			// highest pin in group
	uint8	port;			// Port the group is on
	LPC_GPIO_Type *port_address;	// The address of the port the group is on
	uint8	nBits;			// Number of bits in the group
	uint32	max_val;		// Maximum value the group can hold
	uint32	msb_mask;		// Bit mask for the most significant bit in the group
	uint32	clear_mask;		// Mask used to clear all bits in the group
	uint32	set_mask;		// Mask used to set all bits in the group
	uint32	shift_bits;		// Number of bits to shift data
	uint32	val;			// current value of the group

	uint8	not_object_id;

} pinGroup;

#define CHECK_PARMS		 							\
		if (group >= N_PINGROUPS) {					\
			SYS_ERROR (ERR_INV_PINGROUP);			\
			return ERROR;							\
		}											\
		if (pinGroups[group].max_val == 0) {		\
			SYS_ERROR (ERR_PINGROUP_NOT_DEFINED);	\
			return ERROR;							\
		}

pinGroup * pinGroupCreate(uint32 in_out, uint32 pin_lo, uint32 pin_hi);
uint32 pinGroupWrite		(pinGroup * pg, uint32 val);
uint32 pinGroupRead			(pinGroup * pg);
uint32 pinGroupInc			(pinGroup * pg);
uint32 pinGroupDec			(pinGroup * pg);
uint32 pinGroupClear		(pinGroup * pg);
uint32 pinGroupSet			(pinGroup * pg);
uint32 pinGroupShiftLeft	(pinGroup * pg, uint32 bits);
uint32 pinGroupShiftRight	(pinGroup * pg, uint32 bits);
uint32 pinGroupRotateLeft	(pinGroup * pg);
uint32 pinGroupRotateRight	(pinGroup * pg);
uint32 pinGroupInvert		(pinGroup * pg);
uint32 pinGroupOR			(pinGroup * pg, uint32 val);
uint32 pinGroupAND			(pinGroup * pg, uint32 val);
uint32 pinGroupXOR			(pinGroup * pg, uint32 val);

#endif /* PINGROUP_H_ */
