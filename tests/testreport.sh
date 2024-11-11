#!/usr/bin/bash

# Initialize counters and arrays
PASSED=0
FAILED=0
FAILED_TESTS=()

# Loop through each sub-folder in the tests directory
for subfolder in tests/*; do
  if [ -d "$subfolder" ]; then
    INPUTS=$(ls "$subfolder" | grep entrada_)
    OUTPUT_DIR="output/$(basename "$subfolder")"
    DOT_DIR="dot/$(basename "$subfolder")"
    mkdir -p "$OUTPUT_DIR"
    mkdir -p "$DOT_DIR"

    for file in $INPUTS; do
      OUTPUT_FILE="$OUTPUT_DIR/output_$file.txt"
      DOT_FILE="$DOT_DIR/output_$file.dot"

      # Clear the output file before writing to it
      > "$OUTPUT_FILE"

      echo "Running test for: $subfolder/$file..."

      # Run the test and redirect both stdout and stderr to the output file
      cat "$subfolder/$file" | ./etapa3 > "$OUTPUT_FILE" 2>&1
      EXIT_CODE=$?

      # Check if the test should be marked as failed
      if [ $EXIT_CODE -eq 139 ]; then
        echo -e "\n\033[1;31m[ERROR]\033[0m Segmentation fault occurred in $file." | tee -a "$OUTPUT_FILE"
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("$subfolder/$file")
      elif grep -qi "erro" "$OUTPUT_FILE"; then
        echo -e "\n\033[1;31m[ERROR]\033[0m The word 'Erro' was found in the output of $file." | tee -a "$OUTPUT_FILE"
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("$subfolder/$file")
      else
        echo -e "\033[1;32m[SUCCESS]\033[0m Test passed for $file.\n"
        PASSED=$((PASSED + 1))

        # Run output2dot.sh and save the output to the DOT file
        sh output2dot.sh < "$OUTPUT_FILE" > "$DOT_FILE"
        echo "DOT file generated: $DOT_FILE"
      fi
    done
  fi
done

# Calculate percentage of passed tests
TOTAL=$((PASSED + FAILED))
PASS_PERCENTAGE=$((100 * PASSED / TOTAL))

# Generate test summary report
echo -e "\n==================== Test Summary ===================="
echo -e "Total tests run: \033[1m$TOTAL\033[0m"
echo -e "Tests passed: \033[1;32m$PASSED\033[0m (\033[1m$PASS_PERCENTAGE%\033[0m)"
echo -e "Tests failed: \033[1;31m$FAILED\033[0m"

if [ ${#FAILED_TESTS[@]} -ne 0 ]; then
  echo -e "\n\033[1;31mList of failed tests:\033[0m"
  for test in "${FAILED_TESTS[@]}"; do
    echo -e "- \033[1m$test\033[0m"
  done
else
  echo -e "\033[1;32mAll tests passed successfully!\033[0m"
fi
echo -e "======================================================\n"
