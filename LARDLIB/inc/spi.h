//////////////////////////////// spi.h ////////////////////////////
//
//
//
//
//

#ifndef SPI_H_
#define SPI_H_

typedef struct {

	uint8	object_id;

	uint8	data_bits;		// 4-16, number of bits to transmit/receive
	uint8	frame_format;	// 0, SPI frame format
							// 1, Texas Instruments synchronous serial frame format
							// 2, Microwire frame format

	uint8	mode;			// 0, CPOL=0, CPHA=0
							// 1, CPOL=0, CPHA=1
							// 2, CPOL=1, CPHA=0
							// 3, CPOL=1, CPHA=1
	LPC_SSP_Type * port;
	uint32	bit_rate;
	boolean	enabled;
	boolean master;
	boolean	loopback;

	uint8	not_object_id;

} spiConnection;

enum {
	SPI_FRAME_FORMAT_SPI,
	SPI_FRAME_FORMAT_TI,
	SPI_FRAME_FORMAT_MW
};

////////////////////////////////////////////////////////////////////
//
// Bit offsets in the SPI control register 0
//
enum {
	SPI_CR0_DSS,
	SPI_CR0_FRF 	= 4,
	SPI_CR0_CPOL 	= 6,
	SPI_CR0_CPHA,
	SPI_CR0_SCR
};

////////////////////////////////////////////////////////////////////
//
// Bit offsets in the SPI control register 1
//
enum {
	SPI_CR1_LBM,
	SPI_CR1_SSE,
	SPI_CR1_MS,
	SPI_CR1_SOD
};

////////////////////////////////////////////////////////////////////
//
// Bit offsets in the SPI status register
//
enum {
	SPI_SR_TFE,
	SPI_SR_TNF,
	SPI_SR_RNE,
	SPI_SR_RFF,
	SPI_SR_BSY
};

enum {
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

spiConnection * spiCreate (uint8 data_bits, uint8 frame_format, uint8 mode, uint32 bit_rate);
uint32	spiWrite 		(spiConnection * s, uint16 * buf, uint8 nVals);
uint32	spiExchange		(spiConnection * s);
uint32	spiRead 		(spiConnection * s, uint16 * buf);
uint32	spiEnable		(spiConnection * s);
uint32	spiDisable		(spiConnection * s);
uint32	spiLoopbackOn	(spiConnection * s);
uint32	spiLoopbackOff	(spiConnection * s);
uint32	spiMisoDisable	(spiConnection * s);
uint32	spiMisoEnable	(spiConnection * s);
void 	spiFlush 		(spiConnection * s);

#endif /* SPI_H_ */
