#ifndef __QSTART_H
#define __QSTART_H

#define NORMALMODE 0
#define RECOVERYMODE 1
#define SYSTEM_MTD_NUMBER 20 //I.E /dev/mtd20 is systemfs
#define RECOVERYFS_MTD_NUMBER 15 //I.E /dev/mtd15 is recoveryfs

unsigned int get_bootmode(unsigned int *mode);
#endif
