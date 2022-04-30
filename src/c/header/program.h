#ifndef PROGRAM_H
#define PROGRAM_H

#include "std_type.h"
#include "filesystem.h"
#include "string_lib.h"

void exec(struct file_metadata *metadata, int segment);

void exit();

#endif