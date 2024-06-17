#define HELP_MSG "Usage: ls [OPTION]... [FILE]...\n" \
                "List information about the FILEs (the current directory by default).\n" \
                "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n" \
                "\n" \
                "Mandatory arguments to long options are mandatory for short options too.\n" \
                "-a, --all                  do not ignore entries starting with .\n" \
                "-l                         use a long listing format\n" \
                "-m                         fill width with a comma separated list of entries\n" \
                "-Q, --quote-name           enclose entry names in double quotes\n" \
                "-r, --reverse              reverse order while sorting\n" \
                "-R, --recursive            list subdirectories recursively\n" \
                "-t                         sort by time, newest first; see --time\n" \
                    "--help     display this help and exit\n" \
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

// TODO -mQ