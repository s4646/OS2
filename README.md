324205160 212499347  
Did bonus

*** Don't forget to compile the code using `make` command ***

### mytee:
**Usage:**
```ruby
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$ cat test.txt|./mytee test2.txt 
blablabla
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$ ./mytee test.txt 
this
this
is
is
test
test
```

### mync:
**Usage:**
```ruby
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$ ./mync -l 8080
test
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$ ./mync 127.0.0.1 8080
test
```

### myshell:
**Usage:**  
***when using { } operators, please DONT use spaces between each operator!***
```ruby
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$ ./myshell 
/home/ubuntu/Documents/OS2$ DIR
.vscode
mync.c
test2.txt
myshell
dir.h
dir.o
.
myshell.c
mytee.c
execute.o
test.txt
..
execute.h
copy.o
README.md
.gitignore
.git
Makefile
copy.c
myshell.o
execute.c
copy.h
dir.c
/home/ubuntu/Documents/OS2$ COPY test.txt test2.txt
/home/ubuntu/Documents/OS2$ ls -l|grep d|sort|tee test.txt
-rw-rw-r-- 1 ubuntu ubuntu   152 Dec  2 01:43 dir.h
-rw-rw-r-- 1 ubuntu ubuntu   461 Dec  2 05:55 dir.c
-rw-rw-r-- 1 ubuntu ubuntu   492 Dec  2 06:26 README.md
-rw-rw-r-- 1 ubuntu ubuntu  7440 Dec  2 06:26 dir.o
/home/ubuntu/Documents/OS2$ ls}127.0.0.1:8080       
Makefile
README.md
copy.c
copy.h
copy.o
dir.c
dir.h
dir.o
execute.c
execute.h
execute.o
mync.c
myshell
myshell.c
myshell.o
mytee.c
test.txt
test2.txt

/home/ubuntu/Documents/OS2$ exit
ubuntu@ubuntu-ASUS-TUF:~/Documents/OS2$
```