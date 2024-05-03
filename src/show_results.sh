#!/usr/bin/env bash

declare -A DAT
declare -i columns=0
declare -i rows=0
declare -i added_row=0 # bool flag
HEADER="name,errors,regex_count,data_chunks,duration,match_count"

# @brief Split comma separated values to output values list
#
# Sample:
#   Input data: alpha,bravo,charlie
#   Output data: alpha
#                bravo
#                charlie
# @param $1 Comma separeted values
# @return List values
split_on_commas() {
  local IFS=$1
  local WORD_LIST=($2)
  for word in "${WORD_LIST[@]}"; do
    echo "$word"
  done
}

# @brief Split data from key=value and push to result array
#
# Sample:
#   Input data: key1=value1=wrong1
#   Processing:
#     - find in array header[i]==key1 and remember column i
#     - if not found - return
#     - if added_row==0 then add new row and set added_row=1
#     - write to last row and rememberd column value1
# @param $1 Input data
push_data_to_array() {
    local -A TEMP
    local -i cnt=0
    local values=($1)
    for name in $(split_on_commas "=" "$values"); do
        cnt=$((cnt+1))
        TEMP[$cnt]=$name
    done
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
    fi
}

# @brief Split data from many key=value comma separated and push to result array
#
# Sample:
#   Input data: key1=value1=wrong1,key2=value2,key3=value3=wrong3
#   Processing:
#     - set added_row=0 (for add new row in result array)
#     - split comma separeted input line
#     - try to push key=value via push_data_to_array() 
# @param $1 Line from source file with comma separated key=value
push_line() {
    local one_line=($1)
    added_row=0
    for one_commas_item in $(split_on_commas "," "$one_line"); do
        push_data_to_array "$one_commas_item"
    done
}

##### Function main() #####

# Fill headers in result array
rows=$((rows+1))
for name in $(split_on_commas "," "$HEADER"); do
    columns=$((columns+1))
    DAT[$rows,$columns]=$name
done

# Read file with data results
while read -r line; 
do
    push_line "$line";
done < "$1"

# Show output from result array
for ((r=1;r<=rows;r++)) do
    one_line=
    for ((i=1;i<=columns;i++)) do
        one_val=${DAT[$r,$i]}
        one_item=$(printf "%-15s" "$one_val")
        one_line="$one_line $one_item"
    done
    echo "$one_line"
done

exit 0
