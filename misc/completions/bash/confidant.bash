if [[ $BASH_VERSINFO -lt 4 ]]; then
    echo "This completion script requires bash 4.0 or newer (current is $BASH_VERSION)"
    exit 1
fi

__complgen_match () {
    [[ $# -lt 2 ]] && return 1
    local ignore_case=$1
    local prefix=$2
    [[ -z $prefix ]] && cat
    if [[ $ignore_case = on ]]; then
        prefix=${prefix,,}
        prefix=$(printf '%q' "$prefix")
        while read line; do
            [[ ${line,,} = ${prefix}* ]] && echo $line
        done
    else
        prefix=$(printf '%q' "$prefix")
        while read line; do
            [[ $line = ${prefix}* ]] && echo $line
        done
    fi
}

_confidant_cmd_0 () {
    compgen -A file "$1"
}

_confidant_cmd_1 () {
    compgen -A directory "$1"
}

_confidant () {
    if [[ $(type -t _get_comp_words_by_ref) != function ]]; then
        echo _get_comp_words_by_ref: function not defined.  Make sure the bash-completion system package is installed
        return 1
    fi

    local words cword
    _get_comp_words_by_ref -n "$COMP_WORDBREAKS" words cword

    declare -a literals=(help init link config dump get config get -g --global create-directories color log-level repository repository.url links templates -? -h --help -v --verbose -q --quiet dump -f --file -g --global -j --json link -t --tags -f --file -d --dry-run init -d --dry-run usage version -V --version -u --usage -? -h --help -v --verbose -q --quiet)
    declare -a regexes=()
    declare -A literal_transitions=()
    declare -A nontail_transitions=()
    literal_transitions[0]="([0]=1 [6]=2 [31]=3 [38]=4 [41]=5 [42]=5 [43]=5 [44]=5 [45]=5 [46]=5 [47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5)"
    literal_transitions[1]="([38]=5 [31]=5 [6]=17)"
    literal_transitions[2]="([7]=10 [47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5 [24]=11)"
    literal_transitions[3]="([47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5 [32]=6 [33]=7 [34]=8 [35]=9 [39]=3 [40]=3)"
    literal_transitions[4]="([47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5 [39]=18 [40]=18)"
    literal_transitions[10]="([27]=16 [28]=16 [13]=15 [14]=15 [15]=15 [16]=15 [47]=12 [48]=12 [49]=12 [50]=12 [51]=12 [52]=12 [53]=12)"
    literal_transitions[11]="([47]=12 [48]=12 [49]=12 [50]=12 [51]=12 [52]=12 [53]=12 [34]=13 [35]=14 [27]=15 [28]=15 [29]=15 [30]=15)"
    literal_transitions[12]="([47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5)"
    literal_transitions[15]="([47]=12 [48]=12 [49]=12 [50]=12 [51]=12 [52]=12 [53]=12)"
    literal_transitions[16]="([10]=15 [11]=15 [12]=15)"
    literal_transitions[17]="([24]=5 [7]=5)"
    literal_transitions[18]="([47]=5 [48]=5 [49]=5 [50]=5 [51]=5 [52]=5 [53]=5)"
    declare -A match_anything_transitions=([6]=3 [7]=3 [8]=3 [9]=3 [4]=12 [13]=15 [14]=15 [18]=12)
    declare -A subword_transitions

    local state=0
    local word_index=1
    while [[ $word_index -lt $cword ]]; do
        local word=${words[$word_index]}

        if [[ -v "literal_transitions[$state]" ]]; then
            declare -A state_transitions
            eval "state_transitions=${literal_transitions[$state]}"

            local word_matched=0
            for literal_id in $(seq 0 $((${#literals[@]} - 1))); do
                if [[ ${literals[$literal_id]} = "$word" ]]; then
                    if [[ -v "state_transitions[$literal_id]" ]]; then
                        state=${state_transitions[$literal_id]}
                        word_index=$((word_index + 1))
                        word_matched=1
                        break
                    fi
                fi
            done
            if [[ $word_matched -ne 0 ]]; then
                continue
            fi
        fi

        if [[ -v "match_anything_transitions[$state]" ]]; then
            state=${match_anything_transitions[$state]}
            word_index=$((word_index + 1))
            continue
        fi

        return 1
    done

    declare -A literal_transitions_level_0=([4]="47 48 49 50 51 52 53 39 40" [2]="7 47 48 49 50 51 52 53 24" [0]="0 6 31 38 41 42" [3]="47 48 49 50 51 52 53 32 33 34 35 39 40" [18]="47 48 49 50 51 52 53" [10]="27 28 13 14 15 16 47 48 49 50 51 52 53" [11]="47 48 49 50 51 52 53 34 35 27 28 29 30" [16]="10 11 12" [12]="47 48 49 50 51 52 53" [15]="47 48 49 50 51 52 53" [1]="38 31 6" [17]="24 7")
    declare -A literal_transitions_level_1=([0]="43 44 45 46 47 48 49 50 51 52 53")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=([18]="1" [9]="0" [4]="1" [8]="0" [13]="0" [14]="0")
    declare -A commands_level_1=()
    declare -A nontail_commands_level_0=()
    declare -A nontail_regexes_level_0=()
    declare -A nontail_commands_level_1=()
    declare -A nontail_regexes_level_1=()

    local -a candidates=()
    local -a matches=()
    local ignore_case=$(bind -v | grep completion-ignore-case | cut -d' ' -f3)
    local max_fallback_level=1
    local prefix="${words[$cword]}"
    for (( fallback_level=0; fallback_level <= max_fallback_level; fallback_level++ )) {
        eval "declare literal_transitions_name=literal_transitions_level_${fallback_level}"
        eval "declare -a transitions=(\${$literal_transitions_name[$state]})"
        for literal_id in "${transitions[@]}"; do
            local literal="${literals[$literal_id]}"
            candidates+=("$literal ")
        done
        if [[ ${#candidates[@]} -gt 0 ]]; then
            readarray -t matches < <(printf "%s\n" "${candidates[@]}" | __complgen_match "$ignore_case" "$prefix")
        fi

        eval "declare subword_transitions_name=subword_transitions_level_${fallback_level}"
        eval "declare -a transitions=(\${$subword_transitions_name[$state]})"
        for subword_id in "${transitions[@]}"; do
            readarray -t -O "${#matches[@]}" matches < <(_confidant_subword_$subword_id complete "$prefix")
        done

        eval "declare commands_name=commands_level_${fallback_level}"
        eval "declare -a transitions=(\${$commands_name[$state]})"
        for command_id in "${transitions[@]}"; do
            readarray -t candidates < <(_confidant_cmd_$command_id "$prefix" | cut -f1)
            if [[ ${#candidates[@]} -gt 0 ]]; then
                readarray -t -O "${#matches[@]}" matches < <(printf "%s\n" "${candidates[@]}" | __complgen_match "$ignore_case" "$prefix")
            fi
        done

        eval "declare commands_name=nontail_commands_level_${fallback_level}"
        eval "declare -a command_transitions=(\${$commands_name[$state]})"
        eval "declare regexes_name=nontail_regexes_level_${fallback_level}"
        eval "declare -a regexes_transitions=(\${$regexes_name[$state]})"
        for (( i = 0; i < ${#command_transitions[@]}; i++ )); do
            local command_id=${command_transitions[$i]}
            local regex_id=${regexes_transitions[$i]}
            local regex="^(${regexes[$regex_id]}).*"
            readarray -t output < <(_confidant_cmd_$command_id "$prefix" | cut -f1)
            declare -a candidates=()
            for line in ${output[@]}; do
                if [[ ${line} =~ $regex && -n ${BASH_REMATCH[1]} ]]; then
                    match="${BASH_REMATCH[1]}"
                    candidates+=("$match")
                fi
            done
            if [[ ${#candidates[@]} -gt 0 ]]; then
                readarray -t -O "${#matches[@]}" matches < <(printf "%s\n" "${candidates[@]}" | __complgen_match "$ignore_case" "$prefix")
            fi
        done

        if [[ ${#matches[@]} -gt 0 ]]; then
            local shortest_suffix="$prefix"
            for ((i=0; i < ${#COMP_WORDBREAKS}; i++)); do
                local char="${COMP_WORDBREAKS:$i:1}"
                local candidate=${prefix##*$char}
                if [[ ${#candidate} -lt ${#shortest_suffix} ]]; then
                    shortest_suffix=$candidate
                fi
            done
            local superfluous_prefix=""
            if [[ "$shortest_suffix" != "$prefix" ]]; then
                local superfluous_prefix=${prefix%$shortest_suffix}
            fi
            COMPREPLY=("${matches[@]#$superfluous_prefix}")
            break
        fi
    }

    return 0
}

complete -o nospace -F _confidant confidant
