#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

static const char *version = "1.0";

static void usage(char *argv[]) {
  fprintf(stderr,
      "Usage: %s -u uid [-g gid] [-d dir] [-v] [-h] -- command args...\n",
      argv[0]);
  fprintf(stderr, "\n");
  fprintf(stderr, "-u user : exec as specified uid or username\n");
  fprintf(stderr, "-g gid : exec as specified gid or groupname\n");
  fprintf(stderr, "-d dir : change directory to specified dir\n");
  fprintf(stderr, "-v : show version and exit\n");
  fprintf(stderr, "-h : show this usage\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  int opt;
  int r;
  int uid = -1;
  int gid = -1;
  char *endptr;
  struct passwd *pass;
  struct group *grp;
  const char *dir = NULL;

  while ((opt = getopt(argc, argv, "u:g:d:vh")) != -1) {
    switch (opt) {
    case 'u':
      uid = (int)strtol(optarg, &endptr, 10);
      if (*endptr != '\0') {
        pass = getpwnam(optarg);
        if (!pass) {
          fprintf(stderr, "integer value must be specified after -u option\n");
          return 1;
        }
        uid = pass->pw_uid;
        gid = pass->pw_gid;
      }
      break;
    case 'g':
      gid = (int)strtol(optarg, &endptr, 10);
      if (*endptr != '\0') {
        grp = getgrnam(optarg);
        if (!grp) {
          fprintf(stderr, "integer value must be specified after -g option\n");
          return 1;
        }
        gid = grp->gr_gid;
      }
      break;
    case 'd':
      dir = optarg;
      break;
    case 'v':
      printf("execas %s\n", version);
      return 0;
    default:
      usage(argv);
      break;
    }
  }

  if (uid == -1 || optind >= argc)
    usage(argv);

  if (gid != -1)
    setgid(gid);
  setuid(uid);
  if (dir)
    chdir(dir);
  r = execv(argv[optind], argv + optind);
  return r ? 1 : 0;
}
