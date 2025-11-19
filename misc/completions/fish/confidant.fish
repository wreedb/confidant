function _confidant_cmd_0
    set 1 $argv[1]
    __fish_complete_path "$1"
end

function _confidant_cmd_1
    set 1 $argv[1]
    __fish_complete_directories "$1"
end

function __complgen_match
    set prefix $argv[1]

    set candidates
    set descriptions
    while read c
        set a (string split --max 1 -- "	" $c)
        set --append candidates $a[1]
        if set --query a[2]
            set --append descriptions $a[2]
        else
            set --append descriptions ""
        end
    end

    if test -z "$candidates"
        return 1
    end

    set escaped_prefix (string escape --style=regex -- $prefix)
    set regex "^$escaped_prefix.*"

    set matches_case_sensitive
    set descriptions_case_sensitive
    for i in (seq 1 (count $candidates))
        if string match --regex --quiet --entire -- $regex $candidates[$i]
            set --append matches_case_sensitive $candidates[$i]
            set --append descriptions_case_sensitive $descriptions[$i]
        end
    end

    if set --query matches_case_sensitive[1]
        for i in (seq 1 (count $matches_case_sensitive))
            printf '%s	%s\n' $matches_case_sensitive[$i] $descriptions_case_sensitive[$i]
        end
        return 0
    end

    set matches_case_insensitive
    set descriptions_case_insensitive
    for i in (seq 1 (count $candidates))
        if string match --regex --quiet --ignore-case --entire -- $regex $candidates[$i]
            set --append matches_case_insensitive $candidates[$i]
            set --append descriptions_case_insensitive $descriptions[$i]
        end
    end

    if set --query matches_case_insensitive[1]
        for i in (seq 1 (count $matches_case_insensitive))
            printf '%s	%s\n' $matches_case_insensitive[$i] $descriptions_case_insensitive[$i]
        end
        return 0
    end

    return 1
end


function _confidant
    set COMP_LINE (commandline --cut-at-cursor)

    set COMP_WORDS
    echo $COMP_LINE | read --tokenize --array COMP_WORDS
    if string match --quiet --regex '.*\s$' $COMP_LINE
        set COMP_CWORD (math (count $COMP_WORDS) + 1)
    else
        set COMP_CWORD (count $COMP_WORDS)
    end

    set literals -h --help -V --version -q --quiet init -f --file ... config get -f --file dump -f --file link -f --file -d --dry-run help init config dump get link usage version

    set descrs
    set descrs[1] "help information"
    set descrs[2] "version information"
    set descrs[3] "suppress most messages"
    set descrs[4] "initialize a confidant repository"
    set descrs[5] "specify a file path"
    set descrs[6] "view configuration"
    set descrs[7] "find a value by name"
    set descrs[8] "display current configuration"
    set descrs[9] "apply symlinks"
    set descrs[10] "simulate actions"
    set descrs[11] "display help for subcommands"
    set descrs[12] "brief usage info"
    set descrs[13] "version info"
    set descr_literal_ids 1 2 3 4 5 6 7 8 9 11 12 15 18 19 20 21 22 23 29 30
    set descr_ids 1 1 2 2 3 3 4 5 5 6 7 8 9 5 5 10 10 11 12 13
    set regexes 
    set literal_transitions_inputs
    set nontail_transitions
    set literal_transitions_inputs[1] "1 2 3 4 5 6 7 11 18 23 29 30"
    set literal_transitions_tos[1] "2 2 2 2 2 2 3 4 5 6 2 2"
    set literal_transitions_inputs[3] "19 20"
    set literal_transitions_tos[3] "16 17"
    set literal_transitions_inputs[4] "12 15"
    set literal_transitions_tos[4] "7 8"
    set literal_transitions_inputs[5] "19 20 21 22"
    set literal_transitions_tos[5] "9 10 2 2"
    set literal_transitions_inputs[6] "24 25 28"
    set literal_transitions_tos[6] "2 21 2"
    set literal_transitions_inputs[7] "16 17"
    set literal_transitions_tos[7] "13 14"
    set literal_transitions_inputs[8] "16 17"
    set literal_transitions_tos[8] "11 12"
    set literal_transitions_inputs[18] 10
    set literal_transitions_tos[18] 19
    set literal_transitions_inputs[21] "26 27"
    set literal_transitions_tos[21] "2 2"

    set match_anything_transitions_from 10 11 12 7 14 13 15 17 16 19 20 9
    set match_anything_transitions_to 2 2 2 2 15 15 2 18 18 20 20 2

    set state 1
    set word_index 2
    while test $word_index -lt $COMP_CWORD
        set -- word $COMP_WORDS[$word_index]

        if set --query literal_transitions_inputs[$state] && test -n $literal_transitions_inputs[$state]
            set inputs (string split ' ' $literal_transitions_inputs[$state])
            set tos (string split ' ' $literal_transitions_tos[$state])

            set literal_id (contains --index -- "$word" $literals)
            if test -n "$literal_id"
                set index (contains --index -- "$literal_id" $inputs)
                set state $tos[$index]
                set word_index (math $word_index + 1)
                continue
            end
        end

        if set --query match_anything_transitions_from[$state] && test -n $match_anything_transitions_from[$state]
            set index (contains --index -- "$state" $match_anything_transitions_from)
            set state $match_anything_transitions_to[$index]
            set word_index (math $word_index + 1)
            continue
        end

        return 1
    end

    set literal_froms_level_0 3 1 4 21 6 5 8 7 18
    set literal_inputs_level_0 "19 20|1 2 3 4 5 6 7 11 18 23 29 30|12 15|26 27|24 25 28|19 20 21 22|16 17|16 17|10"
    set nontail_command_froms_level_0 
    set nontail_commands_level_0 
    set nontail_regexes_level_0 
    set command_froms_level_0 10 9 14 11 12 17 13 16 19 20
    set commands_level_0 "0" "0" "0" "0" "0" "0" "0" "0" "1" "1"

    for fallback_level in (seq 0 0)
        set candidates
        set froms_name literal_froms_level_$fallback_level
        set froms $$froms_name
        set index (contains --index -- "$state" $froms)
        if test -n "$index"
            set level_inputs_name literal_inputs_level_$fallback_level
            set input_assoc_values (string split '|' $$level_inputs_name)
            set state_inputs (string split ' ' $input_assoc_values[$index])
            for literal_id in $state_inputs
                set descr_index (contains --index -- "$literal_id" $descr_literal_ids)
                if test -n "$descr_index"
                    set --append candidates (printf '%s\t%s\n' $literals[$literal_id] $descrs[$descr_ids[$descr_index]])
                else
                    set --append candidates (printf '%s\n' $literals[$literal_id])
                end
            end
        end

        set commands_name command_froms_level_$fallback_level
        set commands $$commands_name
        set index (contains --index -- "$state" $commands)
        if test -n "$index"
            set commands_name commands_level_$fallback_level
            set commands (string split ' ' $$commands_name)
            set function_id $commands[$index]
            set function_name _confidant_cmd_$function_id
            set --append candidates ($function_name "$COMP_WORDS[$COMP_CWORD]")
        end
        printf '%s\n' $candidates | __complgen_match $COMP_WORDS[$word_index] && return 0
    end
end

complete --erase confidant
complete --command confidant --no-files --arguments "(_confidant)"
