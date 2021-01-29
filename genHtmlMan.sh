#!/bin/bash
for filename in Docs/*; do
    roffit --mandir=Docs/ $filename >> ${filename%.*}.html
done 
