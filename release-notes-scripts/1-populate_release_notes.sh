#!/bin/bash

RELEASE_NOTES_FILE="../$RELEASE_NOTES_FILE_NAME"
GITHUB_TOKEN="$GITHUB_TOKEN"
GITHUB_REPOSITORY="$GITHUB_REPOSITORY"

set -e

echo "Detecting Release Notes file..."

if [ -f "$RELEASE_NOTES_FILE" ]; then
    echo "File exists: $RELEASE_NOTES_FILE"
else
    echo "File does not exist: $RELEASE_NOTES_FILE"
    populate_initial_data
fi

populate_initial_data() {
    echo "Populating commits data..."
    touch "$RELEASE_NOTES_FILE"

    process_git_log "$RELEASE_NOTES_FILE"
}

process_git_log() {
    local log_file="$1"
    git log --pretty=format:"%s" --merges --first-parent --max-count=20 > "$log_file"

    while IFS= read -r commit_message; do
        pr_id=$(extract_pr_id "$commit_message")
        if [ -n "$pr_id" ]; then
            pr_description=$(fetch_pr_description "$pr_id")
            echo "PR ID: $pr_id"
            echo "PR Description: $pr_description"
            echo "PR ID: $pr_id" >> "$log_file"
            echo "PR Description: $pr_description" >> "$log_file"
        fi
    done < "$log_file"
}

extract_pr_id() {
    local commit_message="$1"
    local pr_id=$(echo "$commit_message" | grep -o -E '\(#\d+\)' | sed 's/[()]//g')
    echo "$pr_id"
}

fetch_pr_description() {
    local pr_id="$1"
    local api_url="https://api.github.com/repos/$GITHUB_REPOSITORY/pulls/$pr_id"
    
    local pr_description=$(curl -s -H "Authorization: token $GITHUB_TOKEN" "$api_url" | jq -r '.body')
    
    echo "$pr_description"
}
