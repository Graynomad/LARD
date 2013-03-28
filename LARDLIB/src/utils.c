///////////////////////////// utils.c /////////////////////////////////
//
//  
//
//
//

#include	"lard.h"


int32 celcius2fahrenheit(int32 celcius)
{
	static int32 prevCelsius = -50;  // some value that will never occur

	if (prevCelsius == -50) prevCelsius = celcius;
	prevCelsius = ((prevCelsius + celcius) * 9 + 325)/10;
	return prevCelsius;

}
