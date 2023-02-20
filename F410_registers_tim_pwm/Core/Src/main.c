/*
* Copyright (c) 2023 by
* Furkan Karagoz, Istanbul/Turkey. All rights reserved.
*
* Permission to use, copy, modify, and distribute this software
* is freely granted, provided that this notice is preserved.
*/

#include "main.h"

int main ( void )
{
	system_clock_config ();
	tim11_config ( 1000, 50000 );

	tim11_enable ( 1 );

	while (1)
		;

	return 0;
}
