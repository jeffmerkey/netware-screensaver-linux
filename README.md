
# netware-screensaver-linux <!-- omit in toc -->

NetWare SMP style worm screensaver for Linux using ncurses

## Table of Contents <!-- omit in toc -->
- [Overview](#overview)
- [Quick Start Guide](#quick-start-guide)
- [Building the Screensaver from Source](#building-the-screensaver-from-source)
- [Installing and Uninstalling the Screensaver from Source](#installing-and-uninstalling-the-screensaver-from-source)
- [Running the Screensaver](#running-the-screensaver)
- [Installing the Screensaver in the Bash Shell](#installing-the-screensaver-in-the-bash-shell)
- [ssh client returns "unknown terminal type" when remotely logging into the system](ssh-client-returns-"unknown-terminal-type"-when-remotely-logging-into-the-system)
- [Using Libraries](#using-libraries)
- [Building the Screensaver as an RPM Package (Redhat/CentOS/SuSe)](#building-as-an-rpm-package)
- [Building the Screensaver as a Debian Package (Debian/Ubuntu)](#building-as-a-debian-package)
- [Issues / Problems / Help](#issues--problems--help)

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

## Quick Start Guide 

You can download precompiled RPM packages and Debian (DEB) packages from
the [release page](https://github.com/jeffmerkey/netware-screensaver-linux/releases) for 
this project, or you can build the program from the git sources.   

If you want to build the program from scratch with the git sources rather than use the pre-compiled RPMS and DEB packages, then please skip to the section [Building the Screensaver from Source](#building-the-screensaver-from-source) for instructions on how to do this. 
 
Packages are provided in binary and source versions, and can be downloaded and 
installed directly or rebuilt for a different target architecture such as ARM64. Package types are
Red Hat Package Manager (RPM) packages and Debian (DEB) packages for binary installation and 
Source RPM packages (SRPMS) and Debbuild SDEB packages for source code installation.  

RPM and DEB packages for each release include a binary architecture specific package
and a source package which can be downloaded and built/rebuilt and which contains the source code.

For example, the release v1.21 contains the following packages in the release section:

### **RPM Based Packages (RedHat, CentOS, SuSe)**

- [netware-screensaver-1.21-1.el8.src.rpm](https://github.com/jeffmerkey/netware-screensaver-linux/releases/download/v1.21/netware-screensaver-1.21-1.el8.src.rpm) 
- [netware-screensaver-1.21-1.el8.x86_64.rpm](https://github.com/jeffmerkey/netware-screensaver-linux/releases/download/v1.21/netware-screensaver-1.21-1.el8.x86_64.rpm)

### **Debian Based Packages (Debian, Ubuntu)**

- [netware-screensaver-1.21-1.sdeb](https://github.com/jeffmerkey/netware-screensaver-linux/releases/download/v1.21/netware-screensaver-1.21-1.sdeb)
- [netware-screensaver_1.21-1_amd64.deb](https://github.com/jeffmerkey/netware-screensaver-linux/releases/download/v1.21/netware-screensaver_1.21-1_amd64.deb) 

### **Installing Binary Packages**

To install the binary package with the rpm package manager for x86_64:
```sh
rpm -i netware-screensaver-1.21-1.el8.x86_64.rpm
```

To deinstall the RPM binary package:
```sh
rpm -e netware-screensaver
```

To install the binary package with the Debian dpkg package manager for amd64:
```sh
dpkg -i netware-screensaver_1.21-1_amd64.deb 
```

To deinstall the Debian dpkg binary package:
```sh
dpkg -r netware-screensaver
```


### **Installing Source Packages**

To install the source package with the rpm package manager:
```sh
rpm -i netware-screensaver-1.21-1.el8.src.rpm
```
*(Note: rpm installs the source code files in /root/rpmbuild/ as top directory for RedHat and CentOS
platforms.  SuSe platforms install the source code files in /usr/src/packages/)*

To install the source package with the Debbuild package tool:
```sh
debbuild -i netware-screensaver-1.21-1.sdeb
```
*(Note: Debbuild installs the source code files in /root/debbuild/ as top directory)*

For building or rebuilding RPMS or DEB Packages after you have installed the associated source packages on your platform, refer to the following:

- [Building the Screensaver as an RPM Package (Redhat/CentOS/SuSe)](#building-as-an-rpm-package)
- [Building the Screensaver as a Debian Package (Debian/Ubuntu)](#building-as-a-debian-package)

## Building the Screensaver from Source

You must have the ncurses-devel packages installed in order to make
ncurses apps on your system.  you can try "dnf install ncurses\\\*" to get 
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

## Installing and Uninstalling the Screensaver from Source

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

You can install the program on CentOS and RHEL with dnf by typing "dnf install screen" 
if the program is not installed by default.  On Debian based systems you can install
the program with "apt-get install screen".

You should also install at the same time the terminfo and termcap files to support the "screen" 
terminal type for ncurses to avoid getting "unknown terminal type" errors when attempting to 
log into the system over ssh.  

Red Hat based systems can use yum or dnf to install the terminfo files:
```sh
dnf install ncurses-term 
```

Debian based systems can use apt or apt-get to install the terminfo files:
```sh
apt-get install ncurses-term 
```

You will need to create a .screenrc file in the user home directory (i.e. ~/.screenrc)
which contains the following commands:

**sample ~/.screenrc file**
```sh
blankerprg netware-worms
idle 60 blanker
```

Please note the idle command which specifies how many seconds to delay before invoking
the screensaver (in this example it is 60 seconds)

You also need to modify the ~/.bash\_profile default bash configuation file to invoke
screen by default after login.  Add the /bin/screen command to the end of the 
~/.bash\_profile config file:

**sample ~/.bash\_profile**
```sh
# .bash_profile
# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi
# User specific environment and startup programs
PATH=$PATH:$HOME/bin
export PATH
/bin/screen  
```
It must be the last line in the file since following login, the /bin/screen 
program will be executed and run as a transparent bash shell. 

After you have configured these two files, logout and log back into the system for
whichever account you have enabled and the screensaver will auto-activate after 
60 seconds of idle time.  The only disadvantage to using this method to enable 
a terminal based screensaver is you will need to type the "exit" command twice 
in order to logout of the session. 

## ssh client returns "unknown terminal type" when remotely logging into the system

Some older CentOS and Red Hat Enterprise Linux distributions do not properly detect
or parse aliased terminal types such as "screen.xterm-256color" when using ssh to remotely
access a system.  This results in an "unknown terminal type" error being returned after
logging in.  This error can be fixed by changing the ~/.bashrc file for the affected account
and add a check for the term type, then export a terminal type that matches one of the 
supported terminal types in the ncurses-term package.

Add the following to the bottom of the .bashrc file to check the terminal type, then change and
export it if necessary.  
```sh
if [[ "$TERM" == "screen.xterm-256color" ]]; then
   export TERM=screen-256color
fi
```
The "screen.term-256color" terminal type is the most common terminal type seen when this ssh error 
occurs and it is caused by the ncurses library terminfo and termcap files either not being installed 
or improperly configured.

If you encounter this error and are running on version CentOS/RHEL 7 or earlier, then 
edit your .bashrc file to append the terminal type and remap check as shown in the example 
below:

**sample ~/.bashrc**
```sh
# .bashrc
# User specific aliases and functions

alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# add the following 3 lines to the end of the .bashrc file  
if [[ "$TERM" == "screen.xterm-256color" ]]; then          
   export TERM=screen-256color                              
fi                                                         
```

## Using Libraries

You can also call the screensaver as a library function "netware\_screensaver(cpus, speedup)"
from another application with your own timer setup, just be sure to link to the
 "-lnetware-screensaver" .so library in your linker statements or link to the static
library "libnetware-screensaver.a"

## Building as an RPM Package

In order to build the screensaver as an RPM, the program must be compressed into a tar.gz
file and the tar.gz file named to match the versioning information contained in the 
associated .spec file.  

Spec files are special files which contain instructions on how to build a particular package
from a source code archive.  On Red Hat and CentOS systems, RPMS are built in the /root/rpmbuild/
 top directory.  SuSe systems build RPMS in the /usr/src/packages/ as top directory.  These 
"top directories" will contain BUILD, BUILDROOT, SPECS, RPMS, SRPMS, and SOURCES subdirectories.  

The SPECS directory contains the \*.spec files used to build RPMS and SRPMS packages.  The SOURCES subdirectory will contain the soure code archive file referred to in the \*.spec file used to build the 
RPM package.

See the [Quick Start Guide](#quick-start-guide) on instructions for installing the 
source rpm which installs both the .spec file and source archive file (tar.gz) into 
the rpm build top directory (i.e. /root/rpmbuild/).  You should have previously 
installed the src.rpm file before attempting to build the rpm.  You can also 
manually install the .spec file into the \<top directory\>/SPECS/ directory and 
the source code tarball in the \<top directory\/SOURCES/ directory, then attempt 
to build the rpm package.

To build the Screensaver using the rpm tool, change directories (cd) into the /root/rpmbuild/SPECS/ directory (/usr/src/packages/SPECS/ for SuSe) and enter the following command:
```sh
rpmbuild -ba netware-screensaver.spec <enter>
```
The program should output the following if the build is successful and verify that the program
wrote both the RPMS and SRPMS packages:

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

## Building as a Debian Package

In order to build the screensaver as a Debian package, the program must be compressed into a tar.gz
file and the tar.gz file named to match the versioning information contained in the associated .spec file. Spec files are special files which contain instructions on how to build a particular package from a source code archive.  

Debian Packages can be built using a utility called "debbuild" and use a top directory structure which is similar to that used by the RPM tool but using /root/debbuild/ as the "top directory".  These "top directories" will contain BUILD, BUILDROOT, SPECS, DEBS, SDEBS, and SOURCES subdirectories and follows a similar layout that is used for RPM files.  

The SPECS directory contains the \*.spec files used to build DEB and SDEB packages.  The SOURCES subdirectory will contain the soure code archive file referred to in the \*.spec file used to build the 
DEB and SDEB packages.

See the [Quick Start Guide](#quick-start-guide) on instructions for installing the 
source SDEB which installs both the .spec file and source archive file (tar.gz) into 
the debbuild top directory (i.e. /root/debbuild/).  You should have previously installed 
the SDEB file before attempting to build the DEB package.  You can also manually 
install the .spec file into the \<top directory\>/SPECS/ directory and the source 
code tarball in the \<top directory\/SOURCES/ directory, then attempt to build the 
DEB package.

To build the Screensaver using debbuild, change directories (cd) into the /root/debbuild/SPECS/ directory and enter the following command:
```sh
debbuild -vv -ba netware-screensaver.spec <enter>
```
The program should output the following if the build is successful and verify that the program
wrote both the DEB and SDEB packages:

```sh
This is debbuild, version 22.02.1\ndebconfigdir:/usr/lib/debbuild\nsysconfdir:/etc\n
Lua: No Lua module loaded
Executing (%prep): /bin/sh -e /var/tmp/deb-tmp.prep.20277
+ umask 022
+ cd /root/debbuild/BUILD
+ /bin/rm -rf netware-screensaver-1.21
+ /bin/gzip -dc /root/debbuild/SOURCES/netware-screensaver-1.21.tar.gz
+ /bin/tar -xf -
+ STATUS=0
+ '[' 0 -ne 0 ']'
+ cd netware-screensaver-1.21
+ /bin/chmod -Rf a+rX,u+w,go-w .
+ exit 0
Executing (%build): /bin/sh -e /var/tmp/deb-tmp.build.33585
+ umask 022
+ cd /root/debbuild/BUILD
+ cd netware-screensaver-1.21
+ /usr/bin/make
g++ -g -c -O3  -fPIC -Wall libnetware-screensaver.c 
ar r libnetware-screensaver.a libnetware-screensaver.o  
ar: creating libnetware-screensaver.a
ld -shared -lc -o libnetware-screensaver.so libnetware-screensaver.o  
g++ -g -O3 netware-worms.c libnetware-screensaver.a -Wall -o netware-worms -lncursesw -lpthread -ltinfo
+ exit 0
Executing (%install): /bin/sh -e /var/tmp/deb-tmp.install.73931
+ umask 022
+ cd /root/debbuild/BUILD
+ cd netware-screensaver-1.21
+ '[' -n /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64 -a /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64 '!=' / ']'
+ /bin/rm -rf /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64
+ /bin/mkdir -p /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/sbin
+ /bin/mkdir -p /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/bin
+ /bin/mkdir -p /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/include
+ /bin/mkdir -p /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/lib/x86_64-linux-gnu
+ /usr/bin/make DESTDIR=/root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64 NOCHK=1 LIBDIR=/usr/lib/x86_64-linux-gnu INCDIR=/usr/include BINDIR=/usr/bin install
install -m 0755 netware-worms /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/bin
install -m 0755 libnetware-screensaver.so /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/lib/x86_64-linux-gnu
install -m 644 libnetware-screensaver.a /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/lib/x86_64-linux-gnu
install -m 644 netware-worms.h /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/usr/include
+ exit 0
Checking library requirements...
dpkg-query: no path found matching pattern /lib64/libncursesw.so.6
dpkg-query: no path found matching pattern /lib64/libtinfo.so.6
dpkg-query: no path found matching pattern /lib64/libpthread.so.0
dpkg-query: no path found matching pattern /lib64/libstdc++.so.6
dpkg-query: no path found matching pattern /lib64/libm.so.6
dpkg-query: no path found matching pattern /lib64/libgcc_s.so.1
dpkg-query: no path found matching pattern /lib64/libc.so.6
dpkg-query: no path found matching pattern /lib64/libdl.so.2
dpkg-query: no path found matching pattern /lib64/libc.so.6
Executing (package-creation): /bin/sh -e /var/tmp/deb-tmp.pkg.37356 for netware-screensaver
+ umask 022
+ cd /root/debbuild/BUILD
+ /usr/bin/fakeroot -- /usr/bin/dpkg-deb -b /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/main /root/debbuild/DEBS/amd64/netware-screensaver_1.21-1_amd64.deb
dpkg-deb: warning: parsing file '/root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64/main/DEBIAN/control' near line 10 package 'netware-screensaver':
 missing 'Maintainer' field
dpkg-deb: warning: ignoring 1 warning about the control file(s)
dpkg-deb: building package 'netware-screensaver' in '/root/debbuild/DEBS/amd64/netware-screensaver_1.21-1_amd64.deb'.
+ exit 0
Executing (%clean): /bin/sh -e /var/tmp/deb-tmp.clean.95144
+ umask 022
+ cd /root/debbuild/BUILD
+ '[' /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64 '!=' / ']'
+ /bin/rm -rf /root/debbuild/BUILDROOT/netware-screensaver-1.21-1.amd64
+ exit 0
Wrote source package netware-screensaver-1.21-1.sdeb in /root/debbuild/SDEBS.
Wrote binary package netware-screensaver_1.21-1_amd64.deb in /root/debbuild/DEBS/amd64
```

## Issues / problems / help

If you have any issues, please log them at <https://github.com/jeffmerkey/netware-screensaver-linux/issues>

If you have any suggestions for improvements then pull requests are
welcomed, or raise an issue.
