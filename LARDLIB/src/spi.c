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
////////////////////////////// spi.c //////////////////////////////////
//
// This is a simple implementation of the SPI object, at present it
// does not use interrupts, can only transfer 8 items at a time and
// can only be a master.
//
//
//

#include "LARD.h"

spiConnection * spiCreate (uint8 data_bits, uint8 frame_format, uint8 mode, uint32 bit_rate) {

	spiConnection * s;
	LPC_SSP_Type * port;

	/////////////////////////////////////////////////////////////////
	//
	// Create the SPI structure
	//
	s = (void*)safeMalloc(sizeof (spiConnection));

	if (s == NULL) {
		SYS_ERROR (ERR_SPI_INIT_FAILED | 1);
		return (spiConnection *)ERROR;
	}

	port 		= LPC_SSP;				// Only a single SPI on the LPC1227
	port->CR1  |= (1 << SPI_CR1_MS);	// Set master mode, no slave for the time being
	s->port		= port;

	/////////////////////////////////////////////////////////////////
	//
	// Set the structure's guard bytes
	//
	s->object_id 	 = OBJID_SPI_CONNECTION;
	s->not_object_id = ~OBJID_SPI_CONNECTION;

	/////////////////////////////////////////////////////////////////
	//
	// Enable the port's AHB clock
	//
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 11);

	/////////////////////////////////////////////////////////////////
	//
	// Setup data bits
	//
	if (data_bits < 4 || data_bits > 16) {
		SYS_ERROR (ERR_SPI_BAD_DATA_BITS | data_bits);
		data_bits = 8;
	}
	s->data_bits 	= data_bits;
	port->CR0		= data_bits;

	/////////////////////////////////////////////////////////////////
	//
	// Setup frame format
	//
	if (frame_format > 3) {
		SYS_ERROR (ERR_SPI_BAD_FRAME_FORMAT | frame_format);
		frame_format = SPI_FRAME_FORMAT_SPI;
	}
	s->frame_format	 = frame_format;
	port->CR0		|= (frame_format << SPI_CR0_FRF);

	/////////////////////////////////////////////////////////////////
	//
	// Setup mode
	//
	if (mode > 3) {
		SYS_ERROR (ERR_SPI_BAD_FRAME_FORMAT | mode);
		mode = SPI_MODE_0;
	}
	s->mode		 = mode;
	port->CR0	|= (mode << SPI_CR0_CPOL);


	s->bit_rate	 = bit_rate;
	port->CR0	|= (bit_rate << SPI_CR0_SCR);  // NOTE: Not right, value needs to be calculated

	/////////////////////////////////////////////////////////////////
	//
	// Make sure the port is disabled
	//
	s->enabled = FALSE;
	s->port->CR1 &= ~(1 << SPI_CR1_SSE);

	return s;

}

void spiFlush (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	uint8 x = 0;

	spiLoopbackOn(s);

	for (int i = 0; i < 8; i++)
		s->port->DR = 0;

	for (int i = 0; i < 8; i++)
		x = s->port->DR;

	spiLoopbackOff(s);
}

uint32 spiWrite (spiConnection * s, uint16 * buf, uint8 nVals) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	for (; nVals; nVals--)
		s->port->DR = *buf++;

	return NOERROR;
}

uint32 spiExchange (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->port->CR1 |= (1 << SPI_CR1_SSE);

	////////////////////////////////////////////////////////////
	// Wait for FIFO to empty
	while (!(s->port->SR & (1 << SPI_SR_TFE)));

	s->port->CR1 &= ~(1 << SPI_CR1_SSE);

	return NOERROR;
}

uint32 spiRead (spiConnection * s, uint16 * buf) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	////////////////////////////////////////////////////////////
	// Read values until FIFO is empty
	while (s->port->SR & (1 << SPI_SR_RNE))
		*buf++ = s->port->DR;

	return NOERROR;
}

uint32 spiEnable (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->enabled = TRUE;
	s->port->CR1 |= (1 << SPI_CR1_SSE);

	return NOERROR;
}

uint32 spiDisable (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->enabled = FALSE;
	s->port->CR1 &= ~(1 << SPI_CR1_SSE);

	return NOERROR;
}

uint32 spiLoopbackOn (spiConnection * s) {


	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->port->CR1 |= (1 << SPI_CR1_LBM);

	return NOERROR;
}

uint32 spiLoopbackOff (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->port->CR1 &= ~(1 << SPI_CR1_LBM);

	return NOERROR;
}

uint32 spiMisoDisable (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->port->CR1 |= (1 << SPI_CR1_SOD);

	return NOERROR;
}

uint32 spiMisoEnable (spiConnection * s) {

	VERIFY_OBJECT(s, OBJID_SPI_CONNECTION)

	s->port->CR1 &= ~(1 << SPI_CR1_SOD);

	return NOERROR;
}
