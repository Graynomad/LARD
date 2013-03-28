//////////////////////////////// vpin.h ////////////////////////////
//
//
//
//
//

#ifndef EXTENSIONS_H_
#define EXTENSIONS_H_


typedef uint32 (*dpinHandlerFuncPtr)(uint32, uint32, uint32, uint32);

uint32 enableVpinHandlers		(uint32 first, uint32 last, dpinHandlerFuncPtr func);
uint32 attachVpinHandler 	(uint32 pin, dpinHandlerFuncPtr handler);
boolean vpinHandlerExists		(uint32 pin);
dpinHandlerFuncPtr vpinHandler	(uint32 pin);


enum {
	EXT_TYPE_DIGPIN	= 0x1,
	EXT_TYPE_ANPIN	= 0x2,
	EXT_TYPE_SERIAL	= 0x4,
	EXT_TYPE_PWM	= 0x8,
	EXT_TYPE_TONE	= 0x10
} extention_enable_flags;

enum	{
	EXT_DIGPIN40,
	EXT_DIGPIN41,
	EXT_DIGPIN42,
	EXT_DIGPIN43,
	EXT_DIGPIN44,
	EXT_DIGPIN45,
	EXT_DIGPIN46,
	EXT_DIGPIN47,
	EXT_DIGPIN48,
	EXT_DIGPIN49,
	EXT_DIGPIN50,
	EXT_DIGPIN51,
	EXT_DIGPIN52,
	EXT_DIGPIN53,
	EXT_DIGPIN54,
	EXT_DIGPIN55,
	EXT_DIGPIN56,
	EXT_DIGPIN57,
	EXT_DIGPIN58,
	EXT_DIGPIN59,
	EXT_DIGPIN60,
	EXT_DIGPIN61,
	EXT_DIGPIN62,
	EXT_DIGPIN63,
	EXT_DIGPIN64,
	EXT_DIGPIN65,
	EXT_DIGPIN66,
	EXT_DIGPIN67,
	EXT_DIGPIN68,
	EXT_DIGPIN69,
	EXT_DIGPIN70,
	EXT_DIGPIN71,
	EXT_DIGPIN72,
	EXT_DIGPIN73,
	EXT_DIGPIN74,
	EXT_DIGPIN75,
	EXT_DIGPIN76,
	EXT_DIGPIN77,
	EXT_DIGPIN78,
	EXT_DIGPIN79,
	EXT_DIGPIN80,
	EXT_DIGPIN81,
	EXT_DIGPIN82,
	EXT_DIGPIN83,
	EXT_DIGPIN84,
	EXT_DIGPIN85,
	EXT_DIGPIN86,
	EXT_DIGPIN87,
	EXT_DIGPIN88,
	EXT_DIGPIN89,
	EXT_DIGPIN90,
	EXT_DIGPIN91,
	EXT_DIGPIN92,
	EXT_DIGPIN93,
	EXT_DIGPIN94,
	EXT_DIGPIN95,
	EXT_DIGPIN96,
	EXT_DIGPIN97,
	EXT_DIGPIN98,
	EXT_DIGPIN99

} digitalpin_extension_types;

enum {
	EXT_ANPIN9,
	EXT_ANPIN10,
	EXT_ANPIN11,
	EXT_ANPIN12,
	EXT_ANPIN13,
	EXT_ANPIN14,
	EXT_ANPIN15,
	EXT_ANPIN16,
	EXT_ANPIN17,
	EXT_ANPIN18,
	EXT_ANPIN29,
	EXT_ANPIN20,
	EXT_ANPIN21,
	EXT_ANPIN22,
	EXT_ANPIN23

} analogpin_extension_types;

enum {
	EXT_SERIAL2,
	EXT_SERIAL3,
	EXT_SERIAL4,
	EXT_SERIAL5,
	EXT_SERIAL6,
	EXT_SERIAL7,
	EXT_SERIAL8,
	EXT_SERIAL9
} serial_extension_types;

enum {
	EXT_PWM8,
	EXT_PWM9,
	EXT_PWM10,
	EXT_PWM11,
	EXT_PWM12,
	EXT_PWM13,
	EXT_PWM14,
	EXT_PWM15,
	EXT_PWM16,
	EXT_PWM17,
	EXT_PWM18,
	EXT_PWM19

} pwm_extension_types;

enum {
	EXT_TONE1
} tone_extension_types;


#endif /* EXTENSIONS_H_ */
