source "${TEST_DIR}/lib/funcs.bash"

expected_output=$(cat <<EOM
[REGION 0]
  [BLOCK] 0  'Allocation [A]' 184 [USED]
  [BLOCK] 1  'Allocation [B]' 416 [USED]
  [BLOCK] 2  'Allocation [C]' 80 [USED]
  [BLOCK] 3  'Allocation [D]' 72 [USED]
  [BLOCK] 4  '' 3280 [FREE]
[REGION 1]
  [BLOCK] 5  'Allocation [E]' 11063 [USED]
  [BLOCK] 6  'Allocation [F]' 960 [USED]
  [BLOCK] 7  '' 201 [FREE]
[REGION 2]
  [BLOCK] 8  'Allocation [G]' 100063 [USED]
  [BLOCK] 9  '' 2273 [FREE]
EOM
)

test_start "print_memory"

output=$(tests/progs/print-test)

echo "${output}"

# Remove memory addresses from the output.
clean_output=$(sed 's/[^ ]*0x[^ ]*//ig' <<< "${output}")

compare --ignore-all-space \
    <(echo "${expected_output}") <(echo "${clean_output}") || test_end

test_end
