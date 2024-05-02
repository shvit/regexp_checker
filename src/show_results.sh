#!/usr/bin/env bash

declare -A DAT
declare -i columns=0
declare -i rows=0
declare -i added_row=0
HEADER="name,errors,regex_count,data_chunks,duration,match_count"

split_on_commas_() {
  local IFS=$1
  local WORD_LIST=($2)
  for word in "${WORD_LIST[@]}"; do
    echo "$word"
  done
}

rows=$((rows+1))
for name in $(split_on_commas_ "," "$HEADER"); do
    columns=$((columns+1))
    DAT[$rows,$columns]=$name
done
#for ((i=1;i<=columns;i++)) do echo "column_$i='${DAT[$rows,$i]}'"; done

push_data_to_array() {
    local -A TEMP
    local -i cnt=0
    local values=($1)
    for name in $(split_on_commas_ "=" "$values"); do
        cnt=$((cnt+1))
        TEMP[$cnt]=$name
    done
    #for ((i=1;i<=cnt;i++)) do echo "item$i='${TEMP[$i]}'"; done
    if (( cnt > 1)); then
        if (( added_row == 0 )); then
            rows=$((rows+1))
            added_row=1
        fi
        for ((i=1;i<=columns;i++)) do 
            if [ "${TEMP[1]}" == "${DAT[1,$i]}" ]; then
                DAT[$rows,$i]=${TEMP[2]}
            fi
        done
        #echo "${TEMP[1]}"
        #echo "${TEMP[2]}"
    fi
}

#push_data_to_array "name=stdlib"
#push_data_to_array "errors=0"
#push_data_to_array "checked_regexps=15000"
#push_data_to_array "checked_data_chunks=45"
#push_data_to_array "duration=0.830s"
#push_data_to_array "rule_metric=1"

push_line() {
    local one_line=($1)
    added_row=0
    for one_commas_item in $(split_on_commas_ "," "$one_line"); do
        push_data_to_array "$one_commas_item"
    done
}

while read -r line; 
do
    push_line "$line";
done < "$1"

#echo "---------------------- DEBUG ----------------------"
#for ((r=1;r<=rows;r++)) do for ((i=1;i<=columns;i++)) do echo "cell($r,$i)='${DAT[$r,$i]}'"; done; done

for ((r=1;r<=rows;r++)) do
    one_line=
    for ((i=1;i<=columns;i++)) do
        one_val=${DAT[$r,$i]}
        one_item=$(printf "%-15s" "$one_val")
        #echo "cell($r,$i)='${DAT[$r,$i]}'"
        one_line="$one_line $one_item"
    done
    echo "$one_line"
done

exit 0
