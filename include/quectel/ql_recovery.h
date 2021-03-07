#ifndef _QL_RECOVERY_H_
#define _QL_RECOVERY_H_

#include <quectel/ql_def.h>
#include <quectel/ql_types.h>

struct ubi_device;

int ql_recovery_partition(const char *partition, int where);
int ql_recovery_rootfs(int where);
int ql_recovery_ubi(struct ubi_device *ubi, int where);
int ql_recovery_ubi_num(int num, int where);
int ql_clear_modem_load_failed_times(void);

#endif //_QL_RECOVERY_H_


