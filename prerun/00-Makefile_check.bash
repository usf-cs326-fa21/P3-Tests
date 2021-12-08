if [[ ! -f "${TEST_DIR}/../liballocator.so" \
        || -L "${TEST_DIR}/../liballocator.so" ]]; then
    echo "Test library was not built correctly. This probably means you need "
    echo -n "an updated makefile. Can I fix this for you automatically? [y/N] "
    read response
    if [[ "${response}" == "y" || "${response}" == "Y" ]]; then
        cp -v "${TEST_DIR}/New-Makefile" "${TEST_DIR}/../Makefile"
        echo "Please do a 'make clean' and retry your tests now."
        exit
    fi
fi
