/*************************************************************************
	> File Name: wwk_main_cfg.h
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Nov 2019 07:42:23 PM PST
 ************************************************************************/

#ifndef _WWK_MAIN_CFG_H
#define _WWK_MAIN_CFG_H

#include "core/wwk_type.h"

//delete first 
void trum(char *strIn, char *strOut);

//write cfg file 
//in-filename, in-key, in-value
int writeCFG(const char *filename, const char *key, const char *value);

//read cfg file 
//filename-in, key-in value-out
void readCFG(const char *filename, const char *key, const char **value);

void wwk_readCFG_int(const char *filename, const char *key, int *value);

#endif
