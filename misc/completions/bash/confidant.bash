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

    declare -a literals=(-v --verbose -h --help --usage -V --version version usage init -f --file -v --verbose config get -s --section -h --help -v --verbose dump -g --global -h --help -f --file -v --verbose -h --help -v --verbose link -f --file -n --name -d --dry-run -h --help -v --verbose link-from -f --file -d --dry-run -h --help -v --verbose help init config dump get link link-from)
    declare -a regexes=()
    declare -A literal_transitions=()
    declare -A nontail_transitions=()
    literal_transitions[0]="([0]=1 [1]=1 [2]=1 [3]=1 [4]=1 [5]=1 [6]=1 [7]=1 [8]=1 [9]=2 [14]=3 [35]=4 [46]=5 [55]=6)"
    literal_transitions[2]="([47]=7 [48]=8 [33]=9 [34]=9)"
    literal_transitions[3]="([15]=16 [22]=17 [51]=1 [52]=1 [33]=1 [34]=1)"
    literal_transitions[4]="([47]=10 [48]=11 [38]=12 [39]=13 [49]=14 [50]=14 [51]=14 [52]=14 [44]=14 [45]=14)"
    literal_transitions[5]="([47]=25 [48]=26 [49]=27 [50]=27 [51]=27 [52]=27 [53]=27 [54]=27)"
    literal_transitions[6]="([56]=28 [57]=29 [60]=28 [61]=28)"
    literal_transitions[9]="([47]=7 [48]=8 [33]=9 [34]=9)"
    literal_transitions[14]="([47]=10 [48]=11 [38]=12 [39]=13 [49]=14 [50]=14 [51]=14 [52]=14 [44]=14 [45]=14)"
    literal_transitions[16]="([16]=21 [17]=22 [51]=23 [52]=23 [33]=23 [34]=23)"
    literal_transitions[17]="([23]=18 [24]=18 [51]=18 [52]=18 [27]=19 [28]=20 [33]=18 [34]=18)"
    literal_transitions[18]="([23]=18 [24]=18 [51]=18 [52]=18 [27]=19 [28]=20 [33]=18 [34]=18)"
    literal_transitions[23]="([16]=21 [17]=22 [51]=23 [52]=23 [33]=23 [34]=23)"
    literal_transitions[27]="([47]=25 [48]=26 [49]=27 [50]=27 [51]=27 [52]=27 [53]=27 [54]=27)"
    literal_transitions[28]="([56]=28 [57]=29 [60]=28 [61]=28)"
    literal_transitions[29]="([58]=28 [59]=28)"
    declare -A match_anything_transitions=([10]=14 [9]=15 [12]=14 [19]=18 [20]=18 [13]=14 [11]=14 [15]=15 [22]=23 [23]=24 [21]=23 [24]=24 [8]=9 [7]=9 [25]=27 [26]=27)
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

    declare -A literal_transitions_level_0=([3]="15 22 51 52 33 34" [23]="16 17 51 52 33 34" [5]="47 48 49 50 51 52 53 54" [29]="58 59" [17]="23 24 51 52 27 28 33 34" [2]="47 48 33 34" [0]="0 1 2 3 4 5 6 7 8 9 14 35 46 55" [9]="47 48 33 34" [14]="47 48 38 39 49 50 51 52 44 45" [27]="47 48 49 50 51 52 53 54" [16]="16 17 51 52 33 34" [28]="56 57 60 61" [4]="47 48 38 39 49 50 51 52 44 45" [6]="56 57 60 61" [18]="23 24 51 52 27 28 33 34")
    declare -A subword_transitions_level_0=()
    declare -A commands_level_0=([9]="1" [8]="0" [20]="0" [10]="0" [11]="0" [19]="0" [26]="0" [15]="1" [7]="0" [25]="0")
    declare -A nontail_commands_level_0=()
    declare -A nontail_regexes_level_0=()

    local -a candidates=()
    local -a matches=()
    local ignore_case=$(bind -v | grep completion-ignore-case | cut -d' ' -f3)
    local max_fallback_level=0
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
