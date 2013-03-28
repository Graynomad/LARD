//////////////////////////////// vpin.c /////////////////////////////////
//
//  
//
//
//

#include "LARD.h"
//int (*(*rdPtrList)[4])(unsigned int addr, unsigned int data);
//int         (*(*p)                        [4])();
static uint32 (*(*_vpinHandlers)) (uint32 pin, uint32 func, uint32 data1, uint32 data2);
//static uint32 _nVpinHandlers	= 0;
static uint32 _nVpins	= 0;

uint32 nDigpins (void) {
	return _nVpins;
}

uint32 enableVpinHandlers (uint32 first, uint32 last, dpinHandlerFuncPtr handler) {

	uint32 nPins  = last - first + 1;
	uint32 array_size = sizeof(void*) * nPins;

	_vpinHandlers = safeMalloc (array_size);

	if (_vpinHandlers == NULL) {
		SYS_ERROR (ERR_MALLOC_FAILED);
		return ERROR;
	}

	for (int i = 0; i < nPins; i++, _nVpins++) {
		*(_vpinHandlers + _nVpins) = handler;
//		_nVpinHandlers++;
	}

	_vpinHandlers = NULL;

	return NOERROR;
}

uint32 attachVpinHandler (uint32 pin, dpinHandlerFuncPtr handler) {

	if (pin <= NATIVE_PINS || pin >= _nVpins + NATIVE_PINS) {
		ASSERT (FORCE, ERR_INV_VPIN | pin);
		return ERROR;
	}

	if (_vpinHandlers == NULL) {
		ASSERT (FORCE, ERR_DPIN_HANDLER_NOT_SET | pin);
		return ERROR;
	}

	*(_vpinHandlers + pin - NATIVE_PINS -1) = handler;

	return NOERROR;

}

boolean vpinHandlerExists (uint32 pin) {

	if (_vpinHandlers == NULL) {
		SYS_ERROR (ERR_DPIN_HANDLER_NOT_SET);
		return FALSE;
	}

	return *(_vpinHandlers + pin -1 - NATIVE_PINS) != NULL;

}


dpinHandlerFuncPtr vpinHandler(uint32 pin) {

	if (_vpinHandlers == NULL) {
		SYS_ERROR (ERR_DPIN_HANDLER_NOT_SET);
		return NULL;
	}

	return *(_vpinHandlers + pin -1 -NATIVE_PINS);

}

uint32 enableApinExtensions (uint32 first, uint32 last) {

	return NOERROR;
}

uint32 enableSerialExtensions (uint32 first, uint32 last) {

	return NOERROR;
}

uint32 enablePwmExtensions (uint32 first, uint32 last) {

	return NOERROR;
}

uint32 enableToneExtensions (uint32 first, uint32 last) {

	return NOERROR;
}
