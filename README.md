
# netware-screensaver-linux <!-- omit in toc -->

NetWare SMP style worm screesnsaver for Linux using ncurses

## Table of Contents <!-- omit in toc -->
- [Overview](#overview)
- [Building the Screensaver](#building-the-screensaver)
- [Installing and Uninstalling the Screensaver](#installing-and-uninstalling-the-screensaver)
- [Running the Screensaver](#running-the-screensaver)
- [Installing the Screensaver in the Bash Shell](#installing-the-screensaver-in-the-bash-shell)
- [Using Libraries](#using-libraries)
- [Building the Screensaver as an RPM Package (Redhat/CentOS/SuSe)](#building-as-an-rpm-package)
- [Installing and Deinstalling the RPM Package](#installing-and-deinstalling-the-rpm-package)
- [Building the Screensaver as a Debian Package (Debian/Ubuntu)](#building-as-a-debian-package)
- [Installing and Deinstalling the Debian Package](#installing-and-deinstalling-the-debian-package)
- [Issues / Problems / Help](#issues-problems-help)

## Overview

This code is Copyright(c) Jeff V. Merkey 1997-2022 and some portions were adapted from xscreensaver loadsnake program and those portions are Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>.  This software is licensed under the Lesser GNU Public License (LGPL) v2.1.

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.  No representations are made about the suitability of
this software for any purpose.  It is provided "as is" without express or
implied warranty.

```sh
USAGE:  netware-worms [cpus|speedup]
    i.e. worm cpus=<num=8 to 256> speedup=<divisor=1|2|3|4>
examples:
    netware-worms cpus=8;
       start worm screensaver with 8 worm threads
    netware-worms speedup=4
       run worm screensaver at 4X speed
```

This screensaver is built as both a dynamic shared object (\*.so) and static
archive (\*.a) library module which allows other user space programs and
consoles to invoke this screen saver as well as tmux, screen, and other
BASH type programs setups.

This screensaver uses ncurses and is text based which is identical
to the Novell Netware SMP multi processor screensaver which displayed
a color coded worm for each processor running.  As each processor load
increases on a particular processor, the longer the worm becomes and
the faster it moves across the screen corresponding to that particular
processor.  As the overall system load average across the system increases,
the base clock rate for all the worms increases slightly to indicate 
load average is increasing.

## Building the Screensaver

You must have the ncurses-devel packages installed in order to make
ncurses apps on your system.  you can try "yum install ncurses\*" to get 
all of the ncurses packages for RedHat and CentOS systems.  This version 
was developed on a Red Hat 7 / CentOS 7 system. 

to make, type:

```sh
make -f Makefile <enter>
```

the build process should display:

```sh
g++ -g -c -O3  -fPIC -Wall libnetware-screensaver.c 
ar r libnetware-screensaver.a libnetware-screensaver.o  
ar: creating libnetware-screensaver.a
ld -shared -lc -o libnetware-screensaver.so libnetware-screensaver.o  
g++ -g -O3 netware-worms.c libnetware-screensaver.a -Wall -o netware-worms -lncursesw -lpthread -ltinfo
```

to perform a clean build:

```sh
make -f Makefile clean <enter>
```

should display:

```sh
rm -rf *.o libnetware-screensaver.a libnetware-screensaver.so netware-worms
```

## Installing and Uninstalling the Screensaver

To install:

```sh
make -f Makefile install <enter>
```

should display:

```sh
install -m 0755 netware-worms /usr/bin
install -m 0755 libnetware-screensaver.so /usr/lib64
install -m 644 libnetware-screensaver.a /usr/lib64
install -m 644 netware-worms.h /usr/include
ldconfig
ldconfig
```

To uninstall:

```sh
make -f Makefile uninstall <enter>
```

should display:

```sh
rm -vf /usr/bin/netware-worms
removed '/usr/bin/netware-worms'
rm -vf /usr/lib64/libnetware-screensaver.so
removed '/usr/lib64/libnetware-screensaver.so'
rm -vf /usr/lib64/libnetware-screensaver.a
removed '/usr/lib64/libnetware-screensaver.a'
rm -vf /usr/include/netware-worms.h
removed '/usr/include/netware-worms.h'
ldconfig
ldconfig
```

## Running the Screensaver

you can execute the program directly as:

```sh
./netware-worms <enter>
```
you can also add arguments to force number of cpus and a speedup divisor

```sh
./netware-worms cpus=8 speedup=4 <enter>
```

## Installing the Screensaver in the Bash Shell

To install the screensaver as a bash program which auto activates, use the program called
"screen" coupled with Bash.  The "screen" program has support for adding and invoking
terminal based screensavers under Bash.

You can install the program on CentOS and RHEL with yum by
typing "yum install screen" if the program is not installed by default.  You will need
to create a .screenrc file in the user home directory (i.e. ~/.screenrc)
which contains the following commands:

Sample ~/.screenrc file
```sh
blankerprg netware-worms
idle 60 blanker
```

Please note the idle command which specifies how many seconds to delay before invoking
the screensaver (in this example it is 60 seconds)

You also need to modify the ~/.bash\_profile default bash configuation file to invoke
screen by default after login.  Add the following command to the ~/.bash\_profile
config file:

Sample ~/.bash\_profile
```sh
# .bash_profile
# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi
# User specific environment and startup programs
PATH=$PATH:$HOME/bin
export PATH
/bin/screen  <------------ add this line to the default .bash_profile config file.  
                           It must be the last line in the file.
```

After you have configured these two files, logout and log back into the system for
whichever account you have enabled and the screensaver will auto-activate after 
60 seconds of idle time.  The only disadvantage to using this method to enable 
a terminal based screensaver is you will need to type the "exit" command twice 
in order to logout of the session. 


## Using Libraries

You can also call the screensaver as a library function "netware\_screensaver(cpus, speedup)"
from another application with your own timer setup, just be sure to link to the
 "-lnetware-screensaver" .so library in your linker statements or link to the static
library "libnetware-screensaver.a"

## Building as an RPM Package

```sh
netware-screensaver-1.21/
netware-screensaver-1.21/.screenrc
netware-screensaver-1.21/netware-worms.c
netware-screensaver-1.21/COPYING
netware-screensaver-1.21/README
netware-screensaver-1.21/.gitignore
netware-screensaver-1.21/.git/
netware-screensaver-1.21/.git/hooks/
netware-screensaver-1.21/.git/hooks/pre-applypatch.sample
netware-screensaver-1.21/.git/hooks/pre-push.sample
netware-screensaver-1.21/.git/hooks/commit-msg.sample
netware-screensaver-1.21/.git/hooks/update.sample
netware-screensaver-1.21/.git/hooks/applypatch-msg.sample
netware-screensaver-1.21/.git/hooks/prepare-commit-msg.sample
netware-screensaver-1.21/.git/hooks/pre-rebase.sample
netware-screensaver-1.21/.git/hooks/post-update.sample
netware-screensaver-1.21/.git/hooks/pre-commit.sample
netware-screensaver-1.21/.git/refs/
netware-screensaver-1.21/.git/refs/heads/
netware-screensaver-1.21/.git/refs/tags/
netware-screensaver-1.21/.git/refs/remotes/
netware-screensaver-1.21/.git/refs/remotes/origin/
netware-screensaver-1.21/.git/info/
netware-screensaver-1.21/.git/info/refs
netware-screensaver-1.21/.git/info/exclude
netware-screensaver-1.21/.git/description
netware-screensaver-1.21/.git/index
netware-screensaver-1.21/.git/ORIG_HEAD
netware-screensaver-1.21/.git/config
netware-screensaver-1.21/.git/HEAD
netware-screensaver-1.21/.git/COMMIT_EDITMSG
netware-screensaver-1.21/.git/FETCH_HEAD
netware-screensaver-1.21/.git/packed-refs
netware-screensaver-1.21/.git/objects/
netware-screensaver-1.21/.git/objects/pack/
netware-screensaver-1.21/.git/objects/pack/pack-0b572c64ba3e068597888b5903b1c6c227c9de6f.pack
netware-screensaver-1.21/.git/objects/pack/pack-0b572c64ba3e068597888b5903b1c6c227c9de6f.idx
netware-screensaver-1.21/.git/objects/info/
netware-screensaver-1.21/.git/objects/info/packs
netware-screensaver-1.21/.git/objects/info/commit-graph
netware-screensaver-1.21/.git/logs/
netware-screensaver-1.21/.git/logs/refs/
netware-screensaver-1.21/.git/logs/refs/remotes/
netware-screensaver-1.21/.git/logs/refs/remotes/origin/
netware-screensaver-1.21/.git/logs/refs/remotes/origin/master
netware-screensaver-1.21/.git/logs/refs/heads/
netware-screensaver-1.21/.git/logs/refs/heads/master
netware-screensaver-1.21/.git/logs/HEAD
netware-screensaver-1.21/.git/branches/
netware-screensaver-1.21/Makefile
netware-screensaver-1.21/netware-worms.h
netware-screensaver-1.21/netware-screensaver.spec
netware-screensaver-1.21/libnetware-screensaver.c
```

```sh
Executing(%prep): /bin/sh -e /var/tmp/rpm-tmp.oqGyza
+ umask 022
+ cd /root/rpmbuild/BUILD
+ cd /root/rpmbuild/BUILD
+ rm -rf netware-screensaver-1.21
+ /usr/bin/gzip -dc /root/rpmbuild/SOURCES/netware-screensaver-1.21.tar.gz
+ /usr/bin/tar -xof -
+ STATUS=0
+ '[' 0 -ne 0 ']'
+ cd netware-screensaver-1.21
+ /usr/bin/chmod -Rf a+rX,u+w,g-w,o-w .
+ exit 0
Executing(%build): /bin/sh -e /var/tmp/rpm-tmp.JTscEj
+ umask 022
+ cd /root/rpmbuild/BUILD
+ cd netware-screensaver-1.21
+ /usr/bin/make
g++ -g -c -O3  -fPIC -Wall libnetware-screensaver.c 
ar r libnetware-screensaver.a libnetware-screensaver.o  
ar: creating libnetware-screensaver.a
ld -shared -lc -o libnetware-screensaver.so libnetware-screensaver.o  
g++ -g -O3 netware-worms.c libnetware-screensaver.a -Wall -o netware-worms -lncursesw -lpthread -ltinfo
+ exit 0
Executing(%install): /bin/sh -e /var/tmp/rpm-tmp.GtWplM
+ umask 022
+ cd /root/rpmbuild/BUILD
+ '[' /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64 '!=' / ']'
+ rm -rf /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
++ dirname /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
+ mkdir -p /root/rpmbuild/BUILDROOT
+ mkdir /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
+ cd netware-screensaver-1.21
+ '[' -n /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64 -a /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64 '!=' / ']'
+ /usr/bin/rm -rf /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
+ /usr/bin/mkdir -p /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/sbin
+ /usr/bin/mkdir -p /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/bin
+ /usr/bin/mkdir -p /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/include
+ /usr/bin/mkdir -p /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/lib64
+ /usr/bin/make DESTDIR=/root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64 NOCHK=1 LIBDIR=/usr/lib64 INCDIR=/usr/include BINDIR=/usr/bin install
install -m 0755 netware-worms /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/bin
install -m 0755 libnetware-screensaver.so /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/lib64
install -m 644 libnetware-screensaver.a /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/lib64
install -m 644 netware-worms.h /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64/usr/include
+ /usr/lib/rpm/check-buildroot
+ /usr/lib/rpm/redhat/brp-ldconfig
/sbin/ldconfig: Warning: ignoring configuration file that cannot be opened: /etc/ld.so.conf: No such file or directory
+ /usr/lib/rpm/brp-compress
+ /usr/lib/rpm/brp-strip /usr/bin/strip
+ /usr/lib/rpm/brp-strip-comment-note /usr/bin/strip /usr/bin/objdump
+ /usr/lib/rpm/brp-strip-static-archive /usr/bin/strip
+ /usr/lib/rpm/brp-python-bytecompile '' 1
+ /usr/lib/rpm/brp-python-hardlink
+ PYTHON3=/usr/libexec/platform-python
+ /usr/lib/rpm/redhat/brp-mangle-shebangs
Processing files: netware-screensaver-1.21-1.el8.x86_64
Provides: libnetware-screensaver.so()(64bit) netware-screensaver = 1.21-1.el8 netware-screensaver(x86-64) = 1.21-1.el8
Requires(interp): /bin/sh /bin/sh /bin/sh /bin/sh
Requires(rpmlib): rpmlib(CompressedFileNames) <= 3.0.4-1 rpmlib(FileDigests) <= 4.6.0-1 rpmlib(PayloadFilesHavePrefix) <= 4.0-1
Requires(pre): /bin/sh
Requires(post): /bin/sh
Requires(preun): /bin/sh
Requires(postun): /bin/sh
Requires: libc.so.6()(64bit) libc.so.6(GLIBC_2.2.5)(64bit) libgcc_s.so.1()(64bit) libm.so.6()(64bit) libncursesw.so.6()(64bit) libpthread.so.0()(64bit) libpthread.so.0(GLIBC_2.2.5)(64bit) libstdc++.so.6()(64bit) libtinfo.so.6()(64bit) rtld(GNU_HASH)
Checking for unpackaged file(s): /usr/lib/rpm/check-files /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
Wrote: /root/rpmbuild/SRPMS/netware-screensaver-1.21-1.el8.src.rpm
Wrote: /root/rpmbuild/RPMS/x86_64/netware-screensaver-1.21-1.el8.x86_64.rpm
Executing(%clean): /bin/sh -e /var/tmp/rpm-tmp.PbtDr4
+ umask 022
+ cd /root/rpmbuild/BUILD
+ cd netware-screensaver-1.21
+ /usr/bin/rm -rf /root/rpmbuild/BUILDROOT/netware-screensaver-1.21-1.el8.x86_64
+ exit 0
```


## Installing and Deinstalling the RPM Package

## Building as a Debian Package

## Installing and Deinstalling the Debian Package

## Issues / problems / help

If you have any issues, please log them at <https://github.com/jeffmerkey/netware-screensaver-linux/issues>

If you have any suggestions for improvements then pull requests are
welcomed, or raise an issue.
