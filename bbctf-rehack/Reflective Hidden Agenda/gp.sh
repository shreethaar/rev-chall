#!/bin/bash

# Loop through each file, skipping .git, System Volume Information, and Thumbs.db
find . -type f \
    ! -path "*/.git/*" \
    ! -path "*System Volume Information*" \
    ! -name "Thumbs.db" \
    ! -name "Thumbs.db:encryptable" | while read file; do
    echo "Processing: $file"
    git add "$file"
    git commit -m "[autocommit] $file"
    git push
done
