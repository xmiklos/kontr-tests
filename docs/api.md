# API
There are some minor changes to the way you use kontr. These changes are because of the way you now describe a test, but usually **everything is just delegated to the original [kontr](https://github.com/HappyCerberus/kontr)**.

## There are no "$master_test" nor "$unit_test"
Let's you want to stage a file. In the original, you would write something like this:

    $master_test->stage_file("file");
    # or in an Unit test
    $unit_test->stage_file("file");

However, now you **omit the `$master_test->` or `$unit_test->` part**:

```C++
stage_file("file");
// or
this->stage_file("file");
```

But, if you want to use session in the unit test (for example for `has_tag`), you have to specify it (also see the next rule *Everything is a call*):

```C++
session()->has_tag("nanecisto");
```

## Everything is a call

In the original kontr, there where some variables you could use, like:

```Perl
$unit_test->compilation->success;
```

Now, even if you are getting a variable, you **add brackets () to every name**:

```C++
this->compilation()->success();
```

This may be a bit weird at first, especially if the variable is on the left hand side:

```C++
compilation_log_errors() = true;
```

But this is completely legal and will compile (and work) without any problems.

## No methods with variable arguments
This is an implementation limit and may change in the future. But for now, if method took variable count of arguments, it is changed:

* `run` and `run_grind`: `run("stdin", {"arguments", "for", "the", "binary"}); //same for run_grind`
* `add_tag` now only takes one parameter (this may change in the future).
* `add_points` now takes two parameters - name and points: `add_points("points", 1);`

This should all be hinted to you by your editor. (See [Installation](installation.md).)

### Passing arguments to `run*` methods
Methods `run_split` and `run_grind_split` offers a way how to pass variable number of arguments to application being tested. Their second argument is a (perl) string used like command line. For example, calling
```C++
run_grind_split("stdin", "--arg1 -a --long\\ arg\\\\3");
```
is similar to code
```C++
run_grind("stdin", {"--arg1", "-a", "--long arg\\3"});
```
Double quotes are treated as normal character, but it may change in future. Do not use them unless really needed.


## Control flow
In order to make the translation work, I needed to create a new way to control flow of the program. If try to use the C++ keyword, **a warning will be issued**.

### Variables

```C++
// Create a variable with value 10
VAR(mine, 10);
// Now you can do:
mine = "10";
```

You can use these variables in most of the calls (there are some implementation exceptions, `name()` and `register_unit()`). A small note - since this is C++, use only double quotes (") for strings.

### If/While
This is a bit more unnatural, but with correct formatting it is not hard to get used to it:

```C++
IF(mine != 40,
	{ 
		// "IF" part
		log("incorrect"); 
	},
	{
		// "ELSE" part
		log("correct");
	});
//or for one-liners
WHILE(cond,
	{ block; });
```

### Return and others
These are now all macros, which are calls:

```C++
RETURN();
BREAK();
CONTINUE();
```

Note - the brackets are mandatory (otherwise it will not compile).

### File checks
You can check for file existence, emptiness and size with these calls:

```C++
FILE_EXISTS("filename"); //Returns bool
FILE_EMPTY("filename"); //Returns bool
FILE_SIZE("filename"); //Returns int
```

### Direct perl code

Since the tests are still compiled to Perl, it is possible to have a raw Perl code generated:

```C++
PERL("raw code");
```

However, this code *cannot be verified*.

## Session is different
In the original kontr, there was a lot of options how to create a session file. These options have now been limited in order to make the usage easier and less error-prone.

The result is one macro declaration in the following format (`(number)` is for explanation, it is not part of the code):

```c++
SESSION("scripts/" (1), "files/" (2), {master_basic} (3), {master_full} (4), false (5), false (6) )
//Or for your own post-test:
SESSION("scripts/" (1), "files/" (2), {master_basic} (3), {master_full} (4), { add_summary("post_test"); } (7) )
```

Notice that there is **no semicolon** after this declaration.

### Paths to scripts and files
First of all you have to provide directories, to which **the script files will be generated** `(1)` and where the files during the test are used `(2)` - this is for verification only, no files will be created nor modified there.

### Pre-test
You now only provide two sets of master tests:

* Those which will run during both "basic" and "full" tests `(3)`
* Those which will run only during "full" tests `(4)`

*Czech translation: "basic" je nanečisto, "full" je naostro.*

So lets say you have two master tests, one for basic and for full (as in the example above). The code generated will look like this (assuming the names are set in the master tests):

```Perl
sub pre_test {
	$session->register_master("master_basic.pl");
	if ($session->run_type eq 'teacher') {
		$session->register_master("master_full.pl");
	}
}
```

### Post-test
There are now to options to create a post test. First one is **generic** and is prefered for the usual tests, and the other is **custom**.

The **generic** is as the name states - it should be used if you are creating a normal test. There are options to include results for Valgrind `(5)` and bonus `(6)`. You can see the full source code for it [here](https://github.com/xbrukner/kontr-tests/blob/master/Generator/Session.h#L93). (*Note - the text is in Czech.*)

But if you have completely different test (for example with only full tests), you can provide **your own post test**. That is the option `(7)` and it can be seen in use in the [Hello World!](https://github.com/xbrukner/kontr-tests/blob/master/Hello.cpp) example.

**Important:** the prefered way is the generic post-test (it is easier to check). So if you can use that, use it.

You finished reading here? Good! Start working on your tests now :-)
