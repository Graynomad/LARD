//////////////////////////////// character.h ////////////////////////////
//
//
//
//
//

#ifndef CHARACTER_H_
#define CHARACTER_H_

/*
 WCharacter.h - Character utility functions for Wiring & Arduino
 Copyright (c) 2010 Hernando Barragan.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <ctype.h>

// WCharacter.h prototypes
INLINE boolean isAlphaNumeric(uint8 c);
INLINE boolean isAlpha(uint8 c);
INLINE boolean isAscii(uint8 c);
INLINE boolean isWhitespace(uint8 c);
INLINE boolean isControl(uint8 c);
INLINE boolean isDigit(uint8 c);
INLINE boolean isGraph(uint8 c);
INLINE boolean isLowerCase(uint8 c);
INLINE boolean isPrintable(uint8 c);
INLINE boolean isPunct(uint8 c);
INLINE boolean isSpace(uint8 c);
INLINE boolean isUpperCase(uint8 c);
INLINE boolean isHexadecimalDigit(uint8 c);
INLINE int toAscii(uint8 c);
INLINE int toLowerCase(uint8 c);
INLINE int toUpperCase(uint8 c);


// Checks for an alphanumeric character.
// It is equivalent to (isalpha(c) || isdigit(c)).
INLINE boolean isAlphaNumeric(uint8 c) {
  return ( isalnum(c) == 0 ? false : true);
}


// Checks for an alphabetic character.
// It is equivalent to (isupper(c) || islower(c)).
INLINE boolean isAlpha(uint8 c) {
  return ( isalpha(c) == 0 ? false : true);
}


// Checks whether c is a 7-bit unsigned char value
// that fits into the ASCII character set.
INLINE boolean isAscii(uint8 c) {
  return ( c < 128 ? TRUE : FALSE);
}


// Checks for a blank character, that is, a space or a tab.
INLINE boolean isWhitespace(uint8 c) {
  return ( isblank (c) == 0 ? false : true);
}


// Checks for a control character.
INLINE boolean isControl(uint8 c) {
  return ( iscntrl (c) == 0 ? false : true);
}


// Checks for a digit (0 through 9).
INLINE boolean isDigit(uint8 c) {
  return ( isdigit (c) == 0 ? false : true);
}


// Checks for any printable character except space.
INLINE boolean isGraph(uint8 c) {
  return ( isgraph (c) == 0 ? false : true);
}


// Checks for a lower-case character.
INLINE boolean isLowerCase(uint8 c) {
  return (islower (c) == 0 ? false : true);
}


// Checks for any printable character including space.
INLINE boolean isPrintable(uint8 c) {
  return ( isprint (c) == 0 ? false : true);
}


// Checks for any printable character which is not a space
// or an alphanumeric character.
INLINE boolean isPunct(uint8 c) {
  return ( ispunct (c) == 0 ? false : true);
}


// Checks for white-space characters. For the avr-libc library,
// these are: space, formfeed ('\f'), newline ('\n'), carriage
// return ('\r'), horizontal tab ('\t'), and vertical tab ('\v').
INLINE boolean isSpace(uint8 c) {
  return ( isspace (c) == 0 ? false : true);
}


// Checks for an uppercase letter.
INLINE boolean isUpperCase(uint8 c) {
  return ( isupper (c) == 0 ? false : true);
}


// Checks for a hexadecimal digits, i.e. one of
// 0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.
INLINE boolean isHexadecimalDigit(uint8 c) {
  return ( isxdigit (c) == 0 ? false : true);
}


// Converts c to a 7-bit unsigned char value that fits into the
// ASCII character set, by clearing the high-order bits.
INLINE int toAscii(uint8 c) {
  return (c & 127);
}


// Warning:
// Many people will be unhappy if you use this function.
// This function will convert accented letters into random
// characters.

// Converts the letter c to lower case, if possible.
INLINE int toLowerCase(uint8 c) {
  return tolower (c);
}


// Converts the letter c to upper case, if possible.
INLINE int toUpperCase(uint8 c) {
  return toupper (c);
}

#endif /* CHARACTER_H_ */
