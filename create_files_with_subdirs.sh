#!/bin/bash


timestamp() {
  date +"%Y-%m-%d %H:%M:%S"
}

#create log file
LOG_FILE="script.log"

echo "$(timestamp) - Script started" >> "$LOG_FILE"

#create main dir with date and time
dirname=$(date +"%Y-%m-%d_%H-%M-%S")
mkdir -p "$dirname"
echo "$(timestamp) - Main directory created: $dirname" >> "$LOG_FILE"

#list of languages to populate txt files
langs=("Python" "Java" "C" "C++" "C#" "JavaScript" "Go" "Rust" "Ruby" "Kotlin")

#create the subdirs and txt files
for i in {101..110}; do
  subdir="file$i"
  mkdir -p "$dirname/$subdir"
  echo "$(timestamp) - Subdirectory created: $subdir" >> "$LOG_FILE"

  for j in {1..10}; do
    filename="tuser$((500 + j)).txt"
    echo "${langs[$j]}" > "$dirname/$subdir/$filename"
    echo "$(timestamp) - File created: $subdir/$filename (content: ${langs[$j]})" >> "$LOG_FILE"
  done
done

#end of script
echo "$(timestamp) - Script finished successfully" >> "$LOG_FILE"
