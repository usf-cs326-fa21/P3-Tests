source "${TEST_DIR}/lib/funcs.bash"

expected_order=$(cat <<EOM
Test Allocation: 0
Test Allocation: 1
Test Allocation: 2
Test Allocation: 6
Test Allocation: 4
Test Allocation: 5
EOM
)

test_start "Basic First Fit"

# !!!!!!!!!!!!!!!!
# Please remember that the FSM algorithms use the **free list** to find a free
# block so the 'first' available block is not necessarily what would print
# first.
#
# The head of the free list is the most recently-freed block.
# !!!!!!!!!!!!!!!!

output=$( \
    ALLOCATOR_ALGORITHM=first_fit \
    tests/progs/allocations-1)

echo "${output}"

# Get the block ordering from the output. We ignore unnamed allocations.
block_order=$(grep 'Test Allocation:' <<< "${output}" \
    | sed "s/.*'Test Allocation: \([0-9]*\)'.*/Test Allocation: \1/g")

# Get the number of regions:
regions=$(grep '\[REGION [0-9]*\]' <<< "${output}" | wc -l)
if [[ ${regions} -gt 1 ]]; then
    # There were too many regions in the output!
    test_end 1
fi

compare <(echo "${expected_order}") <(echo "${block_order}") || test_end

test_end
