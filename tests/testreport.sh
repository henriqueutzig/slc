#!/usr/bin/bash

INPUTS=$(ls tests/e2 | grep entrada_)
mkdir -p output/
PASSED=0
FAILED=0
FAILED_TESTS=()

for file in $INPUTS; do
  OUTPUT_FILE="output/output_$file.txt"
  ERROR_FILE="output/error_$file.txt"

  # Clear the output and error files before writing to them
  > "$OUTPUT_FILE"
  > "$ERROR_FILE"

  echo "Running test for: $file..."

  # Run the test and redirect output
  if ! cat tests/e2/$file | ./etapa2 > "$OUTPUT_FILE" 2> "$ERROR_FILE"; then
    echo -e "\n\033[1;31m[ERROR]\033[0m An error occurred while processing $file with etapa2." >> "$OUTPUT_FILE"
    cat "$ERROR_FILE" >> "$OUTPUT_FILE"

    # Extract the line number from the error message (assuming format: "Error at line X")
    ERROR_LINE=$(grep -o 'line [0-9]*' "$ERROR_FILE" | awk '{print $2}')
    if [ -n "$ERROR_LINE" ]; then
      ERROR_CONTENT=$(sed -n "${ERROR_LINE}p" "tests/e2/$file")
      echo -e "\033[1;31mError Details:\033[0m\nFile: \033[1m$file\033[0m\nLine: \033[1m$ERROR_LINE\033[0m\n> \033[1;33m$ERROR_CONTENT\033[0m\n"  # Formatted error output from test file
    else
      echo -e "\033[1;31mError line number not found in the error message.\033[0m\n"
    fi

    FAILED=$((FAILED + 1))
    FAILED_TESTS+=("$file")
    rm "$ERROR_FILE"  # Remove temporary error file
  else
    echo "OK" > "$OUTPUT_FILE"
    echo -e "\033[1;32m[SUCCESS]\033[0m Test passed for $file.\n"
    PASSED=$((PASSED + 1))
    rm "$ERROR_FILE"  # Clean up temporary error file if the test passed
  fi
done

# Calculate percentage of passed tests
TOTAL=$((PASSED + FAILED))
PASS_PERCENTAGE=$((100 * PASSED / TOTAL))

# Generate test summary report
echo -e "\n==================== Test Summary ===================="
echo -e "Total tests run: \033[1m$TOTAL\033[0m"
echo -e "Tests passed: \033[1;32m$PASSED\033[0m (\033[1m$PASS_PERCENTAGE%%\033[0m)"
echo -e "Tests failed: \033[1;31m$FAILED\033[0m"

if [ ${#FAILED_TESTS[@]} -ne 0 ]; then
  echo -e "\n\033[1;31mList of failed tests:\033[0m"
  for test in "${FAILED_TESTS[@]}"; do
    echo -e "- \033[1m$test\033[0m"
  done
else
  echo -e "\033[1;32mAll tests passed successfully!\033[0m"
fi
echo -e "=====================================================\n"
