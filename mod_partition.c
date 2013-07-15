#include "common.h"
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <mntent.h>



void read_partion_stat()
{
  FILE *mntfile;
  struct mntent *mnt = NULL;

  mntfile = setmntent("/etc/mtab", "r");
  while((mnt = getmntent(mntfile)) != NULL){
   if(! strncmp(mnt->mnt_fsname, "/", 1)){
      struct statfs fsbuf;
      if(statfs(mnt->mnt_dir, &fsbuf) >= 0){
        long total = fsbuf.f_blocks;
        long available = fsbuf.f_bavail;
        long available_to_root = fsbuf.f_bfree;
        long used = total - available_to_root;
        int scale = fsbuf.f_bsize / 1024;
        double pct = 0;
        if(total != 0){
          pct = (used * 100.0) /(used + available);
        }
        statpartion.total = total * scale;
        statpartion.used = used *scale;
        statpartion.pct = pct;
        strncpy(statpartion.dir , mnt->mnt_dir, 32);
//        printf(" %s %ld %ld %f\n", statpartion.dir, statpartion.total, statpartion.used,statpartion.pct);
      }
    }
  }
    endmntent(mntfile);
}

