#pragma once

#define HELP_MSG "Usage: ls [OPTION]... [FILE]...\n" \
                "List information about the FILEs (the current directory by default).\n" \
                "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n" \
                "\n" \
                "Mandatory arguments to long options are mandatory for short options too.\n" \
                "\t-a, --all                  do not ignore entries starting with .\n" \
                "\t-A, --almost-all           do not list implied . and ..\n" \
                "\t-d, --directory            list directories themselves, not their contents\n" \
                "\t-g                         like -l, but do not list owner\n" \
                "\t-l                         use a long listing format\n" \
                "\t-m                         fill width with a comma separated list of entries\n" \
                "\t-Q, --quote-name           enclose entry names in double quotes\n" \
                "\t-r, --reverse              reverse order while sorting\n" \
                "\t-R, --recursive            list subdirectories recursively\n" \
                "\t-t                         sort by time, newest first\n" \
                "\t\t--help     display this help and exit\n" \
                "\n" \
                "Use the define in include/header.c to modify the colors.\n" \
                "\n" \
                "Exit status:\n" \
                "0  if OK,\n" \
                "1  if minor problems (e.g., cannot access subdirectory),\n" \
                "2  if serious trouble (e.g., cannot access command-line argument).\n" \
                "\n" \
                "GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n" \
                "Full documentation <https://www.gnu.org/software/coreutils/ls>\n" \
                "or available locally via: info '(coreutils) ls invocation'\n" \
                "DinoMalin's github <https://www.github.com/DinoMalin/>\n"
