
# ft_ls

The ft_ls project aims to reproduce a minimalist version of the ls command.

## Installation

To install, clone this project then ```make```. Done ! You can enter ```./ft_ls``` to see if it works.
## Man
You can also see this man by typing ```ft_ls --help```

```
Usage: ls [OPTION]... [FILE]...
List information about the FILEs (the current directory by default).
Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.

Mandatory arguments to long options are mandatory for short options too.
	-a, --all                  do not ignore entries starting with .
	-A, --almost-all           do not list implied . and ..
	-d, --directory            list directories themselves, not their contents
	-g                         like -l, but do not list owner
	-l                         use a long listing format
	-m                         fill width with a comma separated list of entries
	-Q, --quote-name           enclose entry names in double quotes
	-r, --reverse              reverse order while sorting
	-R, --recursive            list subdirectories recursively
	-t                         sort by time, newest first
		--help     display this help and exit

Use the define in include/header.c to modify the colors.

Exit status:
0  if OK,
1  if minor problems (e.g., cannot access subdirectory),
2  if serious trouble (e.g., cannot access command-line argument).

GNU coreutils online help: <https://www.gnu.org/software/coreutils/>
Full documentation <https://www.gnu.org/software/coreutils/ls>
or available locally via: info '(coreutils) ls invocation'
DinoMalin's github <https://www.github.com/DinoMalin/>

```
## Implementation

I choose to implement certain things and not implement other things on this project.

First, I implemented all the mandatory parts, which is :
- Handling of the options ```-l```, ```-R```, ```-a```, ```-r``` and ```-t```.
- Handling of the multi-columns display for ```-l```.

I also did some of the proposed bonuses, like :
- Management of views in color (A bit basic but functionnal).
- Management of options ```-g```, ```-d``` (I didn't handled -u and -f because they are useless).
- Optimization of the code (Using a quicksort, itering only one time through the file system, ...). It is far from being as fast as the original ls, but it's as good as I can think of doing it.

I even did non-proposed bonuses :
- Creation of a man page (```./ft_ls --help```)
- Management of options ```-A```, ```-m``` and ```-Q```.
- Management of verbose options (or long options), like ```--recursive```.
- Management of errors the same way as ls, like when you do ```ls --re``` for example.

And, unlike ls, I do not have any still reachable when exiting :)
