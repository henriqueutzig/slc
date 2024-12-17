#!/usr/bin/bash

PASSED=0
FAILED=0
FAILED_TESTS=()

rm -rf output
rm -rf dot
mkdir -p output/e5
mkdir -p dot/e5

# Define the specific subfolder
SUBFOLDER="tests/e5"

if [ -d "$SUBFOLDER" ]; then
  INPUTS=$(ls "$SUBFOLDER" | grep entrada_)
  OUTPUT_DIR="output/e5"
  DOT_DIR="dot/e5"

  for file in $INPUTS; do
    OUTPUT_FILE="$OUTPUT_DIR/output_$file.txt"
    DOT_FILE="$DOT_DIR/output_$file.dot"

    > "$OUTPUT_FILE"

    echo "Running test for: $SUBFOLDER/$file..."

    cat "$SUBFOLDER/$file" | ./etapa5 > "$OUTPUT_FILE" 2>&1
    EXIT_CODE=$?

    if [ $EXIT_CODE -eq 139 ]; then
      echo -e "\n\033[1;31m[ERROR]\033[0m Segmentation fault occurred in $file." | tee -a "$OUTPUT_FILE"
      FAILED=$((FAILED + 1))
      FAILED_TESTS+=("$SUBFOLDER/$file")
    elif grep -Eqi "erro|failed" "$OUTPUT_FILE"; then
      echo -e "\n\033[1;31m[ERROR]\033[0m The word 'Erro' was found in the output of $file." | tee -a "$OUTPUT_FILE"
      FAILED=$((FAILED + 1))
      FAILED_TESTS+=("$SUBFOLDER/$file")
    else
      echo -e "\033[1;32m[SUCCESS]\033[0m Test passed for $file.\n"

      sh output2dot.sh < "$OUTPUT_FILE" > "$DOT_FILE"
      echo "DOT file generated: $DOT_FILE"

      if timeout 10s python3 ilocsim.py -m < "$OUTPUT_FILE" > "$OUTPUT_DIR/ilocsim_$file.txt"; then
        echo "Ilocsim result stored in: $OUTPUT_DIR/ilocsim_$file.txt"
        PASSED=$((PASSED + 1))
      else
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("$SUBFOLDER/$file")
        echo -e "\033[1;31m[ERROR]\033[0m Ilocsim script failed or timed out after 10 seconds."
      fi
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
