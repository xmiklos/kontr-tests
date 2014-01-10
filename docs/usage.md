# Usage

How to use this project? It is very simple! (For an inspiration, a [Hello world!](https://github.com/xbrukner/kontr-tests/blob/master/Hello.cpp) example is at your disposal.)

1. Create a new CPP file. *For example my-test.cpp.*
If you have the full instalation and you create the file inside kontr-tests directory, it can be automatically compiled for you - just run ```cmake .``` and after compilation (```make```) you will find your binary in the bin/ folder (*in our example, bin/my-test*).

2. ```#include "kontr.h"```
3. Create a unit test:

```c++
UNIT_TEST(unit_name) {
	name("filename.pl");
	// Put the rest here
}
```

4. Create a master test:

```c++
MASTER_TEST(master_name) {
	name("master_name.pl");
	// Put the rest here
	//like: register_unit(unit_name);
}
```

5. Describe a [Session](TODO):

```c++
SESSION("scripts/", "files/", {master_name}, {}, false, false)
```

6. "Include" the main:
```c++
MAIN
```

**The order** is important, as you are actually creating a class each time you use one of these macros. Therefore you must describe an unit test before you register it in a master test, a master test before session and main at the end (because it uses your session).

Now, when you compile your test and run it, it will **generate the script files for kontr in the "scripts/" directory.** You may also see some notices, warnings or errors in order to *improve* your tests.

*Notice: So far you will mostly see errors, which results in imidiate abortion. When a verificator will be created, more reports are going to be generated.*
