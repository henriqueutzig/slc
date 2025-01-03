#!/usr/bin/bash

PASSED=0
FAILED=0
FAILED_TESTS=()

rm -rf output
rm -rf dot
mkdir -p output/e5

# Define the specific subfolder
SUBFOLDER="tests/e5"

if [ -d "$SUBFOLDER" ]; then
  INPUTS=$(ls "$SUBFOLDER")
  OUTPUT_DIR="output/e5"

  for file in $INPUTS; do
    OUTPUT_FILE="$OUTPUT_DIR/$file.s"
    EXECUTABLE_FILE="$OUTPUT_DIR/$file"

    > "$OUTPUT_FILE"

    echo "Running test for: $SUBFOLDER/$file..."

    # Step 1: Generate assembly file
    if ./etapa6 < "$SUBFOLDER/$file" > "$OUTPUT_FILE"; then
      echo "Assembly generated for $file."

      # Step 2: Compile assembly to executable
      if gcc "$OUTPUT_FILE" -o "$EXECUTABLE_FILE"; then
        echo "Executable compiled for $file."

        # Step 3: Execute the compiled program
        if ./$EXECUTABLE_FILE > "$OUTPUT_DIR/run_output_$file.txt"; then
          echo "Program executed successfully for $file." | tee -a "$OUTPUT_FILE"
          PASSED=$((PASSED + 1))
        else
          # echo -e "\033[1;31m[ERROR]\033[0m Execution failed for $file." | tee -a "$OUTPUT_FILE"
          FAILED=$((FAILED + 1))
          FAILED_TESTS+=("$SUBFOLDER/$file")
        fi
      else
        echo -e "\033[1;31m[ERROR]\033[0m GCC compilation failed for $file." | tee -a "$OUTPUT_FILE"
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("$SUBFOLDER/$file")
      fi
    else
      echo -e "\033[1;31m[ERROR]\033[0m etapa6 failed for $file." | tee -a "$OUTPUT_FILE"
      FAILED=$((FAILED + 1))
      FAILED_TESTS+=("$SUBFOLDER/$file")
    fi
  done
else
  echo -e "\033[1;31m[ERROR]\033[0m Subfolder $SUBFOLDER does not exist."
fi

TOTAL=$((PASSED + FAILED))
PASS_PERCENTAGE=$((100 * PASSED / TOTAL))

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
