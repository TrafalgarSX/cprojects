/*
 *  Copyright 2014-2022 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <EsCommon/EsCommon.h>
#include <gmssl/error.h>


int rand_bytes(uint8_t *buf, size_t len)
{

	int ret = -1;

	if (!buf) {
		error_print();
		return -1;
	}
	if (len > INT_MAX) {
		error_print();
		return -1;
	}

	if (!len || len > INT_MAX) {
		error_print();
		return -1;
	}

    EsGetRand(buf, len);

	ret = 1;
end:
	return ret;
}
