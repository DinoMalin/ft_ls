
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
	    --color                colorize the output
	-d, --directory            list directories themselves, not their contents
	-f                         do not sort, enable -aU, disable -ls --color
	-g                         like -l, but do not list owner
	-l                         use a long listing format
	-m                         fill width with a comma separated list of entries
	-N, --literal              print entry names without quoting
	-Q, --quote-name           enclose entry names in double quotes
	-r, --reverse              reverse order while sorting
	-R, --recursive            list subdirectories recursively
	-t                         sort by time, newest first
	-U                         do not sort; list entries in directory order
	    --help     display this help and exit

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
- Management of the columns without ```-l```. 
- Management of views in color.
- Management of proposed options ```-g```, ```-d```, ```-f```.
- Optimization : My implementation is nearly as fast as the basic ls.

I even did non-proposed bonuses :
- Creation of a man page (```./ft_ls --help```)
- Management of options ```-A```, ```--color```, ```-m```, ```-N```, ```-Q``` and ```-U```.
- Management of verbose options (or long options), like ```--recursive``` or just ```--```.
- Management of errors the same way as ls, like when you do ```ls --re``` for example.

And, unlike ls, I do not have any still reachable when exiting :)
