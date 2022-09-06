#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sysexits.h>

#define WWW "Well Well Well "
#define World "World!"

int 
main (int ac, char *av[])
{
  pid_t p1, p2;
  int fd1[2], fd2[2];

  if (pipe(fd1) || pipe(fd2))
    err(EX_OSERR, "pipe");

  // Запустим 2-х потомков
  if ((p1 = fork()) == 0) { // Первый
    write(fd1[1], WWW, sizeof(WWW) - 1);
    exit(0);
  }
  if (p1 > 0 && (p2 = fork()) == 0) { // Второй
    write(fd2[1], World, sizeof(World) - 1);
    exit(0);
  }
  if (p1 < 0 || p2 < 0)
    err(EX_OSERR, "fork");

  // Прочтем их вывод
  char buf[sizeof(WWW) + sizeof(World)];
  int l, l1;
  if ((l = read(fd1[0], buf, sizeof(WWW))) < 0 ||
      (l1 = read(fd2[0], buf + l, sizeof(World))) < 0)
    err(EX_SOFTWARE, "read pipes");
  buf[l + l1] = 0;

  return puts(buf) == EOF;
}
